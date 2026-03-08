#ifndef BUILTINS_H
#define BUILTINS_H

#include <vector>
#include <string>

bool run_builtin(std::vector<std::string> &args);
void add_history(const std::string &cmd);
void show_history();
void list_jobs();
bool bring_fg(int id);

#endif