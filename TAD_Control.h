#ifndef TAD_CONTROL_H
#define	TAD_CONTROL_H
#include <xc.h>
#include "SiTSio.h"
#include "TPWM.h"
#include "AuTAudio.h"
#include "LcTLCD.h"
#include "AdTADC.h"
#include "TAD_MSG.h"

#define CONTROL_1 "\n\rPlaca LS69. Sistemes Digitals i uProcessadors\r\n\0"
#define CONTROL_2 "vCli v1.0.\r\n\0"
#define DB9 TRISBbits.TRISB10 = 1;


void myItoa(int num);
//Pre: 0<= num <= 9999
//Post: deixa a temp[3..0] el num en ASCII

void Menu(void);
//Pre: La SIO està inicialitzada
//Post: Pinta el menu pel canal sèrie

void initControl(void);
//Pre: La SIO està inicialitzada
//Post: Inicialitza el timestamp i pinta el menu per la SIO

void MotorControl(void);
//Pre: initControl().

void initMotorLCD(void);
//Pre: el LCD està inicialitzat
//Post: inicialitza el LCD per posar la marquesina a 0

void MotorLCD(void);
//Pre: initMotorLCD().

void printaFrase(char estat);
//Pre: estat = 1, 2 o 5.
//Post: Printa la informaicó corresponent a cada estat.

unsigned int CalcularVelocidad(unsigned int x);
//Pre: 0 <= x < 1024.
//Post: Retorna el numero de tics corresponent a la velocitat proporcional al valor x.

void putNewId(char aux[3]);
//Pre: 0 <= ID[i] < 10.
//Post: Assigna l'ID com a id propi.

char ComparaID(char aux[3]);
//Pre: 0 <= aux[i] < 10. 
//Post: Retorna 1 si aux és igual a l'ID actual. 0 altrament.

void IncTramesRebudes(char CheckID);
//Pre: CheckID = 0 o 1.
//Post: Si CheckID és 1 incrementa les trames totals, sinó avisa que estem rebent una trama.

void HiHaTrama(unsigned int numBytes);
//Post: Avisa que hi ha un missatge rebut a punt per mostrar.

void calculoMelodia(unsigned int numBytes);
//Pre: 0 <= numBytes < 300.
//Post: Calcula la freqüència proporcional al numero de bytes.

#endif	/* TAD_CONTROL_H */

