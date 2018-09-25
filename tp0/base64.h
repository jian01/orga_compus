#include <stdlib.h>

char* base64_transform_3bytes(char* bytes);

int base64_encode_bytes(char* bytes, int tam, char** dest);

char decode_byte(char byte);

int base64_decode_4bytes(char* bytes, char** dest);

int base64_decode_bytes(char* bytes, int tam, char** dest);
