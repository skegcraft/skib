#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Macro {
	char name[128];
	char value[128];
};

struct Macro macros[64];
int macrosI = 0;

void processFile(FILE *o, char name[]);

int alpha(char c) {
	return (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| c == '_';
}

void processLine(FILE *o, char *buffer) {
	char temp[512];
	int tempC;
	int c = 0;

	while (1) {
		while (buffer[c] == '\t' || buffer[c] == ' ') {
			fputc(buffer[c], o);
			c++;
		}

		// Preprocessor
		if (buffer[c] == '#') {
			strtok(buffer, "\n");

			c++;
			char *token = strtok(buffer + c, " ");
			if (!strcmp(token, "define")) {
				token = strtok(NULL, " ");
				strcpy(macros[macrosI].name, token);
				token += strlen(token) + 1;
				strcpy(macros[macrosI].value, token);
				macrosI++;
			} else if (!strcmp(token, "include")) {
				token = strtok(NULL, " ");
				processFile(o, token);
			} else {
				// TODO: Ignore comments? Change pp symbol?
			}

			return;
		}

		// Partial token lexer
		if (alpha(buffer[c])) {
			tempC = 0;
			while (alpha(buffer[c])) {
				temp[tempC] = buffer[c];
				tempC++;
				c++;
			}

			temp[tempC] = '\0';

			for (int i = 0; i < macrosI; i++) {
				if (!strcmp(macros[i].name, temp)) {
					fputs(macros[i].value, o);
					goto next;
				}
			}

			fputs(temp, o);
		} else if (buffer[c] == '\"') {
			fputc(buffer[c], o);
			c++;
			while (buffer[c] != '\"') {
				fputc(buffer[c], o);
				c++;
			}

			fputc(buffer[c], o);
			c++;
		} else if (buffer[c] == '\0') {
			return;
		} else {
			// Regular characters/sybols
			fputc(buffer[c], o);
			c++;
		}
		next:;
	}
}

void processFile(FILE *o, char name[]) {
	char buffer[512];
	FILE *f = fopen(name, "r");
	if (f == NULL) {
		printf("ERR: %s not found.", name);
		exit(-1);
	}
	
	while (fgets(buffer, sizeof(buffer), f)) {
		processLine(o, buffer);
	}
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
		return -1;
	}

	FILE *o = fopen(argv[2], "wb");
	processFile(o, argv[1]);
}
