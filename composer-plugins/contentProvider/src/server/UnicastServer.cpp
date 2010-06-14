#include "../../include/server/UnicastServer.h"

namespace org {
namespace isdtv {
namespace network {
namespace server {
	UnicastServer::UnicastServer(int port) {
		this->portNumber = port;
		this->sock = 0;
		memset(&server, 0, sizeof(struct sockaddr_in));
	}

	bool UnicastServer::createSocket() {
#ifdef WIN32
		WORD L_Versao;
		WSADATA wsaData;

		L_Versao = MAKEWORD( 2, 2);

		int L_Erro = WSAStartup(L_Versao, &wsaData);
		if (L_Erro != 0) {
			cout << "UnicastServer::createSocket Warning!";
			cout << " Can not find winsok dll!" << endl;
			return false;
		}

		if (LOBYTE(wsaData.wVersion) < 2 || HIBYTE(wsaData.wVersion) < 2) {
			cout << "UnicastServer::createSocket Warning!";
			cout << " Winsok dll is too old!" << endl;
			return false;
		}
#endif
		cout << "creating unicast socket" << endl;

		// Name socket using wildcards
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = INADDR_ANY;
		server.sin_port = htons(portNumber);

		int i;
		for(i = 0; i < 8; i++) {
			server.sin_zero[i] = 0;
		}

		/* Create socket */
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

#ifndef WIN32
		if (sock == -1) {

			perror("socket");
			cout << "opening stream socket error sock = '" << sock;
			cout << "'" << endl;
			return false;
		}
#endif

		if (bind(sock, (struct sockaddr*)(&server), sizeof(server)) == -1) {
			perror("bind");
			return false;
		}

		// Find out assigned port number and print it out
		int length;
		length = sizeof(server);
#ifndef WIN32
		getsockname(sock, (sockaddr*)(&server), (socklen_t*)(&length));
#else
		getsockname(sock, (sockaddr*)(&server), (&length));
#endif
		cout << "serving at port " << ntohs(server.sin_port) << ".";
		cout << endl << "Accepting connections... ";

		// Start accepting connections
		if (listen(sock, 5) == -1) {
			perror("listen");
			return false;
		}

		sockDescriptor = accept(sock, 0, 0);
		cout << "done!" << endl;
#ifndef WIN32
		if (sockDescriptor == -1) {
			perror("accept");
			return false;
		}
#endif
		return true;
	}

	UnicastServer::~UnicastServer() {
#ifdef WIN32
		closesocket(sockDescriptor);
		closesocket(sock);
#else
		close(sockDescriptor);
		close(sock);
#endif
	}

	int UnicastServer::send(char* buff, unsigned int size) {
		return ::send(sockDescriptor, buff, size, 0);
	}

	void UnicastServer::printStream(char buffer[BUFFSIZE]) {
		unsigned int i;
		for (i=0; i<BUFFSIZE; i++) {
			if (i % 188 == 0)
				cout << endl;

			cout << i << "=" << (buffer[i]  & 0xFF) << " ";
		}
	}
}
}
}
}
