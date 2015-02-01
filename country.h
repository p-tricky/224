#ifndef _country_h
#define _country_h

 // the country struct will hold 
 // all of the country data
typedef struct country {
    char *code;
    char *name;
    char *pop;
    float lifeExp;
} country;

    country *country_init();
    void country_destroy(country *self);
#endif
