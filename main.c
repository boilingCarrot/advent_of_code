#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

char *loadFile(char *filename, uint32_t *size) {
  char *empty = '\0';

  FILE *file;
  file = fopen(filename, "rb+");

  if (file == NULL) {
    printf("Failed to open %s, returning zero char\n", filename);
    return empty;
  }

  fseek(file, 0l, SEEK_END); // moves to end of file
  *size = (uint32_t)ftell(file); // gets position in file
  rewind(file);

  char *file_content = (char *)malloc((*size) * sizeof(char));
  fread(file_content, 1, *size, file);

  fclose(file);
  return file_content;
}

uint32_t countLines(char *filename, uint32_t size)
{
  FILE *file;
  file = fopen(filename, "rb+");
  char buf[size];
  uint32_t counter = 1;
  for(uint32_t i = 0; i < 100000; i++) {
    size_t res = fread(buf, 1, size, file);
    if (ferror(file)) {
      return -1;
    }
    for(uint32_t i = 0; i < res; i++) {
      if (buf[i] == '\n') {
        counter++;
      }
    }

    if (feof(file)) {
      break;
    }
  }
  fclose(file);
  return counter;
}

uint32_t getNextNewline(char *file, uint32_t size, uint32_t start_pos) {
  char current_char = '\0';

  uint32_t i;

  for (i = start_pos; i < 1000 + start_pos; i++) {
    current_char = file[i];
    if (current_char == '\n') {
      break;
    }
  }

  return i;
}

char firstCharacter(char *file, uint32_t size, uint32_t start_pos, uint32_t end_pos) {
  uint32_t first_index = 0;
  
  for (uint32_t i = start_pos; i < end_pos; i++) {
    uint32_t num = atoi(&file[i]);

    if (num != 0) {
     first_index = i;
     break;
    }
  }
  return file[first_index];
}

char finalCharacter(char *file, uint32_t size, uint32_t start_pos, uint32_t end_pos) {
  uint32_t furthest_index = 0;

  for (uint32_t i = start_pos; i < end_pos; i++) {
    uint32_t num = atoi(&file[i]);

    if (num != 0) {
      furthest_index = i;
    }
  }
  return file[furthest_index];
}

int main() {
  uint32_t *sum_array;

  uint32_t datafile_size;
  uint32_t datafile_lines;
  char *datafile = loadFile("data.txt", &datafile_size);
  datafile_lines = countLines("data.txt", datafile_size);

  sum_array = malloc(datafile_lines * sizeof(uint32_t));

  uint32_t current_pos = 0;

  for (uint32_t i = 0; i < datafile_lines; i++) {
    char final_char[2];

    uint32_t next_newline = getNextNewline(datafile, datafile_size, current_pos);

    char char0 = firstCharacter(datafile, datafile_size, current_pos, next_newline);
    char char1 = finalCharacter(datafile, datafile_size, current_pos, next_newline);
    
    final_char[0] = char0;
    final_char[1] = char1;

    uint32_t smashed_int = atoi(final_char);
    sum_array[i] = smashed_int;

    current_pos = next_newline + 1;
  }

  uint32_t final_int = 0;

  for (uint32_t i = 0; i < datafile_lines; i++) {
    final_int += sum_array[i];
  }

  printf("Final Answer: %d\n", final_int);
  
  free(sum_array);
  free(datafile);

  return 0;
}