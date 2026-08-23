#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "db_handler.h"
#include "colors.h"

static sqlite3 *db = NULL;

void db_init(void) {
    if (sqlite3_open("data/career_system.db", &db) != SQLITE_OK) {
        printError("Failed to open database.");
        exit(1);
    }

    const char *schema = 
        "CREATE TABLE IF NOT EXISTS Students ("
        " id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " name TEXT,"
        " studentID TEXT UNIQUE,"
        " password TEXT,"
        " cgpa REAL,"
        " department TEXT,"
        " isActive INTEGER);"
        
        "CREATE TABLE IF NOT EXISTS SkillProfiles ("
        " studentRef INTEGER PRIMARY KEY,"
        " level_programming INTEGER, level_networking INTEGER, level_design INTEGER,"
        " level_analytics INTEGER, level_communication INTEGER, level_security INTEGER,"
        " programming REAL, networking REAL, design REAL,"
        " analytics REAL, communication REAL, security REAL,"
        " course_ml_ai INTEGER, course_web_dev INTEGER, course_data_science INTEGER,"
        " course_cybersecurity INTEGER, course_cloud_computing INTEGER,"
        " lang_python INTEGER, lang_java INTEGER, lang_c_cpp INTEGER, lang_js INTEGER,"
        " lang_php INTEGER, lang_go INTEGER, lang_kotlin INTEGER, lang_sql INTEGER,"
        " tool_git INTEGER, tool_docker INTEGER, tool_linux INTEGER, tool_cloud INTEGER,"
        " tool_react_vue INTEGER, tool_tf_pytorch INTEGER, tool_figma INTEGER, tool_mongodb INTEGER,"
        " proj_web INTEGER, proj_mobile INTEGER, proj_db INTEGER, proj_ai INTEGER, proj_security INTEGER,"
        " exp_internship INTEGER, exp_hackathon INTEGER, exp_competitive_prog INTEGER, exp_research INTEGER,"
        " career_interest INTEGER,"
        " FOREIGN KEY(studentRef) REFERENCES Students(id));"
        
        "CREATE TABLE IF NOT EXISTS Predictions ("
        " id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " studentRef INTEGER,"
        " topCareer TEXT,"
        " score REAL,"
        " timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,"
        " FOREIGN KEY(studentRef) REFERENCES Students(id));";

    char *errMsg = NULL;
    if (sqlite3_exec(db, schema, NULL, NULL, &errMsg) != SQLITE_OK) {
        printf("SQL Error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
}

void db_close(void) {
    if (db) sqlite3_close(db);
}

int db_insert_student(const Student *s) {
    const char *sql = "INSERT INTO Students (name, studentID, password, cgpa, department, isActive) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return 0;
    
    sqlite3_bind_text(stmt, 1, s->name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, s->studentID, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, s->password, -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 4, s->cgpa);
    sqlite3_bind_text(stmt, 5, s->department, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 6, s->isActive);
    
    int result = (sqlite3_step(stmt) == SQLITE_DONE) ? 1 : 0;
    sqlite3_finalize(stmt);
    return result;
}

int db_update_student(const Student *s) {
    const char *sql = "UPDATE Students SET name=?, password=?, cgpa=?, department=?, isActive=? WHERE studentID=?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return 0;
    
    sqlite3_bind_text(stmt, 1, s->name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, s->password, -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, s->cgpa);
    sqlite3_bind_text(stmt, 4, s->department, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, s->isActive);
    sqlite3_bind_text(stmt, 6, s->studentID, -1, SQLITE_TRANSIENT);
    
    int result = (sqlite3_step(stmt) == SQLITE_DONE) ? 1 : 0;
    sqlite3_finalize(stmt);
    return result;
}

int db_delete_student(const char *studentID) {
    const char *sql = "UPDATE Students SET isActive=0 WHERE studentID=?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return 0;
    
    sqlite3_bind_text(stmt, 1, studentID, -1, SQLITE_TRANSIENT);
    int result = (sqlite3_step(stmt) == SQLITE_DONE) ? 1 : 0;
    sqlite3_finalize(stmt);
    return result;
}

static void fill_student_from_stmt(sqlite3_stmt *stmt, Student *s) {
    s->id = sqlite3_column_int(stmt, 0);
    strcpy(s->name, (const char *)sqlite3_column_text(stmt, 1));
    strcpy(s->studentID, (const char *)sqlite3_column_text(stmt, 2));
    strcpy(s->password, (const char *)sqlite3_column_text(stmt, 3));
    s->cgpa = sqlite3_column_double(stmt, 4);
    strcpy(s->department, (const char *)sqlite3_column_text(stmt, 5));
    s->isActive = sqlite3_column_int(stmt, 6);
}

int db_get_student_by_id(const char *studentID, Student *s) {
    const char *sql = "SELECT id, name, studentID, password, cgpa, department, isActive FROM Students WHERE studentID=?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return 0;
    
    sqlite3_bind_text(stmt, 1, studentID, -1, SQLITE_TRANSIENT);
    int found = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        fill_student_from_stmt(stmt, s);
        found = 1;
    }
    sqlite3_finalize(stmt);
    return found;
}

int db_get_student_by_num(int id, Student *s) {
    const char *sql = "SELECT id, name, studentID, password, cgpa, department, isActive FROM Students WHERE id=?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return 0;
    
    sqlite3_bind_int(stmt, 1, id);
    int found = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        fill_student_from_stmt(stmt, s);
        found = 1;
    }
    sqlite3_finalize(stmt);
    return found;
}

