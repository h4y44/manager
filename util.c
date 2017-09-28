/*
utils
*/
#include "util.h"

size_t flen(FILE *f) {
	fseek(f, 0, SEEK_END);
	size_t len = ftell(f);
	
	fseek(f, 0, SEEK_SET);
	return len;
}

int confirm(const char *message) {
	char *buffer = malloc(strlen(message) + strlen(C_MESSAGE) + 1);
	char c;
	if (!buffer) {
		return -1;
	}
	strcpy(buffer, message);
	strcat(buffer, C_MESSAGE);
	
	printf("%s ", buffer);
	free(buffer);
	scanf("%c", &c);
	
	if (c == 'n' || c == 'N') {
		return 0;
	}
	else {
		return 1;
	}
}

int match_password(const char *password, entry_user_t *eu) {
	return (strcmp(password, eu->password) == 0);
}

void print_help(const char *name) {
	printf("Usage: %s [option]\nWhere option is\n\t\
reset: to reset all database to default\n\t\
help: to print this help\n\t\
version: to print program version\n", name);
}

void print_version() {
	printf("Manager v%s\n", VERSION);
}
