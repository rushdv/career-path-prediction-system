#ifndef COLORS_H
#define COLORS_H

/* ANSI Color Codes */
#define RESET      "\033[0m"
#define BOLD       "\033[1m"
#define DIM        "\033[2m"

#define C_PRIMARY  "\033[38;5;39m"   // Bright Blue
#define C_ACCENT   "\033[38;5;208m"  // Orange
#define C_SUCCESS  "\033[38;5;46m"   // Green
#define C_WARNING  "\033[38;5;226m"  // Yellow
#define C_ERROR    "\033[38;5;196m"  // Red
#define C_INFO     "\033[38;5;51m"   // Cyan
#define C_VALUE    "\033[38;5;213m"  // Pink
#define C_DIM      "\033[38;5;240m"  // Gray
#define C_BORDER   "\033[38;5;244m"  // Light Gray
#define C_TITLE    "\033[38;5;117m"  // Sky Blue
#define C_HEADER   "\033[38;5;147m"  // Lavender
#define C_MENU_NUM "\033[38;5;141m"  // Purple
#define C_MENU_TEXT "\033[38;5;252m" // White-ish

/* Career Ranking Colors */
#define C_RANK1    "\033[38;5;220m"  // Gold
#define C_RANK2    "\033[38;5;250m"  // Silver
#define C_RANK3    "\033[38;5;208m"  // Bronze

/* Gap Analysis Status Colors */
#define C_OK       "\033[38;5;46m"   // Green (same as SUCCESS)
#define C_NEEDS    "\033[38;5;196m"  // Red   (same as ERROR)

/* Box Drawing Characters (single line) */
#define BOX_TL     "+"
#define BOX_TR     "+"
#define BOX_BL     "+"
#define BOX_BR     "+"
#define BOX_V      "|"
#define BOX_H      "-"

/* Box Drawing Characters (double line) */
#define DBL_TL     "+"
#define DBL_TR     "+"
#define DBL_BL     "+"
#define DBL_BR     "+"
#define DBL_V      "|"
#define DBL_H      "="

/* Symbols */
#define SYM_ARROW  "->"
#define SYM_CHECK  "[OK]"
#define SYM_CROSS  "[X]"
#define SYM_STAR   "*"
#define SYM_WARN   "[!]"

/* Box Dimensions */
#define BOX_WIDTH  48
#define BOX_INNER  44  // BOX_WIDTH - 4 (2 padding left + 2 padding right)

/* Screen Management */
#define CLEAR_SCREEN() printf("\033[2J\033[H")

/* Helper Functions */
void boxTop(void);
void boxBottom(void);
void boxRowRaw(const char *text, int displayLen);
void printBanner(const char *text, int cols);
void inputPrompt(const char *label);
void printSkillBar(const char *label, float value, int width);
void printScoreBar(float value, int width);
void printKV(const char *key, const char *value);
void printSuccess(const char *msg);
void printError(const char *msg);
void printWarning(const char *msg);
void printInfo(const char *msg);
void pauseScreen(void);

#endif
