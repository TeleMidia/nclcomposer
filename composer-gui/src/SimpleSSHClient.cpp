/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "SimpleSSHClient.h"
#include <gcrypt.h>

// This variable tell us if the libssh2 was initialized properly. This value
// will be equal to zero only if the libssh2 are safely initialized through
// \ref SimpleSSHClient::init()
int SimpleSSHClient::libssh2_init_rc = -1;
QMutex SimpleSSHClient::mutex;

// QMutex thread callbacks for libgcrypt
static int boost_mutex_init(void **priv)
{
  QMutex *lock = new QMutex();
  if (!lock)
    return ENOMEM;
  *priv = lock;
  return 0;
}

static int boost_mutex_destroy(void **lock)
{
  delete reinterpret_cast<QMutex*>(*lock);
  return 0;
}

static int boost_mutex_lock(void **lock)
{
  reinterpret_cast<QMutex*>(*lock)->lock();
  return 0;
}

static int boost_mutex_unlock(void **lock)
{
  reinterpret_cast<QMutex*>(*lock)->unlock();
  return 0;
}

static struct gcry_thread_cbs gcry_threads_boost =
{ GCRY_THREAD_OPTION_USER, NULL,
  boost_mutex_init, boost_mutex_destroy,
  boost_mutex_lock, boost_mutex_unlock };

int SimpleSSHClient::init()
{
  // make libgcrypt thread safe
  // this must be called before any other libgcrypt call
  gcry_control( GCRYCTL_SET_THREAD_CBS, &gcry_threads_boost );

  SimpleSSHClient::libssh2_init_rc = libssh2_init (0);
  return libssh2_init_rc;
}

void SimpleSSHClient::exit()
{
  libssh2_exit();
}

SimpleSSHClient::SimpleSSHClient(const char *username_,
                                 const char *password_,
                                 const char *hostip_,
                                 const char *scppath_)
{
  this->username = username_;
  this->password = password_;
  this->hostip = hostip_;
  this->scppath = scppath_;
}

int SimpleSSHClient::scp_copy_file(const char *localncl)
{
  int sock, i, rc;
  LIBSSH2_SESSION *session;
  LIBSSH2_CHANNEL *channel;
  unsigned long hostaddr = inet_addr(hostip.c_str());
  struct sockaddr_in sin;
  size_t nread;
  const char *fingerprint;
  FILE *local;
  struct stat fileinfo;
  char mem[1024];
  char *ptr;

  size_t found;
  string temp = localncl;
  found = temp.find_last_of("/\\");
  string nclfile = temp.substr(found+1);

  scpfile = scppath + string("/") + nclfile;

#ifdef WIN32
  WSADATA wsadata;
  WSAStartup(MAKEWORD(2,0), &wsadata);
#endif

  mutex.lock();
  if (libssh2_init_rc != 0)
  {
    fprintf (stderr, "libssh2 initialization failed (%d)\n", libssh2_init_rc);
    return 1;
  }

  local = fopen(localncl, "rb");
  if (!local)
  {
    fprintf(stderr, "Can't open local file %s\n", localncl);
    return -1;
  }

  stat(localncl, &fileinfo);

  /* Ultra basic "connect to port 22 on localhost"
   * Your code is responsible for creating the socket establishing the
   * connection
   */
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(-1 == sock)
  {
    fprintf(stderr, "failed to create socket!\n");
    return -1;
  }

  sin.sin_family = AF_INET;
  sin.sin_port = htons(22);
  sin.sin_addr.s_addr = hostaddr;
  if (connect(sock, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in)) != 0)
  {
    fprintf(stderr, "failed to connect!\n");
    return -1;
  }

  /* Create a session instance
   */
  session = libssh2_session_init();

  if(!session)
    return -1;

  /* ... start it up. This will trade welcome banners, exchange keys,
   * and setup crypto, compression, and MAC layers
   */
  rc = libssh2_session_startup(session, sock);

  if(rc)
  {
    fprintf(stderr, "Failure establishing SSH session: %d\n", rc);
    return -1;
  }

  /* At this point we havn't yet authenticated.  The first thing to do
   * is check the hostkey's fingerprint against our known hosts Your app
   * may have it hard coded, may go to a file, may present it to the
   * user, that's your call
   */
  fingerprint = libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);

  fprintf(stderr, "Fingerprint: ");
  for(i = 0; i < 20; i++)
  {
    fprintf(stderr, "%02X ", (unsigned char)fingerprint[i]);
  }
  fprintf(stderr, "\n");

  /* We could authenticate via password */
  if (libssh2_userauth_password(session, username.c_str(), password.c_str()))
  {
    fprintf(stderr, "Authentication by password failed.\n");
    goto shutdown_copy;
  }

  /* Send a file via scp. The mode parameter must only have permissions! */
  channel = libssh2_scp_send(session, scpfile.c_str(), fileinfo.st_mode & 0777,
                             (unsigned long)fileinfo.st_size);

  if (!channel)
  {
    char *errmsg;
    int errlen;
    int err = libssh2_session_last_error(session, &errmsg, &errlen, 0);

    fprintf(stderr, "Unable to open a session: (%d) %s\n", err, errmsg);
    goto shutdown_copy;
  }
  mutex.unlock();

  fprintf(stderr, "SCP session waiting to send file\n");
  do
  {
    nread = fread(mem, 1, sizeof(mem), local);
    if (nread <= 0)
    {
      /* end of file */
      break;
    }
    ptr = mem;

    do
    {
      /* write the same data over and over, until error or completion */
      rc = libssh2_channel_write(channel, ptr, nread);

      if (rc < 0)
      {
        fprintf(stderr, "ERROR %d\n", rc);
        break;
      }
      else
      {
        /* rc indicates how many bytes were written this time */
        ptr += rc;
        nread -= rc;
      }
    } while (nread);
  } while (1);


  fprintf(stderr, "Sending EOF\n");
  libssh2_channel_send_eof(channel);

  fprintf(stderr, "Waiting for EOF\n");
  libssh2_channel_wait_eof(channel);

  fprintf(stderr, "Waiting for channel to close\n");
  libssh2_channel_wait_closed(channel);

  libssh2_channel_free(channel);

  channel = NULL;

