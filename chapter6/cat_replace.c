#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static void do_cat(FILE *f);

int main(int argc, char *argv[])
{
	int i;

	if(argc == 1){
		do_cat(stdin);
	}else{
		for(i = 1; i < argc; i++){
			FILE *f;
			f = fopen(argv[i], "r");
			if(!f){
				perror(argv[i]);
				exit(1);
			}
			do_cat(f);
			fclose(f);
		}
	}
	exit(0);
}

#define BUFFER_SIZE 2048
static void do_cat(FILE *f)
{
	int c;

	while((c = fgetc(f) != EOF)){
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
	}
}
