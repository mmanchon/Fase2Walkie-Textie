 
#ifndef TAD_MSG_H
#define	TAD_MSG_H

#include <xc.h>

void initMSG(void);
//Post: Inicialitza les variables del TAD MSG.

void setCharMSG(char msg,int i);
//Pre: 0 <= i < 320.
//Post: Guarda el char msg a la posició i del missatge.

char getCharMSG(int i);
//Pre: 0 <= i < 320.
//Post: Retorna el valor de missatge[i].

char* getMessage(void);
//Post: Retorna un punter a l'array de missatge.

#endif /* TAD_MSG_H */
