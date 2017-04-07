/**
 * @file Connection.cpp
 * @author Levente Berky
 *
 * Class declarations.
 */

#include "Connection.h"

#include <iostream>
#include <sys/socket.h>
#include <netdb.h> //hostent
#include <unistd.h>
#include <cstring>

using namespace std;

Connection::Connection()
{
    this->port = "80";
    this->socket_desc = -1;
}

Connection::Connection(string port)
{
    this->port = port;
    this->socket_desc = -1;
}

bool Connection::connect(string host)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    s = getaddrinfo(host.c_str(), port.c_str(), &hints, &result);
    if (s != 0) {
        cerr << "Getaddrinfo failed." << endl;
        return false;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        socket_desc = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (socket_desc == -1)
            continue;

        /* Success */
        if (::connect(socket_desc, rp->ai_addr, rp->ai_addrlen) != -1)
            break;

        close(socket_desc);
    }

    if (rp == NULL) {
        cerr << "Cannot connect." << endl;
        return false;
    }
    freeaddrinfo(result);
    return true;
}

bool Connection::sendRequest(string msg)
{
    if ( send(socket_desc, msg.c_str(), msg.length(), 0) != msg.length() )
    {
        cerr << "Connection: Sending failed.";
        return false;
    }
    return true;
}

string Connection::getResponse()
{
    char buffer[BUFFER_SIZE];
    string msg;
    memset(buffer, 0, BUFFER_SIZE);

    if ( recv(socket_desc, buffer, BUFFER_SIZE, 0) <= 0 ) {
        cerr << "Connection: Bad socket transfer when receiving message" << endl;
        return "";
    }

    msg = buffer;
    return msg;
}

Connection::~Connection()
{
    close(socket_desc);
}


