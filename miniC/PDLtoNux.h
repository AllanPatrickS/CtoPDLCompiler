#ifndef PDLTONUX_H_
#define PDLTONUX_H_

#include "Converter.h"
#include <string.h>

void conv           (No *no,Nolist *nolist);
void mod            (No *no,Nolist *nolist);
void var            (No *no,Nolist *nolist);
void prog           (No *no);
void progvalue      (Nolist *nolist);
void Assign         ();
void firstElement   (No *no);
void elements       ();
char* stringMemory(char *aux, char *str);

#endif