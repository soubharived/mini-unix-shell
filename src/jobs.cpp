#include "../include/jobs.h"
#include <iostream>
#include <sys/wait.h>

using namespace std;

vector<Job> jobs;
int next_job_id = 1;

void add_job(pid_t pid, string cmd)
{
    Job job;
    job.id = next_job_id++;
    job.pid = pid;
    job.command = cmd;

    jobs.push_back(job);
}

void list_jobs()
{
    for (auto &job : jobs)
    {
        cout << "[" << job.id << "] Running " << job.command << endl;
    }
}

bool bring_fg(int id)
{
    for (int i = 0; i < jobs.size(); i++)
    {
        if (jobs[i].id == id)
        {
            pid_t pid = jobs[i].pid;

            waitpid(pid, NULL, 0);

            jobs.erase(jobs.begin() + i);
            return true;
        }
    }

    cout << "job not found\n";
    return false;
}