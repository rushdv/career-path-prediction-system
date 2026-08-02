#include "auth.h"
#include "ui.h"

int main(void) {
    Session s = login();

    if (s.role == 0) {
        showAdminMenu();
    } else {
        showStudentMenu(&s);
    }

    return 0;
}
