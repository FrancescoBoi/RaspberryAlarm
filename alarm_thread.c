#include<wiringPi.h>
#include<stdlib.h>
#include<stdio.h>
#include<signal.h>
#include<stdint.h>
#include<curl/curl.h>
#include<string.h>
#include<pthread.h>
#include <unistd.h>
#include "task1.h"
#include"err.h"
#include "globals.h"

/* GLOBAL VARIABLES IMPLEMENTATION */
float g_avg_temp = 0.0;
float g_avg_hum = 0.0;

unsigned char g_new_pic_flag=FALSE;
pthread_cond_t g_new_pic_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t g_new_pic_m = PTHREAD_MUTEX_INITIALIZER;

/* END GLOBAL VARIABLES DECLARATION */

/*We define thread exit functions so that each pin
is lowered by the thread in which it is used avoiding
race condition between the signal handler of the main thread
and the other threads*/
void exitingThreadTask2(void* arg);
void exitingThreadTask3(void* arg);

/* Read Temperature and Humidity value from sensor*/
void read_dht11_dat(float* last_valid_T, float* last_valid_H);

float convertVal2Float(unsigned int int_part, unsigned int dec_part);

void* task2(void *arg); //thread function for the temperature reading
void* task3(void *arg); //thread function to post data on IOT platforms

/*Signal handler to return from pause*/
void sig_handler(int signo);

int main()
{
    int err, signo;
    sigset_t omask, mask, maskSIGINT;
    pthread_t thread_motionSensor;
    pthread_t thread_tempReading;
    pthread_t thread_platformPost;
    printf("Created threads IDs\n");

    if (wiringPiSetup()<0)
    {
        printf("WiringPi error\n");
        return -1;
    }
    printf("WiringPi is ok\n");

    /*Create a new mask to block all signals for the following thread*/
    sigfillset(&mask);
    sigemptyset(&maskSIGINT);
    pthread_sigmask(SIG_SETMASK, &mask, &omask);
    printf("Trying to create threads\n");
    if ((err = pthread_create (&thread_motionSensor, NULL, task1, NULL))!=0)
    {
  	printf("Thread 1 not created: error %d\n", err);
        err_exit((const char)err, "pthread_create error");
    }
    printf("Thread 1 created. Trying to create Thread 2\n");
    if((err = pthread_create (&thread_tempReading,   NULL, task2, NULL))!=0)
    {
  	printf("Thread 2 not created: error %d\n", err);
        err_exit((const char)err, "pthread_create error");
    }
    printf("Thread 2 created. Trying to create Thread 3\n");
    if ((err = pthread_create (&thread_platformPost, NULL, task3, NULL))!=0)
    {
  	 printf("Thread 3 not created: error %d %d\n", err);
         err_exit((const char)err, "pthread_create error");
    }
    printf("Thread 3 created\n");
    /*The main thread must block the SIGALRM but catch
    SIGQUIT, SIGTERM, SIgkILL*/
    sigemptyset(&omask);

    sigaddset(&omask, SIGQUIT);
    sigaddset(&omask, SIGKILL);
    sigaddset(&omask, SIGTERM);
    sigaddset(&maskSIGINT, SIGINT);
    pthread_sigmask(SIG_UNBLOCK, &omask, NULL);
    printf("Main thread waiting for signal\n");
    err = sigwait(&maskSIGINT, &signo);
    if (err!=0)
        err_exit(err, "sigwait failed\n");
    printf("Exit signal received: cancelling threads\n");

    pthread_cancel(thread_motionSensor);
    pthread_cancel(thread_tempReading);
    pthread_cancel(thread_platformPost);
    pthread_join(thread_motionSensor, NULL);
    pthread_join(thread_tempReading,  NULL);
    pthread_join(thread_platformPost, NULL);
    printf("Exiting from main thread and process\n");
    exit(0);
}

// This task reads the output from DHT11
void* task2(void *arg)
{
    static const unsigned char schedule_time = 5;
    static unsigned char oldest_val_idx = 0;
    static float temp_arr[10] = {0,0,0,0,0,0,0,0,0,0};
    static float hum_arr[10] = {0,0,0,0,0,0,0,0,0,0};
    static float sum_t =0.0;
    static float sum_h =0.0;
    static float last_valid_T = 0, last_valid_H =0;
    float temp, hum;
    int signo, err;
    /*
    We set a local mask with SIGALARM for the function sigwait
    All signals have already been blocked
    */
    sigset_t alarm_mask;
    sigemptyset(&alarm_mask);
    sigaddset(&alarm_mask, SIGALRM);
    alarm(schedule_time);
    pthread_cleanup_push(exitingThreadTask2, NULL);
    while (1)
    {
        err = sigwait(&alarm_mask, &signo); //signo == SIGALRM check
        if (err!=0)
            err_exit(err, "sigwait failed\n");
        read_dht11_dat(&temp, &hum);

        if (temp>-273.0)//valid data
        {
            last_valid_T = temp;
            last_valid_H = hum;
            // update the vectors
            sum_t = sum_t + temp - temp_arr[oldest_val_idx];
            sum_h = sum_h + hum - hum_arr[oldest_val_idx];
            g_avg_temp = sum_t/10.0;
            g_avg_hum = sum_h/10.0;
            temp_arr[oldest_val_idx] = temp;
            hum_arr[oldest_val_idx] = hum;
            //recalculate the average

            oldest_val_idx++;
            if(oldest_val_idx==10) oldest_val_idx =0;

        }
        printf("Last valid value: T: %f *C, H: %f \%\n", last_valid_T, last_valid_H);
        printf("Average values: %f C, %f\%\n", g_avg_temp, g_avg_hum);
        alarm(schedule_time);
    }
    pthread_cleanup_pop(0);
    pthread_exit(0);
}

