#include <sstream>
#include <vector>
#include <string>
#include "../include/parser.h"

using namespace std;

vector<string> parse_command(const string &input)
{
    stringstream ss(input);
    string word;
    vector<string> args;

    while (ss >> word)
        args.push_back(word);

    return args;
}