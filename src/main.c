#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "auth.h"
#include "ui.h"
#include "colors.h"
#include "db_handler.h"
#include "input_handler.h"

/* ── Clean exit on Ctrl-C ─────────────────────────────── */
static void sigintHandler(int dummy) {
    (void)dummy;
    restoreTerminal();
    db_close();
    printf("\n\n" UI_PAD "%s[SYSTEM] Terminated by user. Cleanup successful.%s\n\n",
           C_DIM, RESET);
    exit(0);
}

int main(void) {
    /* 1. Detect terminal capabilities (colour + UTF-8) */
    term_init();

    /* 2. Register signal handler and atexit for clean exit */
    signal(SIGINT, sigintHandler);
    atexit(db_close);

    /* 3. Open / create database */
    db_init();

    /* 4. Application Main Loop */
    while (1) {
        /* Authenticate */
        Session s = login();

        /* Route to the correct menu */
        if (s.role == 0)
            adminMenu(s);
        else
            studentMenu(s);
    }

    return 0;
}