void db_list_all_students(void) {
    const char *sql = "SELECT id, name, studentID, cgpa, department FROM Students WHERE isActive=1;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return;
    
    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *name = (const char *)sqlite3_column_text(stmt, 1);
        const char *sid = (const char *)sqlite3_column_text(stmt, 2);
        double cgpa = sqlite3_column_double(stmt, 3);
        const char *dept = (const char *)sqlite3_column_text(stmt, 4);
        
        printf("%s" UI_PAD "%s%-4d%s  %-24.24s  %s%-12.12s%s  %s%-6.2f%s  %-14.14s\n",
               CC(RESET),
               CC(C_VALUE), id, CC(RESET),
               name,
               CC(C_ACCENT), sid,  CC(RESET),
               CC(C_INFO),   cgpa, CC(RESET),
               dept ? dept : "");
        count++;

        if (count % 10 == 0) {
            printf(UI_PAD "%s", CC(C_DIM));
            for (int j = 0; j < 68; j++) fputs(BOX_H, stdout);
            printf("%s\n", CC(RESET));
            printf(UI_PAD "%sPress Enter to continue or 'q' to quit...%s",
                   CC(C_INFO), CC(RESET));

            char c = (char)getchar();
            if (c != '\n') while (getchar() != '\n');
            if (c == 'q' || c == 'Q') break;

            printf("\n");
            printf(UI_PAD "%s%s%-4s  %-24s  %-12s  %-6s  %-14s%s\n",
                   CC(C_PURPLE), CC(BOLD),
                   "ID", "Name", "Student ID", "CGPA", "Department",
                   CC(RESET));
            printf(UI_PAD "%s", CC(C_DIM));
            for (int j = 0; j < 68; j++) fputs(BOX_H, stdout);
            printf("%s\n", CC(RESET));
        }
    }
    sqlite3_finalize(stmt);

    printf(UI_PAD "%s", CC(C_DIM));
    for (int j = 0; j < 68; j++) fputs(BOX_H, stdout);
    printf("%s\n", CC(RESET));

    if (count == 0) {
        printWarning("No active students found.");
    } else {
        printf(UI_PAD "%sTotal active students: %s%s%d%s\n\n",
               CC(C_DIM), CC(RESET), CC(C_VALUE), count, CC(RESET));
    }
}

void db_search_students(const char *name_query) {
    const char *sql = "SELECT id, name, studentID, password, cgpa, department, isActive FROM Students WHERE isActive=1 AND name LIKE ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return;
    
    char like_query[100];
    snprintf(like_query, sizeof(like_query), "%%%s%%", name_query);
    sqlite3_bind_text(stmt, 1, like_query, -1, SQLITE_TRANSIENT);
    
    int found = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Student s;
        fill_student_from_stmt(stmt, &s);
        
        char idStr[12], cgpaStr[10];
        snprintf(idStr, sizeof(idStr), "%d", s.id);
        snprintf(cgpaStr, sizeof(cgpaStr), "%.2f", s.cgpa);
        
        boxTop();
        printKV("Name", s.name);
        printKV("Student ID", s.studentID);
        printKV("Unique ID", idStr);
        printKV("CGPA", cgpaStr);
        printKV("Department", s.department);
        boxBottom();
        printf("\n");
        found = 1;
    }
    sqlite3_finalize(stmt);
    
    if (!found) {
        printWarning("No students found matching that name.");
    }
}

int db_get_active_student_count(void) {
    int count = 0;
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM Students WHERE isActive=1;", -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) count = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
    }
    return count;
}

float db_get_average_cgpa(void) {
    float avg = 0.0f;
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, "SELECT AVG(cgpa) FROM Students WHERE isActive=1;", -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) avg = sqlite3_column_double(stmt, 0);
        sqlite3_finalize(stmt);
    }
    return avg;
}

