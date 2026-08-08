#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif
#include "input_handler.h"
#include "colors.h"

void getStringInput(char *buffer, int max_len) {
    if (!buffer || max_len <= 0) return;
    buffer[0] = '\0';
    while (1) {
        if (fgets(buffer, max_len, stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = 0;
            if (strlen(buffer) > 0) return;
        }
        printf("  " C_ERROR "Input cannot be empty. Please try again: " RESET);
    }
}

float getFloatInput(float min, float max) {
    char buf[128];
    float val;
    while (1) {
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            if (sscanf(buf, "%f", &val) == 1) {
                if (val >= min && val <= max) {
                    return val;
                }
            }
        }
        printf("  " C_ERROR "Invalid input. Enter a number between %.1f and %.1f: " RESET, min, max);
    }
}

int getIntInput(void) {
    char buf[128];
    int val;
    while (1) {
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            if (sscanf(buf, "%d", &val) == 1) {
                return val;
            }
        }
        printf("  " C_ERROR "Invalid input. Please enter a valid integer: " RESET);
    }
}

/* Helper to set terminal mode */
#ifdef _WIN32
static void disableRawMode(void) {}
static void enableRawMode(void) {}
#else
static struct termios orig_termios;
static void disableRawMode(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
static void enableRawMode(void) {
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
#endif

int getMenuSelection(const char *title, const char *subtitle, const char **options, int num_options) {
    int selected = 0;
    char c;
    
    int first_draw = 1;
    while (1) {
        if (!first_draw) {
            int lines_up = num_options + 9;
            if (subtitle) lines_up += 2;
            printf("\033[%dA", lines_up);
        }
        first_draw = 0;
        
        printf("\n");
        char tBuf[128];
        snprintf(tBuf, sizeof(tBuf), "  %s  ", title);
        printBanner(tBuf, strlen(title) + 4);
        printf("\n");
        
        if (subtitle) {
            printf("%s\n\n", subtitle);
        }
        
        boxTop();
        for (int i = 0; i < num_options; i++) {
            char rowBuf[256];
            int visible_len = 41;
            if (i == selected) {
                snprintf(rowBuf, sizeof(rowBuf), 
                         C_SUCCESS BOLD "> %-38s " RESET, options[i]);
            } else {
                snprintf(rowBuf, sizeof(rowBuf), 
                         "  %-38s ", options[i]);
            }
            boxRowRaw(rowBuf, visible_len);
        }
        boxBottom();
        
        printf("\n  " C_DIM "Use Up/Down arrows to navigate, Enter to select." RESET "\n");
        fflush(stdout);
        
#ifdef _WIN32
        int ch = _getch();
        if (ch == 0 || ch == 224) {
            int seq = _getch();
            if (seq == 72) { selected--; } /* Up */
            else if (seq == 80) { selected++; } /* Down */
            
            if (selected < 0) selected = num_options - 1;
            if (selected >= num_options) selected = 0;
        } else if (ch == '\r' || ch == '\n') {
            return selected;
        }
#else
        enableRawMode();
        c = getchar();
        if (c == '\033') { 
            int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
            fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
            
            int seq0 = getchar();
            int seq1 = getchar();
            
            fcntl(STDIN_FILENO, F_SETFL, flags);
            
            if (seq0 == '[') {
                if (seq1 == 'A') { selected--; } /* Up */
                if (seq1 == 'B') { selected++; } /* Down */
            }
            if (selected < 0) selected = num_options - 1;
            if (selected >= num_options) selected = 0;
            disableRawMode();
        } else if (c == '\n' || c == '\r') {
            disableRawMode();
            return selected;
        } else {
            disableRawMode();
        }
#endif
    }
}
