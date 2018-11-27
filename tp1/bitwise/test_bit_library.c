#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

extern bool byte_nth_bit(char byte, int n);
extern char byte_mask_nth_bit(char byte, int n);
extern char byte_extract_n_bits(char byte, int initial_n, int final_n);
extern bool bytes_nth_bit(char* byte, int n);
extern char* bytes_extract_n_bits(char* bytes, int initial_n, int last_n);


void print_test(const char* msg, bool cond){
  if(cond) printf("%s... %s", msg, "OK");
  else printf("%s... %s", msg, "ERROR");
  printf("\n");
}



void test_byte_nth_bit(){
  printf("\n");
  printf("--- INICIO DE PRUEBAS DE BYTE_NTH_BIT ---\n");

  print_test("El bit 0 de 'a' es correcto", !byte_nth_bit('a',0));
  print_test("El bit 1 de 'a' es correcto", byte_nth_bit('a',1));
  print_test("El bit 2 de 'a' es correcto", byte_nth_bit('a',2));
  print_test("El bit 3 de 'a' es correcto", !byte_nth_bit('a',3));
  print_test("El bit 4 de 'a' es correcto", !byte_nth_bit('a',4));
  print_test("El bit 5 de 'a' es correcto", !byte_nth_bit('a',5));
  print_test("El bit 6 de 'a' es correcto", !byte_nth_bit('a',6));
  print_test("El bit 7 de 'a' es correcto", byte_nth_bit('a',7));

  print_test("El bit 0 de '*' es correcto", !byte_nth_bit('*',0));
  print_test("El bit 1 de '*' es correcto", !byte_nth_bit('*',1));
  print_test("El bit 2 de '*' es correcto", byte_nth_bit('*',2));
  print_test("El bit 3 de '*' es correcto", !byte_nth_bit('*',3));
  print_test("El bit 4 de '*' es correcto", byte_nth_bit('*',4));
  print_test("El bit 5 de '*' es correcto", !byte_nth_bit('*',5));
  print_test("El bit 6 de '*' es correcto", byte_nth_bit('*',6));
  print_test("El bit 7 de '*' es correcto", !byte_nth_bit('*',7));

  print_test("El bit 0 de 0xff es correcto", byte_nth_bit(0xff,0));
  print_test("El bit 1 de 0xff es correcto", byte_nth_bit(0xff,1));
  print_test("El bit 2 de 0xff es correcto", byte_nth_bit(0xff,2));
  print_test("El bit 3 de 0xff es correcto", byte_nth_bit(0xff,3));
  print_test("El bit 4 de 0xff es correcto", byte_nth_bit(0xff,4));
  print_test("El bit 5 de 0xff es correcto", byte_nth_bit(0xff,5));
  print_test("El bit 6 de 0xff es correcto", byte_nth_bit(0xff,6));
  print_test("El bit 7 de 0xff es correcto", byte_nth_bit(0xff,7));

  print_test("El bit 0 de 0x0f es correcto", !byte_nth_bit(0x0f,0));
  print_test("El bit 1 de 0x0f es correcto", !byte_nth_bit(0x0f,1));
  print_test("El bit 2 de 0x0f es correcto", !byte_nth_bit(0x0f,2));
  print_test("El bit 3 de 0x0f es correcto", !byte_nth_bit(0x0f,3));
  print_test("El bit 4 de 0x0f es correcto", byte_nth_bit(0x0f,4));
  print_test("El bit 5 de 0x0f es correcto", byte_nth_bit(0x0f,5));
  print_test("El bit 6 de 0x0f es correcto", byte_nth_bit(0x0f,6));
  print_test("El bit 7 de 0x0f es correcto", byte_nth_bit(0x0f,7));

  print_test("El bit 0 de 0xf0 es correcto", byte_nth_bit(0xf0,0));
  print_test("El bit 1 de 0xf0 es correcto", byte_nth_bit(0xf0,1));
  print_test("El bit 2 de 0xf0 es correcto", byte_nth_bit(0xf0,2));
  print_test("El bit 3 de 0xf0 es correcto", byte_nth_bit(0xf0,3));
  print_test("El bit 4 de 0xf0 es correcto", !byte_nth_bit(0xf0,4));
  print_test("El bit 5 de 0xf0 es correcto", !byte_nth_bit(0xf0,5));
  print_test("El bit 6 de 0xf0 es correcto", !byte_nth_bit(0xf0,6));
  print_test("El bit 7 de 0xf0 es correcto", !byte_nth_bit(0xf0,7));
}

