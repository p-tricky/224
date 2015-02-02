#include "directory.h"
#include "dbg.h"

void clean_stdin() {
	char c;
	do {
		c = getchar();
	} while (c != '\n' && c != EOF);
}

int process_input(directory *d) {
	char code[4];
	char *in = NULL;
	printf("\n> ");
	in = fgets(code, 4, stdin);
	check(in!=NULL, "Failed to get code.");
	clean_stdin();
	if (strcmp(code, "000") == 0)
		return 1;
	else {
		int codeAddress = binary_search(0, d->size-1, code, d);
		if (codeAddress == -1) {
			printf("%s\n %s\n", "Sorry, I'm not familiar with that code.",
					"May I please have another sir!");
		}
		else {
			int rc = read_country_data_from_bin_file(codeAddress);
			check(rc != -1, "Failed to read country from countries.bin");
		}
	}
	return 0;

error:
	return -1;
}

int main() {
	printf("%s\n %s\n",
			"Hello, I'm a database!", 
			"Give me a 3 letter country code (e.g., USA), and I will tell you what I know about that country.");

	directory *d = build_directory_from_countries_file();
	bubble_sort(d);
	while (process_input(d) != 1) {}
	printf("%s\n", "Have a great day!");
	return 1;
}
