#include "PrTPropaganda.h"


static int timestamp,numeroChar;
static char timerPropaganda, estatPropaganda;
static char temp[50], opcio, hihaID;
static char IDtemp[3];


#define     MAXCOLUMNES 16
static char estatLCD = 0;
const unsigned char noID[]={"Waiting for ID  "}; //Més val que tingui 16 caràcters...
const unsigned char noMessage[]={"Waiting message "};
static unsigned char timerLCD, caracterInici, i,j;
static unsigned int mostra;
static unsigned char segonaLinia[MAXCOLUMNES];
static char ID[3];

void myItoa(int num){
    //Post: escriu el valor ascii de num a tmp;
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
    SiPutsCooperatiu("4-Consultar trames rebudes totals\r\n\0");
    SiPutsCooperatiu("5-Visualitzar l'ultim missatge rebut\r\n\0");
    SiPutsCooperatiu("6-Reset\r\n\n\0");
    SiPutsCooperatiu("\r\n\0");
    SiPutsCooperatiu("Per sortir de qualsevol opcio, prem ESC\r\n\0");
}

void initPropaganda(void){
    //Pre: La sio està inicialitzada
    int i;
    for(i=0;i<3;i++){
        IDtemp[i] = '0';
    }
    numeroChar = 2;
    SiPutsCooperatiu(PROPAGANDA_1);
    SiPutsCooperatiu(PROPAGANDA_2);
    Menu();
    timestamp = 0;
    timerPropaganda= TiGetTimer();
    estatPropaganda = 0;
}

void MotorPropaganda(void){

    switch(estatPropaganda){
        case 0:
            //Aquí estem esperant ordre del menú principal
            if (SiCharAvail() != 0){
                //SiPutsCooperatiu("\r\nArribo al estat 1\r\n\0");
                opcio= SiGetChar();
                if ((opcio <= '6') && (opcio >= '1')){
                    estatPropaganda = opcio-'0';
                    printaFrase(estatPropaganda);
                 
                }else{
                    SiSendChar('\r');
                    SiSendChar(opcio);
                }
            }
            if (TiGetTics(timerPropaganda) > 1000){
                TiResetTics(timerPropaganda);
                if (++timestamp == 10000) timestamp=0;
            }
            break;
        case 1:
            //Nou identificador
            if (SiCharAvail() != 0){
                estatPropaganda = 8;
            }
            break;
        case 8:
            IDtemp[numeroChar]=SiGetChar();
            if(IDtemp[numeroChar] == 27){
                Menu();
                estatPropaganda = 0;
            }else if(numeroChar > 0){
                estatPropaganda = 1;
                numeroChar--;
            }else{
                SiPutsCooperatiu("\n Gracies pel nou ID!\r\n\0");
                Menu();
                estatPropaganda = 0;
                numeroChar = 2;
                hihaID = 1;
                SetPWMID(ID);
            }
            break;

        case 2:
            //Visualització uInterruptors
            if(TiGetTics(timerPropaganda) > 50){
                if(hihaID == 1){
                    SiSendChar('\n');
                    SiSendChar(ID[2]);
                    SiSendChar(ID[1]);
                    SiSendChar(ID[0]);
                    SiPutsCooperatiu("\r\n\n\0");
                    Menu();
                    estatPropaganda = 0;
                }else{
                    SiPutsCooperatiu("\nError no hi ha ID\r\n\0");
                    Menu();
                    estatPropaganda = 0;
                }
            }
            /*if (TiGetTics(timerPropaganda) > 1000){
                TiResetTics(timerPropaganda);
                if (++timestamp == 10000) timestamp=0;
                SiSendChar('\r');
                SiPutsCooperatiu("SW1: \0");
                SiSendChar(getSwitch1()+'0');
                estatPropaganda = 21;
            }
            if (SiCharAvail() != 0){
                if (SiGetChar() == 27){
                    Menu();
                    estatPropaganda=0;
                }
            }*/
            break;
        case 3:
            //Visualització pulsadors
            if (TiGetTics(timerPropaganda) > 1000){
                TiResetTics(timerPropaganda);
                if (++timestamp == 10000) timestamp=0;
                SiSendChar('\r');
                SiPutsCooperatiu("PB1: \0");
                SiSendChar(getPB1()+'0');
                estatPropaganda = 31;
            }
            if (SiCharAvail() != 0){
                if (SiGetChar() == 27){
                    Menu();
                    estatPropaganda=0;
                }
            }
            break;
        case 4:
            if (TiGetTics(timerPropaganda) > 1000){
                TiResetTics(timerPropaganda);
                if (++timestamp == 10000) timestamp=0;
                SiSendChar('\r');
                myItoa(getBlDuty()*5);
                SiSendChar(temp[1]);
                SiSendChar(temp[2]);
                SiSendChar(temp[3]);
            }
            if (SiCharAvail() != 0){
                if (SiGetChar() == 27){
                    Menu();
                    estatPropaganda=0;
                }
            }
            break;
        case 5:
            SiPutsCooperatiu("\n\rIntrodueix valor de la nova intensitat (0-4):\n\r");
            estatPropaganda = 51;
            break;
        case 6:
            __asm__ volatile ("reset");
            break;

        case 7:
                        SiPutsCooperatiu("\n\rPrem J per encendre o apagar l'altaveu \n\r");
            if (getAudioStatus()) SiPutsCooperatiu("\rAltaveu ences! \0");
                        else SiPutsCooperatiu("\rAltaveu apagat!\0");
            estatPropaganda = 61;
            break;
        case 21:
            SiPutsCooperatiu("\t\tSW2: \0");
            SiSendChar(getSwitch2()+'0');
            estatPropaganda= 2;
            break;
       case 31:
            SiPutsCooperatiu("\t\tPB2: \0");
            SiSendChar(getPB2()+'0');
            estatPropaganda= 3;
            break;
        case 51:
            if (TiGetTics(timerPropaganda) > 1000){
                TiResetTics(timerPropaganda);
                if (++timestamp == 10000) timestamp=0;
            }
            if (SiCharAvail() != 0){
                switch (SiGetChar()){
                    case 27:
                        Menu();
                        estatPropaganda=0;
                        break;
                    case '0':
                        SiPutsCooperatiu("\r000\0");
                        setBlDuty(0);
                        break;
                    case '1':
                        SiPutsCooperatiu("\r025\0");
                        setBlDuty(PERIODBL/4);
                        break;
                    case '2':
                        SiPutsCooperatiu("\r050\0");
                        setBlDuty(PERIODBL/2);
                        break;
                    case '3':
                        SiPutsCooperatiu("\r075\0");
                        setBlDuty(PERIODBL*3/4);
                        break;
                    case '4':
                        SiPutsCooperatiu("\r100\0");
                        setBlDuty(PERIODBL);
                        break;
                }
            }
            break;
        case 61:

            if (TiGetTics(timerPropaganda) > 1000){
                TiResetTics(timerPropaganda);
                if (++timestamp == 10000) timestamp=0;
            }
            if (SiCharAvail() != 0){
                switch (SiGetChar()){
                    case 27:
                        Menu();
                        estatPropaganda=0;
                        break;
                    case 'J':
                        if (changeAudioStatus()) SiPutsCooperatiu("\rAltaveu ences! \0");
                        else SiPutsCooperatiu("\rAltaveu apagat!\0");
                        break;
                    case 'j':
                        if (changeAudioStatus()) SiPutsCooperatiu("\rAltaveu ences! \0");
                        else SiPutsCooperatiu("\rAltaveu apagat!\0");
                        break;
                }
            }
            break;
    }

}


