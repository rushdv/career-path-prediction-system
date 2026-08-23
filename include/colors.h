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
#define BOX_TL     "\xe2\x94\x8c" // "┌"
#define BOX_TR     "\xe2\x94\x90" // "┐"
#define BOX_BL     "\xe2\x94\x94" // "└"
#define BOX_BR     "\xe2\x94\x98" // "┘"
#define BOX_V      "\xe2\x94\x82" // "│"
#define BOX_H      "\xe2\x94\x80" // "─"

/* Box Drawing Characters (double line) */
#define DBL_TL     "\xe2\x95\x94" // "╔"
#define DBL_TR     "\xe2\x95\x97" // "╗"
#define DBL_BL     "\xe2\x95\x9a" // "╚"
#define DBL_BR     "\xe2\x95\x9d" // "╝"
#define DBL_V      "\xe2\x95\x91" // "║"
#define DBL_H      "\xe2\x95\x90" // "═"

/* Symbols */
#define SYM_ARROW  "\xe2\x86\x92" // "→"
#define SYM_CHECK  "\xe2\x9c\x93" // "✓"
#define SYM_CROSS  "\xe2\x9c\x97" // "✗"
#define SYM_STAR   "\xe2\x98\x85" // "★"
#define SYM_WARN   "\xe2\x9a\xa0" // "⚠"

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
