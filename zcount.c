/*Zcount, por Juan Ureña.
El programa recibe un directorio como unico parametro,
este lo abrira para leer los ficheros internos y accederá
a estos para contar los NULL que contenga. Acto seguido
escribira el resultado en un fichero z.txt, en el propio
directorio.*/

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

//Fichero donde escribo nulos
char *Fichero="z.txt";
//Utiles varios
enum{
In_Dir=1,
In_Max=2,
};

int 
count_null(char *fich)
//Abro el fichero y cuento los nulos que tenga
{
	int null_aux=0;
	char c;
	int fd;
	
	fd=open(fich, O_RDONLY);
	while (read(fd, &c, sizeof(c))){
		if (c=='\0'){
			null_aux++;					
		}
	}
	close(fd);
	return null_aux;
}

char *
generate_path(char *path, char *file)
//Genero ruta absoluta con ruta y el nombre del fichero
{
	char *absolute=malloc(strlen(path) +strlen(file)+2);
	if (absolute){
		strcpy(absolute, path);
		strcat(absolute, "/");
		strcat(absolute, file);
	}else{
		fprintf(stderr, "\nError en la memoria\n");
		exit(EXIT_FAILURE);
	}
	
	return absolute;
}

void 
write_z(char *fich, char *name, int null_aux)
//Escribo en el fichero indicado, numero ceros y el fichero
{
	FILE *file;
	file=fopen(fich, "a");
	if (file){
		fprintf(file,"%d",null_aux);
		fprintf(file,"	");
		fprintf(file,"%s",name);
		fprintf(file,"\n");
		fclose(file);
	}else{
		fprintf(stderr, "\nError al escribir el fichero\n");
		exit(EXIT_FAILURE);
	}
} 

void
clear_z(char *path)
//Vacio el fichero z.txt si tiene algo
//si no puedo abrirlo, error
{
	int fd;
	fd=open(path, O_TRUNC);
	if (fd){
		close(fd);
	}
}

void
read_dir(char *dir)
{
	char *aux_path;
	char *z_path;
	int num_null=0;
	struct dirent *dt;
	DIR *dire;
	
//Mi fichero z.txt y lo vacio
	z_path=generate_path(dir,Fichero);
	clear_z(z_path);

//Leo los ficheros en mi directorio	
	dire=opendir(dir);
	if (dire!=NULL){
		while((dt=readdir(dire)) != NULL){
//Mientras haya otro sigo. No entro en ., .. y z.txt 
			if ((strcmp(dt->d_name, ".")) && (strcmp(dt->d_name, ".."))&& (strcmp(dt->d_name, "z.txt"))){
//Me creo la ruta completa, cuento nulos y los añado a z.txt
				aux_path=generate_path(dir,dt->d_name);
				num_null=count_null(aux_path);
				free(aux_path);
				write_z(z_path, dt->d_name, num_null);
			}
		}
//libero memoria  y cierro directorio
		free(z_path);
		closedir(dire);
	}else{
//Si no puedo abrir directorio, error
		fprintf(stderr, "\nError en el directorio\n");
		exit(EXIT_FAILURE);
	}
	
}

int 
main (int argc, char *argv[])
{
//Compruebo entrada correcta, si no error
	if (argc==In_Max){
		read_dir(argv[In_Dir]);
	}else{		
		fprintf(stderr, "\nError en la entrada, solo debe pasarse el directorio\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
