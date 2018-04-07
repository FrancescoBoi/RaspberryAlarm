#include<pthread.h>
/* GPIO USAGE DEFINITION */
#define OUTPIN 0
#define INPIN 25 //used in task 1 only

#define DHTPIN 26 //used in task 2 only

/*END*/
/* END GPIO USAGE DEFINITION */

/* GLOBAL VARIABLES DECLARATION */
extern float g_avg_temp;
extern float g_avg_hum;

extern unsigned char g_new_pic_flag;
extern pthread_cond_t g_new_pic_cond;
extern pthread_mutex_t g_new_pic_m;

/* END GLOBAL VARIABLES DECLARATION */
