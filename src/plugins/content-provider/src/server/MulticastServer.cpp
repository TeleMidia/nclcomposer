#include "../../include/server/MulticastServer.h"

namespace org {
namespace isdtv {
namespace network {
namespace server {
	MulticastServer::MulticastServer(char* group, int portNumber) {
		this->groupAddr = group;
		this->portNumber = portNumber;

		mAddr.sin_family = AF_INET;
		mAddr.sin_addr.s_addr = inet_addr(this->groupAddr);
		mAddr.sin_port = htons(this->portNumber);
	}

	MulticastServer::~MulticastServer() {

	}

	bool MulticastServer::createSocket() {
		cout << "MulticastServer::createSocket" << endl;

#ifdef WIN32
		WORD L_Ver;
		WSADATA wsaData;

		L_Ver = MAKEWORD( 2, 2);

		int L_Err = WSAStartup(L_Ver, &wsaData);
		if (L_Err != 0) {
			cout << "MulticastServer::createSocket Warning!";
			cout << " Can not find winsok dll!" << endl;
			return false;
		}

		if (LOBYTE(wsaData.wVersion) < 2 || HIBYTE(wsaData.wVersion) < 2) {
			cout << "MulticastServer::createSocket Warning!";
			cout << " Winsok dll is too old!" << endl;
			return false;
		}
#endif

		sockDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
		if (sockDescriptor < 0) {
			perror("MulticastServer::createSocket");
			return false;
		}

		if (tryToBind()) {
			if (setSocketOptions()) {
				return true;
			}
		}

		cout << "MulticastServer::createSocket Warning!";
		cout << " return false" << endl;
		return false;
	}

	bool MulticastServer::tryToBind() {
		int ret;

		struct sockaddr_in stSourceAddr;

		stSourceAddr.sin_family = AF_INET;
		stSourceAddr.sin_port = htons(this->portNumber);
		stSourceAddr.sin_addr.s_addr = INADDR_ANY;

		/*
		* Calling bind() is not required, but some implementations need it
		* before you can reference any multicast socket options
		*/
		ret = bind(
				sockDescriptor,
				(struct sockaddr*)&stSourceAddr,
				sizeof(struct sockaddr));

		if (ret < 0) {
			perror ("MulticastServer::tryToBind bind");
			return false;
		}

		if (setsockopt(
				sockDescriptor,
				IPPROTO_IP,
				IP_MULTICAST_IF,
				(char*)&stSourceAddr, sizeof(struct in_addr)) < 0) {

			perror ("MulticastServer::tryToBind IP_MULTICAST_IF");
	    }

		return true;
	}

	bool MulticastServer::setSocketOptions() {
		int opt = 0;
		int ret = setsockopt(
				sockDescriptor,
				IPPROTO_IP,
				IP_MULTICAST_LOOP,
				(char*)opt, sizeof(int));

		if (ret < 0) {
			perror("Multicast::setSocketOptions loop");
		}

		opt = 15;
		ret = setsockopt(
				sockDescriptor,
				IPPROTO_IP,
				IP_MULTICAST_TTL,
				(char*)opt, sizeof(int));

		if (ret < 0) {
			perror("Multicast::setSocketOptions ttl");
		}

		return true;
	}

	int MulticastServer::send(char* buff, unsigned int size) {
		int bytes;
\
		bytes = sendto(
				sockDescriptor,
				buff,
				size,
				0,
				(struct sockaddr *)&mAddr,
				sizeof(mAddr));

		if (bytes < 0) {
#ifdef WIN32
			cout << "MulticastServer::send '" << bytes << "' bytes";
			cout << ", requested '" << size << "'. Error: ";
			cout << WSAGetLastError() << endl;

#else
			herror ("MulticastServer::send");
#endif
		}

		return bytes;
	}
}
}
}
}
