#ifndef COLORS_H
#define COLORS_H

/* ══════════════════════════════════════════════════════════
   CROSS-PLATFORM TERMINAL CAPABILITY DETECTION
   Call term_init() once at program start (before any output).
   ══════════════════════════════════════════════════════════ */
extern int TERM_HAS_COLOR;   /* 1 = 256-colour ANSI supported  */
extern int TERM_HAS_UTF8;    /* 1 = UTF-8 box/symbol chars OK   */
void term_init(void);
int utf8_display_len(const char *str);

/* ══════════════════════════════════════════════════════════
   ANSI TEXT FORMATTING  (basic SGR — always safe)
   ══════════════════════════════════════════════════════════ */
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"

/* ══════════════════════════════════════════════════════════
   GLOBAL LAYOUT PADDING
   ══════════════════════════════════════════════════════════ */
#define UI_PAD      "          "   /* 10 spaces */

/* ══════════════════════════════════════════════════════════
   COLOUR PALETTE  (256-colour — filtered at runtime by term_write)
   ══════════════════════════════════════════════════════════ */
#define C_PRIMARY   "\033[38;5;252m"   /* Crisp Gray        */
#define C_ACCENT    "\033[38;5;42m"    /* Emerald Green     */
#define C_PURPLE    "\033[38;5;214m"   /* Muted Gold        */
#define C_SUCCESS   "\033[38;5;46m"    /* Bright Green      */
#define C_WARNING   "\033[38;5;214m"   /* Solar Gold        */
#define C_ERROR     "\033[38;5;196m"   /* Alert Red         */
#define C_INFO      "\033[38;5;250m"   /* Light Slate       */
#define C_VALUE     "\033[38;5;255m"   /* Pure White        */
#define C_DIM       "\033[38;5;240m"   /* Dark Gray         */
#define C_BORDER    "\033[38;5;245m"   /* Slate Gray        */
#define C_BORDER2   "\033[38;5;237m"   /* Charcoal          */
#define C_HIGHLIGHT "\033[38;5;255m"   /* Bright White      */
#define C_MUTED     "\033[38;5;246m"   /* Mid Gray          */

/* ── Rank colours ────────────────────────────────────────── */
#define C_RANK1     "\033[38;5;220m"   /* Gold              */
#define C_RANK2     "\033[38;5;250m"   /* Silver            */
#define C_RANK3     "\033[38;5;208m"   /* Bronze            */

/* ── Background highlights ──────────────────────────────── */
#define BG_SELECT   "\033[48;5;236m"   /* Dark Charcoal     */

/* ══════════════════════════════════════════════════════════
   BOX / BORDER CHARACTERS
   Use tc_*() helpers that return UTF-8 or ASCII based on runtime flag.
   ══════════════════════════════════════════════════════════ */
static inline const char *tc_b(const char *utf8, const char *ascii) {
    return TERM_HAS_UTF8 ? utf8 : ascii;
}

/* Double-line box */
#define DBL_TL  tc_b("\xe2\x95\x94", "+")   /* ╔ */
#define DBL_TR  tc_b("\xe2\x95\x97", "+")   /* ╗ */
#define DBL_BL  tc_b("\xe2\x95\x9a", "+")   /* ╚ */
#define DBL_BR  tc_b("\xe2\x95\x9d", "+")   /* ╝ */
#define DBL_V   tc_b("\xe2\x95\x91", "|")   /* ║ */
#define DBL_H   tc_b("\xe2\x95\x90", "=")   /* ═ */

/* Single-line box */
#define BOX_TL  tc_b("\xe2\x95\xad", "+")   /* ╭ */
#define BOX_TR  tc_b("\xe2\x95\xae", "+")   /* ╮ */
#define BOX_BL  tc_b("\xe2\x95\xb0", "+")   /* ╰ */
#define BOX_BR  tc_b("\xe2\x95\xaf", "+")   /* ╯ */
#define BOX_V   tc_b("\xe2\x94\x82", "|")   /* │ */
#define BOX_H   tc_b("\xe2\x94\x80", "-")   /* ─ */

/* ══════════════════════════════════════════════════════════
   SYMBOLS
   ══════════════════════════════════════════════════════════ */
#define SYM_ARROW   tc_b("\xe2\x96\xb6",   ">")    /* ▶  */
#define SYM_RETURN  tc_b("\xe2\x8f\x8e",   "<")    /* ⏎  */
#define SYM_CHECK   tc_b("\xe2\x9c\x94",   "v")    /* ✔  */
#define SYM_CROSS   tc_b("\xe2\x9c\x98",   "x")    /* ✘  */
#define SYM_STAR    tc_b("\xe2\x98\x85",   "*")    /* ★  */
#define SYM_WARN    tc_b("\xe2\x9a\xa0",   "!")    /* ⚠  */
#define SYM_DOT     tc_b("\xe2\x80\xa2",   ".")    /* •  */
#define SYM_DIAMOND tc_b("\xe2\x97\x86",   "#")    /* ◆  */
#define SYM_BOLT    tc_b("\xe2\x9a\xa1",   "~")    /* ⚡ */
#define SYM_BAR_F   tc_b("\xe2\x96\x88",   "#")    /* █  */
#define SYM_BAR_E   tc_b("\xe2\x96\x91",   "-")    /* ░  */
#define SYM_RANK1   tc_b("\xe2\x98\x85\xe2\x98\x85\xe2\x98\x85", "***")  /* ★★★ */
#define SYM_RANK2   tc_b("\xe2\x98\x85\xe2\x98\x85\xe2\x98\x86", "** ")  /* ★★☆ */
#define SYM_RANK3   tc_b("\xe2\x98\x85\xe2\x98\x86\xe2\x98\x86", "*  ")  /* ★☆☆ */

/* ══════════════════════════════════════════════════════════
   BOX DIMENSIONS
   ══════════════════════════════════════════════════════════ */
#define BOX_WIDTH   56
#define BOX_INNER   54   /* BOX_WIDTH - 2 */

/* ══════════════════════════════════════════════════════════
   COLOUR FILTER WRAPPER
   When TERM_HAS_COLOR == 0, color escape sequences are suppressed.
   Use tc_col() to print a color code safely.
   ══════════════════════════════════════════════════════════ */
static inline const char *tc_col(const char *esc) {
    return TERM_HAS_COLOR ? esc : "";
}

/* Convenience: wrap every color macro for no-colour terminals.
   Use these in printf calls instead of bare C_XXX macros.       */
#define CC(x)   tc_col(x)

/* ══════════════════════════════════════════════════════════
   SCREEN MANAGEMENT
   \033[3J clears scrollback (prevents password scroll-reveal)
   ══════════════════════════════════════════════════════════ */
#define CLEAR_SCREEN() printf("\033[2J\033[3J\033[H")

/* ══════════════════════════════════════════════════════════
   UI COMPONENT DECLARATIONS
   ══════════════════════════════════════════════════════════ */
void boxTop(void);
void boxBottom(void);
void boxRowRaw(const char *text, int displayLen);

void printBanner(const char *text, int textLen);
void printSectionHeader(const char *text);
void printHRule(int width);

void inputPrompt(const char *label);

void printSkillBar(const char *label, float value, int width);
void printScoreBar(float value, int width);

void printKV(const char *key, const char *value);
void printKVf(const char *key, float value);

void printSuccess(const char *msg);
void printError(const char *msg);
void printWarning(const char *msg);
void printInfo(const char *msg);

void pauseScreen(void);

#endif /* COLORS_H */
