#include "session.h"

#ifdef __LOG__
int session_log(session_t *s, const char *message) {
	size_t len = strlen(s->log);
	s->log = realloc(s->log, sizeof(message) +len + 2);
	
	if (!s->log) {
		return -1;
	}
	strcpy(s->log + len, message);
#ifdef __DEBUG__
	P_DEBUG("Done logging with new log size: %d\n", strlen(s->log));
#endif
	return 1;
}
#endif

session_t *session_init() {
	session_t *s = malloc(sizeof(session_t));

	if (!s) {
#ifdef __DEBUG__
		P_DEBUG("%s\n", "Error allocating memory");
#endif
		return NULL;
	}
	s->level = 0;
	s->state = SS_LOGGED_OUT;
	s->db_a = db_load_user("admin.dat");
	s->db_e = db_load_user("employees.dat");

	if (!s->db_a || !s->db_e) {
#ifdef __DEBUG__
		P_DEBUG("%s", "Error loading database\n");
#endif
		puts("Error loading database, trying to create a new one ...");
		init_data();
		return NULL;
	}
	
	s->user = malloc(sizeof(entry_user_t));
	if (!s->user) {
		free(s);
#ifdef __DEBUG__
		P_DEBUG("%s\n", "Error allocating memory");
#endif
		return NULL;
	}
	

#ifdef __DEBUG__
	P_DEBUG("Done initializing new session at %p\n", s);
#endif
	
	return s;
}

int session_login(session_t *s) {
	// print menu and get input
	puts("---- Log in ----");
	fflush(stdin);
	printf("Username: ");
	scanf("%s", s->user->username);
	printf("Password: ");
	scanf("%s", s->user->password);
#ifdef __DEBUG__
	P_DEBUG("Done reading username and password into %p\n", s);
#endif
	
	//check if username exists
	//admin?
	entry_user_t *u = db_find_user(s->db_a, s->user->username);
	//normal user (employee) ?
	if (!u) {
#ifdef __DEBUG__
		P_DEBUG("Not an admin, trying to find %s in employees database\n", 
				s->user->username);
#endif
		u = db_find_user(s->db_e, s->user->username);
		if (!u) {
			s->state = SS_LOGIN_FAILED;
			return SS_LOGIN_FAILED;
		}
		//check password on employees database
#ifdef __DEBUG__
		P_DEBUG("Checking password for \"%s\"\n", s->user->username);
#endif
		if (match_password(s->user->password, u)) {
#ifdef __DEBUG__
			P_DEBUG("User %s has logged in successfully\n", s->user->username);
#endif
			s->type = U_EMPLOYEE;
			s->state = SS_LOGGED_IN;
			
			return s->state;
		}
	}
	else {
		//check password on admin database
#ifdef __DEBUG__
		P_DEBUG("Checking password for admin \"%s\"\n", s->user->username);
#endif
		if (match_password(s->user->password, u)) {
#ifdef __DEBUG__
			P_DEBUG("User %s has logged in successfully\n", s->user->username);
#endif
			s->type = U_ADMIN;
			s->state = SS_LOGGED_IN;
			
			return s->state;
		}
	}
	// failed to log in
	s->state = SS_LOGIN_FAILED;
	puts("Login failed");
	return SS_LOGIN_FAILED;
}

#ifdef __LOG__
void session_print_log(session_t *s) {
	printf("--- Logs of session: %d ---\n", s->id);
	if (!s->log) {
		puts(s->log);
	}
	else {
		puts("[none]");
	}
}
#endif

/*
 * main loop for each session
 */
int session_start(session_t *s) {
	menu(s);
	return 0;
}

/*
log out from system and free memories
*/
int session_logout(session_t *s) {
	if (confirm("Are you sure to log out")) {
		free(s->db_a);
		free(s->db_e);
		s->state = SS_READY;
#ifdef __LOG__
		free(s->log);
#endif
		
		return SS_LOGGED_OUT;
	}
	return SS_ABORTED_OPT;
}
