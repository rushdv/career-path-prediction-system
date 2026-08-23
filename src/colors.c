#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#ifdef _WIN32
#  include <windows.h>
#  include <io.h>
#  define ISATTY(fd) _isatty(fd)
#else
#  include <unistd.h>
#  define ISATTY(fd) isatty(fd)
#endif
#include "colors.h"

/* ══════════════════════════════════════════════════════════
   RUNTIME TERMINAL FLAGS
   ══════════════════════════════════════════════════════════ */
int TERM_HAS_COLOR = 1;   /* assume on; detect() may clear */
int TERM_HAS_UTF8  = 1;

/* ── Windows: enable VT processing + UTF-8 ──────────────── */
#ifdef _WIN32
static int win_enable_vt(void) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h == INVALID_HANDLE_VALUE) return 0;
    DWORD mode = 0;
    if (!GetConsoleMode(h, &mode)) return 0;
    mode |= 0x0004; /* ENABLE_VIRTUAL_TERMINAL_PROCESSING */
    if (!SetConsoleMode(h, mode)) return 0;
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    return 1;
}
#endif

/* ── Detect 256-colour support ───────────────────────────── */
static int detect_color(void) {
    if (!ISATTY(1)) return 0;
#ifdef _WIN32
    if (getenv("WT_SESSION"))   return 1;   /* Windows Terminal */
    if (getenv("TERM_PROGRAM")) return 1;   /* VS Code terminal etc. */
    return win_enable_vt();
#else
    const char *ct   = getenv("COLORTERM");
    const char *term = getenv("TERM");
    if (ct && (strstr(ct, "truecolor") || strstr(ct, "24bit") ||
               strstr(ct, "256"))) return 1;
    if (term && strstr(term, "256color")) return 1;
    if (term && (strstr(term, "xterm")  || strstr(term, "rxvt") ||
                 strstr(term, "screen") || strstr(term, "tmux") ||
                 strstr(term, "alacritty") || strstr(term, "kitty"))) return 1;
    return 0;
#endif
}

/* ── Detect UTF-8 support ────────────────────────────────── */
static int detect_utf8(void) {
#ifdef _WIN32
    return (GetConsoleOutputCP() == 65001) ? 1 : 0;
#else
    const char *vars[] = { getenv("LANG"), getenv("LC_ALL"), getenv("LC_CTYPE"), NULL };
    for (int i = 0; vars[i]; i++) {
        if (strstr(vars[i], "UTF-8") || strstr(vars[i], "utf8")) return 1;
    }
    return 0;
#endif
}

void term_init(void) {
#ifdef _WIN32
    win_enable_vt();
#endif
    TERM_HAS_COLOR = detect_color();
    TERM_HAS_UTF8  = detect_utf8();
}

/* ══════════════════════════════════════════════════════════
   INTERNAL HELPERS
   ══════════════════════════════════════════════════════════ */
/* Print a color escape — or nothing if no color support */
static void pc(const char *esc) {
    if (TERM_HAS_COLOR) fputs(esc, stdout);
}

/* ══════════════════════════════════════════════════════════
   BOX DRAWING
   ══════════════════════════════════════════════════════════ */
void boxTop(void) {
    fputs(UI_PAD, stdout); pc(C_BORDER);
    fputs(BOX_TL, stdout);
    for (int i = 0; i < BOX_WIDTH; i++) fputs(BOX_H, stdout);
    fputs(BOX_TR, stdout); pc(RESET); putchar('\n');
}

void boxBottom(void) {
    fputs(UI_PAD, stdout); pc(C_BORDER);
    fputs(BOX_BL, stdout);
    for (int i = 0; i < BOX_WIDTH; i++) fputs(BOX_H, stdout);
    fputs(BOX_BR, stdout); pc(RESET); putchar('\n');
}

int utf8_display_len(const char *str) {
    if (!str) return 0;
    int len = 0;
    int i = 0;
    while (str[i]) {
        if (str[i] == '\033') {
            /* Skip ANSI escape code */
            while (str[i] && str[i] != 'm') i++;
            if (str[i] == 'm') i++;
        } else {
            unsigned char c = (unsigned char)str[i];
            uint32_t cp = 0;
            int bytes = 0;

            if (c < 0x80) {
                cp = c;
                bytes = 1;
            } else if ((c & 0xE0) == 0xC0) {
                cp = c & 0x1F;
                bytes = 2;
            } else if ((c & 0xF0) == 0xE0) {
                cp = c & 0x0F;
                bytes = 3;
            } else if ((c & 0xF8) == 0xF0) {
                cp = c & 0x07;
                bytes = 4;
            } else {
                bytes = 1;
            }

            for (int j = 1; j < bytes && str[i + j]; j++) {
                cp = (cp << 6) | ((unsigned char)str[i + j] & 0x3F);
            }
            i += bytes;

            /* Wide character check for display width (emojis, CJK, etc.) */
            if ((cp >= 0x2600 && cp <= 0x27BF) ||   /* Misc Symbols & Dingbats (e.g., ⚡ U+26A1, ⚠ U+26A0) */
                (cp >= 0x2300 && cp <= 0x23FF) ||   /* Misc Technical */
                (cp >= 0x2B00 && cp <= 0x2BFF) ||   /* Misc Symbols and Arrows */
                (cp >= 0x1F000 && cp <= 0x1FFFF) || /* Emoji & Pictographs */
                (cp >= 0x2E80 && cp <= 0x9FFF) ||   /* CJK */
                (cp >= 0xAC00 && cp <= 0xD7A3) ||   /* Hangul */
                (cp >= 0xF900 && cp <= 0xFAFF) ||   /* CJK Compatibility */
                (cp >= 0xFF01 && cp <= 0xFF60) ||   /* Fullwidth Forms */
                (cp >= 0xFFE0 && cp <= 0xFFE6)) {
                len += 2;
            } else {
                len += 1;
            }
        }
    }
    return len;
}