void test_byte_mask_nth_bit(){
  printf("\n");
  printf("--- INICIO DE PRUEBAS DE BYTE_MASK_NTH_BIT ---\n");

  print_test("El bit 0 de 'a' es correcto", !(byte_mask_nth_bit('a',0)==0x80) );
  print_test("El bit 1 de 'a' es correcto", byte_mask_nth_bit('a',1)==64);
  print_test("El bit 2 de 'a' es correcto", byte_mask_nth_bit('a',2)==32);
  print_test("El bit 3 de 'a' es correcto", !(byte_mask_nth_bit('a',3)==16) );
  print_test("El bit 4 de 'a' es correcto", !(byte_mask_nth_bit('a',4)==8) );
  print_test("El bit 5 de 'a' es correcto", !(byte_mask_nth_bit('a',5)==4) );
  print_test("El bit 6 de 'a' es correcto", !(byte_mask_nth_bit('a',6)==2) );
  print_test("El bit 7 de 'a' es correcto", byte_mask_nth_bit('a',7)==1);
}


void test_byte_extract_n_bits(){
  printf("\n");
  printf("--- INICIO DE PRUEBAS DE BYTE_EXTRACT_N_BITS ---\n");

  print_test("Si agarro 'a' con un slice sigue siendo 'a'", byte_extract_n_bits('a', 0, 7)=='a');
  print_test("Si agarro 97 con un bit menos se conviente en 96", byte_extract_n_bits('a', 0, 6)==96);
  print_test("Si agarro 97[1:6] se conviente en 96", byte_extract_n_bits('a', 1, 6)==96);
  print_test("Si agarro 97[2:6] se conviente en 32", byte_extract_n_bits('a', 2, 6)==32);
  print_test("Si agarro 1[7:7] obtengo 1", byte_extract_n_bits(1, 7, 7)==1);
  print_test("Si agarro r[2:7] obtengo 50", byte_extract_n_bits('r', 2, 7)==50);
}

void test_bytes_nth_bit(){
  printf("\n");
  printf("--- INICIO DE PRUEBAS DE BYTES_NTH_BIT ---\n");
  char arreglo[]={'a','*',(char)0xff,(char)0x0f,(char)0xf0};
  print_test("El bit 0 de 'a' es correcto", !bytes_nth_bit(arreglo,0));
  print_test("El bit 1 de 'a' es correcto", bytes_nth_bit(arreglo,1));
  print_test("El bit 2 de 'a' es correcto", bytes_nth_bit(arreglo,2));
  print_test("El bit 3 de 'a' es correcto", !bytes_nth_bit(arreglo,3));
  print_test("El bit 4 de 'a' es correcto", !bytes_nth_bit(arreglo,4));
  print_test("El bit 5 de 'a' es correcto", !bytes_nth_bit(arreglo,5));
  print_test("El bit 6 de 'a' es correcto", !bytes_nth_bit(arreglo,6));
  print_test("El bit 7 de 'a' es correcto", bytes_nth_bit(arreglo,7));

  print_test("El bit 0 de '*' es correcto", !bytes_nth_bit(arreglo,0+8));
  print_test("El bit 1 de '*' es correcto", !bytes_nth_bit(arreglo,1+8));
  print_test("El bit 2 de '*' es correcto", bytes_nth_bit(arreglo,2+8));
  print_test("El bit 3 de '*' es correcto", !bytes_nth_bit(arreglo,3+8));
  print_test("El bit 4 de '*' es correcto", bytes_nth_bit(arreglo,4+8));
  print_test("El bit 5 de '*' es correcto", !bytes_nth_bit(arreglo,5+8));
  print_test("El bit 6 de '*' es correcto", bytes_nth_bit(arreglo,6+8));
  print_test("El bit 7 de '*' es correcto", !bytes_nth_bit(arreglo,7+8));

  print_test("El bit 0 de 0xff es correcto", bytes_nth_bit(arreglo,0+16));
  print_test("El bit 1 de 0xff es correcto", bytes_nth_bit(arreglo,1+16));
  print_test("El bit 2 de 0xff es correcto", bytes_nth_bit(arreglo,2+16));
  print_test("El bit 3 de 0xff es correcto", bytes_nth_bit(arreglo,3+16));
  print_test("El bit 4 de 0xff es correcto", bytes_nth_bit(arreglo,4+16));
  print_test("El bit 5 de 0xff es correcto", bytes_nth_bit(arreglo,5+16));
  print_test("El bit 6 de 0xff es correcto", bytes_nth_bit(arreglo,6+16));
  print_test("El bit 7 de 0xff es correcto", bytes_nth_bit(arreglo,7+16));

  print_test("El bit 0 de 0x0f es correcto", !bytes_nth_bit(arreglo,0+24));
  print_test("El bit 1 de 0x0f es correcto", !bytes_nth_bit(arreglo,1+24));
  print_test("El bit 2 de 0x0f es correcto", !bytes_nth_bit(arreglo,2+24));
  print_test("El bit 3 de 0x0f es correcto", !bytes_nth_bit(arreglo,3+24));
  print_test("El bit 4 de 0x0f es correcto", bytes_nth_bit(arreglo,4+24));
  print_test("El bit 5 de 0x0f es correcto", bytes_nth_bit(arreglo,5+24));
  print_test("El bit 6 de 0x0f es correcto", bytes_nth_bit(arreglo,6+24));
  print_test("El bit 7 de 0x0f es correcto", bytes_nth_bit(arreglo,7+24));
}

