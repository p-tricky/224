#include "directory.h"
#include "dbg.h"
#include <stdlib.h>

countryAddress *countryAddress_init() {
	countryAddress *countryAdd = (countryAddress *)malloc(sizeof(countryAddress *));
	check_mem(countryAdd);
	countryAdd->countryCode = malloc(4*sizeof(char));
	check_mem(countryAdd->countryCode);
	return countryAdd;

error:
	return NULL;
}

directory *directory_init() {
  directory *d= malloc(sizeof(directory));
	check_mem(d);
  d->size = 0;
  d->blocks = 0;
  return d;

error:
	return NULL;
}

void directory_destroy(directory *self) {
  int i = 0;
  while (i < self->blocks+1) {
    if (i<self->size) free(self->index[i]->countryCode);
    free(self->index[i]);
    i++;
  }
  free(self->index);
  free(self);
}

void addCountryAddress(directory* self) {
  if ((self->size % 50) == 0) {
    self->index = realloc(self->index, (self->size+50)*sizeof(countryAddress *));
    self->blocks+=50;
  }
  self->index[(self->size)++] = countryAddress_init();
}

directory *build_directory_from_countries_file() {
  FILE *directoryFP = fopen("./directory.bin", "rb");
	check(directoryFP != NULL, "Failed to open file");
  fseek(directoryFP, 0, SEEK_END);
  int lastByte = ftell(directoryFP);
  fseek(directoryFP, 0, SEEK_SET);
  directory *dir = directory_init();
  while (ftell(directoryFP) < lastByte) {
    addCountryAddress(dir);
    fread(dir->index[dir->size-1]->countryCode, 4, 
        sizeof(char), directoryFP);
    fread(&dir->index[dir->size-1]->byteOffset, 1, 
        sizeof(int), directoryFP);
  }
  return dir;

error:
	if (directoryFP) fclose(directoryFP);
	if (dir) directory_destroy(dir);
	return NULL;
}

void swap(int j, int k, directory *self) {
  countryAddress *temp = self->index[j];
  self->index[j] = self->index[k];
  self->index[k] = temp;
}
void bubble_sort(directory *self) {
  int j = 0;
  int i = 0;
  while (j < self->size) {
    i = 0;
    while(i+1 < self->size-j) {
      if (strcmp(self->index[i]->countryCode, 
            self->index[i+1]->countryCode) > 0) {
        swap(i, i+1, self);
      }
      i++;
    }
    j++;
  }
}

int binary_search(int start, int end, char code[], directory *self) {
  if (start>end) return -1;
  int mid = (start+end)/2;
  int comparison = strcmp(code, self->index[mid]->countryCode);
  if (comparison == 0) return self->index[mid]->byteOffset;
  else if (comparison > 0) 
    return binary_search(mid+1, end, code, self);
  else
    return binary_search(start, mid-1, code, self);
}

int read_country_data_from_bin_file(int i) {
  FILE *countriesBinFp = fopen("./countries.bin", "rb");
	check(countriesBinFp != NULL, "Failed to open file");
  fseek(countriesBinFp, i, SEEK_SET);
  char code[4];
  int name_size;
  char *name;
  int pop_size;
  char *pop;
  float lifeExp;
  fread(code, 1, 4, countriesBinFp);
  fread(&name_size, 1, sizeof(int), countriesBinFp);
  name = malloc(name_size);
	check_mem(name);
  fread(name, 1, name_size, countriesBinFp);
  fread(&pop_size, 1, sizeof(int), countriesBinFp);
  pop = malloc(pop_size);
	check_mem(pop);
  fread(pop, 1, pop_size, countriesBinFp);
  fread(&lifeExp, 1, sizeof(float), countriesBinFp);
  printf("%s %50s %10s %f\n", code, name, pop, lifeExp);
  free(name);
  free(pop);
  fclose(countriesBinFp);
	return 0;
error:
	return -1;
}

void print_directory(directory *self) {
  int i = 0;
  while (i < self->size) {
    printf("Code: %s, Address %4d\n", self->index[i]->countryCode,
        self->index[i]->byteOffset);
    i++;
  }
}
