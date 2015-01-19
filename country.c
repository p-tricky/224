#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/**
 * the country struct will hold 
 * all of the country data
 */
typedef struct country {
  char *code;
  char *name;
  char *pop;
  float lifeExp;
} country;

country *country_init(char *code, char *name, char *pop,
    float lifeExp) {
  country *ctry = (country *)malloc(sizeof(country));
  ctry->code = strdup(code);
  ctry->name = strdup(name);
  ctry->pop = strdup(pop);
  ctry->lifeExp = lifeExp;
  return ctry;
}

void country_destroy(country *self) {
  if (self->code) {free(self->code);}
  if (self->name) {free(self->name);}
  if (self->pop) {free(self->pop);}
  free(self);
}

int main() {
 country *country = country_init("USA", "United States", "300,000,000", 77.6);
 printf("Code: %s\n", country->code);
 printf("Name: %s\n", country->name);
 printf("Population: %s\n", country->pop);
 printf("Life Expectancy: %f\n", country->lifeExp);
 country_destroy(country);

 return 0;
}
