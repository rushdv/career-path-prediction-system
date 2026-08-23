#include <stdio.h>
#include <string.h>
#include "analytics.h"
#include "db_handler.h"
#include "colors.h"

void showAdminDashboard(void) {
    int   total_students    = db_get_active_student_count();
    float avg_cgpa          = db_get_average_cgpa();
    int   total_predictions = db_get_total_predictions();

    printf("\n");
    printBanner("ANALYTICS DASHBOARD", 19);
    printf("\n");

    /* ── Key metrics ────────────────────────────────── */
    printSectionHeader("System Overview");
    printKVf("Active Students",  (float)total_students);
    printKVf("Average CGPA",     avg_cgpa);
    printKVf("Total Predictions",(float)total_predictions);

    /* ── Career distribution ────────────────────────── */
    printSectionHeader("Career Recommendation Distribution");
    db_print_prediction_stats();
    printHRule(BOX_WIDTH + 4);

    /* ── CGPA by department ─────────────────────────── */
    printSectionHeader("Average CGPA by Department");
    db_print_dept_stats();
    printHRule(BOX_WIDTH + 4);

    pauseScreen();
}
