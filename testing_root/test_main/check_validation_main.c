#include "../client.h"
#include <fcntl.h>

int main(int argc, char *argv[]){
    struct stat st;
    if(stat("../game_data/map2.json", &st) == -1){
        perror("stat");
    }
    char *json = (char*)malloc(sizeof(char) * st.st_size);
    FILE *fp = fopen("../game_data/map2.json", "r");
    if(fp == NULL){
        perror("fopen");
    }
    if(fread(json, 1, st.st_size, fp) != st.st_size){
        perror("fread");
    }
    parseJson(json);
    
    map = (int **) malloc (sizeof(int *) * Model.map_width);
	for(int i=0;i<Model.map_width;i++){
		map[i] =(int *) malloc(sizeof(int) * Model.map_height);
	} 

    update_cell();
    int movement;
    char buf;
    int fd = open("testcases/check_validation_testcase", O_RDONLY);
    while(read(fd, &buf, 1) == 1){
        int event = buf;
        if(event <= 0 && event >= 16){
            continue;
        }
        if((movement = check_validation(event)) == 0) fprintf(stderr,"invalid movement!\n");
	    else{	
		    move(event, movement);
	    } 
    }

}