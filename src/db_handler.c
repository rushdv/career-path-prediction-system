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
