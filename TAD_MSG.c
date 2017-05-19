#include "TAD_MSG.h"

static char message[320];


void initMSG(){
    int i;
    for(i=0;i<320;i++){
        message[i] = ' ';
    }
}


void setCharMSG(char msg, int i){
    message[i] = msg;
}

char getCharMSG(int i){
    return message[i];
}

char* getMessage(void){
    return message;
}