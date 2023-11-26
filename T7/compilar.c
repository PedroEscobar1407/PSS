#define _POSIX_C_SOURCE 200809L
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "pss.h"

Queue *gcola;


void compilar(char* nom){
  struct stat st_nom;
  int rc;
  rc= stat(nom, &st_nom);


  if (rc!=0) {
    printf("%s no existe\n", nom);
    exit(0);
  }

  if (S_ISREG(st_nom.st_mode)) {
    char *p = nom;
    p += strlen(nom)-1;
    if (*p == 'c'){
        char *archivo2 = malloc(strlen(nom)+2);
        strcpy(archivo2,nom);
        p=archivo2;
        p+=strlen(archivo2)-1;
        *p = 'o';
        struct stat st_nom2;
        int rc2 = stat(archivo2,&st_nom2);
        if( rc2 !=0  ||  st_nom.st_mtime > st_nom2.st_mtime){
            put(gcola,strdup(nom));

        }
        free(archivo2);

    }

  }
  else if (S_ISDIR(st_nom.st_mode)) {
    // Es un directorio
    DIR *dir = opendir(nom);
    if (dir == NULL) {
      perror(nom);
      exit(1);
    }
    for (struct dirent *entry = readdir(dir);
         entry != NULL;
         entry = readdir(dir)) {
      if (strcmp(entry->d_name, ".")==0 || strcmp(entry->d_name, "..")==0) {
        continue;
      }
      char *nom_arch= malloc(strlen(nom)+strlen(entry->d_name)+2);
      strcpy(nom_arch, nom);
      strcat(nom_arch, "/");
      strcat(nom_arch, entry->d_name);
      compilar(nom_arch);
      free(nom_arch);
    }
    closedir(dir);
  }
  else {
    // Podria ser un dispositivo, un link simbolico, etc.
    fprintf(stderr, "Archivo %s es de tipo desconocido\n", nom);
    exit(1);
  }
}
int compare(void *c, int a, int b){
    char **nombres = (char**)c;
    return strcmp(nombres[a],nombres[b]);
}


int main(int argc, char *argv[]) {
   if (argc!=2) {
      fprintf(stderr, "uso: %s <arch|dir>\n", argv[0]);
      return 0;
   }
   gcola = makeQueue();
   compilar(argv[1]);
   int a = queueLength(gcola);
   char *nombres[a];
   for (int i = 0; i < a; i++){
      nombres[i]=get(gcola);
   }
   sortPtrArray(nombres,0,a-1,compare);
   for(int j = 0; j < a; j++){
      printf("%s\n", nombres[j]);
      free(nombres[j]);

   }
   destroyQueue(gcola);
   return 0;
}
