#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <vector>
#include <string>
#include <iostream>

#include "../include/executor.h"

using namespace std;

void execute_command(vector<string> &args)
{
    bool background = false;

    if (!args.empty() && args.back() == "&")
    {
        background = true;
        args.pop_back();
    }

    int redirect_index = -1;

    for (size_t i = 0; i < args.size(); i++)
    {
        if (args[i] == ">")
        {
            redirect_index = i;
            break;
        }
    }

    string outfile;

    if (redirect_index != -1)
    {
        if (redirect_index + 1 >= args.size())
        {
            cerr << "syntax error: expected file after '>'\n";
            return;
        }

        outfile = args[redirect_index + 1];
        args.resize(redirect_index);
    }

    vector<vector<string>> commands;
    vector<string> current;

    for (auto &arg : args)
    {
        if (arg == "|")
        {
            if (current.empty())
            {
                cerr << "syntax error near '|'\n";
                return;
            }

            commands.push_back(current);
            current.clear();
        }
        else
        {
            current.push_back(arg);
        }
    }

    if (!current.empty())
        commands.push_back(current);

    int n = commands.size();

    int prev_pipe[2];
    vector<pid_t> children;

    for (int i = 0; i < n; i++)
    {
        int pipefd[2];

        if (i < n - 1)
        {
            if (pipe(pipefd) == -1)
            {
                perror("pipe failed");
                return;
            }
        }

        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork failed");
            return;
        }

        if (pid == 0)
        {
            if (i > 0)
            {
                dup2(prev_pipe[0], STDIN_FILENO);
                close(prev_pipe[0]);
                close(prev_pipe[1]);
            }

            if (i < n - 1)
            {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
            }

            if (redirect_index != -1 && i == n - 1)
            {
                int fd = open(outfile.c_str(),
                              O_WRONLY | O_CREAT | O_TRUNC,
                              0644);

                if (fd < 0)
                {
                    perror("open failed");
                    exit(1);
                }

                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            vector<char*> cargs;

            for (auto &arg : commands[i])
                cargs.push_back(&arg[0]);

            cargs.push_back(nullptr);

            execvp(cargs[0], cargs.data());

            perror("exec failed");
            exit(1);
        }

        children.push_back(pid);

        if (background && i == 0)
            cout << "[bg] pid: " << pid << endl;

        if (i > 0)
        {
            close(prev_pipe[0]);
            close(prev_pipe[1]);
        }

        if (i < n - 1)
        {
            prev_pipe[0] = pipefd[0];
            prev_pipe[1] = pipefd[1];
        }
    }

    if (!background)
    {
        for (pid_t pid : children)
            waitpid(pid, NULL, 0);
    }
    
}