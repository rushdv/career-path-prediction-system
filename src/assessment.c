#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assessment.h"
#include "colors.h"
#include "input_handler.h"
#include "db_handler.h"

/* Convert 1-5 level to 2.0 - 10.0 score */
float levelToScore(int level) {
    switch (level) {
        case 1: return 2.0f;
        case 2: return 4.0f;
        case 3: return 6.0f;
        case 4: return 8.0f;
        case 5: return 10.0f;
        default: return 0.0f;
    }
}

/* Get string name of level */
const char* levelName(int level) {
    switch (level) {
        case 1: return "Beginner";
        case 2: return "Elementary";
        case 3: return "Intermediate";
        case 4: return "Advanced";
        case 5: return "Expert";
        default: return "Unknown";
    }
}

/* Collect skill level (1-5) with instant visual feedback */
static int promptSkillRating(const char *skillTitle, const char *subtitle) {
    printf(UI_PAD " %s%s%s %-32s %s(%s)%s\n",
           CC(C_PRIMARY), SYM_DIAMOND, CC(RESET),
           skillTitle,
           CC(C_DIM), subtitle, CC(RESET));
    
    inputPrompt("Rating (1-5)");
    int level = getIntInputRange(1, 5);
    float score = levelToScore(level);

    printf(UI_PAD "   ");
    printSkillBar(levelName(level), score, 16);
    printf("\n");
    return level;
}

/* Helper to parse comma/space separated numeric checklist input */
static void parseMultiSelect(const char *promptStr, int *flags, int maxCount) {
    inputPrompt(promptStr);
    char buf[256];
    if (fgets(buf, sizeof(buf), stdin) != NULL) {
        buf[strcspn(buf, "\r\n")] = 0;
        char *token = strtok(buf, " ,;");
        while (token != NULL) {
            int num = atoi(token);
            if (num >= 1 && num <= maxCount) {
                flags[num - 1] = 1;
            }
            token = strtok(NULL, " ,;");
        }
    }
}

