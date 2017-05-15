#ifndef TAD_RF_H
#define TAD_RF_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "time.h"
#include "SiTSio.h"
#include "PrTPropaganda.h"

#define SET_RF_DIR()  TRISBbits.TRISB13 = 1;
#define GET_RF() PORTBbits.RB13;

void InitRF(void);
void MotorRF(void);

#endif