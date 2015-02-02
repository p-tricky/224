#include "countryList.h"
#include "dbg.h"

int main() {
  countryList *countries = read_countries();
	check(countries != NULL, "Failed to read countries from .dat file");
  print_list_to_bin_file(countries);
  countryList_destroy(countries);
  return 0;

error:
	return -1;
}
