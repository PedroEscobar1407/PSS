// Plantilla para maleta.c

#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>

#include "maleta.h"

// Use la funcion leer para obtener los resultados del pipe
int leer(int fd, void *vbuf, int n) {
  char *buf= vbuf;
  do {
    int rc= read(fd, buf, n);
    if (rc<=0)
      return 1; /* fracaso: error o fin del archivo/pipe/socket */
    n-= rc; /* descontamos los bytes leídos */
    buf+= rc; /* avanzamos el buffer para no reescribir lo leido previamente */
  } while (n>0); /* mientras no leamos todo lo que esperamos */
  return 0; /* exito */
}

double llenarMaleta(double w[], double v[], int z[], int n,
                    double maxW, int k) {
    int pids[8];
    int fds[8][2];

    for(int i=0; i<8; i++){
        pipe(fds[i]);
        pids[i] = fork();
        srandom(getUSecsOfDay()*getpid());

        if(pids[i] == 0){ // hijo
          close(fds[i][0]);
          double res = llenarMaletaSec(w, v, z, n, maxW, k/8);
          write(fds[i][1], &res, sizeof(double));
          for( int j=0;j<n; j++){
              write(fds[i][1], &z[j], sizeof(int));
          }
        exit(0);
        }
        else{ // padre
          close(fds[i][1]);
        }
    }

    double res = -1;
    int z_cpy[n];
    for(int i=0; i<8; i++){
        double res_hijo;
        leer(fds[i][0], &res_hijo, sizeof(double));
        for(int j=0;j<n;j++){
            leer(fds[i][0], &z_cpy[j], sizeof(int));
        }
        close(fds[i][0]);
        waitpid(pids[i], NULL, 0);
        if(res_hijo > res){
            res = res_hijo;
            for( int j=0; j<n; j++){
                z[j] = z_cpy[j];
            }
        }
    }
    return res;
}


