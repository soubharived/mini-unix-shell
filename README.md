Mini Unix Shell (C++)

A lightweight Unix-like command line shell implemented in C++ to understand how real shells work internally.
This project demonstrates core Operating System concepts such as process creation, pipes, job control, and signal handling.

The shell allows users to execute commands, create pipelines, redirect output, run background jobs, and manage running processes.

Features
Command Execution

Execute any Linux command using system process calls.

cwushell> ls
cwushell> pwd
cwushell> whoami

Internally implemented using:

fork()
execvp()
waitpid()
Pipes

Supports multi-stage pipelines.

cwushell> ls | grep txt
cwushell> ls | grep txt | sort
cwushell> cat test.txt | grep hello | sort

Implemented using:

pipe()
dup2()
Output Redirection

Redirect command output into files.

cwushell> echo hello > file.txt
cwushell> cat file.txt

Uses:

open()
dup2()
Background Processes

Run commands without blocking the shell.

cwushell> sleep 20 &

Output:

[bg] pid: 3170

The shell immediately returns control to the user.

Job Control

List and manage background jobs.

cwushell> jobs
[1] Running sleep 20

Bring job to foreground:

cwushell> fg 1
Built-in Commands

The shell implements several built-in commands internally:

cd        change directory
exit      exit shell
history   show command history
jobs      list background jobs
fg        bring job to foreground

Example:

cwushell> cd docs
cwushell> history
Command History

The shell stores previously executed commands.

cwushell> history
1 ls
2 pwd
3 echo hello
Signal Handling

The shell handles Ctrl+C (SIGINT) properly.

Example:

cwushell> sleep 100
^C
cwushell>

The shell remains running while the child process is interrupted.

Zombie Process Cleanup

The shell continuously checks for finished background processes using:

waitpid(-1, &status, WNOHANG)

This prevents zombie processes.

Project Structure
mini-unix-shell
│
├── src
│   ├── main.cpp
│   ├── shell.cpp
│   ├── parser.cpp
│   ├── executor.cpp
│   ├── builtins.cpp
│   └── jobs.cpp
│
├── include
│   ├── parser.h
│   ├── executor.h
│   ├── builtins.h
│   └── jobs.h
│
├── README.md
└── .gitignore
Software Requirements
Operating System

Linux environment required.

Tested on:

Ubuntu (WSL2)
Linux
Compiler

Requires a C++ compiler supporting C++11 or later.

Recommended:

g++ (GNU Compiler)

Check version:

g++ --version
Required System Libraries

The program uses standard Linux system headers:

unistd.h
sys/wait.h
fcntl.h
signal.h

These are included by default in Linux environments.

Dependencies

No external libraries are required.

Only standard Linux system calls and the C++ standard library are used.

How to Build

Clone the repository:

git clone https://github.com/YOUR_USERNAME/mini-unix-shell.git

Navigate to project directory:

cd mini-unix-shell

Compile the program:

g++ src/*.cpp -o cwushell
How to Run

Run the shell:

./cwushell

Example session:

cwushell> ls
cwushell> pwd
cwushell> echo hello > file.txt
cwushell> cat file.txt
cwushell> sleep 10 &
cwushell> jobs
cwushell> fg 1
Example Commands
Pipes
ls | grep txt
ls | grep txt | sort
Redirection
echo hello > file.txt
cat file.txt
Background jobs
sleep 20 &
jobs
Foreground job
fg 1
Pipeline with redirection
cat test.txt | grep hello | sort > result.txt
Example Output
cwushell> ls | grep txt
file.txt
test.txt
result.txt

cwushell> sleep 10 &
[bg] pid: 3170

cwushell> jobs
[1] Running sleep 10
Concepts Demonstrated

This project demonstrates several important Operating System concepts:

Process creation (fork)
Program execution (exec)
Inter-process communication (pipes)
File descriptor manipulation (dup2)
Signal handling
Background process management
Job control
Zombie process prevention
Limitations

This is a minimal educational shell and does not implement full Bash functionality.

Missing features include:

environment variable expansion ($HOME)
logical operators (&&, ||)
tab completion
advanced parsing
Learning Objectives

The goal of this project is to understand:

how real Unix shells work internally
process lifecycle management
Linux system calls
inter-process communication
signal handling
Future Improvements

Possible enhancements:

environment variable expansion
support for && and ||
better command parser
tab completion
improved job control
Author

Ved Parkash

