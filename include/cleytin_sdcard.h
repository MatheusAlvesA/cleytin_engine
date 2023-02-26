#ifndef CLEYTIN_SDCARD_H
#define CLEYTIN_SDCARD_H

#include <dirent.h> 
#include <sys/stat.h>
#include "cleytin_commons.h"

#define MAX_FILE_NAME_SIZE 200
#define MAX_FILE_LIST_SIZE 200

class CleytinSdcard {
public:
    CleytinSdcard();
    char **list(const char *path = "/sdcard", uint maxResults = MAX_FILE_LIST_SIZE);
    char *removeExtension(char *src);
    bool isRegularFile(const char *path);
    bool loadFileToFlash(char *path);
    uint8_t getLoadProgress();

private:
    sdmmc_card_t *card;
};

#endif