/* displayLen = visible character count of text (no ANSI bytes)
   Inner usable width = BOX_INNER = BOX_WIDTH - 4              */
void boxRowRaw(const char *text, int displayLen) {
    int padRight = BOX_INNER - displayLen;
    if (padRight < 0) padRight = 0;

    fputs(UI_PAD, stdout); pc(C_BORDER);
    fputs(BOX_V, stdout);  pc(RESET); putchar(' ');
    fputs(text, stdout);
    for (int i = 0; i < padRight; i++) putchar(' ');
    putchar(' '); pc(C_BORDER);
    fputs(BOX_V, stdout);  pc(RESET); putchar('\n');
}

/* ══════════════════════════════════════════════════════════
   BANNER  — double-line frame, bold centred title
   textLen = visible character count of `text`
   ══════════════════════════════════════════════════════════ */
void printBanner(const char *text, int textLen) {
    int barWidth = textLen + 4;

    fputs(UI_PAD, stdout); pc(C_BORDER);
    fputs(BOX_TL, stdout);
    for (int i = 0; i < barWidth; i++) fputs(BOX_H, stdout);
    fputs(BOX_TR, stdout); pc(RESET); putchar('\n');

    fputs(UI_PAD, stdout); pc(C_BORDER);
    fputs(BOX_V,  stdout); pc(RESET);
    fputs("  ",   stdout); pc(C_ACCENT); pc(BOLD);
    fputs(text,   stdout); pc(RESET);
    fputs("  ",   stdout); pc(C_BORDER);
    fputs(BOX_V,  stdout); pc(RESET); putchar('\n');

    fputs(UI_PAD, stdout); pc(C_BORDER);
    fputs(BOX_BL, stdout);
    for (int i = 0; i < barWidth; i++) fputs(BOX_H, stdout);
    fputs(BOX_BR, stdout); pc(RESET); putchar('\n');
}

/* ══════════════════════════════════════════════════════════
   SECTION HEADER
   ══════════════════════════════════════════════════════════ */
void printSectionHeader(const char *text) {
    int len  = (int)strlen(text);
    int fill = 50 - len - 4;
    if (fill < 2) fill = 2;

    putchar('\n');
    fputs(UI_PAD, stdout); pc(C_PURPLE);
    fputs(BOX_H, stdout); fputs(BOX_H, stdout);
    putchar(' '); pc(RESET); pc(C_ACCENT); pc(BOLD);
    fputs(text, stdout); pc(RESET); putchar(' ');
    pc(C_PURPLE);
    for (int i = 0; i < fill; i++) fputs(BOX_H, stdout);
    pc(RESET); putchar('\n'); putchar('\n');
}

/* ── Horizontal rule ─────────────────────────────────────── */
void printHRule(int width) {
    fputs(UI_PAD, stdout); pc(C_BORDER2);
    for (int i = 0; i < width; i++) fputs(BOX_H, stdout);
    pc(RESET); putchar('\n');
}

/* ══════════════════════════════════════════════════════════
   INPUT PROMPT
   ══════════════════════════════════════════════════════════ */
void inputPrompt(const char *label) {
    fputs(UI_PAD, stdout);
    pc(C_PRIMARY); pc(BOLD); fputs(SYM_BOLT, stdout); pc(RESET);
    putchar(' ');
    pc(C_INFO); fputs(label, stdout); pc(RESET);
    fputs(": ", stdout);
}

/* ══════════════════════════════════════════════════════════
   SKILL BAR  (labelled, ends with newline)
   ══════════════════════════════════════════════════════════ */
void printSkillBar(const char *label, float value, int width) {
    int filled = (int)((value / 10.0f) * (float)width);
    if (filled > width) filled = width;
    if (filled < 0)     filled = 0;

    const char *barColor = (value >= 7.0f) ? C_SUCCESS :
                           (value >= 4.0f) ? C_WARNING : C_ERROR;

    if (label && label[0]) {
        fputs(UI_PAD, stdout); pc(C_INFO);
        printf("%-18s", label); pc(RESET); putchar(' ');
    }

    pc(C_BORDER2); putchar('['); pc(RESET);
    pc(barColor);
    for (int i = 0; i < filled; i++)  fputs(SYM_BAR_F, stdout);
    pc(C_DIM);
    for (int i = filled; i < width; i++) fputs(SYM_BAR_E, stdout);
    pc(C_BORDER2); putchar(']'); pc(RESET);

    if (label && label[0]) {
        putchar(' '); pc(C_VALUE); pc(BOLD);
        printf("%.1f", value); pc(RESET);
        pc(C_DIM); fputs("/10", stdout); pc(RESET);
        putchar('\n');
    } else {
        putchar('\n');
    }
}

