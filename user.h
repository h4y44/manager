#ifndef __USER_H__
#define __USER_H__

#include "common.h"
#include "database.h"

enum user_type {
	U_ADMIN,
	U_EMPLOYEE
};

enum user_err {
	U_DOES_NOT_EXIST,
	U_WRONG_PASSWORD,
	U_NO_DATA
};

int user_change_password(entry_user_t *u, const char *new_password);
int user_change_info(entry_info_t *u, entry_info_t *ei);

#endif
