/*
 * ls.c
 * Lists the file in given directories, similar to ls(1).
 */

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_OUT_LINE 1024
#define MAX_FILES 1024


typedef struct list_node {
    char *data;
    struct list_node *next;
} Node;

/*
 * get_regex_files: get all files in dir that match regex.
 * A pointer to the head of a linked list is returned.
 * Warning: make sure to call list_free_all() on the returned Node when done.
 */
Node *get_regex_files(const char *regex, const char *dir)
{
    char errbuf[MAX_OUT_LINE];      // for holding error messages by regex.h
    struct dirent *entry;
    DIR *dp;
    int errcode;
    Node *head = NULL;

    // open db dir
    dp = opendir(dir);
    if (dp == NULL) {
        perror("Error (opendir)");
        return NULL;
    }
    
    // compile regex
    regex_t preg;
    errcode = regcomp(&preg, regex, REG_EXTENDED);
    if (errcode != 0) {
        regerror(errcode, &preg, errbuf, sizeof errbuf);
        fprintf(stderr, "Error (regcomp): %s\n", errbuf);
        return NULL;
    }

    Node *current = NULL;
    while ((entry = readdir(dp)) != NULL) {
        // make sure type is not dir, then do a regex on it
        struct stat fstat;
        if (stat(entry->d_name, &fstat) != 0) {
            perror("stat");
            return NULL;
        }
        if (! S_ISREG(fstat.st_mode))
            continue;

        errcode = regexec(&preg, entry->d_name, 0, NULL, 0);
        if (errcode == 0) {
            Node *new = malloc(sizeof (Node));
            new->data = malloc((strlen(entry->d_name)+1) * sizeof (char));
            strcpy(new->data, entry->d_name);
            if (current == NULL) {
                head = current = new;
            } else {
                current->next = new;
                current = new;
            }
        } else if (errcode == REG_NOMATCH) {
            continue;
        } else {
            regerror(errcode, &preg, errbuf, sizeof errbuf);
            fprintf(stderr, "Error (regexec): %s\n", errbuf);
            return NULL;
        }
    }

    if (current != NULL)
        current->next = NULL;
    return head;
}

int listdir(const char *path)
{
    struct dirent *entry;
    struct stat sfile;
    DIR *dp;
    
    stat(path, &sfile);
    if (S_ISREG(sfile.st_mode)) {
        puts(path);
        return 0;
    }

    dp = opendir(path);
    if (dp == NULL) {
        perror("opendir");
        return -1;
    }
     
    while((entry = readdir(dp))) {
        stat(entry->d_name, &sfile);
        if (S_ISDIR(sfile.st_mode))
            printf("%s/\n", entry->d_name);
        else
            puts(entry->d_name);
    }
 
    closedir(dp);
    return 0;
}


int main(int argc, char **argv)
{
    int counter = 1;
    
    if (argc == 1)
        listdir(".");
    
    while (++counter <= argc) {
        Node *head;

        printf("\nListing '%s'...\n", argv[counter-1]);
        head = get_regex_files(argv[counter-1], ".");
        while (head != NULL) {
            Node *node = head;
            puts(node->data);
            free(node->data);
            head = head->next;
            free(node);
        }
    }

    return 0;
}
