#include <iostream>
#include <vector>
#include <string>
#include <sys/wait.h>
#include <signal.h>

#include "../include/parser.h"
#include "../include/executor.h"
#include "../include/builtins.h"
#include "../include/shell.h"

using namespace std;

void start_shell()
{
    // Shell should ignore Ctrl+C
    signal(SIGINT, SIG_IGN);

    string input;

    while (true)
    {
        // Clean up finished background processes
        int status;
        while (waitpid(-1, &status, WNOHANG) > 0);

        cout << "cwushell> " << flush;

        if (!getline(cin, input))
            break;

        if (input.find_first_not_of(" \t") == string::npos)
            continue;

        add_history(input);

        vector<string> args = parse_command(input);

        if (args.empty())
            continue;

        if (run_builtin(args))
            continue;

        execute_command(args);
    }
}