/* MaTest.c - Matrix Tester. Tester de matrices para lógicas multivaluadas.
   Copyright (C) 2008, César González Gutiérrez <ceguel@gmail.com>

   Este programa es software libre: usted puede redistribuirlo y/o modificarlo
   bajo los términos de la Licencia Pública General GNU publicada
   por la Fundación para el Software Libre, ya sea la versión 3
   de la Licencia, o (a su elección) cualquier versión posterior.

   Este programa se distribuye con la esperanza de que sea útil, pero
   SIN GARANTÍA ALGUNA; ni siquiera la garantía implícita
   MERCANTIL o de APTITUD PARA UN PROPÓSITO DETERMINADO.
   Consulte los detalles de la Licencia Pública General GNU para obtener
   una información más detallada.

   Debería haber recibido una copia de la Licencia Pública General GNU
   junto a este programa.
   En caso contrario, consulte <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string.h>

#define MAX_FORMULA_LENGHT 256


int main(void) {
  int i,j;
  int dim,mdv;
  char opt;
  char formula[MAX_FORMULA_LENGHT];


  /* Petición de la dimensión de las matrices y asignación del mínimo valor
     designado según el modelo de Łukassiewizc (único valor y el mayor) */
  do {
    printf("Introduzca la dimension de la matriz: ");
    scanf("%i", &dim);
  } while ( dim<2 || dim>10 );
  mdv = dim-1;


  /* Inicialización de las matrices, segun la definición de Łukassiewizc */
  int C[dim][dim];  /* Implicación */
  for (i=0;i<dim;i++) {
    for (j=0;j<dim;j++) {
      if ((dim-1)<((dim-1)-i+j))
        C[i][j]=(dim-1);
      else C[i][j]=((dim-1)-i+j);
    }
  }
  int K[dim][dim];  /* Conjunción */
  for (i=0;i<dim;i++) {
    for (j=0;j<dim;j++) {
      if (i<j)
        K[i][j]=i;
      else K[i][j]=j;
    }
  }
  int A[dim][dim];  /* Disyunción */
  for (i=0;i<dim;i++) {
    for (j=0;j<dim;j++) {
      if (i>j) A[i][j]=i;
      else A[i][j]=j;
    }
  }
  int N[dim];       /* Negación */
  for (i=0;i<dim;i++)
    N[i]=dim-1-i;
  int L[dim];       /* Necesidad L */
  for (i=0;i<dim;i++)
    L[i]=dim-1;
  int M[dim];       /* Posibilidad M */
  for (i=0;i<dim;i++)
    M[i]=dim-1-i;


  /***
    Procecimiento menu():
     imprime el menú principal y guarda la opcion seleccionada.
  ***/
  void menu(void) {
    printf("\n\n");
    printf("V) Redefinir minimo valor designado\n\n");
    printf("P) Ver matrices\n");
    printf("Redefinir matrices:\n");
    printf("  I) Implicacion  C) Conjuncion  D) Disyuncion\n");
    printf("  N) Negacion     L) Necesidad   M) Posibilidad\n\n");
    printf("F) Introducir formula\n");
    printf("E) Evaluar formula\n\n");
    printf("Q) Salir\n\n");
    printf("Elija su opcion: ");
    scanf("\n%c", &opt);
    opt=tolower(opt);
  }

  /***
    Procecimiento show_matrix():
     imprime las matrices.
  ***/
  void show_matrix(void) {
    printf("\n");
    printf("->|");    /* Implicación */
    for (i=0;i<dim;i++) printf("  %i", i);
    printf("\n");
    for (i=0;i<=dim;i++) printf("---");
    printf("-\n");
    for (i=0;i<dim;i++) {
      if (i>=mdv) printf("*%i|", i);
      else printf(" %i|", i);
      for (j=0;j<dim;j++) {
        if (C[i][j]>=mdv) printf(" *%i", C[i][j]);
        else printf("  %i", C[i][j]);
      }
      printf("\n");
    }
    printf("\n ^|");    /* Conjunción */
    for (i=0;i<dim;i++) printf("  %i", i);
    printf("\n");
    for (i=0;i<=dim;i++) printf("---");
    printf("-\n");
    for (i=0;i<dim;i++) {
      if (i>=mdv) printf("*%i|", i);
      else printf(" %i|", i);
      for (j=0;j<dim;j++) {
        if (K[i][j]>=mdv) printf(" *%i", K[i][j]);
        else printf("  %i", K[i][j]);
      }
      printf("\n");
    }
    printf("\n v|");    /* Disyunción */
    for (i=0;i<dim;i++) printf("  %i", i);
    printf("\n");
    for (i=0;i<=dim;i++) printf("---");
    printf("-\n");
    for (i=0;i<dim;i++) {
      if (i>=mdv) printf("*%i|", i);
      else printf(" %i|", i);
      for (j=0;j<dim;j++) {
        if (A[i][j]>=mdv) printf(" *%i", A[i][j]);
        else printf("  %i", A[i][j]);
      }
      printf("\n");
    }
    printf("\n ~|");    /* Negacion */
    for (i=0;i<dim;i++) printf("  %i", i);
    printf("\n");
    for (i=0;i<=dim;i++) printf("---");
    printf("-\n  |");
    for (i=0;i<dim;i++) {
      if (N[i]>=mdv) printf(" *%i", N[i]);
      else printf("  %i", N[i]);
    }
    printf("\n\n L|");    /* Necesidad */
    for (i=0;i<dim;i++) printf("  %i", i);
    printf("\n");
    for (i=0;i<=dim;i++) printf("---");
    printf("-\n  |");
    for (i=0;i<dim;i++) {
      if (L[i]>=mdv) printf(" *%i", L[i]);
      else printf("  %i", L[i]);
    }
    printf("\n\n M|");    /* Posibilidad */
    for (i=0;i<dim;i++) printf("  %i", i);
    printf("\n");
    for (i=0;i<=dim;i++) printf("---");
    printf("-\n  |");
    for (i=0;i<dim;i++) {
      if (M[i]>=mdv) printf(" *%i", M[i]);
      else printf("  %i", M[i]);
    }
  printf("\n\nPulse ENTER para continuar");
  getchar();
  getchar();
  }


  /***
    Función is_wf():
     "is well formed", devuelve 1 si la fórmula es una fbf en notación polaca,
     imprime avisos cuando detecta errores.
  ***/
  int is_wf(char *string) {
    int i,crtl;
    int error=1;
    /* Discriminar fórmulas demasiado largas */
    if (string[MAX_FORMULA_LENGHT-1]!=0) {
      printf("La formula es demasiado larga.\n");
      error=0;
      return error;
    }
    /* Detección de caracteres no implementados */
    for (i=0; string[i]!=0; i++) {
      if (string[i]!='C' &&
          string[i]!='K' &&
          string[i]!='A' &&
          string[i]!='N' &&
          string[i]!='L' &&
          string[i]!='M' &&
          string[i]!='p' &&
          string[i]!='q' &&
          string[i]!='r' &&
          string[i]!='s' &&
          string[i]!='t') {
        printf("No se reconoce el caracter \"%c\"!\n", string[i]);
        error=0;
      }
    }
    if (error==0) return error;
    /* Chequeo propiamente dicho */
    crtl=1;
    for (i=0; string[i]!=0; i++) {
      if ( string[i]=='N' || string[i]=='L' || string[i]=='M' )
        i++;
      if ( string[i]=='A' || string[i]=='C' || string[i]=='K' )
        crtl++;
      if ( string[i]>='p' && string[i]<='t' )
        crtl--;
      if ( crtl==0 && string[i+1]!=0 ) {
        printf("La formula \"%s\" es una FMF!\n", string);
        error=0;
        return error;
      }
    }
    if (crtl!=0) {
      printf("La formula \"%s\" es una FMF!\n", string);
      error=0;
    }
    return error;
  }


  /***
    Procedimiento evaluate():
     imprime la evaluación de la fórmula .
  ***/
  void evaluate(char *wff) {
    int p,q,r,s,t;
    int ep=0,eq=0,er=0,es=0,et=0;
    int i,j;
    char eval[strlen(wff)];
    
    printf("\n%s\n", wff);
    for (i=0; i<strlen(wff); i++) printf("-");
    printf("\n");
    
    for (t=0; t<dim; t++) {
      for (s=0; s<dim; s++) {
        for (r=0; r<dim; r++) {
          for (q=0; q<dim; q++) {
            for (p=0; p<dim; p++) {
              /* Evitamos los bucles para las variables
                 inexistentes en la fórmula */
              for ( i=0 ; wff[i]!=0 ; i++ ) {
                if      ( wff[i]=='p' ) ep=1;
                else if ( wff[i]=='q' ) eq=1;
                else if ( wff[i]=='r' ) er=1;
                else if ( wff[i]=='s' ) es=1;
                else if ( wff[i]=='t' ) et=1;
              }
              if (ep==0) p=dim;
              if (eq==0) q=dim;
              if (er==0) r=dim;
              if (es==0) s=dim;
              if (et==0) t=dim;
              /* Damos a cada variable su valor correspondiente
                 convertido en ASCII (+48) */
              for (i=strlen(wff); i>=0; i--) {
                if      ( wff[i]=='p' ) eval[i]=p+48;
                else if ( wff[i]=='q' ) eval[i]=q+48;
                else if ( wff[i]=='r' ) eval[i]=r+48;
                else if ( wff[i]=='s' ) eval[i]=s+48;
                else if ( wff[i]=='t' ) eval[i]=t+48;
                else eval[i]=wff[i];
              }
              printf("%s", eval);
              /* Hallamos el valor que se le asigna a la fórmula. */
              while ( eval[0]<'0' || eval[0]>'9' ) {
                for (i=0; i<=strlen(eval); i++) {
                  if ( eval[i]=='C' && 
                       eval[i+1]>='0' && eval[i+1]<=dim-1+48 && 
                       eval[i+2]>='0' && eval[i+2]<=dim-1+48) {
                    eval[i]=C[eval[i+1]-48][eval[i+2]-48]+48;
                    for (j=i+1; j<=strlen(eval); j++)
                      eval[j]=eval[j+2];
                  }
                  else if ( eval[i]=='K' && 
                            eval[i+1]>='0' && eval[i+1]<=dim-1+48 && 
                            eval[i+2]>='0' && eval[i+2]<=dim-1+48) {
                    eval[i]=K[eval[i+1]-48][eval[i+2]-48]+48;
                    for (j=i+1; j<=strlen(eval); j++)
                      eval[j]=eval[j+2];
                  }
                  else if ( eval[i]=='A' && 
                            eval[i+1]>='0' && eval[i+1]<=dim-1+48 && 
                            eval[i+2]>='0' && eval[i+2]<=dim-1+48) {
                    eval[i]=A[eval[i+1]-48][eval[i+2]-48]+48;
                    for (j=i+1; j<=strlen(eval); j++)
                      eval[j]=eval[j+2];
                  }
                  else if ( eval[i]=='N' && 
                            eval[i+1]>='0' && eval[i+1]<=dim-1+48 ) {
                    eval[i]=N[eval[i+1]-48]+48;
                    for (j=i+1; j<=strlen(eval); j++)
                      eval[j]=eval[j+1];
                  }
                  else if ( eval[i]=='L' && 
                            eval[i+1]>='0' && eval[i+1]<=dim-1+48 ) {
                    eval[i]=L[eval[i+1]-48]+48;
                    for (j=i+1; j<=strlen(eval); j++)
                      eval[j]=eval[j+1];
                  }
                  else if ( eval[i]=='M' && 
                       eval[i+1]>='0' && eval[i+1]<=dim-1+48 ) {
                    eval[i]=M[eval[i+1]-48]+48;
                    for (j=i+1; j<=strlen(eval); j++)
                      eval[j]=eval[j+1];
                  }
                }
              } 
              printf(" %c", eval[0]);
              if ( (eval[0]-48)>=mdv )
                printf("*\n");
              else printf("\n");
            }
          }
        }
      }
    }
  }


  /* Manejador de opciones */
  for (;;) {
    menu();
    switch (opt) {
      case 'v': do {
                  printf("Nuevo minimo valor designado: ");
                  scanf("%i", &mdv);
                  } while (mdv>dim-1 || mdv<0);
                  break;
      case 'i': for (i=0;i<dim;i++) {
                  for (j=0;j<dim;j++) {
                    do {
                      printf("Implicacion %i %i: ", i,j);
                      scanf("%i", &C[i][j]);
                    } while (C[i][j]>=dim);
                  }
                }
                break;
      case 'c': for (i=0;i<dim;i++) {
                  for (j=0;j<dim;j++) {
                    do {
                      printf("Conjuncion %i %i: ", i,j);
                      scanf("%i", &K[i][j]);
                    } while (K[i][j]>=dim);
                  }
                }
                break;
      case 'd': for (i=0;i<dim;i++) {
                  for (j=0;j<dim;j++) {
                    do {
                      printf("Disyuncion %i %i: ", i,j);
                      scanf("%i", &A[i][j]);
                    } while (A[i][j]>=dim);
                  }
                }
                break;
      case 'n': for (i=0;i<dim;i++) {
                  do {
                    printf("Negacion %i: ", i);
                    scanf("%i", &N[i]);
                  } while (N[i]>=dim);
                }
                break;
      case 'l': for (i=0;i<dim;i++) {
                  do {
                    printf("Necesidad (L) %i: ", i);
                    scanf("%i", &L[i]);
                  } while (L[i]>=dim);
                }
                break;
      case 'm': for (i=0;i<dim;i++) {
                  do {
                    printf("Posibilidad (M) %i: ", i);
                    scanf("%i", &M[i]);
                  } while (M[i]>=dim);
                }
                break;
      case 'p': show_matrix();
                break;
      case 'f': do {
                  /* Inicialización del vector de caracteres */
                  for (i=0; i<MAX_FORMULA_LENGHT; i++)
                    formula[i]=0;
                  printf("\nIntroduzca la formula: ");
                  scanf("\n%s", formula);
                } while ( !is_wf(formula) );
                break;           
      case 'e': if (formula[0]==0)
                  printf ("\nPrimero debe introducir una formula!\n");
                else {
                  evaluate(formula);
                  printf("\nPulse ENTER para continuar");
                  getchar();
                  getchar();
                }
                break;
      case 'q': return 0;
    }
  }
}