void test_bytes_extract_n_bits(){
  printf("\n");
  printf("--- INICIO DE PRUEBAS DE BYTES_EXTRACT_N_BITS ---\n");
  char arreglo[] = {'a',1,-128};
  char* resultado;

  resultado = (char*)bytes_extract_n_bits(arreglo, 0, 7);
  print_test("Si agarro 'a' con un slice sigue siendo 'a'", resultado[0]=='a');
   
   

  resultado = (char*)bytes_extract_n_bits(arreglo, 0, 6);
  print_test("Si agarro 97 con un bit menos se conviente en 96", resultado[0]==96);
   
   

  resultado = (char*)bytes_extract_n_bits(arreglo, 1, 6);
  print_test("Si agarro 97[1:6] se conviente en 96", resultado[0]==96);
   
   

  resultado = (char*)bytes_extract_n_bits(arreglo, 2, 6);
  print_test("Si agarro 97[2:6] se conviente en 32", resultado[0]==32);
   
   

  resultado = (char*)bytes_extract_n_bits(arreglo, 7+8, 7+8);
  print_test("Si agarro 1[7:7] obtengo 1", resultado[0]==1);
   
   

  resultado = (char*)bytes_extract_n_bits(arreglo, 0, 23);
  print_test("Si agarro arreglo[0:23][0] obtengo arreglo[0]", resultado[0]==arreglo[0]);
   
  

  resultado = (char*)bytes_extract_n_bits(arreglo, 0, 23);
  print_test("Si agarro arreglo[0:23][1] obtengo arreglo[1]", resultado[1]==arreglo[1]);
   
   

  resultado =(char*) bytes_extract_n_bits(arreglo, 0, 23);
  print_test("Si agarro arreglo[0:23][2] obtengo arreglo[2]", resultado[2]==arreglo[2]);
   
   

  resultado = (char*)bytes_extract_n_bits(arreglo, 8, 23);
  print_test("Si agarro arreglo[8:23][1] obtengo arreglo[2]", resultado[1]==arreglo[2]);
   
   

  resultado = (char*)bytes_extract_n_bits(arreglo, 8, 15);
  print_test("Si agarro arreglo[8:15][0] obtengo arreglo[1]", resultado[0]==arreglo[1]);
   
   

  resultado =(char*) bytes_extract_n_bits(arreglo, 16, 16);
  print_test("Si agarro arreglo[16:16][0] obtengo arreglo[2]", resultado[0]==arreglo[2]);
   
   

  char arreglo2[] = {'*' , 'f', 'r'};
  resultado = (char*)bytes_extract_n_bits(arreglo2, 18, 23);
  print_test("Si agarro arreglo[18:23][0] obtengo 50", resultado[0]==50);
   

}

int main(){
  test_byte_nth_bit();
  test_byte_mask_nth_bit();
  test_byte_extract_n_bits();
  test_bytes_nth_bit();
  test_bytes_extract_n_bits();
  return 0;
}
