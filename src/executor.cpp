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

    // Detect background process
    if (!args.empty() && args.back() == "&")
    {
        background = true;
        args.pop_back();
    }

    // Detect output redirection
    int redirect_index = -1;

    for (int i = 0; i < args.size(); i++)
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
        outfile = args[redirect_index + 1];
        args.resize(redirect_index);
    }

    // Split commands by pipes
    vector<vector<string>> commands;
    vector<string> current;

    for (auto &arg : args)
    {
        if (arg == "|")
        {
            commands.push_back(current);
            current.clear();
        }
        else
        {
            current.push_back(arg);
        }
    }

    commands.push_back(current);

    int n = commands.size();
    int prev_pipe[2];

    for (int i = 0; i < n; i++)
    {
        int pipefd[2];

        if (i < n - 1)
            pipe(pipefd);

        pid_t pid = fork();

        if (pid == 0)
        {
            // Handle input from previous pipe
            if (i > 0)
            {
                dup2(prev_pipe[0], STDIN_FILENO);
                close(prev_pipe[0]);
                close(prev_pipe[1]);
            }

            // Handle output to next pipe
            if (i < n - 1)
            {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
            }

            // Handle output redirection on last command
            if (redirect_index != -1 && i == n - 1)
            {
                int fd = open(outfile.c_str(),
                              O_WRONLY | O_CREAT | O_TRUNC,
                              0644);

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
        else
        {
            // Print background process PID once
            if (background && i == 0)
            {
                cout << "[bg] pid: " << pid << endl;
            }
        }

        // Close previous pipe
        if (i > 0)
        {
            close(prev_pipe[0]);
            close(prev_pipe[1]);
        }

        // Save current pipe
        if (i < n - 1)
        {
            prev_pipe[0] = pipefd[0];
            prev_pipe[1] = pipefd[1];
        }
    }

    // Wait only if not background
    if (!background)
    {
        for (int i = 0; i < n; i++)
            wait(NULL);
    }
}