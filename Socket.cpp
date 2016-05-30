//
// Created by vlad on 30.05.16.
//


#include <iostream>
#include "AbstractSocket.cpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

class Socket : public AbstractSocket {
private:
    // Створюємо екземпляр прослуховуючого сокета:
    typedef int SOCKET; // В Unix типу даних SOCKET немає
    SOCKET socket;

    struct sockaddr socketAddr;

    Socket(SOCKET socket) : AbstractSocket() {
        this->socket = socket;
    }

public:
    Socket() : AbstractSocket() {
        socket = ::socket(AF_INET, SOCK_STREAM, 0);
    }

    virtual void listen(const char *host, int port);

    virtual AbstractSocket &accept();

    virtual void connect(const char *host, int port);

    virtual void write(const char *message);

    virtual const char *read();

    virtual void close();
};

AbstractSocket &createSocket() {
    return *new Socket;
}

void Socket::listen(const char *host, int port) {
    cerr << "I'm listening to port " << port << " at host " << host << endl;

    struct sockaddr_in my_serv_addr;
    my_serv_addr.sin_family = AF_INET;
    my_serv_addr.sin_port = htons((uint16_t) port);
    my_serv_addr.sin_addr.s_addr = inet_addr(host);

    // Присвоюємо прослуховуючому сокету ІР-адресу і порт:
    bind(socket, (const struct sockaddr *) &my_serv_addr, sizeof(my_serv_addr));

    // Розпочинаємо процес прослуховування, дозволяючи одночасне під'єднання не більше 5 клієнтських сокетів:
    ::listen(socket, 5);
}

AbstractSocket &Socket::accept() {
    cerr << "accept" << endl;
    socklen_t sockStructSize = sizeof(struct sockaddr);
    SOCKET client = ::accept(socket, &socketAddr, &sockStructSize);
    return *new Socket(client);
}

void Socket::connect(const char *host, int port) {
    cerr << "I'm connecting to port " << port << " at host " << host << endl;
}

void Socket::write(const char *message) {
    cerr << "Send message " << message << endl;
}

const char *Socket::read() {
    char *buf = new char[1024];
    buf[recv(socket, buf, sizeof(buf), 0)] = 0;
    return buf;
}

void Socket::close() {
    ::close(socket);
}