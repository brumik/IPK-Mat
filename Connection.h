/**
 * @file Conntection.h
 * @author Levente Berky
 *
 * Connection class header
 */

#ifndef IPK1_CONNECTION_H
#define IPK1_CONNECTION_H

#include <string>

#define BUFFER_SIZE 1024

class Connection
{
private:
    int socket_desc;
    std::string port;

public:
    Connection();

    /**
     * Create connection with given IP address.
     * @param port The port, server listens to.
     */
    Connection(std::string port);

    /**
     * Connect to the server on IP stored
     * Exits if there is no valid IP.
     *
     * @return True on success false otherwise.
     */
    bool connect(std::string host);

    /**
     * Sending the given request to the server.
     *
     * @param msg The request message.
     * @return True on success and false otherwise.
     */
    bool sendRequest(std::string msg);

    /**
     * Gets text response body.
     * @param length Length of response body.
     * @return Response body.
     */
    std::string getResponse();

    ~Connection();
};

#endif //IPK1_CONNECTION_H
