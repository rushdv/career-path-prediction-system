#ifndef UI_H
#define UI_H

#include "auth.h"

void showAdminMenu(void);
void showStudentMenu(void);
void handleChoice(int choice, Session *session);

#endif