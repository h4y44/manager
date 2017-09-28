#include "user.h"

int user_change_password(entry_user_t *u, const char *new_password) {
#ifdef __DEBUG__
	P_DEBUG("Changing password of user \"%s\"\n", u->username);
#endif
	memset(u->password, 0x00, sizeof(u->password));
	strcpy(u->password, new_password);
	return 0;
} 

int user_change_info(entry_info_t *u, entry_info_t *ei) {
#ifdef __DEBUG__
	P_DEBUG("Changing info of user \"%s\"\n", u->fullname);
#endif
	strcpy(u->address, ei->address);
	u->age = ei->age;
	strcpy(u->email, ei->email);
	strcpy(u->fullname, ei->fullname);
	strcpy(u->phone, ei->phone);
	return 0;
}
