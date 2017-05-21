#include "TAD_Control.h"

static int timestamp,numeroChar;
static char timerControl, estatControl;
static char temp[50], opcio, hihaID, nouID, hihaMSG;
static char IDtemp[3];

#define     MAXCOLUMNES 16
static char estatLCD = 0;
const unsigned char noID[]={"Waiting for ID  "};
const unsigned char noMessage[]={"Waiting message "};
const unsigned char newMessage[]={"New message!    "};
const unsigned char recivingMSG[]={"Reciving message"};
static unsigned char timerLCD, caracterInici, i,j,x;
static unsigned int mostra,velocidad, timerNM;
static unsigned char segonaLinia[MAXCOLUMNES];
static char ID[3], melodia[4] = {3,2,1,2};
static char tramesTotals, tramesID, character, end, F_newMessage, reciving,enviat;

void myItoa(int num){
    temp[0] = (char)(num/1000);
    num = num - (temp[0]*1000);
    temp[1] = (char)(num /100);
    num = num - (temp[1]*100);
    temp[2] = (char) (num /10);
    num = num - (temp[2]*10);
    temp[3] = num+'0';
    temp[4] = '\0';
    temp[2]+='0';
    temp[1]+='0';
    temp[0]+='0';

}
void Menu(void){
    SiPutsCooperatiu("\r\nBenvingut al Walkie-Textie!\r\n\0");
    SiPutsCooperatiu("\r\n\0");
    SiPutsCooperatiu("1-Introduir un nou identificador\r\n\0");
    SiPutsCooperatiu("2-Consultar ID actual\r\n\0");
    SiPutsCooperatiu("3-Consultar trames identificades\r\n\0");
    SiPutsCooperatiu("4-Consultar trames totals\r\n\0");
    SiPutsCooperatiu("5-Visualitzar l'ultim missatge rebut\r\n\0");
    SiPutsCooperatiu("6-Reset\r\n\n\0");
    SiPutsCooperatiu("\r\n\0");
    SiPutsCooperatiu("Per sortir de qualsevol opcio, prem ESC\r\n\0");
}

void initControl(void){
    int i;
    for(i=0;i<3;i++){
        IDtemp[i] = '0';
    }
    numeroChar = 2;

    DB9;
    x = 16;
    timestamp = end = 0;
    timerControl= TiGetTimer();
    timerNM = TiGetTimer();
    estatControl = 0;
    nouID = velocidad = tramesTotals = tramesID = hihaMSG = i = j = caracterInici = F_newMessage = reciving = enviat = 0;
}

