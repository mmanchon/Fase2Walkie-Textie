#include "TPWM.h"

static unsigned char timerPWM[3];
static unsigned char estatPWM[3];
static unsigned char T0[3],T1[3];
static char ID[3],nouID,trobat[3],var[3];

void PWMInit(){
    SET_PWM_DIR1();
    SET_PWM_DIR2();
    SET_PWM_DIR3();

    PWM_OFF1();
    PWM_OFF2();
    PWM_OFF3();
    
    int i;
    for(i=0;i<3;i++){
        timerPWM[i] = TiGetTimer();
        estatPWM[i] = 0;
        var[i] = 0;
        ID[i] = '0';
        trobat[i] =0;
    }
    nouID =0;
}

void MotorPWM(int i){

    switch(estatPWM[i]){
        case 0:
            T0[i] = 10 - (var[i]+1);
            TiResetTics(timerPWM[i]);
            PutPWMOFF(i);
            estatPWM[i] = 1;
            break;
        case 1:
            PutPWMOFF(i);
            if(TiGetTics(timerPWM[i]) >= T0[i]*2){
                TiResetTics(timerPWM[i]);
                T1[i] = (var[i]+1);
                PutPWMON(i);
                estatPWM[i] = 2;
            }
            break;
        case 2:
            PutPWMON(i);
            if(TiGetTics(timerPWM[i]) >= T1[i]*2){
                if(PWMEqualsID(var[i],ID[i]) != 1 || nouID == 0){
                    var[i]++;
                }
                if(var[i] > 9)var[i]=0;
                PutPWMOFF(i);
                estatPWM[i] = 0;
            }
            break;
    }
}

void PutPWMOFF(int i){
    switch(i){
        case 0:
            PWM_OFF1();
            break;
        case 1:
            PWM_OFF2();
            break;
        case 2:
            PWM_OFF3();
            break;
    }
}

void PutPWMON(int i){
    switch(i){
        case 0:
            PWM_ON1();
            break;
        case 1:
            PWM_ON2();
            break;
        case 2:
            PWM_ON3();
            break;
    }
}

void SetPWMID(char aux[3]){
    ID[0] = aux[0];
    ID[1] = aux[1];
    ID[2] = aux[2];
    nouID = 1;
}

char PWMEqualsID(char var,char ID){
    if(var == (ID-'0')){
        return 1;
    }else{
        return 0;
    }
}