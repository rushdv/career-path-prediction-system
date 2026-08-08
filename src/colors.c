#include <stdio.h>
#include <string.h>
#include "colors.h"

/* ══════════════════════════════════════════════════════════
   BOX DRAWING
   ══════════════════════════════════════════════════════════ */
void boxTop(void) {
    int i;
    printf(C_BORDER "  " BOX_TL);
    for (i = 0; i < BOX_WIDTH; i++) printf(BOX_H);
    printf(BOX_TR RESET "\n");
}

void boxBottom(void) {
    int i;
    printf(C_BORDER "  " BOX_BL);
    for (i = 0; i < BOX_WIDTH; i++) printf(BOX_H);
    printf(BOX_BR RESET "\n");
}

/* Print a centered row inside a box.
   displayLen = visible character count of text (no ANSI bytes).
   Layout: "  │  " + [padLeft] + text + [padRight] + "  │"
   Inner usable width = BOX_WIDTH - 4 = BOX_INNER = 44 */
void boxRowRaw(const char *text, int displayLen) {
    int inner = BOX_INNER;
    int padLeft  = (inner - displayLen) / 2;
    int padRight = inner - displayLen - padLeft;
    int i;
    if (padLeft  < 0) padLeft  = 0;
    if (padRight < 0) padRight = 0;

    printf(C_BORDER "  " BOX_V RESET "  ");
    for (i = 0; i < padLeft;  i++) printf(" ");
    printf("%s", text);
    for (i = 0; i < padRight; i++) printf(" ");
    printf(C_BORDER "  " BOX_V RESET "\n");
}

/* ══════════════════════════════════════════════════════════
   BANNER  (single-line titled box)
   cols = visible character count of text (excluding the two
          surrounding spaces the caller must include, e.g.
          printBanner("  TITLE  ", 9) )
   ══════════════════════════════════════════════════════════ */
void printBanner(const char *text, int cols) {
    int i;
    /* Top border */
    printf(C_BORDER "  " BOX_TL);
    for (i = 0; i < cols + 2; i++) printf(BOX_H);   /* +2 for the spaces around text */
    printf(BOX_TR RESET "\n");

    /* Title row */
    printf(C_BORDER "  " BOX_V RESET " " C_TITLE BOLD "%s" RESET " " C_BORDER BOX_V RESET "\n",
           text);

    /* Bottom border */
    printf(C_BORDER "  " BOX_BL);
    for (i = 0; i < cols + 2; i++) printf(BOX_H);
    printf(BOX_BR RESET "\n");
}

/* ══════════════════════════════════════════════════════════
   INPUT PROMPT
   ══════════════════════════════════════════════════════════ */
void inputPrompt(const char *label) {
    printf("  " C_PRIMARY BOLD "> " RESET C_INFO "%s" RESET ": ", label);
}

/* ══════════════════════════════════════════════════════════
   SKILL BAR
   label=""  → inline bar only (used for mini preview rows)
   label!="" → full "  label [bar] X.X/10" line
   Always adds a blank line after the bar row.
   ══════════════════════════════════════════════════════════ */
void printSkillBar(const char *label, float value, int width) {
    int filled, i;

    filled = (int)((value / 10.0f) * (float)width);
    if (filled > width) filled = width;
    if (filled < 0)     filled = 0;

    const char *barColor;
    if      (value >= 7.0f) barColor = C_SUCCESS;
    else if (value >= 4.0f) barColor = C_WARNING;
    else                    barColor = C_ERROR;

    if (label != NULL && label[0] != '\0') {
        /* Full labelled line */
        printf("  " C_INFO "%-20s" RESET " [", label);
    } else {
        /* Inline / no-label — just print the bar */
        printf("[");
    }

    printf(barColor);
    for (i = 0; i < filled;         i++) printf("#");  /* █ -> # */
    printf(C_DIM);
    for (i = filled; i < width;     i++) printf("-");  /* ░ -> - */
    printf(RESET "]");

    if (label != NULL && label[0] != '\0') {
        printf(" " BOLD "%.1f" RESET "/10\n\n", value);
    } else {
        printf("\n\n");
    }
}

/* ══════════════════════════════════════════════════════════
   SCORE BAR  (inline — no newline, used next to career names)
   ══════════════════════════════════════════════════════════ */
void printScoreBar(float value, int width) {
    int filled, i;

    filled = (int)((value / 10.0f) * (float)width);
    if (filled > width) filled = width;
    if (filled < 0)     filled = 0;

    const char *barColor;
    if      (value >= 7.0f) barColor = C_SUCCESS;
    else if (value >= 5.0f) barColor = C_WARNING;
    else                    barColor = C_ERROR;

    printf("[");
    printf(barColor);
    for (i = 0; i < filled;     i++) printf("#");
    printf(C_DIM);
    for (i = filled; i < width; i++) printf("-");
    printf(RESET "]");
    /* No newline — caller handles layout */
}

/* ══════════════════════════════════════════════════════════
   KEY / VALUE ROW
   ══════════════════════════════════════════════════════════ */
void printKV(const char *key, const char *value) {
    printf("  " C_INFO "%-18s" RESET ": " C_VALUE "%s" RESET "\n", key, value);
}

/* ══════════════════════════════════════════════════════════
   STATUS MESSAGES
   ══════════════════════════════════════════════════════════ */
void printSuccess(const char *msg) {
    printf("\n  " C_SUCCESS BOLD SYM_CHECK RESET "  " C_SUCCESS "%s" RESET "\n\n", msg);
}

void printError(const char *msg) {
    printf("\n  " C_ERROR BOLD SYM_CROSS RESET "  " C_ERROR "%s" RESET "\n\n", msg);
}

void printWarning(const char *msg) {
    printf("\n  " C_WARNING BOLD SYM_WARN RESET "  " C_WARNING "%s" RESET "\n\n", msg);
}

void printInfo(const char *msg) {
    printf("\n  " C_INFO BOLD "i" RESET "  " C_INFO "%s" RESET "\n\n", msg);
}

/* ══════════════════════════════════════════════════════════
   PAUSE
   ══════════════════════════════════════════════════════════ */
void pauseScreen(void) {
    printf("\n  " C_DIM "Press Enter to continue..." RESET " ");
    getchar();
}
