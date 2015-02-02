#include "country.h"
#include "dbg.h"
#include <stdlib.h>

country *country_init() {
  country *ctry = (country *)malloc(sizeof(country));
  check_mem(ctry);
  return ctry;

error:
  return NULL;
}

void country_destroy(country *self) {
  if (self->code) {free(self->code);}
  if (self->name) {free(self->name);}
  if (self->pop) {free(self->pop);}
  free(self);
}
