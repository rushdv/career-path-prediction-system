#ifndef DB_HANDLER_H
#define DB_HANDLER_H

#include <sqlite3.h>
#include "student.h"
#include "assessment.h"
#include "prediction.h"

/* Initialize the database (creates tables if they don't exist) */
void db_init(void);

/* Clean up database resources */
void db_close(void);

/* -- Student Operations -- */
int db_insert_student(const Student *s);
int db_update_student(const Student *s);
int db_delete_student(const char *studentID);
int db_get_student_by_id(const char *studentID, Student *s);
int db_get_student_by_num(int id, Student *s);
void db_list_all_students(void); /* Prints the list directly */
void db_search_students(const char *name_query);

/* Analytics */
int db_get_active_student_count(void);
float db_get_average_cgpa(void);
void db_print_dept_stats(void);

/* -- Assessment Operations -- */
int db_save_skill_profile(const SkillProfile *sp);
int db_load_skill_profile(int studentRef, SkillProfile *sp);

/* -- Prediction Operations -- */
int db_save_prediction(const PredictionRecord *pr);
int db_get_last_prediction(int studentRef, PredictionRecord *pr);
void db_print_prediction_stats(void);
int db_get_total_predictions(void);

#endif
