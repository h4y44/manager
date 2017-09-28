/*
noob databse :<
using linked list to save
*/
#include "database.h"
#include "util.h"

static int db_err = 0;
#ifdef __THREAD_SAFE__
static pthread_mutex_t db_lock;
#endif

entry_user_t *db_load_user(const char *dbfile) {
	FILE *fp = fopen(dbfile, "rb");
	if (!fp) {
		db_err = DB_NO_SUCH_FILE;
		return NULL;
	}
	
	size_t len =  flen(fp);
	void *db_buffer = malloc(len);
	
	if (!db_buffer) {
		db_err = DB_NOMEM;
		fclose(fp);
		return NULL;
	}
#ifdef __DEBUG__
	P_DEBUG("Reading from %s\n", dbfile);
#endif
	fread(db_buffer, 1, len, fp);
	fclose(fp);
	
	/*
	 * loop over the db and recalculate the offset of next pointer
	 */
#ifdef __DEBUG__ 
	P_DEBUG("Starting recalculating next pointers at %p\n", db_buffer);
	int count = 0;
#endif
	entry_user_t *ptr = db_buffer;
	while ((void *)ptr < db_buffer + len) {
		ptr->next = (void *)ptr + sizeof(entry_user_t);
		++ptr;
#ifdef __DEBUG__
		++count;
#endif
	}
	//the last element
	(ptr-1)->next = NULL;

#ifdef __DEBUG__
	P_DEBUG("Done recalculating next pointer within %d element(s)\n", count);
#endif
	return (entry_user_t *)db_buffer;
}

entry_info_t *db_load_info(const char *username) {
	char *dbfile = malloc(strlen(username) + 5);
	strcpy(dbfile, username);
	strcat(dbfile, ".dat");
	
#ifdef __DEBUG__
	P_DEBUG("Reading %s\n", dbfile);
#endif
	FILE *fp = fopen(dbfile, "rb");
	if (!fp) {
		db_err = DB_NO_SUCH_FILE;
		free(dbfile);
		return NULL;
	}

	size_t len = flen(fp);
	void *db_buffer = malloc(len);
	
	if (!db_buffer) {
		db_err = DB_NOMEM;
		free(dbfile);
		fclose(fp);
		return NULL;
	}
	fread(db_buffer, 1, len, fp);
	fclose(fp);
	
	return (entry_info_t *)db_buffer;
}

int db_save_user(const entry_user_t *data, const char *dbfile) {
#ifdef __THREAD_SAFE__
	pthread_mutex_lock(&db_lock);
#endif
	const entry_user_t *ptr = data;
	FILE *fp = fopen(dbfile, "wb+");
	if (!fp) {
		db_err = DB_ERR_WRITING;
#ifdef __THREAD_SAFE__
		pthread_mutex_unlock(&db_lock);
#endif
		return -1;
	}
	while (ptr->next != NULL) {
		fwrite((void *)ptr, sizeof(entry_user_t), 1, fp);
		ptr = ptr->next;
	}
	//the last element
	fwrite((void *)ptr, sizeof(entry_user_t), 1, fp);
	fclose(fp);
#ifdef __THREAD_SAFE__
	pthread_mutex_unlock(&db_lock);
#endif	
	return 0;
}

int db_save_info(const entry_info_t *data, const char *dbfile) {
#ifdef __THREAD_SAFE__
	pthread_mutex_lock(&db_lock);
#endif
	FILE *fp = fopen(dbfile, "wb+");
	
	if (!fp) {
		fclose(fp);
#ifdef __THREAD_SAFE__
		pthread_mutex_unlock(&db_lock);
#endif
		return -1;
	}
#ifdef __DEBUG__ 
	P_DEBUG("Writing data into \"%s\"\n", dbfile);
#endif
	fwrite((void *)data, sizeof(entry_info_t), 1, fp);
	fclose(fp);
#ifdef __THREAD_SAFE__
	pthread_mutex_unlock(&db_lock);
#endif
	return 0;
}
/*
 * returns a pointer if any
 * return NULL if couldn't find any
 */
