#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include "../include/builtins.h"

using namespace std;

vector<string> history_list;

void add_history(const string &cmd)
{
    history_list.push_back(cmd);
}

void show_history()
{
    for (int i = 0; i < history_list.size(); i++)
    {
        cout << i + 1 << "  " << history_list[i] << endl;
    }
}

bool run_builtin(vector<string> &args)
{
    if (args[0] == "cd")
    {
        if (args.size() < 2)
        {
            cout << "cd: missing argument\n";
        }
        else
        {
            if (chdir(args[1].c_str()) != 0)
                perror("cd failed");
        }
        return true;
    }

    if (args[0] == "exit")
    {
        exit(0);
    }

    if (args[0] == "history")
    {
        show_history();
        return true;
    }

    return false;
}