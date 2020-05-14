#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>

char * find_largest_file(char * directory_name, int explore_subdirectories_recursively, long * largest_file_size);
char * concat(const char *s1, const char *s2);

int main(int argc, char *argv[]) {
	long * size_file = calloc(1, sizeof(long));
	char * largest_file = find_largest_file("/home/andrea/Scrivania/elimina/", 0, size_file);
	printf("largest_file : %s\n", largest_file);
	printf("size_file : %ld\n", *size_file);

	exit(EXIT_SUCCESS);
}

char * find_largest_file(char * directory_name, int explore_subdirectories_recursively, long * largest_file_size){
	char * largest_file = NULL;
	long *big_size = calloc(1, sizeof(long));

	DIR * dir_stream_ptr;
	struct dirent *ep;

	dir_stream_ptr = opendir(directory_name);

	if (dir_stream_ptr == NULL) {
		printf("cannot open directory %s!", directory_name);
		exit(EXIT_FAILURE);
	}

	while ((ep = readdir(dir_stream_ptr)) != NULL) {

		if(ep->d_type == DT_REG){
			printf("regolar %s \n", ep->d_name);
			char * file = concat(directory_name, ep->d_name);

			struct stat sb;
			int res = stat(file, &sb);
			if (res == -1) {
				perror("stat()");
				exit(1);
			}

			if(sb.st_size > *big_size){
				largest_file = malloc(strlen(file)+1);
				largest_file = strcpy(largest_file, file);
				*big_size = sb.st_size;

			}


		}

		if(ep->d_type == DT_DIR){
			printf("dir %s \n", ep->d_name);
		}


		// come trovo il file size? posso usare stat (man 2 stat)
	}

	if (errno) {
		perror("readdir() error");
	}

	closedir(dir_stream_ptr);
	*largest_file_size = *big_size;
	return largest_file;
}



char * concat(const char *s1, const char *s2){
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    if(result == NULL){
    	perror("malloc()");
    }
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

