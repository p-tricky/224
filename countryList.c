#include "apue.h"
#include "dbg.h"
#include "country.h"
#include "countryList.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

countryList *countryList_init() {
  countryList *list = malloc(sizeof(countryList));
  list->size = 0;
  list->blocks = 0;
  return list;
}

void countryList_destroy(countryList *self) {
  int i = 0;
  while (i < (self->blocks+1)) {
    if (self->countries[i]) (country_destroy(self->countries[i]));
    i++;
  }

  free(self->countries);
  free(self);
}

void addEmptyCountry(countryList *self) {
  if ((self->size % 50) == 0) {
    self->countries = realloc(self->countries, (self->size+50)*sizeof(country *));
    self->blocks+=50;
  }
  self->countries[(self->size)++] = country_init();
}

countryList *read_countries() {
  // declare local variables
  int i;
  boolean lineTokenIsCompleteLine;
  int numBytesAfterLastEndLineInBuffer;
  char *bufPtr, *currPositionInBuffer, *lastEndLineInBuffer, *currPositionInLineToken, *lineToken, *fieldToken;
  char *lineTokenCp = calloc(1, 200);
  char buf[BUFFSIZE];

  // create countryList container
  countryList *countryL = countryList_init();

  // open file, find the end of the file, then reset to beginning of file
  int allCountriesFileDescriptor = open("./AllCountries.dat", O_RDONLY);
  check(allCountriesFileDescriptor != -1, "Failed to open file");
  int endOfFile = lseek(allCountriesFileDescriptor, 0, SEEK_END);
  lseek(allCountriesFileDescriptor, 0, SEEK_SET);

  // loop to fill buffer
  while (read(allCountriesFileDescriptor, buf, BUFFSIZE) > 0) {
    int currFilePosition = lseek(allCountriesFileDescriptor, 0, SEEK_CUR);
    bufPtr = buf;
    lineToken = "";

    // loop to get line tokens from buffer
    while (lineToken != NULL) {
      lineToken = strtok_r(bufPtr, "\n", &currPositionInBuffer);
      lineTokenIsCompleteLine = *(currPositionInBuffer-1) == 0;

      // If the line was cut off by the end of the buffer and 
      // we are not at the end of the file, then we need to seek
      // to the last complete line in the file before reading the buffer
      if ((!lineTokenIsCompleteLine) && (currFilePosition != endOfFile)) {
        numBytesAfterLastEndLineInBuffer
          = (unsigned long)&buf + BUFFSIZE - (unsigned long)lastEndLineInBuffer;
        lseek(allCountriesFileDescriptor, 
            currFilePosition-numBytesAfterLastEndLineInBuffer, 
            SEEK_SET);
        break;
      }

      // If the line wasn't cut off then tokenize it 
      // and store the appropriate tokens in the countries
      // in the list
      if (lineTokenIsCompleteLine) {
        lastEndLineInBuffer = currPositionInBuffer;
        lineTokenCp = strdup(lineToken);
        i = 0;
        addEmptyCountry(countryL);

        // looping to get field tokens from line token
        while (i<9) {
          fieldToken = strtok_r(lineTokenCp, ",", &currPositionInLineToken);
          if (fieldToken == NULL) break;
          if (i==1) {
            (countryL->countries)[(countryL->size-1)]->code 
              = strdup(fieldToken);
            check((countryL->countries)[(countryL->size-1)]->code != 0, 
                "Failed to copy code");
          }
          if (i==2) {
            (countryL->countries)[(countryL->size-1)]->name 
              = strdup(fieldToken);
            check((countryL->countries)[(countryL->size-1)]->name 
                != 0, "Failed to copy name");
          }
          if (i==7) {
            (countryL->countries)[(countryL->size-1)]->pop 
              = strdup(fieldToken);
            check((countryL->countries)[(countryL->size-1)]->pop
                != 0, "Failed to copy pop");
          }
          if (i==8) 
            (countryL->countries)[(countryL->size-1)]->lifeExp 
              = atof(fieldToken);
          lineTokenCp = NULL;
          i++;
        }
      }
      bufPtr = NULL;
    }
  }
  close(allCountriesFileDescriptor);
  return countryL;

error:
  printf("There was an error\n");
  if (countryL) countryList_destroy(countryL);
  return NULL;

}

void write_country_to_bin_files(country *self, FILE *countryFP, 
    FILE *directoryFP) {
  int file_position = ftell(countryFP);
  fwrite(self->code, 1, strlen(self->code)+1, directoryFP);
  fwrite(&file_position, 1, sizeof(int), directoryFP);
  int name_size = strlen(self->name)+1;
  int pop_size = strlen(self->pop)+1;
  fwrite(self->code, 1, strlen(self->code)+1, countryFP);
  fwrite(&name_size, 1, sizeof(name_size), countryFP);
  fwrite(self->name, 1, name_size, countryFP);
  fwrite(&pop_size, 1, sizeof(pop_size), countryFP);
  fwrite(self->pop, 1, pop_size, countryFP);
  fwrite(&self->lifeExp, 1, sizeof(self->lifeExp), countryFP);
}

void print_list_to_bin_file(countryList *self) {
  int i = 0;
  FILE *countryBinFile = fopen("./countries.bin", "w+");
  FILE *directoryBinFile = fopen("./directory.bin", "w+");
  while (i<self->size) {
    write_country_to_bin_files(self->countries[i], countryBinFile,
        directoryBinFile);
    i++;
  }
  fclose(countryBinFile);
  fclose(directoryBinFile);
}

void print_list_to_stdin(countryList *self) {
  int i = 0;
  while (i < self->size) {
    printf("%d %s %50s %10s %f\n", i, 
        self->countries[i]->code,
        self->countries[i]->name,
        self->countries[i]->pop,
        self->countries[i]->lifeExp);
    i++;
  }
}
