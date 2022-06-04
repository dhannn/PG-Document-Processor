#include "pgdocs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CONFIG_FILENAME "dat/config"

void __write_config(int index, char *str);
void __insert_int(int index, int data);

bool check_config_initialized()
{
    FILE *configFile = fopen(CONFIG_FILENAME, "r");
    bool flag = true;

    if(configFile == NULL)
        flag = false;

    fclose(configFile);

    return flag;
}

void load_config(Config *config)
{
    FILE *file = fopen(CONFIG_FILENAME, "r");
    int flag = 1;
    char *arr[] = {
        config->rawDocumentPath,
        config->cleanedDocumentPath,
        config->analysisOutputPath
    };

    char buff[MAX_CHAR];

    for(int i = 0; flag != EOF; i++) {
        flag = fscanf(file, "%s", buff);
        
        if(i < 3) {
            strcpy(arr[i], buff);
        }
    }

    fclose(file);
}

void set_config_path(Config *config, int index, char *path)
{

    switch(index) {
        case 0:
            strcpy(config->rawDocumentPath, path);
            break;
        case 1:
            strcpy(config->cleanedDocumentPath, path);
            break;
        case 2:
            strcpy(config->analysisOutputPath, path);
            break;
    }

    __write_config(index, path);
}

void set_config_int(Config *config, int index, int data)
{
    switch(index) {
        case 4:
            config->numChar = data;
            break;
        case 5:
            config->multiSelect = data;
    }

    char buff[MAX_CHAR];
    sprintf(buff, "%d", data);

    __write_config(index, buff);
}

void __write_config(int index, char *str)
{
    FILE *configFile = fopen(CONFIG_FILENAME, "r");
    FILE *temp = fopen("dat/temp", "w");

    char buff[MAX_CHAR];
    int flag = fscanf(configFile, "%s", buff);
    int i;

    for(i = 0; flag != EOF; i++) {
        if(index == i) {
            fprintf(temp, "%s\n", str);
        } else {
            fprintf(temp, "%s\n", buff);
        }

        flag = fscanf(configFile, "%s", buff);
    }

    if(i <= index) {
        fprintf(temp, "%s\n", str);
    }

    fclose(configFile);
    fclose(temp);

    remove(CONFIG_FILENAME);
    rename("dat/temp", CONFIG_FILENAME);
}
