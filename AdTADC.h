#ifndef ADTADC_H
#define	ADTADC_H

#include <xc.h>

void AdInit(void);
//Inicialitza el conversor A/D únicament per al canal RA0

int AdGetMostra(void);
//Post: Retorna la mostra convertida en 10 bits

#endif /* ADTADC_H */