void db_print_dept_stats(void) {
    const char *sql = "SELECT department, COUNT(*), AVG(cgpa) FROM Students WHERE isActive=1 GROUP BY department ORDER BY AVG(cgpa) DESC;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return;
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *dept = (const char *)sqlite3_column_text(stmt, 0);
        double avg = sqlite3_column_double(stmt, 2);
        
        printf(UI_PAD "%s%-24.24s%s | ", CC(C_INFO), dept ? dept : "Unknown", CC(RESET));
        int bar_len = (int)((avg / 4.0f) * 30.0f);
        if (bar_len > 0) {
            printf("%s", CC(C_SUCCESS));
            for (int b = 0; b < bar_len; b++) fputs(SYM_BAR_F, stdout);
            printf("%s", CC(RESET));
        }
        printf(" %s%.2f%s\n", CC(C_VALUE), avg, CC(RESET));
    }
    sqlite3_finalize(stmt);
}

int db_save_skill_profile(const SkillProfile *sp) {
    const char *sql = "REPLACE INTO SkillProfiles (studentRef, level_programming, level_networking, level_design, level_analytics, level_communication, level_security, programming, networking, design, analytics, communication, security, course_ml_ai, course_web_dev, course_data_science, course_cybersecurity, course_cloud_computing, lang_python, lang_java, lang_c_cpp, lang_js, lang_php, lang_go, lang_kotlin, lang_sql, tool_git, tool_docker, tool_linux, tool_cloud, tool_react_vue, tool_tf_pytorch, tool_figma, tool_mongodb, proj_web, proj_mobile, proj_db, proj_ai, proj_security, exp_internship, exp_hackathon, exp_competitive_prog, exp_research, career_interest) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return 0;
    
    sqlite3_bind_int(stmt, 1, sp->studentRef);
    sqlite3_bind_int(stmt, 2, sp->level_programming);
    sqlite3_bind_int(stmt, 3, sp->level_networking);
    sqlite3_bind_int(stmt, 4, sp->level_design);
    sqlite3_bind_int(stmt, 5, sp->level_analytics);
    sqlite3_bind_int(stmt, 6, sp->level_communication);
    sqlite3_bind_int(stmt, 7, sp->level_security);
    
    sqlite3_bind_double(stmt, 8, sp->programming);
    sqlite3_bind_double(stmt, 9, sp->networking);
    sqlite3_bind_double(stmt, 10, sp->design);
    sqlite3_bind_double(stmt, 11, sp->analytics);
    sqlite3_bind_double(stmt, 12, sp->communication);
    sqlite3_bind_double(stmt, 13, sp->security);
    
    sqlite3_bind_int(stmt, 14, sp->course_ml_ai);
    sqlite3_bind_int(stmt, 15, sp->course_web_dev);
    sqlite3_bind_int(stmt, 16, sp->course_data_science);
    sqlite3_bind_int(stmt, 17, sp->course_cybersecurity);
    sqlite3_bind_int(stmt, 18, sp->course_cloud_computing);
    
    sqlite3_bind_int(stmt, 19, sp->lang_python);
    sqlite3_bind_int(stmt, 20, sp->lang_java);
    sqlite3_bind_int(stmt, 21, sp->lang_c_cpp);
    sqlite3_bind_int(stmt, 22, sp->lang_js);
    sqlite3_bind_int(stmt, 23, sp->lang_php);
    sqlite3_bind_int(stmt, 24, sp->lang_go);
    sqlite3_bind_int(stmt, 25, sp->lang_kotlin);
    sqlite3_bind_int(stmt, 26, sp->lang_sql);
    
    sqlite3_bind_int(stmt, 27, sp->tool_git);
    sqlite3_bind_int(stmt, 28, sp->tool_docker);
    sqlite3_bind_int(stmt, 29, sp->tool_linux);
    sqlite3_bind_int(stmt, 30, sp->tool_cloud);
    sqlite3_bind_int(stmt, 31, sp->tool_react_vue);
    sqlite3_bind_int(stmt, 32, sp->tool_tf_pytorch);
    sqlite3_bind_int(stmt, 33, sp->tool_figma);
    sqlite3_bind_int(stmt, 34, sp->tool_mongodb);
    
    sqlite3_bind_int(stmt, 35, sp->proj_web);
    sqlite3_bind_int(stmt, 36, sp->proj_mobile);
    sqlite3_bind_int(stmt, 37, sp->proj_db);
    sqlite3_bind_int(stmt, 38, sp->proj_ai);
    sqlite3_bind_int(stmt, 39, sp->proj_security);
    
    sqlite3_bind_int(stmt, 40, sp->exp_internship);
    sqlite3_bind_int(stmt, 41, sp->exp_hackathon);
    sqlite3_bind_int(stmt, 42, sp->exp_competitive_prog);
    sqlite3_bind_int(stmt, 43, sp->exp_research);
    
    sqlite3_bind_int(stmt, 44, sp->career_interest);
    
    int res = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return res;
}

