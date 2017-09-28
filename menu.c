#include "session.h"

#define P_HEADER(s) \
	do {printf("---- Current user: %s ----\n", s->user->username); } while (0);

static int menu_admin(session_t *s) {
	switch (s->level) {
		case 0: //main menu
			{
				P_HEADER(s);
				puts("---- Admin menu ----");
				printf("1. List all employees\n2. Add an employee\n3. Delete an employee\
						\n4. Change my password\n5. Log out\nYour selection: ");
				fflush(stdin);
				scanf("%d", &s->level);
				break;
			}
		case 1: //list all
			{
				puts("---- All employees ----");
				db_list_all(s->db_e);

				s->level = 0;
				break;
			}

		case 2: //add an employee
			{
				entry_user_t *new = malloc(sizeof(entry_user_t));
				if (!new) {
					return -1;
				}
				puts("---- Add an employee ----");
				printf("username: ");
				scanf("%s", new->username);
				printf("password: ");
				scanf("%s", new->password);
#ifdef __DEBUG__
				P_DEBUG("%s", "Done reading, writing changes into database\n");
#endif
				db_add_user(s->db_e, new);
				db_save_user(s->db_e, "employees.dat");

				//reset menu level
				s->level = 0;
				break;
			}

		case 3: //delete an employee
			{
				char username[100];
				puts("---- Delete an employee ----");
				printf("username: ");
				scanf("%s", username);

#ifdef __DEBUG__
				P_DEBUG("Finding \"%s\" in database\n", username);
#endif
				db_remove_user(s->db_e, username);
				db_save_user(s->db_e, "employees.dat");

				s->level = 0;
				break;
			}

		case 4: //change admin password
			{
				char password[100];
				puts("---- Change my password ----");
				//to do: check old password
				printf("new password: ");
				scanf("%s", password);
#ifdef __DEBUG__ 
				P_DEBUG("Done reading, looking up for admin %s in database\n",
						s->user->username);
#endif


#ifdef __DEBUG__ 
				P_DEBUG("%s", "Writing changes into database\n");
#endif

				s->level = 0;
				break;
			}

		case 5: //log out
			{
				s->level = -1;
				return 0;
				break;
			}

		default:
			s->level = 0;
			break;
	}
	return 0;
}

static int menu_employee(session_t *s) {
	switch (s->level) {
		case 0:
			{
				P_HEADER(s);
				puts("---- Employee menu ----");
				printf("1. Change password\n2. Log out\nYour selection: ");
				fflush(stdin);
				scanf("%d", &s->level);
				break;
			}
		case 1: //change password
			{
				puts("---- Change your password ----");
				//todo: check for old password
				printf("new password: ");
				scanf("%s", s->user->password);
#ifdef __DEBUG__
				P_DEBUG("Looking for %s in database\n", s->user->username);
#endif
				
				//return to main menu
				s->level = 0;
				break;
			}

		case 2:
			{
				s->level = -1;
				return 0;
				break;
			}

		default:
			s->level = 0;
			return 0;
	}
	return 0;
}

void menu(session_t *s) {
	if (s->type == U_ADMIN) {
		while (s->level != -1) 
			menu_admin(s);
	}
	else if (s->type == U_EMPLOYEE) {
		while (s->level != -1) {
			menu_employee(s);
		}
	}
}