shutdown_copy:
  mutex.lock();
  if(session)
  {
    libssh2_session_disconnect(session,
                               "Normal Shutdown, Thank you for playing");

    libssh2_session_free(session);

  }
#ifdef WIN32
  closesocket(sock);
#else
  close(sock);
#endif
  if (local)
    fclose(local);
  mutex.unlock();
  return 0;
}

int SimpleSSHClient::exec_cmd(const char *command)
{
  int sock, rc;
  LIBSSH2_SESSION *session;
  LIBSSH2_CHANNEL *channel;
  LIBSSH2_KNOWNHOSTS *nh;
  unsigned long hostaddr = inet_addr(hostip.c_str());
  struct sockaddr_in sin;
  const char *fingerprint;
  // size_t nread;
  size_t len;
  int type;

  int exitcode;
  char *exitsignal=(char *)"none";
  int bytecount = 0;
//  string command = "/misc/launcher.sh " + scpfile;

  mutex.lock();
  /* Ultra basic "connect to port 22 on localhost"
   * Your code is responsible for creating the socket establishing the
   * connection
   */
  sock = socket(AF_INET, SOCK_STREAM, 0);

  sin.sin_family = AF_INET;
  sin.sin_port = htons(22);
  sin.sin_addr.s_addr = hostaddr;
  if (connect(sock, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in)) != 0)
  {
    fprintf(stderr, "failed to connect!\n");
    return -1;
  }

  /* Create a session instance */
  session = libssh2_session_init();
  if (!session) return -1;

  /* tell libssh2 we want it all done non-blocking */
  libssh2_session_set_blocking(session, 0);

  /* ... start it up. This will trade welcome banners, exchange keys,
   * and setup crypto, compression, and MAC layers
   */
  while ((rc = libssh2_session_startup(session, sock)) == LIBSSH2_ERROR_EAGAIN);
  if (rc)
  {
    fprintf(stderr, "Failure establishing SSH session: %d\n", rc);
    return -1;
  }

  nh = libssh2_knownhost_init(session);

  if(!nh)
  {
    /* eeek, do cleanup here */
    return 2;
  }

  /* read all hosts from here */
  libssh2_knownhost_readfile(nh, "known_hosts", LIBSSH2_KNOWNHOST_FILE_OPENSSH);

  /* store all known hosts to here */
  libssh2_knownhost_writefile(nh, "dumpfile", LIBSSH2_KNOWNHOST_FILE_OPENSSH);
  fingerprint = libssh2_session_hostkey(session, &len, &type);

  if(fingerprint)
  {
    struct libssh2_knownhost *host;
#if LIBSSH2_VERSION_NUM >= 0x010206
    /* introduced in 1.2.6 */
    int check = libssh2_knownhost_checkp(nh, hostip.c_str(), 22,
                                         fingerprint, len,
                                         LIBSSH2_KNOWNHOST_TYPE_PLAIN|
                                         LIBSSH2_KNOWNHOST_KEYENC_RAW,
                                         &host);
#else
    /* 1.2.5 or older */
    int check = libssh2_knownhost_check(nh, hostip.c_str(),
                                        fingerprint, len,
                                        LIBSSH2_KNOWNHOST_TYPE_PLAIN|
                                        LIBSSH2_KNOWNHOST_KEYENC_RAW,
                                        &host);
#endif
    fprintf(stderr, "Host check: %d, key: %s\n", check,
            (check <= LIBSSH2_KNOWNHOST_CHECK_MISMATCH)?
              host->key:"<none>");

    /*****
     * At this point, we could verify that 'check' tells us the key is
     * fine or bail out.
     *****/
  }
  else
  {
    /* eeek, do cleanup here */
    return 3;
  }
  libssh2_knownhost_free(nh);

  mutex.unlock();

  if ( strlen(password.c_str()) != 0 )
  {
    /* We could authenticate via password */
    while ((rc = libssh2_userauth_password(session, username.c_str(),
                                           password.c_str()))
           == LIBSSH2_ERROR_EAGAIN);
    if (rc) {
      fprintf(stderr, "Authentication by password failed.\n");
      goto shutdown_start;
    }
  }
  else
  {
    /* Or by public key */
    while ((rc = libssh2_userauth_publickey_fromfile(session, username.c_str(),
                                                     "/home/user/"
                                                     ".ssh/id_rsa.pub",
                                                     "/home/user/"
                                                     ".ssh/id_rsa",
                                                     password.c_str())) ==
           LIBSSH2_ERROR_EAGAIN);
    if (rc)
    {
      fprintf(stderr, "\tAuthentication by public key failed\n");
      goto shutdown_start;
    }
  }

