#ifndef TAD_RF_H
#define TAD_RF_H

#include <xc.h>  
#include "time.h"
#include "SiTSio.h"
#include "TAD_Control.h"
#include "TAD_MSG.h"

#define SET_RF_DIR()  TRISBbits.TRISB13 = 1;
#define GET_RF() PORTBbits.RB13;

void InitRF(void);
//Post: Prepara les variables i el timer pr al motor RF.

void MotorRF(void);
//Pre: InitRF().

#endif /* TAD_RF_H */