int db_load_skill_profile(int studentRef, SkillProfile *sp) {
    const char *sql = "SELECT * FROM SkillProfiles WHERE studentRef=?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return 0;
    
    sqlite3_bind_int(stmt, 1, studentRef);
    int found = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        memset(sp, 0, sizeof(SkillProfile));
        sp->studentRef = sqlite3_column_int(stmt, 0);
        
        sp->level_programming = sqlite3_column_int(stmt, 1);
        sp->level_networking = sqlite3_column_int(stmt, 2);
        sp->level_design = sqlite3_column_int(stmt, 3);
        sp->level_analytics = sqlite3_column_int(stmt, 4);
        sp->level_communication = sqlite3_column_int(stmt, 5);
        sp->level_security = sqlite3_column_int(stmt, 6);
        
        sp->programming = sqlite3_column_double(stmt, 7);
        sp->networking = sqlite3_column_double(stmt, 8);
        sp->design = sqlite3_column_double(stmt, 9);
        sp->analytics = sqlite3_column_double(stmt, 10);
        sp->communication = sqlite3_column_double(stmt, 11);
        sp->security = sqlite3_column_double(stmt, 12);
        
        sp->course_ml_ai = sqlite3_column_int(stmt, 13);
        sp->course_web_dev = sqlite3_column_int(stmt, 14);
        sp->course_data_science = sqlite3_column_int(stmt, 15);
        sp->course_cybersecurity = sqlite3_column_int(stmt, 16);
        sp->course_cloud_computing = sqlite3_column_int(stmt, 17);
        
        sp->lang_python = sqlite3_column_int(stmt, 18);
        sp->lang_java = sqlite3_column_int(stmt, 19);
        sp->lang_c_cpp = sqlite3_column_int(stmt, 20);
        sp->lang_js = sqlite3_column_int(stmt, 21);
        sp->lang_php = sqlite3_column_int(stmt, 22);
        sp->lang_go = sqlite3_column_int(stmt, 23);
        sp->lang_kotlin = sqlite3_column_int(stmt, 24);
        sp->lang_sql = sqlite3_column_int(stmt, 25);
        
        sp->tool_git = sqlite3_column_int(stmt, 26);
        sp->tool_docker = sqlite3_column_int(stmt, 27);
        sp->tool_linux = sqlite3_column_int(stmt, 28);
        sp->tool_cloud = sqlite3_column_int(stmt, 29);
        sp->tool_react_vue = sqlite3_column_int(stmt, 30);
        sp->tool_tf_pytorch = sqlite3_column_int(stmt, 31);
        sp->tool_figma = sqlite3_column_int(stmt, 32);
        sp->tool_mongodb = sqlite3_column_int(stmt, 33);
        
        sp->proj_web = sqlite3_column_int(stmt, 34);
        sp->proj_mobile = sqlite3_column_int(stmt, 35);
        sp->proj_db = sqlite3_column_int(stmt, 36);
        sp->proj_ai = sqlite3_column_int(stmt, 37);
        sp->proj_security = sqlite3_column_int(stmt, 38);
        
        sp->exp_internship = sqlite3_column_int(stmt, 39);
        sp->exp_hackathon = sqlite3_column_int(stmt, 40);
        sp->exp_competitive_prog = sqlite3_column_int(stmt, 41);
        sp->exp_research = sqlite3_column_int(stmt, 42);
        
        sp->career_interest = sqlite3_column_int(stmt, 43);
        found = 1;
    }
    sqlite3_finalize(stmt);
    return found;
}

int db_get_last_prediction(int studentRef, PredictionRecord *pr) {
    const char *sql = "SELECT studentRef, topCareer, score FROM Predictions "
                      "WHERE studentRef = ? ORDER BY id DESC LIMIT 1;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return 0;
    sqlite3_bind_int(stmt, 1, studentRef);
    int found = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        pr->studentRef = sqlite3_column_int(stmt, 0);
        const char *career = (const char *)sqlite3_column_text(stmt, 1);
        if (career) strncpy(pr->topCareer, career, sizeof(pr->topCareer) - 1);
        pr->topCareer[sizeof(pr->topCareer) - 1] = '\0';
        pr->score = (float)sqlite3_column_double(stmt, 2);
        found = 1;
    }
    sqlite3_finalize(stmt);
    return found;
}
