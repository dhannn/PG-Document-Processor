/*******************************************************************************
 * 
 * FILE             .config.c
 * LAST MODIFIED    06-17-2022
 * 
 * DESCRIPTION
 *      This file contains function implementations that handles config
 * 
 ******************************************************************************/

#include "pgdocs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CONFIG_FILENAME "dat/config"

/* -------------------------------------------------------------------------- */
/*                         PRIVATE FUNCTION PROTOTYPES                        */
/* -------------------------------------------------------------------------- */
/**
 * write_config()
 * writes to the config file a specified data
 * 
 * @param           int         the index of the config item
 * @param           char*       the data in the config item
 */
void __write_config(int index, char *str);

/* -------------------------------------------------------------------------- */
/*                              PUBLIC FUNCTIONS                              */
/* -------------------------------------------------------------------------- */
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
    char *paths[] = {
        config->rawDocumentPath,
        config->cleanedDocumentPath,
        config->analysisOutputPath
    };

    int *ints[2] = {
        &(config->numChar),
        &(config->numDocs)
    };

    char buff[MAX_CHAR];

    for(int i = 0; flag != EOF && i < 5; i++) {
        flag = fscanf(file, "%s", buff);
        
        if(i < 3) {
            strcpy(paths[i], buff);
        } else {
            *(ints[i - 3]) = atoi(buff);
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
            config->numDocs = data;
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
