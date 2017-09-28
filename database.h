#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "common.h"

enum {
	DB_NOMEM,
	DB_NO_SUCH_FILE,
	DB_NO_SUCH_USER,
	DB_NO_SUCH_INFO,
	DB_ERR_WRITING
};

typedef struct entry_user_t {
	char username[100];
	char password[100];
	
	struct entry_user_t *next;
} entry_user_t;

typedef struct {
	char fullname[100];
	unsigned int age;
	char email[100], address[100], phone[100];
	int type;
} entry_info_t;

entry_user_t *db_load_user(const char *dbfile);
entry_info_t *db_load_info(const char *username);
int db_save_user(const entry_user_t *data, const char *dbfile);
int db_save_info(const entry_info_t *data, const char *dbfile);
entry_user_t *db_find_user(entry_user_t *db, const char *username);
int db_add_user(entry_user_t *db, entry_user_t *new);
int db_remove_user(entry_user_t *db, const char *username);
int db_add_info(const char *username, entry_info_t *new);
void db_list_all(entry_user_t *data);

int init_data();
#endif
