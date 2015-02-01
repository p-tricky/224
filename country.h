#ifndef _country_h
#define _country_h

#define BUFFSIZE 512
typedef unsigned char boolean;
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

typedef struct countryList {
    int size;
    int blocks;
    country **countries;
} countryList;

    void country_destroy(country *self);

    countryList *read_countries(void);
#endif
