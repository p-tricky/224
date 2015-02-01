CC = gcc
CFLAGS=-Wall -g
P1_OBJECTS = country.o countryList.o setup.o
P2_OBJECTS = directory.o interface.o
BINARIES = countries.bin directory.bin A1Part1 A1Part2

A1Part1: $(P1_OBJECTS)
	$(CC)  $(CFLAGS) $(P1_OBJECTS) -o A1Part1

A1Part2: $(P2_OBJECTS)
	$(CC)  $(CFLAGS) $(P2_OBJECTS) -o A1Part2

clean:
	rm -f $(P1_OBJECTS) $(P2_OBJECTS) $(BINARIES)
