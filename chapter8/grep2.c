#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>

#define _GNU_SOURCE
#include <getopt.h>

static struct option longopts[] = {
	{"invert-match", no_argument, NULL, 'v'},
	{"ignore-case", no_argument, NULL, 'i'},
	{"help", no_argument, NULL, 'h'},
	{0, 0, 0, 0}
};

static void do_grep(regex_t *pat, FILE *src);
int opt_ignorecase = 0;
int opt_invert = 0;

int main(int argc, char *argv[]){
	regex_t pat;
	int err, i, opt;

	while((opt = getopt_long(argc, argv, "ivh", longopts, NULL)) != -1){
		switch(opt){
			case 'i':
				opt_ignorecase = 1;
				break;
			case 'v':
				opt_invert = 1;
				break;
			case 'h':
				fprintf(stdout, "Usage: %s [-i, -v FILE...]", argv[0]);
				exit(0);
			case '?':
				fprintf(stderr, "Usage: %s [-i, -v FILE...]", argv[0]);
				exit(1);
		}
	}
	argc -= optind;
	argv += optind;

	if(argc < 1){
		fputs("no pattern\n", stderr);
		exit(1);
	}
	char *pattern = argv[0];
	argc--;
	argv++;

	int re_mode = REG_EXTENDED | REG_NOSUB | REG_NEWLINE;
	if(opt_ignorecase){
		re_mode |= REG_ICASE;
	}

	err = regcomp(&pat, pattern, re_mode);
	if(err != 0){
		char buf[1024];
		regerror(err, &pat, buf, sizeof buf);
		puts(buf);
		exit(1);
	}
	if(argc == 0){
		do_grep(&pat, stdin);
	}else{
		for(i = 0; i < argc; i++){
			FILE *f;

			f = fopen(argv[i], "r");
			if(!f){
				perror(argv[i]);
				exit(1);
			}
			do_grep(&pat, f);
			fclose(f);
		}
	}
	regfree(&pat);
	exit(0);
}

static void do_grep(regex_t *pat, FILE *src)
{
	char buf[4096];
	int matched;

	while(fgets(buf, sizeof buf, src)){
		matched = (regexec(pat, buf, 0, NULL, 0) == 0);
		if(opt_invert){
			matched = !matched;
		}
		if(matched){
			fputs(buf, stdout);
		}
	}
}

