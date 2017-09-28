#ifndef __SESSION_H__
#define __SESSION_H__

#include "common.h"
#include "database.h"
#include "user.h"
#include "util.h"
#include "menu.h"

/*
session, saves a pointer to current user,
status, log, bla bla
level of screen
*/
enum session_state {
	SS_READY,
	SS_LOGGED_IN,
	SS_LOGGED_OUT,
	SS_ABORTED_OPT,
	SS_LOGIN_FAILED
};

typedef struct session_t {
	// database
	//
	entry_user_t *db_e; //db employees
	entry_user_t *db_a; //db admin
	
	//points to current logged in user in the db
	entry_user_t *user;
	entry_info_t *info;
	int level; //level of menu
	int state; //current state
	int type; // type of session: admin or employee
#ifdef __LOG__	
	char *log; // log of current session
#endif
	int id; //id of this session
} session_t;

session_t *session_init();
int session_login(session_t *s);
#ifdef __LOG__
int session_log(session_t *s, const char *message);
void session_print_log(session_t *s);
#endif
int session_start(session_t *s);
int session_logout(session_t *s);
void menu(session_t *s);

#endif
