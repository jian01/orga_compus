#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

extern char* base64_transform_3bytes(char* bytes);
extern int base64_decode_4bytes(char* bytes, char** dest);
extern char decode_byte(char byte);

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
    

  char prueba2[] = {'$','%','$'};
  encoding = base64_transform_3bytes(prueba2);
  print_test("Si encodeo '$%$' recibo 'JCUk'", byte_compare(encoding, "JCUk", 4));

  char prueba3[] = {'*','f','r'};
  encoding = base64_transform_3bytes(prueba3);
  print_test("Si encodeo '*fr' recibo 'KmZy'", byte_compare(encoding, "KmZy", 4));
  
  char prueba4[] = {144, 126, 0};
  encoding = base64_transform_3bytes(prueba4);
  print_test("Si encodeo 0x90 0x7E 0x0 recibo kH4A", byte_compare(encoding, "kH4A", 4));

  char prueba5[] = {0x2E, 0xF9, 0x49};
  const char* salida_esperada4="LvlJ";
  encoding = base64_transform_3bytes(prueba5);
  print_test("Caso de prueba 5", byte_compare(encoding, salida_esperada4, 4));
    
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
  

  char prueba2[] = {'J', 'C', 'U', 'k'};
  longitud = base64_decode_4bytes(prueba2, &decode);
  print_test("Si desencodeo 'JCUk' recibo '$%$'", byte_compare(decode, "$%$", longitud));
  

  char prueba3[] = {'K', 'm', 'Z', 'y'};
  longitud = base64_decode_4bytes(prueba3, &decode);
  print_test("Si desencodeo 'KmZy' recibo '*fr'", byte_compare(decode, "*fr", longitud));
  

  char prueba4[] = {'J', 'j', 'g', '='};
  longitud = base64_decode_4bytes(prueba4, &decode);
  print_test("Si desencodeo 'Jjg=' recibo '&8'", byte_compare(decode, "&8", longitud));
  

  char prueba5[] = {'J', 'j', '=', '='};
  longitud = base64_decode_4bytes(prueba5, &decode);
  print_test("Si desencodeo 'Jj==' recibo '&'", byte_compare(decode, "&", longitud));
  

  char prueba6[] = {'@', 'j', '=', '='};
  longitud = base64_decode_4bytes(prueba6, &decode);
  print_test("Si desencodeo '@j==' falla", longitud==-1);
}



int main(){
  test_encode_3bytes();
  test_decode_byte();
  test_decode_4bytes();
  return EXIT_SUCCESS;
}
