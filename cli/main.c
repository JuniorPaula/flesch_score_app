#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
 size_t sentences;
 size_t words;
 size_t syllables;
} TextStats;

extern double flesch_score(const char* text, size_t len);

extern TextStats get_text_stats(const char* text, size_t len);

char* read_file(const char* path)
{
  FILE *f = fopen(path, "rb");
  if (!f) return NULL;

  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  rewind(f);

  char* buffer = malloc(size + 1);
  fread(buffer, 1, size, f);
  buffer[size] = '\0';
  fclose(f);

  return buffer;
}

int main(int argc, char** argv)
{
  if (argc < 2) {
    printf("Uso: ./app arquivo.txt\n");
    return 1;
  }

  char* text = read_file(argv[1]);
  if (!text) {
    printf("[ERROR] O arquivo não pode ser aberto.\n");
    return 1;
  }

  double score = flesch_score(text, strlen(text));

  printf("-- Lib sendo executada a partir da Implementação em C ---\n");
  printf("\nFlesch Score: %.2f\n", score);

  TextStats stats = get_text_stats(text, strlen(text));

  printf("\nEstatísticas: \n");
  printf(" Palavras: %zu\n", stats.words);
  printf(" Sentenças: %zu\n", stats.sentences);
  printf(" Sílabas: %zu\n", stats.syllables);

  return 0;
}
