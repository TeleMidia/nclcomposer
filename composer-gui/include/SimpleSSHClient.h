/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef SIMPLESSHCLIENT_H
#define SIMPLESSHCLIENT_H

extern "C" {
// #include "libssh2_config.h"
#include <libssh2.h>
#include <libssh2_sftp.h>

#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif

#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
}

#include <string>
using namespace std;

#include <QMutex>

//\todo A better way to send error report than fprintf
class SimpleSSHClient
{
private:
  string username;
  string password;
  string hostip;
  string sftp_path;
  string sftp_file;
  static int libssh2_init_rc; /* Keeps the value returned by lissh2_init */

  int sock;

  LIBSSH2_SESSION *session;
  LIBSSH2_SFTP *sftp_session;

public:
  /*!
   * \brief Configures the machine properties.
   */
  SimpleSSHClient(const char *username, const char *password,
                  const char *hostip, const char *remotepath);

  /*!
   * \brief Initialize the libssh2 library. This function must be called only
   *  one time (possible in the beginning of your program).
   */
  static int init();

  /*!
   * \brief Free all data of libssh2.
   */
  static void exit();

  /*!
   * \brief Connect to a remote machine
   */
  int doConnect(); // I do not call connect because it conflicts with win32
                   // connect.
  /*!
   * \brief Disconnect from the current connected remote machine.
   */
  void doDisconnect(); // For consistency I also call this doDisconnect.

  /*!
   * \brief Copy the file from localpath to the remotepath.
   */
  int sftp_copy_file(const char *localfile, const char* destpath);

  /*!
   * \brief Executes a command in the remote machine.
   */
  int exec_cmd(const char *command);

  static int waitsocket(int socket_fd, LIBSSH2_SESSION *session);
};

#endif // SIMPLESSHCLIENT_H