/* ══════════════════════════════════════════════════════════
   SCORE BAR  (inline, no newline)
   ══════════════════════════════════════════════════════════ */
void printScoreBar(float value, int width) {
    int filled = (int)((value / 10.0f) * (float)width);
    if (filled > width) filled = width;
    if (filled < 0)     filled = 0;

    const char *barColor = (value >= 7.0f) ? C_SUCCESS :
                           (value >= 5.0f) ? C_WARNING : C_ERROR;

    pc(C_BORDER2); putchar('['); pc(RESET);
    pc(barColor);
    for (int i = 0; i < filled; i++)  fputs(SYM_BAR_F, stdout);
    pc(C_DIM);
    for (int i = filled; i < width; i++) fputs(SYM_BAR_E, stdout);
    pc(C_BORDER2); putchar(']'); pc(RESET);
}

/* ══════════════════════════════════════════════════════════
   KEY / VALUE ROWS
   ══════════════════════════════════════════════════════════ */
void printKV(const char *key, const char *value) {
    fputs(UI_PAD, stdout);
    pc(C_PURPLE); fputs(SYM_DIAMOND, stdout); pc(RESET); putchar(' ');
    pc(C_INFO);   printf("%-20s", key);        pc(RESET);
    pc(C_DIM);    fputs(" : ", stdout);         pc(RESET);
    pc(C_VALUE);  fputs(value, stdout);          pc(RESET);
    putchar('\n');
}

void printKVf(const char *key, float value) {
    fputs(UI_PAD, stdout);
    pc(C_PURPLE); fputs(SYM_DIAMOND, stdout); pc(RESET); putchar(' ');
    pc(C_INFO);   printf("%-20s", key);        pc(RESET);
    pc(C_DIM);    fputs(" : ", stdout);         pc(RESET);
    pc(C_VALUE);  pc(BOLD); printf("%.2f", value); pc(RESET);
    putchar('\n');
}

/* ══════════════════════════════════════════════════════════
   STATUS MESSAGES
   ══════════════════════════════════════════════════════════ */
void printSuccess(const char *msg) {
    putchar('\n');
    fputs(UI_PAD, stdout);
    pc(BG_SELECT); pc(C_SUCCESS); pc(BOLD);
    putchar(' '); fputs(SYM_CHECK, stdout); fputs(" SUCCESS", stdout);
    pc(RESET); fputs("  ", stdout);
    pc(C_SUCCESS); fputs(msg, stdout); pc(RESET);
    putchar('\n'); putchar('\n');
}

void printError(const char *msg) {
    putchar('\n');
    fputs(UI_PAD, stdout);
    pc(C_ERROR); pc(BOLD);
    fputs(SYM_CROSS, stdout); fputs(" ERROR", stdout);
    pc(RESET); fputs("  ", stdout);
    pc(C_ERROR); fputs(msg, stdout); pc(RESET);
    putchar('\n'); putchar('\n');
}

void printWarning(const char *msg) {
    putchar('\n');
    fputs(UI_PAD, stdout);
    pc(C_WARNING); pc(BOLD);
    fputs(SYM_WARN, stdout); fputs(" WARNING", stdout);
    pc(RESET); fputs("  ", stdout);
    pc(C_WARNING); fputs(msg, stdout); pc(RESET);
    putchar('\n'); putchar('\n');
}

void printInfo(const char *msg) {
    putchar('\n');
    fputs(UI_PAD, stdout);
    pc(C_INFO); pc(BOLD);
    fputs(SYM_BOLT, stdout); fputs(" INFO", stdout);
    pc(RESET); fputs("  ", stdout);
    pc(C_INFO); fputs(msg, stdout); pc(RESET);
    putchar('\n'); putchar('\n');
}

/* ══════════════════════════════════════════════════════════
   PAUSE SCREEN
   Flush any stray newline left in the buffer, then wait for Enter.
   ══════════════════════════════════════════════════════════ */
void pauseScreen(void) {
    putchar('\n');
    fputs(UI_PAD, stdout); pc(C_DIM);
    fputs(BOX_H, stdout); fputs(BOX_H, stdout);
    pc(RESET);
    fputs("  Press ", stdout);
    pc(C_ACCENT); pc(BOLD); fputs("Enter", stdout); pc(RESET);
    fputs(" to continue  ", stdout);
    pc(C_DIM); fputs(BOX_H, stdout); pc(RESET);
    putchar('\n');
    fflush(stdout);

    /* Wait for Enter, discarding any preceding chars in the buffer */
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
    putchar('\n');
}
