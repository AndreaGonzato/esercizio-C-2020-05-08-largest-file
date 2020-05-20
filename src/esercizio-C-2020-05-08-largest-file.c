#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>



char * find_largest_file(char * directory_name, int explore_subdirectories_recursively, long * largest_file_size);
char * concat(const char *s1, const char *s2);



int main(int argc, char *argv[]) {

	printf("give an argument for this program if you want to specify the folder from where to start searching, otherwise it will be your home\n");

	long * size_file = calloc(1, sizeof(long));
    if(size_file == NULL){
    	perror("calloc()");
    	exit(1);
    }

    char * user = getlogin();
    char * path;
    if(argc >= 2){
    	path = argv[1];
    }else{
    	path = concat(concat("/home/", user),"/");
    }

    int research_depth = 0;
    printf("write how deep you want to serach:");
    scanf("%d", &research_depth);


    printf("path : %s\n", path);
    printf("research_depth : %d\n\n", research_depth);

	char * largest_file = find_largest_file(path, research_depth, size_file);
	printf("largest_file : %s\n", largest_file);
	printf("size_file    : %ld bytes\n", *size_file);

	exit(EXIT_SUCCESS);
}

char * find_largest_file(char * directory_name, int explore_subdirectories_recursively, long * largest_file_size){
	char * largest_file = NULL;
	long * biggest_size_found = calloc(1, sizeof(long));
	if(biggest_size_found == NULL){
		perror("calloc()\n");
		exit(1);
	}

	//printf("\n%s\n", directory_name);//TEST

	DIR * dir_stream_ptr;
	struct dirent *ep;

	dir_stream_ptr = opendir(directory_name);

	if (dir_stream_ptr == NULL) {
		printf("cannot open directory %s!", directory_name);
		exit(EXIT_FAILURE);
	}

	while ((ep = readdir(dir_stream_ptr)) != NULL) {

		if(ep->d_type == DT_REG){
			//printf( "%*sregular   : %s\n", (research_depth-explore_subdirectories_recursively)*2, " ", ep->d_name );//TEST
			char * file = concat(directory_name, ep->d_name);

			struct stat sb;
			int res = stat(file, &sb);
			if (res == -1) {
				perror("stat()");
				exit(1);
			}

			if(sb.st_size > *biggest_size_found){
				largest_file = malloc(strlen(file)+1);
				if(largest_file == NULL){
					perror("malloc()\n");
					exit(1);
				}
				largest_file = strcpy(largest_file, file);
				*biggest_size_found = sb.st_size;
			}
		}

		if(ep->d_type == DT_DIR){
			//printf( "%*sdirectory : %s\n", (research_depth-explore_subdirectories_recursively)*2, " ", ep->d_name );//TEST

			char * dir_relative_path = malloc(strlen(ep->d_name)+ 1);
		    if(dir_relative_path == NULL){
		    	perror("malloc()");
		    	exit(1);
		    }
		    strcpy(dir_relative_path, ep->d_name);

			if(dir_relative_path[0] != '.' && explore_subdirectories_recursively >= 1){

				long * biggest_file_in_subdir_size = calloc(1, sizeof(long));
				if(biggest_size_found == NULL){
					perror("calloc()\n");
					exit(1);
				}

				char * sub_dir_path = concat(concat(directory_name, ep->d_name), "/");
				//printf("\n%s\n", sub_dir_path);
				char * biggest_file_in_subdir = find_largest_file(sub_dir_path, explore_subdirectories_recursively-1, biggest_file_in_subdir_size);
				if(*biggest_file_in_subdir_size > *biggest_size_found){
					*biggest_size_found = *biggest_file_in_subdir_size;
					largest_file = malloc(strlen(biggest_file_in_subdir)+1);
					if(largest_file == NULL){
						perror("malloc()\n");
						exit(1);
					}
					largest_file = strcpy(largest_file, biggest_file_in_subdir);
				}
				free(biggest_file_in_subdir_size);
				free(biggest_file_in_subdir);
				free(sub_dir_path);
			}
			free(dir_relative_path);
		}

	}

	if (errno) {
		perror("readdir() error");
	}

	closedir(dir_stream_ptr);
	*largest_file_size = *biggest_size_found;
	free(biggest_size_found);
	return largest_file;
}



char * concat(const char *s1, const char *s2){
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    if(result == NULL){
    	perror("malloc()");
    	exit(1);
    }
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

