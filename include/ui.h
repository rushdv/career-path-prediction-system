#ifndef UI_H
#define UI_H

#include "auth.h"
#include "assessment.h"

void showAdminMenu(void);
void showStudentMenu(Session *session);
void handleChoice(int choice, Session *session, SkillProfile *sp, int *hasAssessment);

#endif