#include <stdlib.h>
// Todos los parametros de posicion son bits y empiezan en 0

// Recibe un byte y una posicion y devuelve un booleano indicando si el byte es 1 o 0
bool byte_nth_bit(char byte, int n);

// Recibe un byte y una posicion y hace una mascara de ese bit
char byte_mask_nth_bit(char byte, int n);

// Recibe byte y dos numeros y devuelve todos los bits comprendidos en ese intervalo
char byte_extract_n_bits(char byte, int initial_n, int last_n);

// Recibe una cadena de bytes y una posicion y toma el bit enesimo y lo devuelve como booleano
bool bytes_nth_bit(char* bytes, int n);

// Recibe una cadena de bytes y del byte en donde se encuentra el bit enesimo, hace una mascara con ese bit
char bytes_mask_nth_bit(char* bytes, int n);

// Recibe una cadena de bytes y devuelve un intervalo de bits
// Se debe extraer almenos un byte
char* bytes_extract_n_bits(char* bytes, int intial_n, int last_n);
