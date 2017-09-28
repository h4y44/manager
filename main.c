
#include "common.h"
#include "session.h"
#include "util.h"

int main(int argc, char *argv[]) {
	if (argc == 2) {
		if (strcmp(argv[1], "reset") == 0) {
			if(confirm("Are you sure to remove all the old data and create new")) {
				puts("Creating new data ... Done!");
				init_data();
				return 0;
			}
			else {
				puts("OK everything is still ok");
				return 0;
			}
		}
		else if (strcmp(argv[1], "help") == 0) {
			print_help(argv[0]);
			return 0;
		}
		else if (strcmp(argv[1], "version") == 0) {
			print_version();
			return 0;
		}
	}

	printf("Employees manager v%s\n", VERSION);
	session_t *s = session_init();

	if (!s) {
		puts("Error creating new session");
		return 1;
	}
	while (s->state != SS_LOGGED_IN) {
		session_login(s);
		//check if login ok or failed
		if (s->state == SS_LOGGED_IN)
			session_start(s);
	}
	return 0;
}
