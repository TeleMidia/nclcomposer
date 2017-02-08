/* Copyright (C) 2011-2017 PUC-Rio/TeleMídia Lab.

 NCL Composer is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 NCL Composer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU General Lesser Public License
 along with NCL Composer.  If not, see <http://www.gnu.org/licenses/>. */

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
public:
  /*!
   * \brief Configures the machine properties.
   */
  SimpleSSHClient( const char *username, const char *password,
                   const char *hostip, const char *remotepath );

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
   *
   * \param localfile
   * \param destpath
   * \return
   */
  int sftp_copy_file(const char *localfile, const char* destpath);

  /*!
   * \brief Executes a command in the remote machine.
   *
   * \param command
   * \return
   */
  int exec_cmd(const char *command);

  /*!
   * \brief waitsocket
   * \param socket_fd
   * \param session
   * \return
   */
  static int waitsocket(int socket_fd, LIBSSH2_SESSION *session);

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
};

#endif // SIMPLESSHCLIENT_H
