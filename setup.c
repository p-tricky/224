#include "countryList.h"

int main() {
  countryList *countries = read_countries();
  print_list_to_bin_file(countries);
  countryList_destroy(countries);
  return 0;
}
