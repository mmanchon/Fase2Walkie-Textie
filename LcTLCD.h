#ifndef LCTLCD_H
#define	LCTLCD_H

#include <xc.h>

#define SetD4_D7Sortida()		(TRISBbits.TRISB6 = TRISBbits.TRISB7 = TRISBbits.TRISB8 = TRISBbits.TRISB9 = 0)
#define SetD4_D7Entrada()		(TRISBbits.TRISB6 = TRISBbits.TRISB7 = TRISBbits.TRISB8 = TRISBbits.TRISB9 = 1)
#define SetControlsSortida()            (TRISBbits.TRISB3 = TRISBbits.TRISB15 = TRISBbits.TRISB5 = 0)
#define SetD4(On)				(LATBbits.LATB6 = (On))
#define SetD5(On)				(LATBbits.LATB7 = (On))
#define SetD6(On)				(LATBbits.LATB8 = (On))
#define SetD7(On)				(LATBbits.LATB9 = (On))
#define GetBusyFlag()                           (PORTBbits.RB9)
#define RSUp()					(LATBbits.LATB3 = 1)
#define RSDown()				(LATBbits.LATB3 = 0)
#define RWUp()					(LATBbits.LATB15 = 1)
#define RWDown()				(LATBbits.LATB15 = 0)
#define EnableUp()				(LATBbits.LATB5 = 1)
#define EnableDown()                            (LATBbits.LATB5 = 0)

void LcInit(char Files, char Columnes);
// Pre: Files = {1, 2, 4}  Columnes = {8, 16, 20, 24, 32, 40 }
// Pre: Hi ha un timer lliure
// Post: L'Hitachi merd€s necessita 40ms de tranquilitat desde
// la pujada de Vcc fins cridar aquest constructor
// Post: Aquesta rutina pot trigar fins a 100ms
// Post: El display queda esborrat, el cursor apagat i a la
// posici€ 0, 0.

void LcEnd(void);
// Pre: S'ha demant un timer per al TAD LCD.
//Post: Allivera el timer del TAD LCD.

void LcClear(void);
// Post: Esborra el display i posa el cursor a la posici€ zero en
// l'estat en el que estava.
// Post: La propera ordre pot trigar fins a 1.6ms

void LcCursorOn(void);
// Post: Encen el cursor
// Post: La propera ordre pot trigar fins a 40us

void LcCursorOff(void);
// Post: Apaga 0el cursor
// Post: La propera ordre pot trigar fins a 40us

void LcGotoXY(char Columna, char Fila);
// Pre : Columna entre 0 i 39, Fila entre 0 i 3
// Post: Posiciona el cursor en aquestes coordenades
// Post: La propera ordre pot trigar fins a 40us

void LcPutChar(char c);
// Post: Pinta C en l'actual poscici€ del cursor i incrementa
// la seva posici€. Si la columna arriba a 39, salta a 0 tot
// incrementant la fila si el LCD »s de dues files.
// Si es de 4 files, incrementa de fila en arribar a la columna 20
// AixÃ mateix, la fila 4 passa a la zero.
// En els LCDs d'una fila, quan la columna arriba a 39, torna
// a zero. No s'incrementa mai la fila

void LcPutString(char *s);
// Post: Pinta l'string a apartir de la posici€ actual del cursor.
// El criteri de coordenades »s el mateix que a LcPutChar
// Post: Pot trigar fins a 40us pel nombre de chars de s a sortir de
// la rutina

#endif	/* LCTLCD_H */

