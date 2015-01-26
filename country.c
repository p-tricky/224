#include "../apue.h"
#include "dbg.h"
#include "country.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

country *country_init() {
  country *ctry = (country *)calloc(1, sizeof(country));
  return ctry;
}

countryList *countryList_init() {
  countryList *list = malloc(sizeof(countryList));
  list->size = 0;
  return list;
}

void addEmptyCountry(countryList *self) {
  if ((self->size % 50) == 0) 
    self->countries = realloc(self->countries, (self->size+50)*sizeof(country *));
  self->countries[(self->size)++] = country_init();
}

void country_destroy(country *self) {
  if (self->code) {free(self->code);}
  if (self->name) {free(self->name);}
  if (self->pop) {free(self->pop);}
  free(self);
}

void countryList_destroy(countryList *self) {
  int i = 0;
  while (i < (self->size/50+50)) {
    if (self->countries[i]) (country_destroy(self->countries[i]));
    i++;
  }
  free(self->countries);
  free(self);
}

void print_list(countryList *self) {
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

countryList *read_countries(void) {
  // declare local variables
  int i;
  boolean lineTokenIsCompleteLine;
  int numBytesAfterLastEndLineInBuffer;
  char *bufPtr, *currPositionInBuffer, *lastEndLineInBuffer, *currPositionInLineToken, *lineToken, *fieldToken;
  char *lineTokenCp = calloc(1, 200);
  char buf[BUFFSIZE];

  // create countryList container
  countryList *countryList = countryList_init();

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
        addEmptyCountry(countryList);

        // looping to get field tokens from line token
        while (i<9) {
          fieldToken = strtok_r(lineTokenCp, ",", &currPositionInLineToken);
          if (fieldToken == NULL) break;
          if (i==1) {
            (countryList->countries)[(countryList->size-1)]->code 
              = strdup(fieldToken);
            check((countryList->countries)[(countryList->size-1)]->code != 0, 
                "Failed to copy code");
          }
          if (i==2) {
            (countryList->countries)[(countryList->size-1)]->name 
              = strdup(fieldToken);
            check((countryList->countries)[(countryList->size-1)]->name 
                != 0, "Failed to copy name");
          }
          if (i==7) {
            (countryList->countries)[(countryList->size-1)]->pop 
              = strdup(fieldToken);
            check((countryList->countries)[(countryList->size-1)]->pop
                != 0, "Failed to copy pop");
          }
          if (i==8) 
            (countryList->countries)[(countryList->size-1)]->lifeExp 
              = atof(fieldToken);
          lineTokenCp = NULL;
          i++;
        }
      }
      bufPtr = NULL;
    }
  }
  close(allCountriesFileDescriptor);
  return countryList;

error:
  printf("There was an error\n");
  if (countryList) countryList_destroy(countryList);
  return NULL;

}

int main() {
  countryList *countries = read_countries();
  print_list(countries);
  countryList_destroy(countries);

  return 0;
}
