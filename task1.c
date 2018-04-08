#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<wiringPi.h>
#include"globals.h"
#include "utils.h"
#include<pthread.h>
#define INPIN 25

void exitingThreadTask1(void* arg);
void takePic(char picname[24]);

//This thread reads the PIR sensor output
void* task1(void *arg)
{
    unsigned char val_read = 0;
    static unsigned char alarm_on = FALSE;
    static const unsigned int maxNumPics=10;
    static char folderPath[] = "./pics/";
    char fileNameTodelete[31];
    fileNameTodelete[0] = '\0';
    strcat(fileNameTodelete, folderPath);
    //INITIALIZING
    pinMode(INPIN, INPUT);
    pinMode(OUTPIN, OUTPUT);
    digitalWrite(OUTPIN, LOW); //buzz if writing LOW
    digitalWrite(INPIN, LOW);
    pthread_cleanup_push(exitingThreadTask1, NULL);
    //create folder where to save pics
    createFolder(folderPath);

<<<<<<< HEAD
    char names[10][24];
    char picname[24];
    int res = 0;
    picname[0] = '\0';
    static unsigned int numPicsInFolder;
    //delete if more than 10 files
<<<<<<< HEAD
    numPicsInFolder = removeFiles(folderPath, maxNumPics, names);

    static unsigned int nexEl;
    nexEl = numPicsInFolder % maxNumPics;
    printf("Entering while\n");
    while(1)
    {
        //static const unsigned int del = 300;
        val_read = digitalRead(INPIN);
        if (val_read && (!alarm_on)) //motion detected
        {
            printf("\nDetected movement\n");
            if (numPicsInFolder >= maxNumPics)
            {
                printf("\nMax num pics\n");
                fileNameTodelete[0] = '\0';
                strcat(fileNameTodelete, folderPath);
                strcat(fileNameTodelete, names[nexEl]);
                printFiles(names, numPicsInFolder);
                printf("File to be deleted %d: %s, ", nexEl, names[nexEl]);

                if ((res = remove(fileNameTodelete))!=0)
                {
                    printf("Error deleting file: %d\n", res);
                }

            }
            else
            {
                printf("\nNot reached max num pics\n");
                numPicsInFolder++;
            }
            //update buffer
            takePic(picname);
            printf("value returned by takePic: %s\n", picname);
            strcpy(names[nexEl], picname);
            printFiles(names, numPicsInFolder);

            printf("curr element %d: %s\n",nexEl, names[nexEl]);
            nexEl++;
            nexEl %= maxNumPics;
            digitalWrite(OUTPIN, LOW);
            printf("\nDetected movement: alarm tripped\n\n");
            alarm_on = TRUE;
            /*Give some time before another pic*/
        }
        else if (alarm_on && !val_read)
        {
            alarm_on = FALSE;
            digitalWrite(OUTPIN, HIGH);
            printf("\nAlarm backed off\n\n");
        }
    }
    pthread_cleanup_pop(0);
    pthread_exit(0);

}


void takePic(char picname[24])
{
    /*Build string to take picture*/
    int err;
    //finalcmd is very long
    char finalcmd[150], finalcmd_ln[150];
    finalcmd[0] = '\0';
    finalcmd_ln[0] = '\0';
    getDateStr(picname);

    char cmd1[] = "raspistill --nopreview -w 1280 -h 720 -q 80 -o /home/pi/alarmSys/pics/";
    char cmdlast[] = " -t 2 -th none";

    strcat(finalcmd, cmd1);
    strcat(picname, ".jpg");
    strcat(finalcmd, picname);
    strcat(finalcmd, cmdlast);
    system(finalcmd);

    /*Build string to create symbolic link*/
    strcat(finalcmd_ln, "sudo ln /home/pi/alarmSys/pics/");
    strcat(finalcmd_ln, picname);
<<<<<<< HEAD
=======
    strcat(finalcmd_ln, ".jpg");
>>>>>>> a9f44282db2c70a8b4ac7e87009fd13ce41b26ab
    strcat(finalcmd_ln, " /var/www/html/pic.jpg");
    //printf("%s\n", finalcmd_ln);
    if ((err=remove("/var/www/html/*.jpg"))!=0)
    {
        printf("Error deleting /var/www/html/*.jpg, maybe not existing\n" );
    }
    system(finalcmd_ln);
    pthread_mutex_lock(&g_new_pic_m);
    g_new_pic_flag = TRUE;
    pthread_mutex_unlock(&g_new_pic_m);
    printf("\nPicture taken\n\n");

}


void exitingThreadTask1(void* arg)
{
    printf("Thread of task 1 exiting\n");
    digitalWrite(OUTPIN, LOW);
    digitalWrite(INPIN, LOW);
    printf("Pins lowered\n");
}
