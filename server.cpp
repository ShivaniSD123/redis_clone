#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cerrno>
#include <cstdlib>
#include <unistd.h>

using namespace std;

// for error

void die(string msg)
{
    int err = errno;
    cerr << "[" << err << "] " << "(" << msg << ") " << strerror(err) << endl;
    abort();
}

// our demo read and write

static void do_something(int conn_fd)
{
    // reading data from the client

    char rbuff[64] = {};
    ssize_t n = read(conn_fd, rbuff, sizeof(rbuff) - 1);
    if (n < 0)
    {
        cerr << stderr << "read() error" << endl;
        return;
    }
    cerr << stderr << "Client says " << rbuff << endl;
    string msg = "world";

    // Sending information to the client

    write(conn_fd, msg.c_str(), msg.size());
}

int main()
{
    // Obtain socket handle
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        die("socket()");

    // set Socket option-
    // As we stop server, it can be reused immidiately if want

    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    // defing and binding the socket

    struct sockaddr_in addr{};
    addr.sin_family = AF_INET; // for IPv4
    addr.sin_addr.s_addr = ntohl(0);
    addr.sin_port = ntohs(1234);
    int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));

    // listen

    rv = listen(fd, SOMAXCONN); // SOMAXCONN-> user defind constant (for maximum reasonable length of pending connections in queue)
    if (rv)
        die("listen()");

    // accept connections

    while (true)
    {
        struct sockaddr_in client_addr = {};
        socklen_t addlen = sizeof(client_addr);
        int conn_fd = accept(fd, (struct sockaddr *)&client_addr, &addlen);
        if (conn_fd < 0)
            continue;
        do_something(conn_fd);
        close(conn_fd);
    }
    close(fd);
    return 0;
}
