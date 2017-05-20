#include "TAD_RF.h"

static unsigned char estat;
static char ID[3],numero;
static char MSG[350];
static int timer;
static char mostra, qbits;
static unsigned char i;
static unsigned int j,numBytes;
static char byte, idTrama, linia;
static char HHTrama, CheckID;

void InitRF(void) {
    
    int i;
    
    estat = 0;
    
    SET_RF_DIR();
   
    for (i=0; i<350; i++) {
        MSG[i] = '\0';
    }
    
    ID[0] = 0;
    ID[1] = 0;
    ID[2] = 0; 
    numero = CheckID = 0;
    mostra = 0;
    byte = numBytes = 0;
    idTrama = 0;
    qbits = 0;
    HHTrama = 0;
    linia = 0;
    i = 0;
    j = 16;
    
    timer = TiGetTimer();
    
}

void MotorRF(void) {
    switch(estat) {
        case 0:
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
                    TiResetTics(timer);
                    estat = 2;
                }
            }
            
            break;
        case 2:
            if((PORTBbits.RB13 == 0)){
                if((TiGetTics(timer) >= 10 && TiGetTics(timer) < 11)){
                     TiResetTics(timer);
                     linia = 0;
                     estat = 3;
                }
            }else if(PORTBbits.RB13 == 1) {
                TiResetTics(timer);
                estat = 2;
            }
            
            break;
        case 3:
            if (PORTBbits.RB13 != linia) {
                linia = PORTBbits.RB13;
                estat = 4;
            }
            break;
        case 4:
            if (PORTBbits.RB13 != linia) {
                TiResetTics(timer);
                byte = ((byte << 1) & 0xFE) | PORTBbits.RB13;
                linia = PORTBbits.RB13;
                qbits++;
                estat = 5;
            }
            break;
        case 5:
            if ((PORTBbits.RB13 != linia) && (TiGetTics(timer) > 6)) {
                TiResetTics(timer);
                byte = ((byte << 1) & 0xFE) | PORTBbits.RB13;
                linia = PORTBbits.RB13;
                qbits++;
                estat = 5;
            }else if ((PORTBbits.RB13 != linia) && (TiGetTics(timer) <= 6)) {
                linia = PORTBbits.RB13;
                estat = 4;
            }
            
            if ( (qbits == 8) && (HHTrama == 0)) {
                idTrama = byte;
                HHTrama = 1;
                qbits = 0;
                estat = 5;
            }
            if ((qbits == 8) && (i < 3) && (idTrama == 'N') && (HHTrama == 1)) {
                //SiPutsCooperatiu("\nTINC un byte\r\n\0");
                ID[i++] = byte;
                numBytes = 0;
                qbits = 0;
                estat = 5;
            }
            
            if(i==3 && CheckID == 0){
                CheckID = ComparaID(ID);
               // SiPutsCooperatiu("\nCHECK ID\r\n\0");
                IncTramesRebudes(CheckID);
                if(CheckID == 0){
                    qbits = 0;
                    i = 0;
                    j = 16;
                    idTrama = 0;
                    HHTrama = 0;
                    estat = 0;
                    TiResetTics(timer);
                }
            }
            if ((qbits == 8) && (i >= 3) && (byte != 0x3D) && (idTrama == 'N') && (HHTrama == 1)) {
                /*SiPutsCooperatiu("\nTINC ID\r\n\0");
                SiSendChar(ID[0]+'0');
                SiSendChar(ID[1]+'0');
                SiSendChar(ID[2]+'0');*/
                
                numBytes++;
                setCharMSG(byte,j++);
                //MSG[j++] = byte;
                qbits = 0;
                estat = 5; 
            }
            if ((qbits == 8) && (idTrama != 'N')){
                qbits = 0;
                HHTrama = 0;
                estat = 0;
            }

            if ((qbits == 8) && (i >= 3) && (byte == 0x3D) && (idTrama == 'N')) {
                //SiPutsCooperatiu("\nTINC MISSATGE\r\n\0");
                setCharMSG('\0',j);
                HiHaTrama(numBytes);
                qbits = 0;
                i = 0;
                j = 16;
                idTrama = 0;
                HHTrama = 0;
                CheckID = 0;
                estat = 0;
                TiResetTics(timer);
            }
            break;
         
    }
}