/* ── Main Multi-Screen Assessment Flow ──────────────────── */
SkillProfile runAssessment(int studentRef) {
    SkillProfile sp;
    memset(&sp, 0, sizeof(SkillProfile));
    sp.studentRef = studentRef;

    /* ══════════════════════════════════════════════════════════
       SCREEN 1: Core Competency Ratings
       ══════════════════════════════════════════════════════════ */
    CLEAR_SCREEN();
    printf("\n");
    printBanner("PROFESSIONAL SKILL ASSESSMENT", 29);
    printf("\n");

    printf(UI_PAD "%s%sStep 1 of 3: Core Engineering Competencies%s\n",
           CC(C_PRIMARY), CC(BOLD), CC(RESET));
    printf(UI_PAD "%sRate your competency on each core technical pillar:%s\n\n",
           CC(C_DIM), CC(RESET));

    /* Visual scale legend */
    printf(UI_PAD "%s[1] Beginner (2.0)  [2] Elementary (4.0)  [3] Intermediate (6.0)  [4] Advanced (8.0)  [5] Expert (10.0)%s\n\n",
           CC(C_MUTED), CC(RESET));

    sp.level_programming = promptSkillRating("Programming & Algorithms", "C / C++, Python, Java, DSA");
    sp.programming       = levelToScore(sp.level_programming);

    sp.level_networking  = promptSkillRating("Networking & Infrastructure", "TCP/IP, Routing, Architecture");
    sp.networking        = levelToScore(sp.level_networking);

    sp.level_design      = promptSkillRating("UI/UX & System Design", "Wireframes, Figma, UX Flow");
    sp.design            = levelToScore(sp.level_design);

    sp.level_analytics   = promptSkillRating("Data Analytics & Math", "SQL, Statistics, Modeling");
    sp.analytics         = levelToScore(sp.level_analytics);

    sp.level_communication = promptSkillRating("Professional Communication", "Technical Writing, Teamwork");
    sp.communication     = levelToScore(sp.level_communication);

    sp.level_security    = promptSkillRating("Cybersecurity & Defense", "Cryptography, Audit, SecOps");
    sp.security          = levelToScore(sp.level_security);

    printSuccess("Core competency ratings saved.");
    pauseScreen();

    /* ══════════════════════════════════════════════════════════
       SCREEN 2: Technical Stack & Practical Portfolio Matrix
       ══════════════════════════════════════════════════════════ */
    CLEAR_SCREEN();
    printf("\n");
    printBanner("TECH STACK & PRACTICAL PORTFOLIO", 32);
    printf("\n");

    printf(UI_PAD "%s%sStep 2 of 3: Languages, Tools, Projects & Experience%s\n",
           CC(C_PRIMARY), CC(BOLD), CC(RESET));
    printf(UI_PAD "%sEnter numbers separated by commas (e.g. 1, 3, 7) or press Enter to skip.%s\n\n",
           CC(C_DIM), CC(RESET));

    /* Group A: Languages & Tools */
    printSectionHeader("A. Programming Languages & Developer Tools");
    printf(UI_PAD "%s 1. Python          2. Java            3. C / C++         4. JavaScript%s\n", CC(C_VALUE), CC(RESET));
    printf(UI_PAD "%s 5. PHP             6. Go              7. Kotlin          8. SQL (RDBMS)%s\n", CC(C_VALUE), CC(RESET));
    printf(UI_PAD "%s 9. Git / GitHub   10. Docker/Containers11. Linux Admin   12. AWS/Cloud%s\n", CC(C_VALUE), CC(RESET));
    printf(UI_PAD "%s13. React/Vue/Next 14. TF / PyTorch   15. Figma / Design 16. MongoDB / NoSQL%s\n\n", CC(C_VALUE), CC(RESET));

    int tech_flags[16] = {0};
    parseMultiSelect("Languages & Tools known (e.g. 1, 4, 9)", tech_flags, 16);
    sp.lang_python    = tech_flags[0];  sp.lang_java       = tech_flags[1];
    sp.lang_c_cpp     = tech_flags[2];  sp.lang_js         = tech_flags[3];
    sp.lang_php       = tech_flags[4];  sp.lang_go         = tech_flags[5];
    sp.lang_kotlin    = tech_flags[6];  sp.lang_sql        = tech_flags[7];
    sp.tool_git       = tech_flags[8];  sp.tool_docker     = tech_flags[9];
    sp.tool_linux     = tech_flags[10]; sp.tool_cloud      = tech_flags[11];
    sp.tool_react_vue = tech_flags[12]; sp.tool_tf_pytorch = tech_flags[13];
    sp.tool_figma     = tech_flags[14]; sp.tool_mongodb    = tech_flags[15];
    printf("\n");

    /* Group B: Practical Projects */
    printSectionHeader("B. Practical Projects Built");
    printf(UI_PAD "%s 1. Full-stack Web Application          2. Mobile Application (Android/iOS)%s\n", CC(C_VALUE), CC(RESET));
    printf(UI_PAD "%s 3. Database-backed Enterprise System    4. AI / ML Pipeline Model%s\n", CC(C_VALUE), CC(RESET));
    printf(UI_PAD "%s 5. Security Tool / Penetration Testing%s\n\n", CC(C_VALUE), CC(RESET));

    int proj_flags[5] = {0};
    parseMultiSelect("Projects completed (e.g. 1, 2)", proj_flags, 5);
    sp.proj_web      = proj_flags[0];
    sp.proj_mobile   = proj_flags[1];
    sp.proj_db       = proj_flags[2];
    sp.proj_ai       = proj_flags[3];
    sp.proj_security = proj_flags[4];
    printf("\n");

    /* Group C: Academic Courses & Experience Highlights */
    printSectionHeader("C. Academic Courses & Experience Highlights");
    printf(UI_PAD "%s 1. AI / ML Course          2. Web Development Course 3. Data Science & Big Data%s\n", CC(C_VALUE), CC(RESET));
    printf(UI_PAD "%s 4. Cybersecurity Course    5. Cloud Computing Course 6. Software Internship%s\n", CC(C_VALUE), CC(RESET));
    printf(UI_PAD "%s 7. Hackathon Participation  8. Competitive Coding (CP)9. Research Paper / Thesis%s\n\n", CC(C_VALUE), CC(RESET));

    int exp_flags[9] = {0};
    parseMultiSelect("Courses & Experience (e.g. 1, 6, 7)", exp_flags, 9);
    sp.course_ml_ai           = exp_flags[0];
    sp.course_web_dev         = exp_flags[1];
    sp.course_data_science    = exp_flags[2];
    sp.course_cybersecurity   = exp_flags[3];
    sp.course_cloud_computing = exp_flags[4];
    sp.exp_internship         = exp_flags[5];
    sp.exp_hackathon          = exp_flags[6];
    sp.exp_competitive_prog   = exp_flags[7];
    sp.exp_research           = exp_flags[8];
    printf("\n");

    printSuccess("Portfolio matrix recorded successfully.");
    pauseScreen();

    /* ══════════════════════════════════════════════════════════
       SCREEN 3: Primary Career Aspiration & Interest
       ══════════════════════════════════════════════════════════ */
    CLEAR_SCREEN();
    printf("\n");
    printBanner("CAREER GOALS & SPECIALIZATION", 29);
    printf("\n");

    printf(UI_PAD "%s%sStep 3 of 3: Primary Career Direction Alignment%s\n",
           CC(C_PRIMARY), CC(BOLD), CC(RESET));
    printf(UI_PAD "%sSelect the domain that aligns most with your future aspirations:%s\n\n",
           CC(C_DIM), CC(RESET));

    printf(UI_PAD "%s  1. Software & App Engineering %s%s(Web, Mobile, Systems, Games, Backend)%s\n",
           CC(C_VALUE), CC(RESET), CC(C_DIM), CC(RESET));
    printf(UI_PAD "%s  2. AI, Data Science & Analytics%s%s(ML/AI, Data Engineering, DBA, BI)%s\n",
           CC(C_VALUE), CC(RESET), CC(C_DIM), CC(RESET));
    printf(UI_PAD "%s  3. Cybersecurity & InfoSec     %s%s(SOC Analyst, Pentesting, Audit, Networks)%s\n",
           CC(C_VALUE), CC(RESET), CC(C_DIM), CC(RESET));
    printf(UI_PAD "%s  4. Cloud, DevOps & SRE         %s%s(Cloud Architect, DevOps, SRE, Platform)%s\n",
           CC(C_VALUE), CC(RESET), CC(C_DIM), CC(RESET));
    printf(UI_PAD "%s  5. UI/UX, Product & Management %s%s(Product Design, Tech Product Manager)%s\n\n",
           CC(C_VALUE), CC(RESET), CC(C_DIM), CC(RESET));

    inputPrompt("Select Primary Domain (1-5)");
    sp.career_interest = getIntInputRange(1, 5);

    /* Save profile to SQLite */
    db_save_skill_profile(&sp);

    printf("\n");
    printSuccess("Assessment complete! Your profile has been evaluated and saved.");
    printf(UI_PAD "%sSelect '%s%sView Career Prediction%s%s' from the menu to inspect your matches.%s\n\n",
           CC(C_DIM), CC(RESET), CC(C_ACCENT), CC(RESET), CC(C_DIM), CC(RESET));
    pauseScreen();

    return sp;
}
