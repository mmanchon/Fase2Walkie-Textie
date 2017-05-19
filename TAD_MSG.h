 
#ifndef TAD_MSG_H
#define	TAD_MSG_H

#include <xc.h> // include processor files - each processor file is guarded.  
void initMSG(void);
void setCharMSG(char msg,int i);
char getCharMSG(int i);
char* getMessage(void);
#endif
