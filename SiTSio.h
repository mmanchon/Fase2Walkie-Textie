#ifndef SITSIO_H
#define	SITSIO_H

#include <xc.h>
#include "time.h"

#define BUFFER_RX_SIZE   64
#define BUFFER_TX_SIZE  128

void MotorSIO(void);

int SiCharAvail(void);
// Pre: retorna el nombre de car?cters rebuts que no s'han recollit
// amb la funciÛ GetChar encara

char SiGetChar(void);
// Pre: SiCharAvail() Ès major que zero
// Post: Treu i retorna el primer car?cter de la cua de recepciÛ.

void SiSendChar(char c);
// Post: espera que el car?cter anterior s'hagi enviat i envia aquest

void SiPuts(char *s);
// Post: Usa SiSendchar

void SiInit(void);
//Post: Inicialitza la SIO als pins RP2 (Tx) i RP4(Rx)

void SiPutsCooperatiu(char *s);
//Pre: La referència de char *s és o bé un const char o bé puc garantir que
//     no es sobreescriurà fins que no l'hagi enviat...
//Post: Encua *s a la cua de cadenes per enviar...

void MotorSIO(void);

#endif	/* SITSIO_H */