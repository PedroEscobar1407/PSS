#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "reemplazar.h"

/*PARTE A*/
char *reemplazo(char *s, char c, char *pal) {
  int len_s = 0;
  int len_pal = strlen(pal);  /* Primero recorremos el string*/
  char *p = s;  /*apuntamos al primer caracter de s*/
  while (*p != 0) {
    if (*p == c) {
        len_s += len_pal; /* si lo del puntero es el caracter que vamos a reemplazar por la palabra, entonces le sumamos el largo de len_pal*/
    } else {
        len_s++; /*si no, solamente aumento el largo en 1*/
    }
    p++;
  }
  char *rep = malloc(len_s + 1); /*pedimos la memoria necesaria para el nuevo string*/
  char *ptr_rep = rep;/*creamos el puntero*/
  p = s; /* nos vamos a la direccion del primer caracter del string*/
  while (*p != 0){
    if (*p == c){
        strcpy(ptr_rep, pal); /* si lo del puntero es el caracter que vamos a reemplazar, copiamos pal en la direccion de memoria del rep*/
        ptr_rep += len_pal;   /* y sumamos el len_pal para tener el puntero en el caracter que nos interesa y no examinar los caracter de pal*/
    } else{
        *ptr_rep = *p; /* si no, reemplazamos por el caracter visitado solamente y sumamos uno al puntero de rep*/
        ptr_rep++;
    }
    p++;
  }
  *ptr_rep = 0; /* NO OLVIDAR CERRAR EL STRING!!! */
  return rep;   /*retornamos porque declaramos retornar un char*  */
}



/* PARTE B*/
void reemplazar(char *s, char c, char *pal) {
  int len_s = 0;
  int len_pal = strlen(pal);  /* Primero recorremos el string*/
  char *p = s;  /*apuntamos al primer caracter de s*/
  while (*p != 0) {
    if (*p == c) {
        len_s += len_pal; /* si lo del puntero es el caracter que vamos a reemplazar por la palabra, entonces le sumamos el largo de len_pal*/
    } else {
        len_s++; /*si no, solamente aumento el largo en 1*/
    }
    p++;
  }
/*basicamente la parte anterior fue un copiar/pegar de la parte a*/

  if (len_pal == 0){   /*caso cuando el pal no tiene caracter dentro del string*/
    char *p2 = s;
    p = s; /*definimos ambos punteros al inicio de s*/
    while (*p != 0){
      if (*p == c){
        p++; /*mutamos en 1 la direccion del puntero p para asi copiar el vecino del caracter que queremos reemplazar por "" */
        *p2 = *p;
        p2++;
      } else {
        *p2 = *p; /* el paso base de toda la vida*/
        p2++;
      }
      p++;
    }
    *p2 = 0; /* NO OLVIDAR CERRAR EL STRING!!! */

  } else if (len_pal == 1){
      char *p2 = s;
      p = s; /*definimos ambos punteros al inicio de s*/
      while (*p !=0){
        if(*p == c){
          *p2 = *pal; /*como len_pal = 1, copiamos y pegamos nomas, 0 drama*/
          p2 ++;
        } else {
          *p2 = *p; /* el paso base de toda la vida*/
          p2++;
        }
        p++;
      }
    *p2 = 0;  /* NO OLVIDAR CERRAR EL STRING!!! */
  } else {
    char *p2 = s + len_s; /*puntero al final del string resultante*/
    *p2 = 0;  /* NO OLVIDAR CERRAR EL STRING!!! */
    p2--;/* avanzo a la izquierda para no estar apuntando al 0*/
    p = s + strlen(s) - 1;  /*puntero al final del string del inicio*/
    while (p >= s){     /* mientras no pase a la izquierda de la direccion del inicio del string s */
      if(*p == c){
        char *t = pal + len_pal - 1;  /* nos definimos un puntero para "pal" que se ubique al final tambien*/
        while(t >= pal){  /* misma funcion del while de arriba*/
          *p2 = *t;
          p2--;
          t--;  /*como son mas caracteres que reemplazar hay que darle el espacio para que se copien bien de derecha a izquierda*/
        }
      } else {
        *p2 = *p;   /* el paso base de toda la vida*/
        p2--; /*diferencia que aca vamos de derecha a izquierda*/
      }
      p--;
    }
  }
}