void initMotorLCD(void){
    //Pre: El LCD està inicialitzat
    timerLCD = TiGetTimer();
    caracterInici = 0;
    LcClear();
    int i;
    for (i=0;i<3;i++){
        ID[i] = '0';
    }
    //Hi ha caselles de la segona línia que sempre valdran el mateix, les preparo!
    segonaLinia[0]='X';
    segonaLinia[1]='/';
    segonaLinia[2]='Y';
    segonaLinia[3]=' ';
    segonaLinia[4]=' ';
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
            if(hihaID != 1){
                LcPutChar(noID[j++]);
                if (j==16) j= 0;
                if (i++ > MAXCOLUMNES) {
                    estatLCD = 1;
                    TiResetTics(timerLCD);
                    LcGotoXY(0,1);
                }
            }else{
                LcPutChar(noMessage[j++]);
                if (j==16) j= 0;
                if (i++ > MAXCOLUMNES) {
                    estatLCD = 1;
                    TiResetTics(timerLCD);
                    LcGotoXY(0,1);
                }
            }
            break;

        case 1: //Preparo el string
            if(hihaID != 1){
                ID[0]++;
                ID[1]++;
                ID[2]++;
                if(ID[0] > '9'){
                    ID[0] = '0';
                    ID[1] = '0';
                    ID[2] = '0';
                }
            }else{
                ID[0] = IDtemp[0];
                ID[1] = IDtemp[1];
                ID[2] = IDtemp[2];
            }
            
            segonaLinia[13] = ID[2];
            segonaLinia[14] = ID[1];
            segonaLinia[15] = ID[0];
            estatLCD= 2;
            break;
        case 2: //Aquí faig l'itoa, que deu trigar una bona estona el pobre...
            mostra = AdGetMostra();
            myItoa(mostra);
            estatLCD = 3;
            break;
        case 3:
            if (TiGetTics(timerLCD)>50){
                //Observo que si estresso molt al LCD arriba un punt que alguna
                //vegada pinta malament un caràcter. Deu tenir una cua interna?
                //si la té, aposto a que és de 24 posicions (mal número)...
                //Cap problema, donem 50 ms. de calma entre ràfega i ràfega i gas
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
                //Alerta, ja porto 50 ms. des de l'últim refresc
                caracterInici++;
                if (caracterInici==16)
                    caracterInici=0;
                estatLCD = 0;
                LcGotoXY(0,0);
                j = caracterInici;
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
            TiResetTics(timerPropaganda);
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;

    }
}
/*
 *
 *          Fi  PROPAGANDAAAAAAAAAA!!!!
 *
 */

