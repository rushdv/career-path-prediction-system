#ifndef REPORT_H
#define REPORT_H

#include "student.h"
#include "assessment.h"
#include "prediction.h"

void generateReport(const Student *st, const SkillProfile *sp, CareerPath *cars, float *scores);

#endif