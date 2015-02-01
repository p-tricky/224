#include "country.h"
#include <stdlib.h>

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
