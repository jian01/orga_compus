#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "base64.h"

#define MAX_BUF_SIZE 256

#define AYUDA "-h"
#define VERSION "-V"
#define version_imprimible "1.0"
#define ERROR_PARAMETROS "Invalid parameters. Use help menu and try again."

#define INPUT_FILE "-i"
#define OUTPUT_FILE "-o"
#define ACTION "-a"

#define ENCODE "encode"
#define DECODE "decode"
#define ERROR_ACCION "Invalid action. Please use an allowed action. See the help menu."
#define ERROR_NO_ACCION "Please, specify the action. Default action is encoding."

void imprimir_ayuda(){
  printf("\
  Usage:\
  tp0 -h\n\
  tp0 -V\n\
  tp0 [options]\n\
  Options:\n\
  -V, --version\n\
  Print version and quit.\
  -h, --help\n\
  Print this information.\
  -i, --input\n\
  Location of the input file.\n\
  -o, --output\n\
  Location of the output file.\n\
  -a, --action\n\
  Program action: encode (default) or decode.\n\
  Examples:\n\
  tp0 -a encode -i ~/input -o ~/output\n\
  tp0 -a decode\n\
  ");
}

char* obtener_stdin(){
  char* linea;
  fgets(linea, MAX_BUF_SIZE, stdin);
  printf("%s\n", linea);
  return linea;
}
int encode_stdin(){
  printf("%s\n", "ASDASd");
  char* linea = obtener_stdin();
  char* encoding;
  base64_encode_bytes(linea, strlen(linea), &encoding);
  printf("%s\n", encoding);
  return 1;
}

int decode_stdin(){
  printf("%s\n", "sddasadin");
  return 1;
}

int main(int argc, char* argv[]){
  int i = 1;

  //default encode entrada estandar.
  if (argc == 1){
    encode_stdin();
    return EXIT_SUCCESS;
  }

  //ayuda o version.
  if (argc == 2){
    if (strcmp(argv[i], AYUDA) == 0)
      imprimir_ayuda();
    else if (strcmp(argv[i], VERSION) == 0)
      printf("%s\n", version_imprimible);
    else if (strcmp(argv[i], ACTION) == 0)
      fprintf(stderr, "%s\n", ERROR_NO_ACCION);
    else
      fprintf(stderr, "%s\n", ERROR_PARAMETROS);
    return EXIT_SUCCESS;
  }

  //encode o decode.
  else if (argc > 2 && argc < 8){
    if(strcmp(argv[i], ACTION) == 0){
      i++;
      bool encode;
      if (strcmp(argv[i], ENCODE) == 0) encode = true;
      else if (strcmp(argv[i], DECODE) == 0) encode = false;
      else{
        fprintf(stderr, "%s\n", ERROR_ACCION);
        return EXIT_SUCCESS;
      }
      i++;
      //caso ./main -a encode(decode) sin archivos.
      if (!argv[i]){
        if (encode)
          encode_stdin();
        else
          decode_stdin();
      }
    }
  }

  return EXIT_SUCCESS;
}