entry_user_t *db_find_user(entry_user_t *db, const char *username) {
	entry_user_t *ptr = db;

#ifdef __DEBUG__
	P_DEBUG("Finding \"%s\" in %p\n", username, db);
#endif
	do {
		if (strcmp(username, ptr->username) == 0) {
#ifdef __DEBUG__
			P_DEBUG("Found \"%s\" at %p\n", username, ptr);
#endif
			return ptr;
		}
		ptr = ptr->next;
	} while (ptr != NULL);
#ifdef __DEBUG__
	P_DEBUG("User \"%s\" doesn't exist!\n", username);
#endif
	db_err = DB_NO_SUCH_USER;
	return NULL;
}

int db_add_user(entry_user_t *db, entry_user_t *new) {
	//add to the end of the list
	entry_user_t *ptr = db;
#ifdef __DEBUG__
	int count =0;
#endif
	while (ptr->next != NULL) {
#ifdef __DEBUG__ 
		++count;
#endif
		ptr = ptr->next;
	}
#ifdef __DEBUG__ 
	P_DEBUG("Done seeking within %d round(s)\n", count);
#endif

	ptr->next = new;
	new->next = NULL;

#ifdef __DEBUG__
	P_DEBUG("Creating new user at %p\n", new);
#endif
	return 0;
}

int db_remove_user(entry_user_t *db, const char *username) {
	entry_user_t *ptr = db;

	//haven't done yet
	do {
		if (strcmp(ptr->username, username) == 0) {
			ptr = ptr->next;
		}
	} while (ptr);

	if (!ptr) {
#ifdef __DEBUG__
		P_DEBUG("User %s doesn't exist in the database\n", username);
#endif
		return -1;
	}
#ifdef __DEBUG__ 
	P_DEBUG("User %s found, removing it from the linked list\n", username);
#endif
	return 0;
}

int db_add_info(const char *username, entry_info_t *new) {
	//construct name of the file
	char *fname = malloc(strlen(username) + 5);
	strcpy(fname, username);
	strcat(fname, ".dat");
	FILE *fp = fopen(fname, "wb+");

	free(fname);
	if (!fp) {
		return -1;
	}
	
	fwrite(fp, sizeof(entry_info_t), 1, fp);
#ifdef __DEBUG__ 
	P_DEBUG("Wrote to %s\n", fname);
#endif
	fclose(fp);

	return 1;
}

void db_list_all(entry_user_t *data) {
	entry_user_t *ptr = data;
#ifdef __DEBUG__
	P_DEBUG("Looping over database from %p\n", ptr);
#endif
	do {
		printf("Name: %s\n----\n", ptr->username);
		ptr = ptr->next;
	} while (ptr != NULL);
}

int init_data() {
	entry_user_t u;
#ifdef __DEBUG__ 
	P_DEBUG("%s", "Opening file to write new data\n");
#endif
	FILE *fe = fopen("employees.dat", "wb+");
	FILE *fa = fopen("admin.dat", "wb+");
	
	assert(fa);
	assert(fe);
	
	//shrugs

	strcpy(u.username, "admin");
	strcpy(u.password, "admin");
	u.next = NULL;
	db_save_user(&u, "admin.dat");
#ifdef __DEBUG__
	P_DEBUG("Default admin account is %s with password \"%s\"\n", "admin", "admin");
#endif

	strcpy(u.username, "user0");
	strcpy(u.password, "user0");
	db_save_user(&u, "employees.dat");
#ifdef __DEBUG__ 
	P_DEBUG("Default user account is %s with password \"%s\"\n", "user0", "user0");
#endif

	goto quit;

quit:
	fclose(fa);
	fclose(fe);
	return -1;
}
