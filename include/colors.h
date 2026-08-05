#ifndef COLORS_H
#define COLORS_H

#include <stdio.h>
#include <stdlib.h>

/* Basic console helpers and placeholder color macros */
#ifdef _WIN32
#define CLEAR_SCREEN() system("cls")
#else
#define CLEAR_SCREEN() printf("\033[2J\033[H")
#endif

#define RESET ""
#define BOLD ""

/* Color/style placeholders (no-op strings) */
#define C_BORDER ""
#define C_HEADER ""
#define C_DIM ""
#define C_INFO ""
#define C_VALUE ""
#define C_ACCENT ""
#define C_SUCCESS ""
#define C_WARNING ""
#define C_ERROR ""
#define C_NEEDS ""
#define C_MENU_NUM ""
#define C_MENU_TEXT ""

/* Some code uses bare DIM or other short aliases */
#define DIM C_DIM

/* Title and rank color aliases */
#define C_TITLE ""
#define C_RANK1 ""
#define C_RANK2 ""
#define C_RANK3 ""
#define C_OK ""

/* Symbols used in UI */
#define SYM_WARN "[!]"
#define SYM_STAR "[*]"
#define SYM_ARROW "->"
#define SYM_CHECK "[\xE2\x9C\x94]"


/* Box drawing fallbacks (ASCII safe) */
#define BOX_TL "+"
#define BOX_TR "+"
#define BOX_BL "+"
#define BOX_BR "+"
#define BOX_H "-"
#define BOX_V "|"

/* Double border fallbacks used in some prints */
#define DBL_TL "+"
#define DBL_TR "+"
#define DBL_H "="
#define DBL_V "|"

/* Helper function prototypes used across the project */
void pauseScreen(void);
void printBanner(const char *title, int width);
void printSkillBar(const char *label, float value, int width);
void printError(const char *msg);
void printWarning(const char *msg);
void printInfo(const char *msg);
void printSuccess(const char *msg);
void inputPrompt(const char *prompt);

#endif