void MotorControl(void){

    switch(estatControl){
        case 0:
            if (PORTBbits.RB10 != 0){
                if (enviat == 0){
                    Menu();
                    enviat = 1;
                }
                if (SiCharAvail() != 0){
                    opcio= SiGetChar();
                    if ((opcio <= '6') && (opcio >= '1')){
                        estatControl = opcio-'0';
                        printaFrase(estatControl);

                    }else{
                        SiSendChar('\r');
                        SiSendChar(opcio);
                    }
                }else{
                    estatControl =0;
                }
            }else{
                estatControl = 0;
                enviat = 0;
            }
            break;
        case 1:
            if (SiCharAvail() != 0){
                estatControl = 8;
            }
            break;
        case 8:
            IDtemp[numeroChar]=SiGetChar();
            if(IDtemp[numeroChar] == 27){
                Menu();
                estatControl = 0;
            }else if(numeroChar > 0){
                estatControl = 1;
                numeroChar--;
            }else{
                SiPutsCooperatiu("\n Gracies pel nou ID!\r\n\0");
                Menu();
                estatControl = 0;
                numeroChar = 2;
                nouID=1;
                hihaID = 1;
            }
            break;
        case 2:
            if(TiGetTics(timerControl) > 50){
                if(hihaID == 1){
                    SiPutsCooperatiu("\r\nID: \0");
                    SiSendChar(ID[2]);
                    SiSendChar(ID[1]);
                    SiSendChar(ID[0]);
                    SiPutsCooperatiu("\r\n\n\0");
                    Menu();
                    estatControl = 0;
                }else{
                    SiPutsCooperatiu("\nError no hi ha ID\r\n\0");
                    Menu();
                    estatControl = 0;
                }
            }
            break;
        case 3:
            SiPutsCooperatiu("\r\nTrames Identificades: \0");
            SiSendChar(tramesID +'0');
            SiPutsCooperatiu("\r\n\n\0");
            Menu();
            estatControl = 0;
            break;
        case 4:
            SiPutsCooperatiu("\r\nTrames Totals: \0");
            SiSendChar(tramesTotals+'0');
            SiPutsCooperatiu("\r\n\n\0");
            Menu();
            estatControl = 0;
            break;
        case 5:
            if (hihaMSG == 1) {
                if(getCharMSG(x) == '\0'){
                    SiPutsCooperatiu("\n\r\0");
                    Menu();
                    estatControl = 0;
                }else{
                    estatControl = 10;
                }
            } else {
                SiPutsCooperatiu("\nNo hi ha cap missatge carregat.\n\r\0");
                Menu();
                estatControl = 0;
            }
            break;
        case 10:
            SiSendChar(getCharMSG(x++));
            estatControl = 5;
            break;
        case 6:
            __asm__ volatile ("reset");
            break;
    }
}

void initMotorLCD(void){
    timerLCD = TiGetTimer();
    caracterInici = 0;
    LcClear();
    int i;
    for (i=0;i<3;i++){
        ID[i] = '0';
    }
    j=0;
    segonaLinia[2]='/';
    segonaLinia[5]=' ';
    segonaLinia[6]=' ';
    segonaLinia[7]=' ';
    segonaLinia[8]=' ';
    segonaLinia[9]=' ';
    segonaLinia[10]='I';
    segonaLinia[11]='D';
    segonaLinia[12]=':';
}

void MotorLCD(void){
    switch (estatLCD){
        case 0:
            if(hihaMSG){
                if(TiGetTics(timerNM) < 5000 && F_newMessage == 0){
                    LcPutChar(newMessage[i++]);
                    if (i > MAXCOLUMNES) {
                        estatLCD = 1;
                        TiResetTics(timerLCD);
                        LcGotoXY(0,1);
                    }
                }else if (reciving == 1){
                    LcPutChar(recivingMSG[i++]);
                    if (i > MAXCOLUMNES) {
                        estatLCD = 1;
                        TiResetTics(timerLCD);
                        LcGotoXY(0,1);
                    }
                }else{
                    stopMelodia();
                    F_newMessage = 1;
                    character = getCharMSG(j++);
                    if (character == '\0') {
                        LcPutChar(' ');
                    } else {
                        LcPutChar(character);
                    }
                    if((character == '\0') && (i == 0))end=1;
                    if (i++ > MAXCOLUMNES) {
                        estatLCD = 1;
                        TiResetTics(timerLCD);
                        LcGotoXY(0,1);
                    } 
                }
            }else if (reciving == 1){
                LcPutChar(recivingMSG[i++]);
                if (i > MAXCOLUMNES) {
                    estatLCD = 1;
                    TiResetTics(timerLCD);
                    LcGotoXY(0,1);
                }
            }else if(!hihaMSG && !hihaID){
                LcPutChar(noID[i++]);
                if (i > MAXCOLUMNES) {
                    estatLCD = 1;
                    TiResetTics(timerLCD);
                    LcGotoXY(0,1);
                }   
            }else if(!hihaMSG && hihaID){
                LcPutChar(noMessage[i++]);
                if (i > MAXCOLUMNES) {
                    estatLCD = 1;
                    TiResetTics(timerLCD);
                    LcGotoXY(0,1);
                }
            }
            break;
        case 1:
            if(nouID != 1){
                if(hihaID == 0){
                    ID[0]++;
                    ID[1]++;
                    ID[2]++;
                    if(ID[0] > '9'){
                        ID[0] = '0';
                        ID[1] = '0';
                        ID[2] = '0';
                    }
                }
            }else if(nouID == 1){
                ID[0] = IDtemp[0];
                ID[1] = IDtemp[1];
                ID[2] = IDtemp[2];
                tramesID = 0;
                SetPWMID(ID);
                nouID = 0;
            }
            
            segonaLinia[0] = (tramesID/10) + '0';
            segonaLinia[1] = (tramesID%10) + '0';
            segonaLinia[3] = (tramesTotals/10) + '0';
            segonaLinia[4] = (tramesTotals%10) + '0';
            
            segonaLinia[13] = ID[2];
            segonaLinia[14] = ID[1];
            segonaLinia[15] = ID[0];
            estatLCD= 2;
            break;
        case 2: 
            mostra = AdGetMostra();
            velocidad = CalcularVelocidad(mostra);
            estatLCD = 3;
            break;
        case 3:
            if (TiGetTics(timerLCD)>velocidad){
                TiResetTics(timerLCD);
                i=0;
                estatLCD = 4;
            }
            break;
        case 4:
            LcPutChar(segonaLinia[i++]);
            if (i > MAXCOLUMNES) {
                estatLCD = 5;
                TiResetTics(timerLCD);
            }
            break;
        case 5:
            if (TiGetTics(timerLCD)>= 250){
                estatLCD = 0;
                LcGotoXY(0,0);
                if ((TiGetTics(timerNM) >= 5000) && (hihaMSG == 1)) {
                    caracterInici++;
                    if (end == 1){
                        caracterInici=0;
                        end = 0;
                    }
                    j = caracterInici;
                } else {
                    j = 0;
                }
                i=0;
            }
            break;
    }
}

