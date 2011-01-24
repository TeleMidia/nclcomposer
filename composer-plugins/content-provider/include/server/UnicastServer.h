#ifndef UNICASTSERVER_H_
#define UNICASTSERVER_H_

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>

#include "SocketServer.h"

#include <iostream>
using namespace std;

namespace org {
namespace isdtv {
namespace network {
namespace server {
	class UnicastServer : public SocketServer {
		private:
#ifdef WIN32
			SOCKET sock; //visible unicast socket
#else
			int sock; //visible unicast socket
#endif
			struct sockaddr_in server;

		public:
			UnicastServer(int port);
			~UnicastServer();
			bool createSocket();
			void printStream(char buffer[BUFFSIZE]);
			int send(char* buff, unsigned int size);
	};
}
}
}
}

#endif /*UNICASTSERVER_H_*/
