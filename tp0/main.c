void imprimir_ayuda(){
  printf("
  Usage:
  tp0 -h\n
  tp0 -V\n
  tp0 [options]\n
  Options:\n
  -V, --version\n
  Print version and quit.
  -h, --help\n
  Print this information.
  -i, --input\n
  Location of the input file.\n
  -o, --output\n
  Location of the output file.\n
  -a, --action\n
  Program action: encode (default) or decode.\n
  Examples:\n
  tp0 -a encode -i ~/input -o ~/output\n
  tp0 -a decode\n")
}

int main(int argc, char* argv[]){

  return EXIT_SUCCESS;
}
