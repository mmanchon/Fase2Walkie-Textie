/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PWM_H
#define	PWM_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "time.h"


#define PERIOD  20  //Periode del hearbeat
#define STEP    4   //Nombre de períodes abans d'incrementar el tempsOn
//Compte, PERIOD ha de ser multiple de STEP, si no, no se què passarà, ja ho miraré...

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
//Post: Demana un timer i posa la sortida del LED a 0
void MotorPWM(int i);

void PutPWMOFF(int i);
void PutPWMON(int i);
void SetPWMID(char ID[3]);

#endif	/* XC_HEADER_TEMPLATE_H */

