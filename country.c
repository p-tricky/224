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

void country_destroy(country *self) {
  if (self->code) {free(self->code);}
  if (self->name) {free(self->name);}
  if (self->pop) {free(self->pop);}
  free(self);
}

void countries_destroy(country **self) {
  int i = 0;
  while (i < 238) {
    if (self[i]) (country_destroy(self[i]));
    i++;
  }
  free(self);
}

country **read_countries(void) {
  int n, i;
  unsigned long bufStart, currBuffPostion;
  char *bufPtr, *savePtr1, *savePtr1LastValue, *savePtr2, *lineToken, *fieldToken, *lineTokenCp;
  char buf[BUFFSIZE];
  int count = 0;
  country **countries = calloc(238, sizeof(country *));
  while (count < 238) {
    countries[count] = calloc(1, sizeof(country));
    count++;
  }
  count = 0;
  check_mem(countries);
  int countriesFd = open("./AllCountries.dat", O_RDONLY);
  check(countriesFd != -1, "Failed to open file");
  while ((n = read(countriesFd, buf, BUFFSIZE)) > 0) {
    int currentPos = lseek(countriesFd, 0, SEEK_CUR);
    bufPtr = buf;
    while (1) {
      lineToken = strtok_r(bufPtr, "\n", &savePtr1);
      char completeLine = *(savePtr1-1) == 0;
      if (lineToken == NULL) {
        if (currentPos != lseek(countriesFd, 0, SEEK_END)) {
          currBuffPostion = (unsigned long)savePtr1LastValue;
          bufStart = (unsigned long)&buf;
          int fileRewindBytes = BUFFSIZE - (currBuffPostion-bufStart);
          lseek(countriesFd, currentPos-fileRewindBytes, SEEK_SET);
        }
        break;
      }
      lineTokenCp = strdup(lineToken);
      i = 0;
      while (completeLine) {
        fieldToken = strtok_r(lineTokenCp, ",", &savePtr2);
        if (fieldToken == NULL) break;
        if (count > 237) break;
        check_mem(countries[count]);
        if (i==1) {
          countries[count]->code = strdup(fieldToken);
          check(countries[count]->code != 0, "Failed to copy code");
        }
        if (i==2) {
          countries[count]->name = strdup(fieldToken);
          check(countries[count]->name != 0, "Failed to copy name");
        }
        if (i==7) {
          countries[count]->pop = strdup(fieldToken);
          check(countries[count]->pop != 0, "Failed to copy pop");
        }
        if (i==8) countries[count]->lifeExp = atof(fieldToken);
        lineTokenCp = NULL;
        i++;
      }
      if (completeLine) {
        savePtr1LastValue = savePtr1;
        count++;
      }
      bufPtr = NULL;
    }
  }
  close(countriesFd);
  return countries;

error:
  printf("There was an error\n");
  close(countriesFd);
  if (countries) countries_destroy(countries);
  return -1;
    
}

int main() {
  /* First tests
 country *country = country_init();
 check_mem(country);
 country->name = strdup("USA");
 check(country->name != 0, "Failed to copy name");
 //printf("Code: %s\n", country->code);
 printf("Name: %s\n", country->name);
 //printf("Population: %s\n", country->pop);
 //printf("Life Expectancy: %f\n", country->lifeExp);
 country_destroy(country);

 return 0;

error:
 if (country) free(country);
 if (country->name) free(country->name);
 return -1;
 */
  country **countries = read_countries();
  int i = 0;
  while (i < 238) {
    printf("%3d: %x %3s %50s %10s %f\n", i, countries[i],
        countries[i]->code, countries[i]->name, 
        countries[i]->pop, countries[i]->lifeExp);
    i++;
  }
  countries_destroy(countries);

  return 0;
}
