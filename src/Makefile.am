# Flags to set
# another possibility is -pedantic
AM_CFLAGS = -std=c99 -Wall -Wextra -pedantic

# Target
bin_PROGRAMS = repo
repo_SOURCES = repo.h main.c \
               actions.h actions.c
repo_LDADD = -lm libcassava/libcassava.a
