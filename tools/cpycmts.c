#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* TODO: fix endless loop bug in while(1) loop. */


/* The maximal number of columns across the whole project is set to `80'. */
#define MAX_COLS 80

/* The project uses exclusively multi-line comments, so we do not need to
 * check for "//" comments. */
#define CMT_BEG "/*"
#define CMT_END "*/"

const char* usage_str = "";

FILE* in;
FILE* out;

static void close_files(void) {
	fclose(out);
	fclose(in);
}

int main(int argc, char** argv) {
	switch (argc) {
	case 1:
		in = stdin;
		out = stdout;

		break;
	case 2:
		if ((in = fopen(argv[1], "r"))) {
			perror("fopen");
			return EXIT_FAILURE;
		}
		out = stdout;

		break;
	case 3:
		if ((in = fopen(argv[1], "r"))) {
			perror("fopen");
			return EXIT_FAILURE;
		}
		if ((out = fopen(argv[2], "w"))) {
			fclose(in);
			perror("fopen");
			return EXIT_FAILURE;
		}
	}

	atexit(close_files);

	/* `+ 1' for the potential newline. */
	char buf[MAX_COLS + 1];
	int cmt = 0;
	while (fgets(buf, sizeof(buf), in)) {
puts("sdsd");
		char* cmt_del = buf;
		while (1) {
puts("sd");
			if (!(cmt_del = strstr(cmt_del, CMT_BEG))) {
				if (!(cmt_del = strstr(cmt_del, CMT_END)))
					break;
				if (cmt_del && !cmt) {
					fprintf(stderr,
					"Misplaced comment end"
					" delimiter\n", stderr);
					return EXIT_FAILURE;
				}
				cmt = 0;
			} else {
				cmt = 1;
			}	
		}

		if (cmt) {
			fprintf(out, cmt_del);
		}
	}
}
