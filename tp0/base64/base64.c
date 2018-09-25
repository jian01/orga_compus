#include "bit_library.h"
#include "base64.h"
#include <string.h>
#include <stdio.h>

static const char base64_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M' ,'N', 'O', 'P',
                                    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                    'g', 'h', 'i', 'j', 'k', 'l', 'm' ,'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                    'w', 'x', 'y', 'z','0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

char decode_byte(char byte){
  if(byte>='A' && byte<='Z') return (byte-'A');
  if(byte>='a' && byte<='z') return (byte-'a')+26;
  if(byte>='0' && byte<='9') return (byte-'0')+52;
  if(byte=='+') return 62;
  return 63;
}

char* base64_transform_3bytes(char* bytes){
  char* base64 = (char*)malloc(4);
  if(!base64) return NULL;
  for(int i=0;i<4;i++){
    char* extracted = bytes_extract_n_bits(bytes, i*6, i*6+6-1);
    if(!extracted){
      free(base64);
      return NULL;
    }
    if(i==0){
      base64[i] = extracted[0] >> 2;
    } else if(i==1){
      base64[i] = extracted[0] << 4 | extracted[1] >> 4;
    } else if(i==2){
      base64[i] = extracted[0] << 2 | extracted[1] >> 6;
    } else {
      base64[i] = extracted[0];
    }
    free(extracted);
    base64[i] = base64_table[base64[i]];
  }
  return base64;
}

int base64_encode_bytes(char* bytes, int tam, char** dest){
  int offset = tam%3>0 ? abs(tam%3-3) : 0;
  int bytes_to_transform = tam + offset;
  
  char* copy = (char*) calloc(bytes_to_transform, 1);
  if(!copy) return -1;
  memcpy(copy, bytes, tam);
  bytes = copy;
  tam = bytes_to_transform;

  int total_bits = (tam*8);
  int result_bytes = total_bits/6;
  char* result = (char*) malloc(result_bytes);
  if(!result){
    return -1;
    free(copy);
  }
  for(int i=0; i < bytes_to_transform/3;i++){
    char* encoding = base64_transform_3bytes(copy+i*3);
    result[i*4] = encoding[0];
    result[i*4+1] = encoding[1];
    result[i*4+2] = encoding[2];
    result[i*4+3] = encoding[3];
    free(encoding);
  }
  for(int i=result_bytes-offset;i<result_bytes;i++){
    result[i]='=';
  }
  free(copy);
  *dest = result;
  return result_bytes;
}

int base64_decode_4bytes(char* bytes, char** dest){
  int iguales = (bytes[2]=='=' ? 1:0) + (bytes[3]=='=' ? 1:0);
  char* aux = (char*)calloc(3,1);
  if(!aux) return -1;
  char* copy = (char*)malloc(4);
  memcpy(copy, bytes, 4);
  for(int i=0;i<4;i++){
    copy[i] = decode_byte(copy[i]);
  }
  for(int i=0;i<4;i++){
    char* extracted = bytes_extract_n_bits(copy, i*8, i*8+8-1);
    if(!extracted){
      free(aux);
      free(copy);
      return -1;
    }
    if(i==0){
      aux[0] |= byte_extract_n_bits(extracted[0], 2, 7) << 2;
    } else if(i==1){
      aux[0] |= byte_extract_n_bits(extracted[0], 2, 4) >> 4;
      aux[1] |= byte_extract_n_bits(extracted[0], 4, 7) << 4;
    } else if(i==2){
      aux[1] |= byte_extract_n_bits(extracted[0], 2, 5) >> 2;
      aux[2] |= byte_extract_n_bits(extracted[0], 5, 7) << 6;
    } else {
      aux[2] |= byte_extract_n_bits(extracted[0], 2, 7);
    }
    free(extracted);
  }
  char* resized = (char*) malloc(3-iguales);
  if(!resized){
    free(aux);
    free(copy);
    return -1;
  }
  memcpy(resized, aux, 3-iguales);
  *dest = resized;
  free(copy);
  free(aux);
  return 3-iguales;
}

int base64_decode_bytes(char* bytes, int tam, char** dest){
  char* result = NULL;
  char* decoded;
  int longitud;
  int longitud_total = 0;
  for(int i=0;i<tam/4;i++){
    longitud = base64_decode_4bytes(bytes+i*4, &decoded);
    if(longitud<0){
      free(result);
      return -1;
    }
    if(!result) result = (char*) malloc(longitud);
    else {
      char* aux = (char*)realloc(result, longitud_total+longitud);
      if(!aux){
        free(result);
        free(decoded);
        return -1;
      }
      result = aux;
    }
    memcpy(result+longitud_total, decoded, longitud);
    longitud_total += longitud;
    free(decoded);
  }
  *dest = result;
  return longitud_total;
}
