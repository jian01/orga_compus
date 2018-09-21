#include "base64.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void print_test(const char* msg, bool cond){
  if(cond) printf("%s... %s", msg, "OK");
  else printf("%s... %s", msg, "ERROR");
  printf("\n");
}

bool byte_compare(const char* cadena1, const char* cadena2, size_t tam){
  char comparer = 0x00;
  for(size_t i=0;i<tam;i++){
      comparer |= cadena1[i] ^ cadena2[i];
  }
  return comparer==0;
}

void test_encode_3bytes(){
  printf("\n");
  printf("--- INICIO DE PRUEBAS ENCODE 3 BYTES ---\n");

  char prueba1[] = {'a','s','d'};
  char* encoding = base64_transform_3bytes(prueba1);
  print_test("Si encodeo 'asd' recibo 'YXNk'", byte_compare(encoding, "YXNk", 4));
  free(encoding);

  char prueba2[] = {'$','%','$'};
  encoding = base64_transform_3bytes(prueba2);
  print_test("Si encodeo '$%$' recibo 'JCUk'", byte_compare(encoding, "JCUk", 4));
  free(encoding);

  char prueba3[] = {'*','f','r'};
  encoding = base64_transform_3bytes(prueba3);
  print_test("Si encodeo '*fr' recibo 'KmZy'", byte_compare(encoding, "KmZy", 4));
  free(encoding);
}

void test_encode_base64(){
  printf("\n");
  printf("--- INICIO DE PRUEBAS ENCODE STRING ---\n");
  char* encoding;

  char prueba1[] = {'a','s','d'};
  int longitud = base64_encode_bytes(prueba1, 3, &encoding);
  print_test("Si encodeo 'asd' recibo 'YXNk'", byte_compare(encoding, "YXNk", 4));
  free(encoding);

  char prueba2[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
  longitud = base64_encode_bytes(prueba2, 7, &encoding);
  print_test("Si encodeo 'abcdefg' recibo 'YWJjZGVmZw=='", byte_compare(encoding, "YWJjZGVmZw==", 12));
  free(encoding);

  char prueba3[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
  longitud = base64_encode_bytes(prueba3, 8, &encoding);
  print_test("Si encodeo 'abcdefgh' recibo 'YWJjZGVmZ2g='", byte_compare(encoding, "YWJjZGVmZ2g=", 12));
  free(encoding);

  const char* prueba4 = "Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.";
  longitud = base64_encode_bytes((char*) prueba4, strlen(prueba4), &encoding);
  const char* salida_esperada = "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=";
  print_test("Encodeo un texto largo (wikipedia)", byte_compare(encoding, salida_esperada, longitud));
  print_test("La longitud devuelta es correcta", longitud==strlen(salida_esperada));
  free(encoding);
}

void test_decode_byte(){
  printf("\n");
  printf("--- INICIO DE PRUEBAS DECODE BYTE ---\n");

  print_test("Decode 'A' es 0", decode_byte('A')==0);
  print_test("Decode 'a' es 26", decode_byte('a')==26);
  print_test("Decode '0' es 52", decode_byte('0')==52);
  print_test("Decode '+' es 62", decode_byte('+')==62);
  print_test("Decode '/' es 63", decode_byte('/')==63);
}

void test_decode_4bytes(){
  printf("\n");
  printf("--- INICIO DE PRUEBAS DECODE 4 BYTES ---\n");

  char prueba1[] = {'Y','X','N','k'};
  char* decode;
  int longitud = base64_decode_4bytes(prueba1, &decode);
  print_test("Si desencodeo 'YXNk' recibo 'asd'", byte_compare(decode, "asd", longitud));
  free(decode);

  char prueba2[] = {'J', 'C', 'U', 'k'};
  longitud = base64_decode_4bytes(prueba2, &decode);
  print_test("Si desencodeo 'JCUk' recibo '$%$'", byte_compare(decode, "$%$", longitud));
  free(decode);

  char prueba3[] = {'K', 'm', 'Z', 'y'};
  longitud = base64_decode_4bytes(prueba3, &decode);
  print_test("Si desencodeo 'KmZy' recibo '*fr'", byte_compare(decode, "*fr", longitud));
  free(decode);

  char prueba4[] = {'J', 'j', 'g', '='};
  longitud = base64_decode_4bytes(prueba4, &decode);
  print_test("Si desencodeo 'Jjg=' recibo '&8'", byte_compare(decode, "&8", longitud));
  free(decode);

  char prueba5[] = {'J', 'j', '=', '='};
  longitud = base64_decode_4bytes(prueba5, &decode);
  print_test("Si desencodeo 'Jj==' recibo '&'", byte_compare(decode, "&", longitud));
  free(decode);
}

void test_decode_bytes(){
  printf("\n");
  printf("--- INICIO DE PRUEBAS DECODE BYTES ---\n");
  char* decode;

  char prueba1[] = {'Y', 'X', 'N', 'k'};
  int longitud = base64_decode_bytes(prueba1, 4, &decode);
  print_test("Si desencodeo 'YXNk' recibo 'asd'", byte_compare(decode, "asd", 3));
  free(decode);

  char prueba2[] = {'Y','W','J','j','Z','G','V','m','Z','w','=','='};
  longitud = base64_decode_bytes(prueba2, 12, &decode);
  print_test("Si desencodeo 'YWJjZGVmZw==' recibo 'abcdefg'", byte_compare(decode, "abcdefg", longitud));
  free(decode);

  char prueba3[] = {'Y', 'W', 'J', 'j', 'Z', 'G', 'V', 'm', 'Z', '2', 'g', '='};
  longitud = base64_decode_bytes(prueba3, 12, &decode);
  print_test("Si desencodeo 'YWJjZGVmZ2g=' recibo 'abcdefgh'", byte_compare(decode, "abcdefgh", longitud));
  free(decode);

  const char* prueba4 = "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=";
  const char* salida_esperada = "Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.";
  longitud = base64_decode_bytes((char*) prueba4, strlen(prueba4), &decode);
  print_test("Desencodeo un texto largo (wikipedia)", byte_compare(decode, salida_esperada, longitud));
  print_test("La longitud devuelta es correcta", longitud==strlen(salida_esperada));
  free(decode);
}

int main(){
  test_encode_3bytes();
  test_encode_base64();
  test_decode_byte();
  test_decode_4bytes();
  test_decode_bytes();
  return EXIT_SUCCESS;
}
