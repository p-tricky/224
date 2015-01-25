#ifndef _country_h
#define _country_h

#define BUFFSIZE 512
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

void country_destroy(country *self);

country **read_countries(void);
#endif
