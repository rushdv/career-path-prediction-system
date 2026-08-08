#include "auth.h"
#include "ui.h"

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
static void enableWindowsANSI(void) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    
    /* Set console encoding to UTF-8 to render box drawing characters properly */
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}
#endif

int main(void) {
#ifdef _WIN32
    enableWindowsANSI();
#endif

    Session s = login();

    if (s.role == 0) {
        showAdminMenu();
    } else {
        showStudentMenu(&s);
    }

    return 0;
}
