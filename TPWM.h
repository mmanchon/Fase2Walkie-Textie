#ifndef PWM_H
#define	PWM_H

#include <xc.h> 
#include "time.h"

#define PERIOD  20  //Periode del hearbeat
#define STEP    4   //Nombre de períodes abans d'incrementar el tempsOn

#define SET_PWM_DIR1()  TRISAbits.TRISA2=0;
#define SET_PWM_DIR2()  TRISAbits.TRISA3=0;
#define SET_PWM_DIR3()  TRISAbits.TRISA4=0;

#define PWM_OFF1() LATAbits.LATA2=0;
#define PWM_ON1() LATAbits.LATA2=1;

#define PWM_OFF2() LATAbits.LATA3=0;
#define PWM_ON2() LATAbits.LATA3=1;

#define PWM_OFF3() LATAbits.LATA4=0;
#define PWM_ON3() LATAbits.LATA4=1;

void PWMInit();
//Post: Demana un timer i posa la sortida del LED a 

void MotorPWM(int i);
//Pre: PWMInit()

void PutPWMOFF(int i);
//Post: Desactiva el PWM i.

void PutPWMON(int i);
//Post: Activa el PWM i.

void SetPWMID(char ID[3]);
//Pre: 0+'0' <= ID[i] <= 9+'0'.
//Post: Assigna ID com a id actual del TAD PWM.

char PWMEqualsID(char var,char ID);
//Pre: 0 <= ID, var <= 9.
//Post: Retorna 1 si ID coincideix amb var.

#endif	/* PWM_H */

