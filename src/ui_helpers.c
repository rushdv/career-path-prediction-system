#include <stdio.h>
#include <string.h>
#include "colors.h"

void pauseScreen(void) {
    printf("\nPress Enter to continue...");
    fflush(stdout);
    getchar();
}

void printBanner(const char *title, int width) {
    int i;
    printf("\n  ");
    for (i = 0; i < width + 4; i++) putchar('=');
    printf("\n  %s\n  ", title);
    for (i = 0; i < width + 4; i++) putchar('=');
    printf("\n");
}

void printSkillBar(const char *label, float value, int width) {
    int filled = (int)((value / 10.0f) * width + 0.5f);
    int i;
    if (label && label[0] != '\0') printf("  %-14s ", label);
    putchar('[');
    for (i = 0; i < width; i++) putchar(i < filled ? '#' : ' ');
    putchar(']');
    printf(" %4.1f/10\n", value);
}

void printError(const char *msg) {
    printf("\n[ERROR] %s\n", msg);
}

void printWarning(const char *msg) {
    printf("\n[WARN] %s\n", msg);
}

void printInfo(const char *msg) {
    printf("\n[INFO] %s\n", msg);
}

void printSuccess(const char *msg) {
    printf("\n[SUCCESS] %s\n", msg);
}

void inputPrompt(const char *prompt) {
    printf("  %s: ", prompt);
    fflush(stdout);
}

/* Small box helpers used in student and auth modules */
void boxTop(void) {
    printf("  " C_BORDER BOX_TL);
    int i; for (i = 0; i < 48; i++) printf(BOX_H);
    printf(BOX_TR RESET "\n");
}

void boxBottom(void) {
    printf("  " C_BORDER BOX_BL);
    int i; for (i = 0; i < 48; i++) printf(BOX_H);
    printf(BOX_BR RESET "\n");
}

void printKV(const char *k, const char *v) {
    printf("  " C_INFO "%-12s" RESET ": " C_VALUE "%s" RESET "\n", k, v);
}

void printScoreBar(float value, int width) {
    int filled = (int)((value / 10.0f) * width + 0.5f);
    int i;
    putchar('[');
    for (i = 0; i < width; i++) putchar(i < filled ? '#' : ' ');
    putchar(']');
}
