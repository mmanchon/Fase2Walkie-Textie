#ifndef AUTAUDIO_H
#define	AUTAUDIO_H
#include "time.h"

#define SET_AUDIO_DIR()     TRISAbits.TRISA1 = 0;
#define AUDIO_ON()          LATAbits.LATA1=1;
#define AUDIO_OFF()         LATAbits.LATA1=0;

void AuInit(void);
//Posa el port RA1 de sortida i demana un timer

void MotorAudio(void);

char changeAudioStatus();
//Post Canvia l'estat d'audio

char getAudioStatus(void);
//Post: retorna 1 si audio ences o 0 si apagat

char getAudioPeriode(void);
// Post: Retorna el valor del periode

void setAudioPeriode(char nouPeriode);
// Pre: nouPeriode >= 1

void setMelodia(char aux[10]);
//Post: Canvi la melodia que reproduir� l'altaveu per aux.

void stopMelodia();
//Post: Desactivael flag que mant� l'altaveu ences.

#endif	/* AUTAUDIO_H */
