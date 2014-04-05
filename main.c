#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char cells[30000];

char *load_script(const char *path) {
	FILE *f;
	long int size;
	char *script;
	f = fopen(path, "r");
	if (f == NULL) return NULL;
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	rewind(f);
	script = (char*)malloc(size + 1);	
	fread(script, 1, size, f);
	script[size] = 0;
	fclose(f);
	return script;	
}

int main(int argc, const char **argv) {
	char command, *script, *ip;
	int dptr = 15000, balanced;

	if (argc != 2) return 1;
	ip = script = load_script(argv[1]);
	if (script == NULL) return 1;

	memset(cells, 0, 30000);	
	setbuf(stdout, NULL);
	setbuf(stdin, NULL);

	while ((command = *ip++) != 0) {
		switch (command) {
		case '>':
			if (++dptr == 30000) return 1;
			break;
		case '<':
			if (--dptr < 0) return 1;
			break;
		case '+':
			++cells[dptr];
			break;
		case '-':
			--cells[dptr];
			break;
		case '.':
			fputc(cells[dptr], stdout);
			break;
		case ',':
			cells[dptr] = fgetc(stdin);
			break;
		case '[':
			if (cells[dptr] == 0) {
				balanced = 0;
				do {
					command = *ip++;
					if (command == 0) break;
					else if (command == '[') --balanced;
					else if (command == ']') ++balanced;
				} while (balanced <= 0);
			}
			break;
		case ']':
			if (cells[dptr] != 0) {
				balanced = 0;
				--ip;
				do {
					command = *--ip;
					if (command == 0) break;
					else if (command == '[') --balanced;
					else if (command == ']') ++balanced;
				} while (balanced >= 0);
				++ip;
			}
		}	
	}
	free(script);
}
