#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#define MKDIR(path) mkdir(path, 0777)
#endif
#include "report.h"
#include "colors.h"
#include "db_handler.h"
#include "assessment.h"

void generateReport(const Student *st, const SkillProfile *sp, CareerPath *cars, float *scores) {
    char filename[100];
    snprintf(filename, sizeof(filename), "reports/report_%s.txt", st->studentID);
    
    MKDIR("reports");

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printError("Could not create report file in reports/ directory.");
        pauseScreen();
        return;
    }

    /* Get current time */
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    fprintf(fp, "=================================================\n");
    fprintf(fp, "        CAREER PATH PREDICTION REPORT            \n");
    fprintf(fp, "=================================================\n\n");

    fprintf(fp, "Date Generated : %d-%02d-%02d %02d:%02d:%02d\n\n",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);

    fprintf(fp, "--- STUDENT PROFILE ---\n");
    fprintf(fp, "Name           : %s\n", st->name);
    fprintf(fp, "Student ID     : %s\n", st->studentID);
    fprintf(fp, "Department     : %s\n", st->department);
    fprintf(fp, "CGPA           : %.2f\n\n", st->cgpa);

    fprintf(fp, "--- SKILL COMPETENCY LEVELS ---\n");
    fprintf(fp, "Programming    : %-14s (%.1f/10)\n", levelName(sp->level_programming), sp->programming);
    fprintf(fp, "Networking     : %-14s (%.1f/10)\n", levelName(sp->level_networking), sp->networking);
    fprintf(fp, "Cybersecurity  : %-14s (%.1f/10)\n", levelName(sp->level_security), sp->security);
    fprintf(fp, "Data Analytics : %-14s (%.1f/10)\n", levelName(sp->level_analytics), sp->analytics);
    fprintf(fp, "UI/UX Design   : %-14s (%.1f/10)\n", levelName(sp->level_design), sp->design);
    fprintf(fp, "Communication  : %-14s (%.1f/10)\n\n", levelName(sp->level_communication), sp->communication);

    fprintf(fp, "--- KNOWN LANGUAGES & TOOLS ---\n");
    fprintf(fp, "Languages      : %s%s%s%s%s%s%s%s\n",
            sp->lang_python ? "Python " : "",
            sp->lang_java   ? "Java "   : "",
            sp->lang_c_cpp  ? "C/C++ "  : "",
            sp->lang_js     ? "JavaScript " : "",
            sp->lang_php    ? "PHP "    : "",
            sp->lang_go     ? "Go "     : "",
            sp->lang_kotlin ? "Kotlin " : "",
            sp->lang_sql    ? "SQL "    : "");
    fprintf(fp, "Tools/Platforms: %s%s%s%s%s%s%s%s\n\n",
            sp->tool_git       ? "Git "    : "",
            sp->tool_docker    ? "Docker " : "",
            sp->tool_linux     ? "Linux "  : "",
            sp->tool_cloud     ? "Cloud(AWS/GCP) " : "",
            sp->tool_react_vue ? "React/Vue " : "",
            sp->tool_tf_pytorch? "TF/PyTorch " : "",
            sp->tool_figma     ? "Figma "  : "",
            sp->tool_mongodb   ? "MongoDB " : "");

    fprintf(fp, "--- PRACTICAL PROJECTS & EXPERIENCE ---\n");
    fprintf(fp, "Web App Built      : %s\n", sp->proj_web ? "Yes" : "No");
    fprintf(fp, "Mobile App Built   : %s\n", sp->proj_mobile ? "Yes" : "No");
    fprintf(fp, "DB System Built    : %s\n", sp->proj_db ? "Yes" : "No");
    fprintf(fp, "AI Model Built     : %s\n", sp->proj_ai ? "Yes" : "No");
    fprintf(fp, "Security Tool Built: %s\n", sp->proj_security ? "Yes" : "No");
    fprintf(fp, "Internship Done    : %s\n", sp->exp_internship ? "Yes" : "No");
    fprintf(fp, "Hackathon Particip.: %s\n", sp->exp_hackathon ? "Yes" : "No");
    fprintf(fp, "Competitive Prog.  : %s\n", sp->exp_competitive_prog ? "Yes" : "No");
    fprintf(fp, "Research Paper/T.  : %s\n\n", sp->exp_research ? "Yes" : "No");

    fprintf(fp, "--- RECOMMENDED CAREER PATHS ---\n");
    for (int i = 0; i < 3; i++) {
        fprintf(fp, "#%d %-30s | Match: %.0f%%\n", i + 1, cars[i].name, scores[i] * 10.0f);
    }
    fprintf(fp, "\n=================================================\n");
    
    fclose(fp);

    CLEAR_SCREEN();
    printf("\n");
    printBanner("REPORT GENERATED", 16);
    printf("\n");
    printf(UI_PAD "%sYour detailed career report has been saved to:%s\n",
           CC(C_DIM), CC(RESET));
    printf(UI_PAD " %s%s%s %s%s%s%s\n\n",
           CC(C_PRIMARY), SYM_ARROW, CC(RESET),
           CC(C_ACCENT), CC(BOLD), filename, CC(RESET));
    printSuccess("You can view or share this report at any time.");
    pauseScreen();
}