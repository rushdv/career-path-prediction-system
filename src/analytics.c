#include <stdio.h>
#include <string.h>
#include "analytics.h"
#include "student.h"
#include "prediction.h"
#include "colors.h"
#include "file_handler.h"

void showAdminDashboard(void) {
    Student arr[100];
    int num_students = 0;
    loadAllStudents(arr, &num_students);
    
    int active_students = 0;
    float total_cgpa = 0.0f;
    for (int i = 0; i < num_students; i++) {
        if (arr[i].isActive) {
            active_students++;
            total_cgpa += arr[i].cgpa;
        }
    }
    float avg_cgpa = (active_students > 0) ? (total_cgpa / active_students) : 0.0f;

    /* Load all histories to find top careers */
    FILE *fp = fopen("data/history.dat", "rb");
    CareerPath cp[NUM_CAREERS];
    int cp_count = 0;
    getCareers(cp, &cp_count);
    
    int career_counts[64] = {0}; // safe max size
    int total_predictions = 0;
    
    if (fp) {
        PredictionRecord pr;
        while (fread(&pr, sizeof(PredictionRecord), 1, fp) == 1) {
            total_predictions++;
            for (int j = 0; j < cp_count; j++) {
                if (strcmp(pr.topCareer, cp[j].name) == 0) {
                    career_counts[j]++;
                    break;
                }
            }
        }
        fclose(fp);
    }
    
    CLEAR_SCREEN();
    printf("\n");
    printBanner("  ADMIN ANALYTICS DASHBOARD  ", 27);
    printf("\n");
    
    printf("  " C_INFO "Total Active Students  : " RESET C_VALUE BOLD "%d\n" RESET, active_students);
    printf("  " C_INFO "Average CGPA           : " RESET C_VALUE BOLD "%.2f\n" RESET, avg_cgpa);
    printf("  " C_INFO "Total Predictions Run  : " RESET C_VALUE BOLD "%d\n" RESET, total_predictions);
    printf("\n");
    
    /* Draw ASCII Bar Chart */
    printf("  " C_TITLE BOLD "Recommended Careers Distribution:" RESET "\n");
    printf("  " C_DIM);
    for (int d = 0; d < 66; d++) printf(BOX_H);
    printf(RESET "\n");
    
    int max_count = 0;
    for (int i = 0; i < cp_count; i++) {
        if (career_counts[i] > max_count) max_count = career_counts[i];
    }
    
    for (int i = 0; i < cp_count; i++) {
        printf("  " C_INFO "%-24s" RESET " | ", cp[i].name);
        
        int bar_len = 0;
        if (max_count > 0) {
            bar_len = (int)(((float)career_counts[i] / max_count) * 30.0f);
        }
        
        if (bar_len > 0) {
            printf(C_SUCCESS);
            for (int b = 0; b < bar_len; b++) printf("#"); /* ASCII bar */
            printf(RESET);
        }
        printf(" " C_VALUE "%d" RESET "\n", career_counts[i]);
    }
    
    printf("  " C_DIM);
    for (int d = 0; d < 66; d++) printf(BOX_H);
    printf(RESET "\n\n");
    
    pauseScreen();
}
