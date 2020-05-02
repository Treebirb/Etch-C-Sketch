#include "args.h"
#include <stdlib.h>
#include <string.h>

char* findicon(const char* fpath, const char* program)
{
  char* exe = strstr(fpath, program);

  if (!exe)
    return NULL;

  int len = strlen(fpath) - strlen(exe) + strlen("./icon.bmp") + 1;

  char* path = calloc(len, sizeof(char));

  if (!path)
    return NULL;

  path = strncpy(path, fpath, strlen(fpath) - strlen(exe));

  path = strcat(path, "./icon.bmp");

  return path;
}
