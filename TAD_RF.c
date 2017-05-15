#include "TAD_RF.h"

static unsigned char estat;
static char ID[3],numero;
static char MSG[300];
static int timer;
static char mostra, qbits, i, j;
static char byte, idTrama;
static char HHTrama;

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
    mostra = 0;
    byte = 0;
    idTrama = 0;
    qbits = 0;
    HHTrama = 0;
    i = 0;
    j = 0;
    
    timer = TiGetTimer();
    
}

void MotorRF(void) {
    switch(estat) {
        case 0:
            //SiPutsCooperatiu("\nestat 0\r\n\0");
            if(PORTBbits.RB13 == 1){
                TiResetTics(timer);
                estat = 1;
            }
            break;
        case 1:
            if((PORTBbits.RB13 == 0)){
                if((TiGetTics(timer) < 5)){
                    estat = 0;

                }else{
                    //SiPutsCooperatiu("estat 1\r\n\0");
                    TiResetTics(timer);
                    estat = 2;
                }
            }
            /*if ((PORTBbits.RB13 == 0) && (TiGetTics(timer) < 5)){
                estat = 0;
            }
            if ((PORTBbits.RB13 == 0) && (TiGetTics(timer) >= 5)) {
                SiPutsCooperatiu("estat 1\r\n\0");
                TiResetTics(timer);
                estat = 2;
            }*/
            
            break;
        case 2:
            if((PORTBbits.RB13 == 0)){
                if((TiGetTics(timer) >= 10 && TiGetTics(timer) < 11)){
                     TiResetTics(timer);
                     estat = 4;
                }
            }else if(PORTBbits.RB13 == 1) {
                TiResetTics(timer);
                estat = 2;
            }
            
            break;
        case 4:
            
            if (TiGetTics(timer) >= 2) {
               // SiPutsCooperatiu("Voy a leer el byte\n\r\0");
                TiResetTics(timer);
                estat = 5;
            }
            break;
        case 5:
            if (TiGetTics(timer) >= 5) {
                byte = ((byte << 1) & 0xFE) | PORTBbits.RB13;
                //SiPutsCooperatiu("LEO EL BYTE\n\r\0");
                qbits++;
                TiResetTics(timer);
                estat = 6;
            }
            break;
        case 6:
            if (qbits < 8){
                estat = 7;
            } else {
                //SiSendChar(byte);
                if ( (qbits == 8) && (HHTrama == 0)) {
                    idTrama = byte;
                    //SiPutsCooperatiu("\nREBO UN IDTRAMA\r\n\0");
                    if (idTrama == 'N'){
                        //SiPutsCooperatiu("\nTINC UNA N\r\n\0");
                    }

                    HHTrama = 1;
                    qbits = 0;
                    estat = 7;
                }
                if ((qbits == 8) && (i < 3) && (idTrama == 'N') && (HHTrama == 1)) {
                    SiPutsCooperatiu("\nTINC un byte\r\n\0");
                    //SiPutsCooperatiu("ESTIC LLEGINT ID\n\r\0");
                    ID[i++] = byte;
                    qbits = 0;
                    estat = 2;
                }
                if ((qbits == 8) && (i >= 3) && (byte != 0x3D) && (idTrama == 'N') && (HHTrama == 1)) {
                    SiPutsCooperatiu("\nTINC ID\r\n\0");
                    //putNewId(ID);
                    MSG[j++] = byte;
                    SiSendChar(byte);
                    qbits = 0;
                    //idTrama = 0;
                    estat = 2; 
                }
                if ((qbits == 8) && (idTrama != 'N')){
                   // SiPutsCooperatiu("PASO POR NO N\n\r\0");
                    qbits = 0;
                    HHTrama = 0;
                    estat = 0;
                }
                
                if ((qbits == 8) && (i >= 3) && (byte == 0x3D) && (idTrama == 'N')) {
                    SiPutsCooperatiu("\nTINC MISSATGE\r\n\0");
                    MSG[j] = '\0';
                    SiSendChar(MSG[0]);
                    SiSendChar(MSG[1]);
                    SiSendChar(MSG[2]);
                    SiSendChar(MSG[3]);
                    SiSendChar(MSG[4]);
                    qbits = 0;
                    i = 0;
                    j = 0;
                    idTrama = 0;
                    HHTrama = 0;
                    estat = 0;
                    TiResetTics(timer);
                }
            }
            
            
            break;
        case 7:
            if (TiGetTics(timer) >= 5) {
                TiResetTics(timer);
                estat = 5;
            }
            break;
        case 8:
            
            break;
            
    }
}

