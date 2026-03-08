Unix Shell Project – Concepts and Interview Notes
1. What is a Shell?

A shell is a program that acts as an interface between the user and the operating system kernel.

The shell allows the user to:

execute programs

manage files

control processes

run commands interactively

Examples of shells:

Bash

Zsh

Fish

Sh

Your project implements a mini shell that mimics some of these features.

Basic shell workflow:

user command → shell → kernel → program execution → output
2. Shell Execution Loop

A shell usually follows this loop:

while(true)
    print prompt
    read command
    parse command
    execute command

Example from your project:

cwushell>

Steps:

print prompt

read input using getline

parse input

check built-in commands

execute external commands

3. Processes in Unix

A process is a running instance of a program.

Each process has:

process ID (PID)

memory space

registers

file descriptors

Example:

sleep 10

The system creates a new process to run sleep.

4. Process Creation – fork()

fork() creates a new process by duplicating the current process.

Prototype:

pid_t fork();

Return values:

child process  → returns 0
parent process → returns child PID
error          → returns -1

Example:

pid = fork()

if pid == 0
    child process
else
    parent process

Key concept:

After fork(), both processes continue execution from the same line.

5. Program Execution – exec

exec replaces the current process image with a new program.

Your shell uses:

execvp()

Prototype:

int execvp(const char *file, char *const argv[]);

Important property:

fork → creates process
exec → replaces process program

Flow in shell:

fork()
child → execvp()
parent → wait()
6. Parent and Child Processes

Example flow:

shell process
    |
    fork
    |
child process
    |
exec command

Example:

cwushell> ls

Steps:

shell forks

child executes ls

parent waits for child

7. Waiting for Child – wait() and waitpid()

The parent must collect the exit status of child processes.

Otherwise a zombie process is created.

Functions:

wait()
waitpid()

Prototype:

waitpid(pid, &status, options)

Used in your shell:

waitpid(pid, NULL, 0)

Meaning:

wait until child finishes
8. Zombie Processes

A zombie process occurs when:

child finishes
parent does not call wait()

The process entry remains in the process table.

Your shell prevents zombies using:

waitpid(-1, &status, WNOHANG)

Meaning:

check for finished children without blocking

9. File Descriptors

Every process has file descriptors:

0 → stdin
1 → stdout
2 → stderr

These represent:

input stream
output stream
error stream

Example:

stdout → terminal

But we can redirect them.

10. Output Redirection

Example:

echo hello > file.txt

Steps:

open file

replace stdout

run command

Implementation uses:

open()
dup2()

Example logic:

fd = open(file)
dup2(fd, STDOUT_FILENO)

Now program output goes to file.

11. Pipes

A pipe connects output of one process to input of another.

Example:

ls | grep txt

Flow:

ls output → pipe → grep input

Pipe system call:

pipe(fd)

Creates two descriptors:

fd[0] → read
fd[1] → write

Pipeline architecture:

process1 → pipe → process2

Implementation:

dup2(pipefd[1], STDOUT)
dup2(pipefd[0], STDIN)
12. Multiple Pipes

Example:

ls | grep txt | sort

Pipeline structure:

ls → pipe1 → grep → pipe2 → sort

Shell must create multiple pipes and connect processes.

13. Built-in Commands

Some commands must be executed inside the shell itself.

Example:

cd
exit
history
jobs

Why?

Example:

cd directory

If executed in child process:

child changes directory
parent shell remains unchanged

So builtins must run in the shell process.

14. Background Processes

Example:

sleep 10 &

Normally shell waits:

waitpid()

But background jobs:

shell does NOT wait

So user can run more commands.

15. Job Control

A job is a process running in background.

Your shell stores jobs in a list:

PID
command
status

Commands:

jobs
fg

Example:

sleep 20 &
jobs
fg 1
16. Signals

Signals are notifications sent to processes.

Examples:

SIGINT  → Ctrl+C
SIGTERM → terminate
SIGKILL → force kill
SIGSTOP → pause
SIGCONT → continue

Your shell handles:

SIGINT

Child processes reset signal behavior:

signal(SIGINT, SIG_DFL)
17. Ctrl+C Handling

When user presses:

Ctrl + C

Terminal sends:

SIGINT

Behavior:

child process → terminated
shell → continues running
18. Command History

Shell stores previously executed commands.

Implementation:

vector<string> history

Example:

history

Output:

1 ls
2 pwd
3 echo hello
19. Command Parsing

User input:

ls | grep txt | sort > file.txt

Shell must parse:

command tokens
pipes
redirection
background symbol

Parsing produces structure:

commands = [
    ["ls"],
    ["grep","txt"],
    ["sort"]
]
20. Environment Variables (Concept)

Not implemented but commonly asked.

Example:

echo $HOME

Shell replaces variable with value.

Implementation concept:

getenv("HOME")
21. PATH Resolution

When user runs:

ls

Shell searches directories in:

$PATH

Example:

/usr/bin
/bin

execvp automatically searches PATH.

22. Foreground vs Background Processes

Foreground:

shell waits

Background:

shell continues

Example:

sleep 20
sleep 20 &
23. Process Groups (Advanced Concept)

Real shells organize jobs using process groups.

Functions:

setpgid()
tcsetpgrp()

Used for job control and terminal signals.

You did not implement this, but interviewers may ask.

24. Logical Operators (Not Implemented)

Example:

make && ./program

Meaning:

run second command only if first succeeds

Operator:

&&
||
25. Subshells (Advanced)

Example:

(ls | grep txt)

Runs commands in a subshell.

Implemented using:

fork()
26. Difference Between Shell and Terminal

Terminal:

interface program

Shell:

command interpreter

Examples:

Terminal:

gnome-terminal
xterm

Shell:

bash
zsh
27. System Calls Used in This Project

Important system calls used in your shell:

fork()
execvp()
waitpid()
pipe()
dup2()
open()
close()
signal()

These are core Unix process management APIs.

28. Why This Project is Important

This project demonstrates knowledge of:

Operating Systems
Process management
Inter-process communication
Signal handling
System calls
Unix architecture

These are important topics in systems programming interviews.

Final Interview Tip

If interviewer asks:

“Explain your shell project.”

You should answer like this:

I implemented a Unix-like shell in C++ to understand process management and inter-process communication. The shell reads commands, parses them, and executes programs using fork and exec. It supports pipelines, output redirection, background processes, job control, built-in commands, and signal handling. The project uses Linux system calls such as fork, execvp, pipe, dup2, and waitpid to manage processes and communication between them.

That answer alone shows strong OS understanding.

If you want, I can also give you a list of the 25 most common interview questions specifically asked about shell projects and Unix process management.