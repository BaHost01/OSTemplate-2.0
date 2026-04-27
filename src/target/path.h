#ifndef PATH_H
#define PATH_H

#include <stddef.h>

void path_init();
void path_add_directory(const char* dir);
const char* path_resolve_command(const char* cmd);

#endif
