#include "pagedir.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crawler.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

bool pagedir_init(const char *pageDirectory) { 
    DIR *direct = opendir(pageDirectory);
    if (direct != NULL) {       // Directory exists
        closedir(direct);
        return true;
    }

    // Directory does not exist → try to create it
    if (mkdir(pageDirectory, 0755) == 0) {
        return true;  // Successfully created
    }

    // Failed to open or create
    perror("pagedir_init");
    return false;
}

void pagedir_save(const webpage_t *page, const char *pageDirectory, const int documentID) { 
    FILE *fp;
    char filePath[100];
    sprintf(filePath, "%s/%d", pageDirectory, documentID);
    fp = fopen(filePath, "w");
    if (fp == NULL) {
        perror("pagedir_save");
        exit(1);
    }
    fprintf(fp, "%s\n%d\n%s", page->url, page->depth, page->html);
    fclose(fp);
}
