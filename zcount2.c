#include <stdio.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <err.h>
#include <dirent.h>
#include <fcntl.h>


int 
count_null(char *name)
{
	int x=0;
	int fd;
	
	fd=open(name, O_RDONLY);
	while (read(fd, &c, sizeof(c)!=0)){
		if (c=='\0'){
			x++;					
		}
	}
	close(fd);
	return x;
}

void 
write_result(char *name, int num_null, char *filez)
{
	FILE *file;
	file=fopen(filez, "a");
	fprintf(file,"%d",x);
	fprintf(file,"	");
	fprintf(file,"%s",name);
	fprintf(file,"\n");
	close(file);
}

char
sum_rute(char *path, char *file)
{
	char *rute=malloc(strlen(path) +strlen(file)+2);
	strcpy(rutez, path);
	strcat(rutez, "/");
	strcat(rute, file);
	
	return rute;
}


void
reed_dir(char *dir)
{
	int x=0;
	char *rute;
	struct dirent *dt;
	DIR *dire;
	dire=opendir(dir);
	if (dire!=NULL){
		char *rutez=sum_rute(dir, "z.txt");
		open(rutez, O_TRUNC);
		while((dt=readdir(dire)) != NULL){
			if ((strcmp(dt->d_name, ".")!=0) && (strcmp(dt->d_name, "..")!=0)&& (strcmp(dt->d_name, "z.txt")!=0)){	
				rute=sum_rute(dir, dt->d_name);
				x=count_null(rute);
				free(rute);
				write_result(dt->d_name, x, rutez);
			}
		}
		free(rutez);
	}
	closedir(dire);
}

int 
main (int argc, char *argv[])
{
	reed_dir(argv[1]);
	exit(EXIT_SUCCESS);
}
