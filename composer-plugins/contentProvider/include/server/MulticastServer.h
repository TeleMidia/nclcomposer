#ifndef MULTICASTSERVER_H_
#define MULTICASTSERVER_H_

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

#include <sys/timeb.h>

#include "SocketServer.h"

#include <iostream>
#include <fstream>
using namespace std;

namespace org {
namespace isdtv {
namespace network {
namespace server {
	class MulticastServer : public SocketServer {
		private:
			char* groupAddr;
			struct sockaddr_in mAddr;

		public:
			//const char MAX=512;
			MulticastServer(char* group, int port);
			virtual ~MulticastServer();
			bool createSocket();

		//private:
			bool tryToBind();
			bool setSocketOptions();

		public:
			int send(char* buff, unsigned int size);
	};
}
}
}
}

#endif /*MULTICASTSERVER_H_*/
