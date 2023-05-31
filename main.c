#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CELLS 30000

static uint8_t tape[MAX_CELLS] = {0};

void execute(char *filepath) {
  FILE *fp = fopen(filepath, "rt");
  if (fp == NULL) {
    fprintf(stderr, "ERROR: could not open file.\n");
    exit(1);
  }

  fseek(fp, 0L, SEEK_END);
  size_t code_size = ftell(fp);
  fseek(fp, 0L, SEEK_SET);

  char *code = (char *)malloc(code_size);

  size_t code_p = 0;
  char current;
  while ((current = fgetc(fp)) != EOF) {
    code[code_p++] = current;
  }

  int memory_p = 0;
  code_p = 0;
  while (true) {
    if (code_p >= code_size) {
      break;
    }

    char current = code[code_p++];
    switch (current) {
    case '>':
      memory_p = (memory_p + 1) % MAX_CELLS;
      break;
    case '<':
      memory_p = (memory_p - 1) % MAX_CELLS;
      break;
    case '+':
      tape[memory_p] += 1;
      break;
    case '-':
      tape[memory_p] -= 1;
      break;
    case '.':
      printf("%c", (char)tape[memory_p]);
      break;
    case ',':
      tape[memory_p] = getchar();
      break;
    case '[': {
      int brackets = 1;
      if (tape[memory_p] == 0) {
        size_t p = code_p;
        while (brackets > 0) {
          if (p >= code_size) {
            fprintf(stderr, "ERROR: missing matching ']' at column %zu.\n",
                    code_p);
            exit(1);
          }
          if (code[p] == ']') {
            brackets -= 1;
          }
          p += 1;
        }
        p -= 1;
        code_p = p;
      }
    } break;
    case ']': {
      int brackets = 1;
      if (tape[memory_p] != 0) {
        size_t p = code_p;
        while (brackets > 0) {
          if (p <= 0) {
            fprintf(stderr, "ERROR: missing matching '[' at column %zu.\n",
                    code_p);
            exit(1);
          }
          if (code[p] == '[') {
            brackets -= 1;
          }
          p -= 1;
        }
        p += 1;
        code_p = p;
      }
    }
    }
  }

  fclose(fp);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "ERROR: source code file path missing.\n");
    exit(1);
  }

  char *filepath = argv[1];
  execute(filepath);

  return 0;
}
