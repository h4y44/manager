#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef __THREAD_SAFE__
#include <pthread.h>
#endif

#define ADMIN_DAT "admin.dat"
#define EMPLOYEES_DAT "employees.dat"

#define P_DEBUG(fmt, ...) \
	do {fprintf(stderr, "[%s:%d:%s()]"fmt, __FILE__, \
			__LINE__, __func__, __VA_ARGS__);} while (0)

enum common_err {
	ERR_NOMEM,
	ERR_NO_DATA,
	ERR_UNEXPECTED
};

#endif
