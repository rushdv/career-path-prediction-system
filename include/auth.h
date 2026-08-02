#ifndef AUTH_H
#define AUTH_H

#define ADMIN_PASSWORD "admin123"

typedef struct {
    int  role;           /* 0 = Admin, 1 = Student */
    char studentID[20];  /* string ID e.g. "CSE-001" */
    int  studentNumId;   /* numeric PK from Student.id — used for file lookups */
} Session;

Session login(void);

#endif