void* task3(void *arg)
{

    //IFTTT
    static const char str[] = "curl -X POST -H \"Content-Type: application/json\" -d '{\"value1\":\"";
    static const char str2[]= "\"}' https://maker.ifttt.com/trigger/";
    static const char event_name[] = "evento"; //PLATFORM: change with the name of your event
    static const char str3[] = "/with/key/";
    static const char key[] = "..."; //PLATFORM put your key here
    //THINGSPEAK
    char final_str_ts[100];
    static const char key_ts[] = "..."; //PLATFORM sostittuire la propria key
    static const char str_ts[] = "curl --data \"key=";
    static const char str2_ts[]= "&field1=";
    static const char str3_ts[] = "\" http://api.thingspeak.com/update";
    char final_str[100];
    char val_str[5];
    pthread_cleanup_push(exitingThreadTask3, NULL);
    while(1)
    {
        pthread_mutex_lock(&g_new_pic_m);
        while(g_new_pic_flag==FALSE)
        {
            pthread_cond_wait(&g_new_pic_cond, &g_new_pic_m);
        }
        pthread_mutex_unlock(&g_new_pic_m);

        //IFTTT post
        strcpy(final_str, str);
        snprintf(val_str, 5, "%2f", g_avg_temp);
        strcat(final_str, val_str);
        strcat(final_str, str2);
        strcat(final_str, event_name);
        strcat(final_str, str3);
        strcat(final_str, key);
        //system(final_str); // PLATFORM uncomment to pos post to IFTTT

        //strcpy(final_str, str_ts);
        printf("%s\n", final_str);
        printf("Valore postato su IFTTT");
        printf("\n");

        //THINGSPEAK
        strcpy(final_str_ts, str_ts);
        strcat(final_str_ts, key_ts);
        strcat(final_str_ts, str2_ts);
        strcat(final_str_ts, val_str);
        strcat(final_str_ts, str3_ts);
        //system(final_str_ts); //PLATFORM decommentare per postare su  ThingSpeak
        printf("%s\n", final_str);
        printf("Valore postato su Thingspeak");
        printf("\n");
    }
    pthread_cleanup_pop(0);
    pthread_exit(0);

}


/* This callback function is called when CTRL+C is pressed */
/* This function is not called for the moment*/
void read_dht11_dat(float* last_valid_T, float* last_valid_H)
{
    static const unsigned int MAXTIMINGS = 85;
    struct timeval begin, end;
    int dht11_dat[5] = {0,0,0,0,0};

    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;
    float f, val_t, val_h; // fahrenheit
    unsigned int timeDiff;
    dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;
    // pull pin down for 18 milliseconds
    pinMode(DHTPIN, OUTPUT);
    digitalWrite(DHTPIN, LOW);
    delay(18);
    // then pull it up for 40 microseconds
    digitalWrite(DHTPIN, HIGH);
    delayMicroseconds(40);
    // prepare to read the pin
    pinMode(DHTPIN, INPUT);

    // detect change and read data
    for ( i=0; i< MAXTIMINGS; i++) {
        counter = 0;
        //unsigned int t = micros();
        while (digitalRead(DHTPIN) == laststate) {
            counter++;
            delayMicroseconds(1);
            if (counter == 255) {
                break;
            }
        }
        laststate = digitalRead(DHTPIN);

        if (counter == 255) break; //in case of no response

        // ignore first 3 transitions
        if ((i >= 4) && (i%2 == 0)) {
            // shove each bit into the storage bytes
            dht11_dat[j/8] <<= 1;
            if (counter > 16)
            dht11_dat[j/8] |= 1;
            j++;
        }
    }
    // check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
    // print it out if data is good
    if ((j >= 40) &&
    (dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF)) ) {
        f = dht11_dat[2] * 9. / 5. + 32;
        printf("Humidity = %d.%d %% Temperature = %d.%d *C (%.1f *F)\n\n",
        dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f);
        val_t = convertVal2Float(dht11_dat[0], dht11_dat[1]);
        val_h = convertVal2Float(dht11_dat[2], dht11_dat[3]);

    }
    else
    {
        printf("\nData not good, skip\n");
        val_t = -273.0;
        val_h = -273;
    }
    *last_valid_T = val_t;
    *last_valid_H = val_h;
}

float convertVal2Float(unsigned int int_part, unsigned int dec_part)
{
    float temp, result;
    temp = dec_part;
    while(temp>1.0)
    {
        temp/=10;
    }
    result = (temp + (float)(int_part) );
    return result;
}


void exitingThreadTask2(void* arg)
{
    printf("Thread of task 2 exiting\n");
    digitalWrite(DHTPIN, LOW);
    printf("Pin lowered\n");
}

void exitingThreadTask3(void* arg)
{
    printf("Thread of task 3 exiting\n");
}

void sig_handler(int signo)
{
    printf("Running handler to return from pause\n");
    return;
}
