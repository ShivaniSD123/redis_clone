#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cerrno>   //for error number
#include <cstdlib>  //for abort
#include <cstring>  //for strerror
#include <unistd.h> // for write

using namespace std;

void die(string msg)
{
    int err = errno;
    cerr << "[" << err << "] " << msg << " (" << strerror(err) << ")" << endl;
    abort();
}
int main()
{
    // Obtaining socket handler
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        die("socket()");

    // Defining The Client Address and binding

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);
    addr.sin_port = ntohs(1234);
    int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));

    // Establishing the Connection

    rv = connect(fd, (const sockaddr *)&addr, sizeof(addr));

    // Read and Write test

    string msg = "hello";
    write(fd, msg.c_str(), msg.size());
    char rbuff[64] = {};
    ssize_t n = read(fd, rbuff, sizeof(rbuff) - 1);
    if (n < 0)
        die("read()");
    cout << "Server says " << rbuff << endl;
    close(fd);
    return 0;
}