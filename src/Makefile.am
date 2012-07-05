# Flags to set
# another possibility is -pedantic
AM_CFLAGS = -std=c99 -Wall -Wextra -pedantic

# Target
bin_PROGRAMS = repo
repo_SOURCES = repo.h repo.c \
               actions.h actions.c
repo_LDADD   = libcassava/libcassava.a
repo_LDFLAGS = -lm

EXTRA_DIST = libcassava
