#include <stdio.h>
#include "country.h"

#ifndef _countryList_h
#define _countryList_h

#define BUFFSIZE 512
typedef unsigned char boolean;

// container for all of the countries
typedef struct countryList {
    int size;
    int blocks;
    country **countries;
} countryList;

    countryList *countryList_init();
    void countryList_destroy(countryList *self);
    countryList *read_countries(void);
    void addEmptyCountry(countryList *self);
    countryList *read_countries();
    void write_country_to_bin_files(country *self, FILE *countryFP, FILE *directoryFP);
    void print_list_to_bin_file(countryList *self);
    void print_list_to_stdin(countryList *self);
#endif
