#include <stdlib.h>
#include <stdbool.h>
#define IS_BIG_ENDIAN true

bool byte_nth_bit(char byte, int n){
  if(IS_BIG_ENDIAN) n = abs(n-7);
  char mask = 0x01;
  byte = byte >> n;
  return mask & byte;
}

char byte_mask_nth_bit(char byte, int n){
  char bit = byte_nth_bit(byte,n);
  if(IS_BIG_ENDIAN) n = abs(n-7);
  return bit << n;
}

char byte_extract_n_bits(char byte, int initial_n, int last_n){
  char retval = 0x00;
  for(int i=initial_n;i<=last_n;i++){
    retval = retval | byte_mask_nth_bit(byte, i);
  }
  return retval;
}

bool bytes_nth_bit(char* bytes, int n){
  int byte_location = n/8;
  int bit_location = n-byte_location*8;
  return byte_nth_bit(bytes[byte_location], bit_location);
}

char bytes_mask_nth_bit(char* bytes, int n){
  int byte_location = n/8;
  int bit_location = n-byte_location;
  return byte_mask_nth_bit(bytes[byte_location], bit_location);
}

char* bytes_extract_n_bits(char* bytes, int initial_n, int last_n){
  int initial_byte = initial_n / 8;
  int last_byte = last_n / 8;
  char* bits = (char*)malloc(last_byte - initial_byte+1);
  if(!bits) return NULL;
  for(int i=initial_byte;i<=last_byte;i++){
    int initial_bit = 0;
    if (i==initial_byte) initial_bit = initial_n%8;
    int last_bit=7;
    if (i==last_byte) last_bit = last_n%8;
    bits[i-initial_byte] = byte_extract_n_bits(bytes[i], initial_bit, last_bit);
  }
  return bits;
}

#include <stddef.h>
#include <stdio.h>
void imprimir_numeros(int n)
{
  for (int i = 0; i <= n; i++)
    printf("%i\n", i);
}