#ifndef REPORT_H
#define REPORT_H

#include "student.h"
#include "assessment.h"
#include "prediction.h"

void generateReport(Student *s, SkillProfile *sp,
                    CareerPath ranked[], float scores[]);

#endif