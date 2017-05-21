#include <xc.h>
#include "time.h"
#include "LcTLCD.h"

#define FUNCTION_SET	0x20
#define BITS_8			0x10
#define DISPLAY_CONTROL	0x08
#define DISPLAY_ON		0x04
#define CURSOR_ON		0x02
#define DISPLAY_CLEAR	0x01
#define ENTRY_MODE		0x04
#define SET_DDRAM		0x80

static unsigned char Files, Columnes;
static unsigned char FilaAct, ColumnaAct;
static int Timer;

void Espera(int Timer, int ms);
void CantaIR(char IR);
void CantaData(char Data);
void WaitForBusy(void);
void EscriuPrimeraOrdre(char);

void LcInit(char files, char columnes) {
	int i;
	Timer = TiGetTimer(); 
	Files = files; Columnes = columnes;
	FilaAct = ColumnaAct = 0;
	SetControlsSortida();
	for (i = 0; i < 2; i++) {
		Espera(Timer, 100);
		EscriuPrimeraOrdre(CURSOR_ON | DISPLAY_CLEAR);
		Espera(Timer, 5);
		EscriuPrimeraOrdre(CURSOR_ON | DISPLAY_CLEAR);
		Espera(Timer, 1);
		EscriuPrimeraOrdre(CURSOR_ON | DISPLAY_CLEAR);
		Espera(Timer, 1);
        
		EscriuPrimeraOrdre(CURSOR_ON);
		Espera(Timer, 1);
		CantaIR(FUNCTION_SET | DISPLAY_CONTROL);
        
		WaitForBusy(); 	CantaIR(DISPLAY_CONTROL);  	// Display Off
		WaitForBusy(); 	CantaIR(DISPLAY_CLEAR);	   	// Tot espais
		Espera(Timer,3); // 1.64ms
		WaitForBusy(); 	CantaIR(DISPLAY_ON | CURSOR_ON); // Auto Increment i shift
		WaitForBusy(); 	CantaIR(DISPLAY_CONTROL | DISPLAY_ON | CURSOR_ON | DISPLAY_CLEAR); 		// Display On
	}
}

void LcEnd(void) {
	TiCloseTimer (Timer);
}

void LcClear(void) {
	WaitForBusy(); 	CantaIR(DISPLAY_CLEAR);
	Espera(Timer, 3);
}

void LcCursorOn(void) {
	WaitForBusy();
	CantaIR(DISPLAY_CONTROL | DISPLAY_ON | CURSOR_ON);
}

void LcCursorOff(void) {
	WaitForBusy();
	CantaIR(DISPLAY_CONTROL | DISPLAY_ON);
}

void LcGotoXY(char Columna, char Fila) {
	int Fisica;
	switch (Files) {
		case 2:
			Fisica = Columna + (!Fila ? 0 : 0x40); break;
		case 4:
			Fisica = Columna;
			if (Fila == 1) Fisica += 0x40; else
			if (Fila == 2) Fisica += Columnes;      /* 0x14; */ else
			if (Fila == 3) Fisica += 0x40+Columnes; /* 0x54; */
			break;
		case 1:
		default:
			Fisica = Columna; break;
	}
	
	WaitForBusy();
	CantaIR(SET_DDRAM | Fisica);
	FilaAct    = Fila;
	ColumnaAct = Columna;
}

void LcPutChar(char c) {
	WaitForBusy(); CantaData(c);
	++ColumnaAct;
	if (Files == 3) {
		if (ColumnaAct >= 20) {
			ColumnaAct = 0;
			if (++FilaAct >= 4) FilaAct = 0;
			LcGotoXY(ColumnaAct, FilaAct);
		}
	} else
	if (Files == 2) {
		if (ColumnaAct >= 40) {
			ColumnaAct = 0;
			if (++FilaAct >= 2) FilaAct = 0;
			LcGotoXY(ColumnaAct, FilaAct);
		}
	} else
	if (FilaAct == 1) {
		if (ColumnaAct >= 40) ColumnaAct = 0;
		LcGotoXY(ColumnaAct, FilaAct);
	}
}


void LcPutString(char *s) {
	while(*s) LcPutChar(*s++);
}

void Espera(int Timer, int ms) {
	TiResetTics(Timer);
	while(TiGetTics(Timer) < ms);
}

void CantaPartAlta(char c) {
	 SetD7(c & 0x80 ? 1 : 0);
	 SetD6(c & 0x40 ? 1 : 0);
	 SetD5(c & 0x20 ? 1 : 0);
	 SetD4(c & 0x10 ? 1 : 0);
}

void CantaPartBaixa(char c) {
	 SetD7(c & 0x08 ? 1 : 0);
	 SetD6(c & 0x04 ? 1 : 0);
	 SetD5(c & 0x02 ? 1 : 0);
	 SetD4(c & 0x01 ? 1 : 0);
}

void CantaIR(char IR) {
	SetD4_D7Sortida();
	RSDown();
	RWDown();
	EnableUp();
	CantaPartAlta(IR); 		// Segons hitachi Data Setup = 80ns (cap problema)
	EnableUp();				// Asseguro els 500ns de durada de pols
	EnableDown();   		// i l'amplada del pols "enable" major que 230n
	EnableDown();
	EnableUp();
	CantaPartBaixa(IR); 	// Segons hitachi Data Setup = 80ns (cap problema)
	EnableUp();				// Asseguro els 500ns de durada de pols
	EnableDown();   		// i l'amplada del pols "enable" major que 230n
	SetD4_D7Entrada();
}

void CantaData(char Data) {
	SetD4_D7Sortida();
	RSUp();
	RWDown();
	EnableUp();
	CantaPartAlta(Data); 	// Segons hitachi Data Setup = 80ns (cap problema)
	EnableUp();				// Asseguro els 500ns de durada de pols
	EnableDown();   		// i l'amplada del pols "enable" major que 230n
	EnableDown();
	EnableUp();
	CantaPartBaixa(Data); 	// Segons hitachi Data Setup = 80ns (cap problema)
	EnableUp();				// Asseguro els 500ns de durada de pols
	EnableDown();   		// i l'amplada del pols "enable" major que 230n
	SetD4_D7Entrada();
}

void WaitForBusy(void) { char Busy;
	SetD4_D7Entrada();
	RSDown();
	RWUp();
	TiResetTics(Timer);
	do {
		EnableUp();EnableUp(); // Asseguro els 500ns de durada de pols
		Busy = GetBusyFlag();
		EnableDown();
		EnableDown();
		EnableUp();EnableUp();
		// AquÌ ve la part baixa del comptador d'adreces, que no ens interessa
		EnableDown();
		EnableDown();
		if (TiGetTics(Timer)) break; // MÈs d'un ms vol dir quel LCD est? boig
	} while(Busy);
}

void EscriuPrimeraOrdre(char ordre) {
	// Escric el primer com si fossin 8 bits
	SetD4_D7Sortida();  RSDown(); RWDown();
	EnableUp(); EnableUp();
	 SetD7(ordre & 0x08 ? 1 : 0);
	 SetD6(ordre & 0x04 ? 1 : 0);
	 SetD5(ordre & 0x02 ? 1 : 0);
	 SetD4(ordre & 0x01 ? 1 : 0);
	EnableDown();
}