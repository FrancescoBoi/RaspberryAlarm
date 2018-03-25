#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<string.h>
#include<dirent.h>

void createFolder(char * path)
{
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        mkdir(path, 0700);
    }
}

void getDateStr(char str[20])
{
    char year[5], common[3];

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    str[0]='\0';
    sprintf(year, "%04d", tm.tm_year+1900);
    strcat(str, year);
    sprintf(common, "_%02d", tm.tm_mon + 1);

    strcat(str, common);

    sprintf(common, "_%02d", tm.tm_mday);
    strcat(str, common);

    sprintf(common, "_%02d", tm.tm_hour);
    strcat(str, common);

    sprintf(common, "_%02d", tm.tm_min);
    strcat(str, common);

    sprintf(common, "_%02d", tm.tm_sec);
    strcat(str, common);
    //printf("%s\n", str);

}

unsigned int countFiles(char* dir)
{
    unsigned int file_count = 0;
    DIR * dirp;
    struct dirent * entry;
    dirp = opendir(dir); /* There should be error handling after this */
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG) { /* If the entry is a regular file */
             file_count++;

        }
    }
    return file_count;

}

void printFiles(char* names[24], unsigned int file_count)
{
    for (int i=0; i<file_count; i++)
    {
        printf("%s\n", names[i]);
    }
}

unsigned int filesByName(char* names[24], char* dir)
{
    unsigned int file_count = 0;
    DIR * dirp;
    struct dirent * entry;
    dirp = opendir(dir); /* There should be error handling after this */

    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG) { /* If the entry is a regular file */
             //strncpy(names[file_count], entry->d_name,20);
            names[file_count] = entry->d_name;
             file_count++;
        }
    }
    closedir(dirp);
    char temp[24];
    if (file_count>0)
    {
        for (int i=0; i<file_count-1; i++)
        {
            for (int j=i; j<file_count; j++)
            {
                if (strcmp(names[i], names[j])>0)
                {
                    strncpy(temp, names[i],24);
                    strncpy(names[i], names[j],24);
                    strncpy(names[j], temp, 24);
                    }
            }
        }
    }
    return file_count;
}
