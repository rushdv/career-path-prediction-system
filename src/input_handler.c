#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#  include <conio.h>
#else
#  include <termios.h>
#  include <unistd.h>
#  include <fcntl.h>
#endif
#include "input_handler.h"
#include "colors.h"

/* ══════════════════════════════════════════════════════════
   BASIC INPUT HELPERS
   ══════════════════════════════════════════════════════════ */
void getStringInput(char *buffer, int max_len) {
    if (!buffer || max_len <= 0) return;
    buffer[0] = '\0';
    while (1) {
        if (fgets(buffer, max_len, stdin) != NULL) {
            buffer[strcspn(buffer, "\r\n")] = 0;
            if (strlen(buffer) > 0) return;
        }
        printf(UI_PAD "%s%s%s Input cannot be empty. Try again: %s",
               C_ERROR, SYM_CROSS, RESET, RESET);
    }
}

float getFloatInput(float min, float max) {
    char buf[128];
    float val;
    while (1) {
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            if (sscanf(buf, "%f", &val) == 1 && val >= min && val <= max)
                return val;
        }
        printf(UI_PAD "%s%s%s Enter a number between %.1f and %.1f: %s",
               C_ERROR, SYM_CROSS, RESET, min, max, RESET);
    }
}

int getIntInput(void) {
    char buf[128];
    int val;
    while (1) {
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            if (sscanf(buf, "%d", &val) == 1)
                return val;
        }
        printf(UI_PAD "%s%s%s Please enter a valid integer: %s",
               C_ERROR, SYM_CROSS, RESET, RESET);
    }
}

int getIntInputRange(int min, int max) {
    char buf[128];
    int val;
    while (1) {
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            if (sscanf(buf, "%d", &val) == 1 && val >= min && val <= max)
                return val;
        }
        printf(UI_PAD "%s%s%s Enter a number between %d and %d: %s",
               C_ERROR, SYM_CROSS, RESET, min, max, RESET);
    }
}

/* ══════════════════════════════════════════════════════════
   RAW TERMINAL MODE HELPERS
   ══════════════════════════════════════════════════════════ */
#ifdef _WIN32
void restoreTerminal(void) {}
#else
static struct termios orig_termios;

void restoreTerminal(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

static void enableRawMode(void) {
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
#endif

/* ══════════════════════════════════════════════════════════
   INTERACTIVE MENU SELECTION
   Supports arrow keys and number-key shortcuts.
   BOX_INNER (52) = BOX_WIDTH (56) - 4 visible chars for row content.
   ══════════════════════════════════════════════════════════ */

/*  Visible widths for each row type:
    Selected  :  "  > [ N ]  <option padded to 37> "  = 52
    Normal    :  "     [ N ]  <option padded to 37> "  = 52
    Separator : (empty row)                              = 0
*/
#define MENU_OPTION_PAD 37   /* option text field width */
#define MENU_PREFIX_W   15   /* "  > [ N ]  " or "     [ N ]  " */

/* How many terminal lines does the menu block occupy? Used for in-place redraw. */
static int menuBlockHeight(int num_options, int has_subtitle) {
    /*  1 blank line before banner
        3 banner lines (top border + title + bottom border)
        1 blank line after banner
        [3 subtitle lines: blank + text + blank]  if subtitle present
        1 boxTop
        num_options rows
        1 boxBottom
        1 blank line
        1 footer line
        1 trailing newline                                           */
    int h = 9 + num_options;
    if (has_subtitle) h += 3;
    return h;
}

int getMenuSelection(const char *title, const char *subtitle,
                     const char **options, int num_options) {
    int selected  = 0;
    int first_draw = 1;

    while (1) {
        /* ── In-place redraw (erase previous render) ──── */
        if (!first_draw) {
            int lines = menuBlockHeight(num_options, subtitle != NULL);
            printf("\033[%dA\033[J", lines);
        }
        first_draw = 0;

        /* ── Banner ──────────────────────────────────── */
        printf("\n");
        {
            char tBuf[160];
            int tLen = (int)strlen(title);
            snprintf(tBuf, sizeof(tBuf), "  %s  ", title);
            printBanner(tBuf, tLen + 4);
        }
        printf("\n");

        /* ── Subtitle ─────────────────────────────────── */
        if (subtitle) {
            printf("\n" UI_PAD "%s%s%s\n\n", C_DIM, subtitle, RESET);
        }

        /* ── Menu card ────────────────────────────────── */
        boxTop();
        for (int i = 0; i < num_options; i++) {
            char rowBuf[640];
            if (i == selected) {
                snprintf(rowBuf, sizeof(rowBuf),
                         "%s%s   %s [ %s%d%s ]  %s%s",
                         BG_SELECT, C_HIGHLIGHT,
                         SYM_ARROW,
                         C_ACCENT, i + 1, C_HIGHLIGHT,
                         options[i],
                         RESET);
                int visLen = 12 + utf8_display_len(options[i]);
                if (i >= 9) visLen++; /* 2 digits */
                boxRowRaw(rowBuf, visLen);
            } else {
                snprintf(rowBuf, sizeof(rowBuf),
                         "%s     [ %s%d%s ]  %s%s%s",
                         C_DIM,
                         C_PRIMARY, i + 1, C_DIM,
                         RESET,
                         options[i],
                         RESET);
                int visLen = 12 + utf8_display_len(options[i]);
                if (i >= 9) visLen++; /* 2 digits */
                boxRowRaw(rowBuf, visLen);
            }
        }
        boxBottom();

        /* ── Footer nav bar ───────────────────────────── */
        printf("\n" UI_PAD "%s", CC(C_BORDER2)); fputs(BOX_H, stdout);
        printf(" %s%s%s Up/Down  %s", CC(C_ACCENT), SYM_ARROW, CC(C_MUTED), CC(C_BORDER2));
        fputs(BOX_H, stdout); fputs(BOX_H, stdout);
        printf(" 1-%d Keys  %s", num_options, CC(C_BORDER2));
        fputs(BOX_H, stdout); fputs(BOX_H, stdout);
        printf(" %s%s%s Enter  %s", CC(C_SUCCESS), SYM_RETURN, CC(C_MUTED), CC(C_BORDER2));
        fputs(BOX_H, stdout); printf("%s\n", CC(RESET));

        fflush(stdout);

        /* ── Key handler ──────────────────────────────── */
#ifdef _WIN32
        int ch = _getch();
        if (ch == 0 || ch == 224) {
            int seq = _getch();
            if (seq == 72)      selected--;          /* Up   */
            else if (seq == 80) selected++;          /* Down */
            if (selected < 0)            selected = num_options - 1;
            if (selected >= num_options) selected = 0;
        } else if (ch >= '1' && ch < '1' + num_options) {
            return ch - '1';
        } else if (ch == '\r' || ch == '\n') {
            return selected;
        }
#else
        enableRawMode();
        int c = getchar();
        if (c == '\033') {
            /* Read escape sequence non-blocking */
            int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
            fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
            int seq0 = getchar();
            int seq1 = getchar();
            fcntl(STDIN_FILENO, F_SETFL, flags);
            restoreTerminal();

            if (seq0 == '[') {
                if (seq1 == 'A') { selected--; }   /* Up   */
                if (seq1 == 'B') { selected++; }   /* Down */
            }
            if (selected < 0)            selected = num_options - 1;
            if (selected >= num_options) selected = 0;
        } else if (c >= '1' && c < '1' + num_options) {
            restoreTerminal();
            return c - '1';
        } else if (c == '\n' || c == '\r') {
            restoreTerminal();
            return selected;
        } else {
            restoreTerminal();
        }
#endif
    }
}
