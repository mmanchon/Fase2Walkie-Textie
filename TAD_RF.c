#include "TAD_RF.h"

static unsigned char estat;
static char ID[3],numero;
static char MSG[300];
static int timer;

void InitRF(void) {
    
    int i;
    
    estat = 0;
    
    SET_RF_DIR();
   
    for (i=0; i<300; i++) {
        MSG[i] = '\0';
    }
    
    ID[0] = 0;
    ID[1] = 0;
    ID[2] = 0; 
    numero = 0;
    
    timer = TiGetTimer();
    
}

void MotorRF(void) {
    switch(estat) {
        case 0:
            if(PORTBbits.RB13 == 1){
                estat = 1;
            }
            break;
        case 1:
            SiPutsCooperatiu("\nESTAT 1\r\n\0");
            if(PORTBbits.RB13 == 0){
                TiResetTics(timer);
                estat = 2;
                numero =0;
            }
            break;
        case 2:
            SiPutsCooperatiu("\nESTAT 2\r\n\0");
            if(TiGetTics(timer) >= 1 && PORTBbits.RB13 == 0 && numero == 10){
                estat = 3;
            }
            if(PORTBbits.RB13 == 1){
                estat = 1;
            }
            numero++;
            break;
        case 3:
            SiPutsCooperatiu("\nTINC TRAMA\r\n\0");
            estat =0;
            break;
            
    }
}

