#pragma once
#include <vector>
#include <string>
#include <unistd.h>

struct Job {
    int id;
    pid_t pid;
    std::string command;
};

void add_job(pid_t pid, std::string cmd);
void list_jobs();
bool bring_fg(int id);