void printaFrase(char estat){
    switch(estat){
        case 1:
            SiPutsCooperatiu("Intridueix el ID de la placa\r\n\0");
            break;
        case 2:
            SiPutsCooperatiu("\nL'ID actual de la placa es: \r\n\0");
            TiResetTics(timerControl);
            break;
        case 5:
            SiSendChar('\n');
            break;
    }
}

unsigned int CalcularVelocidad(unsigned int x){
    //Sabemos que es lineal y que por x=0 los tics(y)) seran 200 y por x=1024 los tics seran 2000
    //Asi que la equacion se define como y=mx+b(b=200,m=1.757)
    unsigned int num;
    num = x*9/5+200;
    return num;
}

void putNewId(char aux[3]){
    ID[0] = aux[0] + '0';
    ID[1] = aux[1] + '0';
    ID[2] = aux[2] + '0';
}

char ComparaID(char aux[3]){
    aux[0] = aux[0]+'0';
    aux[1] = aux[1]+'0';
    aux[2] = aux[2]+'0';

    if((ID[2] == (aux[0])) && (ID[1] == (aux[1])) && (ID[0] == (aux[2]))){
        return 1;
    }else{
        return 0;
    }
}

void IncTramesRebudes(char CheckID){
    if (CheckID == 0) {
        tramesTotals++;
    }else{
        reciving = 1;
    }
}

void HiHaTrama(unsigned int numBytes){
    hihaMSG = 1;
    reciving = 0;
    setCharMSG(' ', 15);
    TiResetTics(timerNM);
    F_newMessage = 0;
    tramesTotals++;
    tramesID++;
    calculoMelodia(numBytes);
    setMelodia(melodia);
}

void calculoMelodia(unsigned int numBytes){
    melodia[0] = ((numBytes*10)/300)+melodia[0];
    melodia[1] = ((numBytes*10)/300)+melodia[1];
    melodia[2] = ((numBytes*10)/300)+melodia[2];
    melodia[3] = ((numBytes*10)/300)+melodia[3];
}