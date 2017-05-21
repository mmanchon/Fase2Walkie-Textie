#include "AuTAudio.h"
#include <xc.h>
static char timerAudio, estat,periode;
static char melodia[10], hihaMelodia;
static int qMelodia;

void AuInit(){
    int i;
    for(i=0;i<10;i++){
        melodia[i] = 0;
    }
    SET_AUDIO_DIR();
    AUDIO_OFF();
    timerAudio = TiGetTimer();
    periode = TiGetTimer();
    estat = hihaMelodia = qMelodia = 0;
}

void setAudioPeriode(char nouPeriode){
    periode= nouPeriode;

}

char getAudioStatus(void){
    return estat != 2;
}

void setMelodia(char aux[10]){
    int i;
    for(i=0;i<10;i++){
        melodia[i] = aux[i];
    }
    hihaMelodia = 1;
}

void stopMelodia(){
    hihaMelodia = 0;
}

void MotorAudio(){
    switch(estat){
        case 0:
            if(hihaMelodia == 1){
                estat=1;
                TiResetTics(timerAudio);
                TiResetTics(periode);
            }
            break;
        case 1:
            if (TiGetTics(timerAudio)>=melodia[qMelodia]){
                TiResetTics(timerAudio);
                AUDIO_ON();
                estat = 2;
            }
            break;
        case 2:
            if (TiGetTics(timerAudio)>=melodia[qMelodia]){
                TiResetTics(timerAudio);
                AUDIO_OFF();
                estat = 3;
            }
            break;
        case 3:
            if(TiGetTics(periode) >= 500){
                TiResetTics(periode);
                qMelodia++;
                if(qMelodia > 3){
                    qMelodia = 0;
                }
            }
            if(hihaMelodia == 1){
               estat = 1;
            }else{
               estat = 0;
            }
            break;
    }
}


char changeAudioStatus(){
    if (estat == 2){
        estat = 0;
    }else{
        estat = 2;
        AUDIO_OFF();
    }
    return estat == 0;
}