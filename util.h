#ifndef __UTIL_H__
#define __UTIL_H__

#include "common.h"
#include "database.h"
#include "session.h"
#include "version.h"

//change this to change behaviour
#define DEFAULT_YES 1
#define C_MESSAGE "[Y/n]?"

size_t flen(FILE *f);
int confirm(const char *message);
int match_password(const char *password, entry_user_t *eu);
void print_help(const char *name);
void print_version();
#endif
