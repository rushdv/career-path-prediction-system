CC     = gcc
CFLAGS = -Wall -Wextra -Iinclude -Wno-unused-parameter -Wno-implicit-fallthrough
SRC    = src/main.c src/auth.c src/student.c \
         src/assessment.c src/prediction.c   \
         src/gap_analysis.c src/db_handler.c \
         src/report.c src/ui.c src/colors.c \
         src/input_handler.c src/analytics.c \
         src/sha256.c src/sqlite3.c

all: career_system

career_system: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o career_system -lpthread -ldl

clean:
	rm -f career_system career_system.exe