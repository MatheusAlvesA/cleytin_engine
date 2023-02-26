#include "cleytin_sdcard.h"

CleytinSdcard::CleytinSdcard() {
    this->card = cleytin_mount_fs();
}

char **CleytinSdcard::list(const char *path, uint maxResults) {
    if(this->card == NULL) {
        return NULL;
    }
    char **result = (char **)malloc(sizeof(char*) * maxResults);

    DIR *d;
    struct dirent *entry;
    d = opendir(path);
    if(d == NULL) {
        cleytin_unmount_fs();
        this->card = NULL;
        return NULL;
    }
    size_t i = 0;
    while ((entry = readdir(d)) != NULL && i < maxResults) {
        size_t cursor = 0;
        result[i] = (char *)malloc(MAX_FILE_NAME_SIZE);
        while(entry->d_name[cursor] != '\0' && cursor < MAX_FILE_NAME_SIZE - 1) {
            result[i][cursor] = entry->d_name[cursor];
            cursor++;
        }
        result[i][cursor] = '\0';
        i++;
    }
    closedir(d);

    while(i < maxResults) {
        result[i] = NULL;
        i++;
    }

    return result;
}

char *CleytinSdcard::removeExtension(char *src) {
    char *res = NULL;
    uint32_t i = 0;
    uint32_t j = 0;
    while(src[i] != '\0') {
        if(src[i] == '.') {
            j = i;
        }
        i++;
    }
    if(src[j] != '.') {
        return NULL;
    }

    i = 0;
    res = (char *)malloc(j+1);
    while(i < j) {
        res[i] = src[i];
        i++;
    }
    res[i] = '\0';

    return res;
}

bool CleytinSdcard::isRegularFile(const char *path)
{
    struct stat path_stat;
    char *fullPath = cleytin_get_root_filesystem(strlen(path));
    strcat(fullPath, path);
    stat(fullPath, &path_stat);
    bool r = S_ISREG(path_stat.st_mode);
    free(fullPath);
    return r;
}

void load_file(void *path) {
    cleytin_load_game_rom((char *)path);
    vTaskDelete(NULL);
}

bool CleytinSdcard::loadFileToFlash(char *path) {
    if(cleytin_game_rom_load_progress > 0 && cleytin_game_rom_load_progress < 100) {
        return false;
    }

    if(!this->isRegularFile(path)) {
        return false;
    }

    xTaskCreate(load_file, "LOAD_FILE_TO_FLASH", 10*1024, path, 1, NULL);
    return true;
}

uint8_t CleytinSdcard::getLoadProgress() {
    return cleytin_game_rom_load_progress;
}
