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
<<<<<<< HEAD
    char year[5], common[4];
=======
    char year[5], common[3];
>>>>>>> a9f44282db2c70a8b4ac7e87009fd13ce41b26ab

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
<<<<<<< HEAD
        if (entry->d_type == DT_REG) /* If the entry is a regular file */
        {
=======
        if (entry->d_type == DT_REG) { /* If the entry is a regular file */
>>>>>>> a9f44282db2c70a8b4ac7e87009fd13ce41b26ab
             file_count++;

        }
    }
    return file_count;

}

<<<<<<< HEAD
void printFiles(char names[][24], unsigned int file_count)
=======
void printFiles(char* names[24], unsigned int file_count)
>>>>>>> a9f44282db2c70a8b4ac7e87009fd13ce41b26ab
{
    for (int i=0; i<file_count; i++)
    {
        printf("%s\n", names[i]);
    }
}

<<<<<<< HEAD

/*
 * The functions returns the oldest maxNameFiles2Retrieve files ordered by name
 * in the folder pics
 */
unsigned int filesByName(char names[][24], char* dir,
		unsigned int maxNameFiles2Retrieve)
=======
unsigned int filesByName(char* names[24], char* dir)
>>>>>>> a9f44282db2c70a8b4ac7e87009fd13ce41b26ab
{
    unsigned int file_count = 0;
    DIR * dirp;
    struct dirent * entry;
    dirp = opendir(dir); /* There should be error handling after this */
<<<<<<< HEAD
    char* localNames[24];
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG) { /* If the entry is a regular file */
             //strncpy(names[file_count], entry->d_name,20);
            //names[file_count] = malloc(24*sizeof(char));
        	localNames[file_count] = malloc(24*sizeof(char));
        	localNames[file_count] = strdup(entry->d_name);
=======

    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG) { /* If the entry is a regular file */
             //strncpy(names[file_count], entry->d_name,20);
            names[file_count] = entry->d_name;
>>>>>>> a9f44282db2c70a8b4ac7e87009fd13ce41b26ab
             file_count++;
        }
    }
    closedir(dirp);
    char temp[24];
    if (file_count>0)
    {
<<<<<<< HEAD
        if (file_count>maxNameFiles2Retrieve)
        {
        	printf("Warning: in filesByName: too much elements in folder to retrieve all the names\n");
        	file_count = maxNameFiles2Retrieve;
        }
=======
>>>>>>> a9f44282db2c70a8b4ac7e87009fd13ce41b26ab
        for (int i=0; i<file_count-1; i++)
        {
            for (int j=i; j<file_count; j++)
            {
<<<<<<< HEAD
                if (strcmp(localNames[i], localNames[j])>0)
                {
                    strncpy(temp, localNames[i],24);
                    strncpy(localNames[i], localNames[j],24);
                    strncpy(localNames[j], temp, 24);
                    }
            }
        }
        //Copy the oldest maxNameFiles2Retrieve files
        for (int i=0; i<file_count; i++)
        {
        	strcpy(names[i], localNames[i]);
        }
    }
    return file_count;
}

/*
 * The function retrieves the files (MAXNUMFILEINFOLDER at most)
 *  in the folder dir ordered alphabetically and  ruturns only
 *  the first maxNumPics elements*/
unsigned int removeFiles(char* dir, unsigned int maxNumPics, char names[][24])
{
	static const int MAXNUMFILEINFOLDER = 100;
	char allFiles[100][24];
	unsigned int numPicsInFolder = 0;
	char fileNameTodelete[73];
    strcat(fileNameTodelete, dir);
	while((numPicsInFolder =  filesByName(allFiles, dir, MAXNUMFILEINFOLDER))>maxNumPics)
	{

		fileNameTodelete[0] = '\0';
		strcat(fileNameTodelete, dir);
		strcat(fileNameTodelete, allFiles[0]);
		printf("%s\n", fileNameTodelete);
		remove(fileNameTodelete);
	}
	for (int i=0; i<numPicsInFolder; i++)
	{
		strcpy(names[i], allFiles[i]);
	}
	return numPicsInFolder;
}
=======
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
>>>>>>> a9f44282db2c70a8b4ac7e87009fd13ce41b26ab
