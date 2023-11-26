#include <stdlib.h>
#include <string.h>

#include "comprimir.h"
typedef unsigned int uint;
uint comprimir(uint a[], int nbits) {
  uint mask = ~(0xffffffff << (nbits-1) << 1); // cuando nbits = 9   1111.....11111 -> 11111....000000000 -> complemento a 1   00000....111111111
  uint i = 0; //variable temporal para revisar el arreglo
  uint resultado = 0; // variable que retornaremos con la concatenacion de binarios con nbits
  for (int k = 0; k+nbits <= sizeof(uint)<<3; k+=nbits){
  resultado = resultado << (nbits-1) << 1; // desplazamos el resultado para poder unir los nuevos bits
  resultado = resultado | (a[i] & mask); // usamos mascara pa borrar todo los bits que no queremos y lo unimos con los resultados anteriores
  i++; // aumentamos el i para seguir buscando los elementos en el arreglo
  }
  return resultado;
}
