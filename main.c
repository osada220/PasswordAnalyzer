#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#define COUNT_OF_ALPHABETS 4 // add new alphabet before changing
#define THREADS_COUNT 16
#define MAX_CHECKED_LENGTH 100
char alphabets[COUNT_OF_ALPHABETS][256] = {"0123456789","0123456789abcdefghijklmnopqrstuvwxyz","0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXZY","0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXZY !\"#$%&'()*+,-./"};
char *password, *p_found;
typedef struct{
    char* password_curr;
    int password_len;
    int current_len;
    char* alphabet;
}arguments;
void* password_selection_number(void* args){
    if(p_found != NULL){
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
        pthread_cancel(pthread_self());
    }
    arguments* params = (arguments*)args;
    if(params->current_len == params->password_len){
        if(strcmp(password, params->password_curr) == 0){
            printf("%s", params->password_curr);
            p_found = malloc(sizeof(char) * strlen(params->password_curr));
            strcpy(p_found, params->password_curr);
        }
    }else{
        int cur_len = params->current_len;
        params->current_len++;
        for(int i = 0; i < strlen(params->alphabet); i++){
            params->password_curr[cur_len] = params->alphabet[i];
            password_selection_number(args);
        }
        params->current_len--;
    }
    return NULL;
}
void* find_password(){
    pthread_t threads[MAX_CHECKED_LENGTH*COUNT_OF_ALPHABETS];
    int ctr = 0;
    for(int j = 0; j <= MAX_CHECKED_LENGTH; j++)
        for(int i = 0; i < COUNT_OF_ALPHABETS; i++){
            int cur_executed;
            do{
                cur_executed = 0;
                for (int k = 0; k < ctr; k++) {
                    if (!pthread_kill(threads[k], 0))
                        cur_executed++;
                }
                sleep(0.1);
                if(p_found != NULL)
                    return(NULL);
            }while (cur_executed >= THREADS_COUNT);
            arguments *args = calloc(sizeof(arguments),1);
            args->password_curr = (char*)calloc(MAX_CHECKED_LENGTH+1,sizeof(char));
            args->password_len = j;
            args->current_len = 0;
            args->alphabet = alphabets[i];
            pthread_create(&threads[ctr], NULL, password_selection_number, args);
            ctr++;
        }
}
int main(){
    time_t begin = time(NULL);
    char array[] = "09999!";
    password = array;
    find_password();
    time_t end = time(NULL);
    printf("\nElapsed time: %lds\n", end - begin);
    return 0;
}