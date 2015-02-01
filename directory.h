#ifndef _country_h
#define _country_h

typedef struct countryAddress {
    char *countryCode;
    int byteOffset;
} countryAddress;

typedef struct directory {
    int size;
    int blocks;
    countryAddress ** index;
} directory;

    directory *directory_init();
    directory *build_directory_from_countries_file();
    void directory_destroy(directory *self);
    void bubble_sort(directory *self);
    int binary_search(int start, int end, char code[], directory *self);
    void read_country_data_from_bin_file(int i);
    void print_directory(directory *self);
#endif
