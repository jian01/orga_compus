#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "base64.h"

#define BYTES_A_LEER 120

#define AYUDA "-h"
#define VERSION "-V"
#define version_imprimible "1.0"
#define ERROR_PARAMETROS "Invalid parameters. Use help menu and try again."
#define ERROR_CANTIDAD_PARAMETROS "Invalid number of parameters."

#define INPUT_FILE "-i"
#define OUTPUT_FILE "-o"
#define ACTION "-a"
#define ERROR_ARCHIVO "Invalid file. Please use a file that exist or check the path."
#define ERROR_NO_INPUT_FILE "Missing input file. Try again."
#define ERROR_NO_OUTPUT_FILE "Missing output file. Try again."


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


int encode_file(FILE* input_file, FILE* output_file){
  bool stop = false;
  int longitud;  
  while (!stop){
    char byte_leido;
    char* encoding;
    char* linea = (char*)malloc(sizeof(char)*BYTES_A_LEER);
    int cantidad_bytes_leidos = 0;  
    while (cantidad_bytes_leidos < BYTES_A_LEER && (byte_leido = fgetc(input_file)) != EOF)  {         
      linea[cantidad_bytes_leidos] = byte_leido;
      cantidad_bytes_leidos++;
    }
    for(int i=0;i<cantidad_bytes_leidos;i++){
      fprintf(stderr, " %#02x ", linea[i]);
    }
    fprintf(stderr, "\n");
    if(byte_leido == EOF) stop = true;
    if(cantidad_bytes_leidos != 0){
      longitud = base64_encode_bytes(linea, cantidad_bytes_leidos, &encoding);
      fwrite((void*)encoding, 1, longitud, output_file);
      free(encoding);
    }       
    free(linea);
  }   
  
  return 1;
}

int decode_file(FILE* input_file, FILE* output_file){
  bool stop = false;
  int longitud;
  while(!stop){
    char* linea = malloc(sizeof(char)*BYTES_A_LEER);
    char byte_leido;
    char* decoding;
    int i;
    int cantidad_bytes_leidos = 0;
    while (cantidad_bytes_leidos < BYTES_A_LEER && (byte_leido = fgetc(input_file)) != EOF) {      
      linea[cantidad_bytes_leidos] = byte_leido;
      cantidad_bytes_leidos++;
    }
    if(byte_leido == EOF) stop = true;  
    if(cantidad_bytes_leidos != 0){
      longitud = base64_decode_bytes(linea, cantidad_bytes_leidos, &decoding);
      for(int i=0;i<longitud;i++){
        fprintf(stderr, " %#02x ", decoding[i]);
      }
      fprintf(stderr, "\n");
      fwrite((void*)decoding, 1, longitud, output_file);
      free(decoding);
    }    
    free(linea);  
  }
  return 1;
}

int main(int argc, char* argv[]){
  int i = 1;

  //default encode entrada estandar.
  if (argc == 1){
    encode_file(stdin, stdout);
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
          encode_file(stdin, stdout);
        else
          decode_file(stdin, stdout);
        return EXIT_SUCCESS;
      }
      //./main -a encode(decode) -i inputfile
      if (strcmp(argv[i], INPUT_FILE) == 0){
        i++;
        if(!argv[i]){
          fprintf(stderr, "%s\n", ERROR_NO_INPUT_FILE);
          return EXIT_SUCCESS;
        }
        FILE* input_file = fopen(argv[i], "r");
        if(!input_file){
          fprintf(stderr, "%s\n", ERROR_ARCHIVO);
          return EXIT_SUCCESS;
        }
        i++;
        if(!argv[i]){
          fprintf(stderr, "%s\n", ERROR_NO_OUTPUT_FILE);
          fclose(input_file);
          return EXIT_SUCCESS;
        }
        // ./main -a encode(decode) -i inputfile -o outputfile
        if(strcmp(argv[i], OUTPUT_FILE) == 0){
          i++;
          if(!argv[i]){
            fprintf(stderr, "%s\n", ERROR_NO_OUTPUT_FILE);
            fclose(input_file);
            return EXIT_SUCCESS;
          }
          FILE* output_file = fopen(argv[i], "w");
          if(encode){
            encode_file(input_file, output_file);
          }
          else{
            decode_file(input_file, output_file);
          }
        }
        else{
          fprintf(stderr, "%s\n", ERROR_PARAMETROS);
          fclose(input_file);
          return EXIT_SUCCESS;
        }
      }
      else{
        fprintf(stderr, "%s\n", ERROR_PARAMETROS);
        return EXIT_SUCCESS;
      }
    }
  }
  else{
    fprintf(stderr, "%s\n", ERROR_CANTIDAD_PARAMETROS);
  }
  return EXIT_SUCCESS;
}

