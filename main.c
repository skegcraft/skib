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

int digit(char c) {
	return (c >= '0' && c <= '9');
}

int matchMacro(FILE *o, char temp[], char buffer[]) {
	int i;
	for (i = 0; i < macrosI; i++) {
		if (!strcmp(macros[i].name, temp)) {
			goto match;
		}
	}

	return 0;

	match:;

	if (buffer[0] != '(') {
		fputs(macros[i].value, o);
		goto finish;
	}

	// Process macro parameters, after the '('
	buffer++;

	char params[128][10];
	int paramsI = 0;
	while (*buffer != '\0') {
		while (*buffer == ' ') {buffer++;}

		// Process one parameter (anything that isn't ',' or ' ')
		if (*buffer != ' ' && *buffer != ',') {
			int c = 0;
			while (*buffer != ' ' && *buffer != ',' && *buffer != ')') {
				params[paramsI][c] = *buffer;
				c++; buffer++;
			}

			params[paramsI][c] = '\0';
			paramsI++;
		}
		
		while (*buffer == ' ') {buffer++;}

		if (*buffer == ',') {
			buffer++; continue;
		} else if (*buffer == ')') {
			break;
		}
	}

	// Parse macro value and ouput parameters
	for (int v = 0; macros[i].value[v] != '\0'; v++) {
		if (macros[i].value[v] == '$') {
			if (digit(macros[i].value[v + 1])) {
				v++;
				int n = macros[i].value[v] - '0';
				fputs(params[n], o);
				continue;
			}
		}

		fputc(macros[i].value[v], o);
	}

	// Finish off the rest of the line
	finish:;
	while (*buffer != '\0') {
		fputc(*buffer, o);
		buffer++;
	}

	return 1;
}

void processLine(FILE *o, char buffer[]) {
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

			// Match macros
			if (matchMacro(o, temp, buffer + c)) {
				return;
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
		puts("Usage:\n"
			"skib <input file> <output file>");
		return -1;
	}

	FILE *o = fopen(argv[2], "wb");
	processFile(o, argv[1]);
}
