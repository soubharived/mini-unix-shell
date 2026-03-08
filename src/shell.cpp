#include <iostream>
#include <vector>
#include <string>

#include "../include/parser.h"
#include "../include/executor.h"
#include "../include/builtins.h"
#include "../include/shell.h"

using namespace std;

void start_shell()
{
    string input;

    while (true)
    {
        cout << "cwushell> ";
        getline(cin, input);

        if (input.size() == 0)
            continue;

        add_history(input);

        vector<string> args = parse_command(input);

        if (args.size() == 0)
            continue;

        if (run_builtin(args))
            continue;

        execute_command(args);
    }
}