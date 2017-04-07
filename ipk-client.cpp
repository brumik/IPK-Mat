#include <iostream>
#include <openssl/md5.h>
#include <vector>
#include <regex>
#include "Connection.h"

using namespace std;

/**
 * Url gets hostname and port.
 * [0] - hostname, [1] - port
 * @param url The url to parse.
 * @return Array of url parts.
 */
vector<string> parse_url(string url) {
    vector<string> values;
    values.push_back(url.substr(0, url.find_last_of(":") ));
    values.push_back(url.substr( url.find_last_of(":")+1 ));
    return values;
}

/**
 * Create md5 hash from the given string.
 * @param str String to hash.
 * @param length String length.
 * @return Hashed string.
 */
string str2md5(const char *str, size_t length) {
    MD5_CTX c;
    unsigned char digest[16];
    char *out = (char*)malloc(33);

    MD5_Init(&c);

    while (length > 512) {
        MD5_Update(&c, str, 512);
        length -= 512;
        str += 512;
    }
    MD5_Update(&c, str, length);
    MD5_Final(digest, &c);

    for (int n = 0; n < 16; ++n) {
        snprintf(&(out[n*2]), 16*2, "%02x", (unsigned int)digest[n]);
    }

    string res = out;
    return res;
}

/**
 * Splits string to substrings by given delimiter
 * @param s String to split.
 * @param c Delimiter char.
 * @return Vector of string split by delimiter.
 */
vector<string> split(string s, char c) {
    vector<string> v;
    string::size_type i = 0;
    string::size_type j = s.find(c);

    while (j != string::npos) {
        v.push_back(s.substr(i, j-i));
        i = ++j;
        j = s.find(c, j);

        if (j == string::npos)
            v.push_back(s.substr(i, s.length()));
    }

    return v;
}

/**
 * Checks for server response validity.
 */
bool valid_response(string resp) {
    return regex_match(resp, regex("SOLVE [\\d]{1,9} (\\+|-|\\*|\\/) [\\d]{1,8}\\n"));
}

/**
 * Return the result of give operation.
 * @param a First param.
 * @param op Operator.
 * @param b Second param.
 * @return Numeric result of operation.
 */
long long do_the_math(long long a, char op, long long b) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        default: return 42;
    }
}

/**
 * Say HELLO to the server.
 * @return Server HELLO.
 */
string generateHello() {
    string login = "xberky02";
    return "HELLO " + str2md5(login.c_str(), login.length()) + "\n";
}

/**
 * Say the results to the server.
 * @param response The exact result of equation.
 * @return Server friendly response.
 */
string generateResponse(string response) {
    return "RESULT " + response + "\n";
}

int main(int argc, char *argv[]) {
    //parlon.holes.sk:55555
    if ( argc != 2 ) {
        cerr << "Usage: ipk-client hostname" << endl;
        return 1;
    }

    vector<string> url = parse_url(argv[1]);
    Connection conn(url[1]);

    if ( !conn.connect(url[0]) ) {
        cerr << "Cannot connect to " << url[0] << ":" << url[1] << endl;
        return 2;
    }

    if ( !conn.sendRequest(generateHello()) )
    {
        cerr << "Sending message failed." << endl;
        return 2;
    }


    string command = conn.getResponse();
    if ( command.empty() )
        return 2;

    vector<string> tokens = split(command, ' ');

    string result = "0";

    while ( tokens.front() != "BYE" )
    {
        if ( !valid_response(command) || (tokens[2][0] == '/' && tokens[3] == "0\n") ) {
            result = "ERROR";
        } else {
            // Process input
            // No check needed because valid_response made it.
            result = to_string(do_the_math( atoll(tokens[1].c_str()), tokens[2][0], atoll(tokens[3].c_str())));
        }

        // Send the answer
        if ( !conn.sendRequest(generateResponse(result)) )
        {
            cerr << "Sending message failed." << endl;
            return 2;
        }

        // Get next command.
        command = conn.getResponse();
        if ( command.empty() )
            return 2;

        tokens = split(command, ' ');
    }

    // Secret printing
    cout << tokens[1];
    return 0;
}