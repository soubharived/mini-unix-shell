#include "../include/shell.h"
#include <signal.h>

int main()
{
    signal(SIGCHLD, SIG_IGN);
    start_shell();
}