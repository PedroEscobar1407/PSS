#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "pss.h"

#define MAXTAM (800+2)

int main(int argc, char *argv[]) {
    //sacado de los apuntes del profesor, tiene que ver con el error de los parametros
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <diccionario> <llave>\n", "./consultar.bin");
        return -1;
    }
    //abrimos el archivo en modo read y si no se puede abrir el archivo, arrojamos error con perror
    FILE* f = fopen(argv[1], "r");
    if (f == NULL) {
        perror(argv[1]);
        return -1;
    }

    //calculamos el tamano de la primera linea del txt
    char primera_linea[MAXTAM];
    size_t largo = strlen(argv[2]); //largo de la llave para comodidad mas adelante
    fgets(primera_linea, sizeof(primera_linea), f);
    size_t tamano_linea = strlen(primera_linea);
    if (tamano_linea == 0|| primera_linea[tamano_linea - 1] != '\n') { //si la primera linea no tiene largo o si no termina con '\n' porque puede haber un espacio vacio entonces arroja diccionario vacio
        fprintf(stderr, "%s: el dicionario esta vacio\n", argv[1]);
        return 1;
    }

    // calcular el número de líneas y el tamaño del archivo
    fseek(f, 0, SEEK_END);
    long tamano_archivo = ftell(f);
    int num_lineas = tamano_archivo / tamano_linea;
    if (tamano_archivo % tamano_linea != 0 ){ //si el tamano del archivo no es multiplo con el tamano de las lineas
      fprintf(stderr,"%s: el tamaño del archivo no es multiplo del tamaño de la linea\n", argv[1]);
      return -1;
    }

    // calcular el hash de la llave consultada
    unsigned int hash_consulta = hash_string(argv[2]);
    int indice_linea = hash_consulta % num_lineas; //hint del profesor en el enunciado

    // buscar la llave en el archivo
    while(1){
        char linea[MAXTAM]; //buffer
        fseek(f,indice_linea * tamano_linea,SEEK_SET); //tengo acceso directo a la linea que me interesa
        fgets(linea, largo + 1, f); //copio la llave que me interesa saber
        if (strcmp(linea,argv[2])==0){//si la llave que copie es igual a la original
            fgets(linea, 2, f);//lo muevo para copiar el caracter ':'
            if (strcmp(linea,":")!=0){ //si no existe el caracter ':', entonces arroja el error
                fprintf(stderr,"%s: linea %d no posee : para terminar la llave\n", argv[1], indice_linea);
                return -1;
            }
            fgets(linea,tamano_linea, f);//copio la definicion del diccionario y la printeo
            printf("%s\n",linea);
            break; //rompo el while
        } else if (strcmp(linea,"\n")==0){ //si la linea es solo un espacio vacio, entonces no contiene la llave por enunciado
            fprintf(stderr,"%s: el diccionario no contiene la llave %s\n", argv[1], argv[2]);
            return -1;
        } else {
            fseek(f,indice_linea*tamano_linea,SEEK_SET);//vuelvo a colocar el acceso directo a la linea de interes
            fgets(linea,sizeof(linea),f);//calculo el tamano de la linea con sizeof para saber si es un largo variable
            if (strlen(linea)!= tamano_linea){ // si tiene un largo distinto a mi primera linea calculada, entonces que arroje el error de largos de lineas variables
                fprintf(stderr,"%s: linea %d de tamaño incorrecto\n", argv[1], indice_linea);
                return -1;
            }
            indice_linea = (indice_linea+1) % num_lineas; //aumento el paso para pasar a la siguiente linea
            if (indice_linea == hash_consulta%num_lineas){ //si es que ya me di toda la vuelta entonces no contiene la llave (para el caso de diccionario completo)
                fprintf(stderr,"%s: el diccionario no contiene la llave %s\n", argv[1], argv[2]);
                return -1;
            }
        }

    }
    fclose(f);//cierro el archivo
    return 0;
}


