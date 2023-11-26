#include <stdio.h>
#include <stdlib.h>

#include "elim-rango.h"

void eliminarRango(Nodo **phead, double y, double z) {
    Nodo *head = *phead; /*creamos un puntero con la informacion del primer nodo*/
    Nodo *ref= NULL; /*inicializamos puntero con direccion de memoria invalida*/

    while (head != NULL && head->x < y) {  /*aqui buscamos que el nodo tenga algun valor que no sea null y que el valor 'x' sea menor que 'y' para encontrar el primer nodo del rango a eliminar*/
        ref = head; /*le cambiamos la direccion de memoria a puntero ref que sea igual a la direccion de head*/
        head = head->prox; /*pasamos al siguiente nodo*/
    }

    while (head!= NULL && head->x <= z) { /*en este while solo ingresarian valores 'x' tal que: y<=x<=z*/
        Nodo *tmp = head; /* creamos otro puntero de tipo Nodo * para eliminar el nodo de la direccion en la que estamos*/
        head = head->prox; /*avanzamos igual que en el while pasado*/
        free(tmp); /*eliminamos el nodo con free para liberar memoria*/
    }

    if (ref == NULL) { /* esto significa que nunca entro en el primer while, asi que simplemente se le asigna el puntero en el que quedo head*/
        *phead = head;
    } else {
        ref->prox = head; /*hacemos que el proximo nodo de ref sea el de la direccion de head*/
    }/*basicamente reacomodamos punteros para que no arrojen errores de estar en direcciones invalidas*/
}
