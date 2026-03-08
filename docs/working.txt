

When a user types a command, the shell must:

1 read command
2 create new process
3 run program
4 wait for program to finish

In Linux this uses:

fork()
execvp()
wait()

Execution will look like:

shell process
     │
     ▼
   fork()
     │
 ┌───┴─────────┐
 │             │
parent       child
(shell)      run command



Step 1 — User types a command

Example:

cwushell> ls

The shell reads the string "ls".

Step 2 — Shell creates a new process

The shell cannot run the program inside itself.

Why?

Because the shell must stay alive.

Instead it creates a child process.

This is done using:

fork()

Think of fork() as copying the current process.

Example:

Shell Process
      │
      ▼
    fork()
      │
 ┌────┴─────┐
 │          │
Parent     Child
(shell)    (copy)


Now there are two processes.

Process 	Role
Parent	    the shell
Child	    will run the command

Step 3 — Child process runs the program

Inside the child process we call:

execvp()                  //exactly  which runs our command inside child process

This replaces the child process with the program we want to run.

After execvp():

child process = ls program

The original shell code inside that child completely disappears.

Example:

Before:

Child process running shell code

After execvp("ls"):

Child process becomes the ls program

So the child is now executing the real ls command.

Step 4 — Parent waits for the child

The shell must wait until the command finishes.

Otherwise you would see:

cwushell> ls
cwushell> cwushell>

before the output appears.

To prevent that, the parent shell calls:

wait()

This pauses the shell until the child process finishes.








//----------- Final Mental Model -------------//

Your shell will eventually behave like this:

cwushell>
      │
      ▼
read command
      │
      ▼
parse command
      │
      ▼
create process
      │
      ▼
execute program
      │
      ▼
wait for completion
      │
      ▼
show prompt again

This is exactly how real shells work.