#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define _GNU_SOURCE
#include <getopt.h>

static struct option longopts[] = {
	{"replace", no_argument, NULL, 'r'},
	{"help", no_argument, NULL, 'h'},
	{0, 0, 0, 0}
};

static void do_cat(FILE *f, int r_flag);

int main(int argc, char *argv[])
{
	int i, opt;
	int r_flag = 0; //replace flag

	while((opt = getopt_long(argc, argv, "rh", longopts, NULL)) != -1){
		switch(opt){
			case 'r':
				r_flag = 1;
				break;
			case 'h':
				fprintf(stdout, "Usage: %s [-r] [FILE...]\n", argv[0]);
				exit(0);
			case '?':
				fprintf(stderr, "Usage: %s [-r] [FILE...]\n", argv[0]);
				exit(1);
		}
	}
	if(optind == argc){
		do_cat(stdin, r_flag);
	}else{
		for(i = optind; i < argc; i++){
			FILE *f;
			f = fopen(argv[i], "r");
			if(!f){
				perror(argv[i]);
				exit(1);
			}
			do_cat(f, r_flag);
			fclose(f);
		}
	}
	exit(0);
}

#define BUFFER_SIZE 2048
static void do_cat(FILE *f, int r_flag)
{
	int c;

	while((c = fgetc(f)) != EOF){
		if(r_flag == 1){
			switch(c){
				case '\t':
					if(fputs("\\t", stdout) == EOF) exit(1);
					break;
				case '\n':
					if(fputs("\\n", stdout) == EOF) exit(1);
					break;
				default:
					if(putchar(c) < 0) exit(1);
					break;
			}
		}else{
			if(putchar(c) < 0) exit(1);
		}
	}
}
