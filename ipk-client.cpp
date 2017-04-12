#include <iostream>
#include <vector>
#include <regex>
#include <iomanip>
#include "md5.h"
#include "Connection.h"

using namespace std;

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
double do_the_math(long long a, char op, long long b) {
    switch (op) {
        case '+': return a + (double)b;
        case '-': return a - (double)b;
        case '*': return a * (double)b;
        case '/': return a / (double)b;
        default: return 42.00;
    }
}

/**
 * Say HELLO to the server.
 * @return Server HELLO.
 */
string generateHello() {
    string login = "xberky02";
    return "HELLO " + md5(login) + "\n";
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
    if ( argc != 2 ) {
        cerr << "Usage: ipk-client hostname" << endl;
        return 1;
    }

//    vector<string> url = parse_url(argv[1]);
    Connection conn("55555");

    if ( !conn.connect(argv[1]) ) {
        cerr << "Cannot connect to " << argv[1] << ":55555" << endl;
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

    // Helping variables to convert result to string
    double temp;
    stringstream stream;
    stream << fixed << setprecision(2);
    while ( tokens.front() != "BYE" )
    {
        if ( !valid_response(command) || (tokens[2][0] == '/' && tokens[3] == "0\n") ) {
            result = "ERROR";
        } else {
            // Process input
            // No check needed because valid_response made it.
            stream.str("");
            temp = do_the_math( atoll(tokens[1].c_str()), tokens[2][0], atoll(tokens[3].c_str()) );
            stream << temp;
            result = stream.str();
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

    // Secret printing - cutting of \n with substring.
    cout << tokens[1].substr(0, tokens[1].size()-1);
    return 0;
}