#if 0
  libssh2_trace(session, ~0 );
#endif

  /* Exec non-blocking on the remove host */
  while( (channel = libssh2_channel_open_session(session)) == NULL &&
         libssh2_session_last_error(session,NULL,NULL,0) == LIBSSH2_ERROR_EAGAIN)
  {
    waitsocket(sock, session);
  }
  if( channel == NULL )
  {
    fprintf(stderr,"Error\n");
    ::exit( 1 );
  }

  while( (rc = libssh2_channel_exec(channel, command)) ==

         LIBSSH2_ERROR_EAGAIN )
  {
    waitsocket(sock, session);
  }
  if( rc != 0 )
  {
    fprintf(stderr,"Error\n");
    ::exit( 1 );
  }
  for( ;; )
  {
    /* loop until we block */
    int rc;
    do
    {
      char buffer[0x4000];
      rc = libssh2_channel_read( channel, buffer, sizeof(buffer) );

      if( rc > 0 )
      {
        int i;
        bytecount += rc;
        fprintf(stderr, "We read:\n");
        for( i=0; i < rc; ++i )
          fputc( buffer[i], stderr);
        fprintf(stderr, "\n");
      }
      else {
        fprintf(stderr, "libssh2_channel_read returned %d\n", rc);
      }
    }
    while( rc > 0 );

    /* this is due to blocking that would occur otherwise so we loop on
    this condition */
    if( rc == LIBSSH2_ERROR_EAGAIN )
    {
      waitsocket(sock, session);
    }
    else
      break;
  }
  exitcode = 127;

  while( (rc = libssh2_channel_close(channel)) == LIBSSH2_ERROR_EAGAIN )
    waitsocket(sock, session);

  if( rc == 0 )
  {
    exitcode = libssh2_channel_get_exit_status( channel );

    // Just for libssh2 1.2.8 or greater
    // libssh2_channel_get_exit_signal(channel, &exitsignal,
    //                                NULL, NULL, NULL, NULL, NULL);
  }

  if (exitsignal)
    printf("\nGot signal: %s\n", exitsignal);
  else
    printf("\nEXIT: %d bytecount: %d\n", exitcode, bytecount);


shutdown_start:
  mutex.lock();
  if(session) {
    libssh2_session_disconnect( session,
                                "Normal Shutdown, Thank you for playing");
    libssh2_session_free(session);
  }
#ifdef WIN32
  closesocket(sock);
#else
  close(sock);
#endif
  mutex.unlock();

  return 0;
}

int SimpleSSHClient::waitsocket(int socket_fd, LIBSSH2_SESSION *session)
{
  struct timeval timeout;
  int rc;
  fd_set fd;
  fd_set *writefd = NULL;
  fd_set *readfd = NULL;
  int dir;

  timeout.tv_sec = 10;
  timeout.tv_usec = 0;

  FD_ZERO(&fd);

  FD_SET(socket_fd, &fd);

  /* now make sure we wait in the correct direction */
  dir = libssh2_session_block_directions(session);

  if(dir & LIBSSH2_SESSION_BLOCK_INBOUND)
    readfd = &fd;

  if(dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
    writefd = &fd;

  rc = select(socket_fd + 1, readfd, writefd, NULL, &timeout);

  return rc;
}
