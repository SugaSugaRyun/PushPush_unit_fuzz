/* taken from https://github.com/fntlnz/fuse-example, and modified */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include "cJSON.h"

#define NAME_SIZE 16

typedef struct location{
	int x;
	int y;
}location_t;

typedef struct user{
	char name[NAME_SIZE];
	int score;
	location_t base_loc;
	location_t user_loc;
}user_t;

typedef struct object_data{
	int map_width;
	int map_height;
	int timeout;
	int max_user;
	struct user * users;
	struct location* item_locations; 
	struct location* block_locations; 
}object_data_t;

cJSON* root;
object_data_t *Object;

int parseJson() {

	char filepath[256] = "file.json";
	
	FILE *file = fopen(filepath,"r");
    	if(file == NULL){
        	fprintf(stderr,"ERROR: open file");
        	return 1;
	}
    	
	struct stat st;
	if(stat(filepath, &st) == -1){
		fprintf(stderr,"ERROR: stat()\n");
		return 1;
	}
	int size = st.st_size;
	
	char* jsonfile = (char*)malloc(size+1);
    	if(jsonfile == NULL){
        	fprintf(stderr,"ERROR: memory allocation\n");
        	return 1;
    	}

    	int read_size = fread(jsonfile, 1, size, file);
    	if(read_size != size){
		fprintf(stderr, "ERROR: read file\n");
        	return 1;
    	}

    	fclose(file);
    	jsonfile[size] = '\0';
	
	root = cJSON_Parse(jsonfile);
	if (root == NULL) {
		printf("JSON 파싱 오류: %s\n", cJSON_GetErrorPtr());
        	return 1;
    	}
	Object = (object_data_t *)malloc(sizeof(object_data_t));
        
	cJSON* timeout = cJSON_GetObjectItem(root, "timeout");
	Object->timeout = timeout->valueint;
	cJSON* max_user = cJSON_GetObjectItem(root, "max_user");
	Object->max_user = max_user->valueint;

	cJSON* map = cJSON_GetObjectItem(root, "map");
	cJSON* map_width = cJSON_GetObjectItem(map, "map_width");
	Object->map_width = map_width->valueint;	
	cJSON* map_height = cJSON_GetObjectItem(map, "map_height");
	Object->map_height = map_height->valueint;

	cJSON* user = cJSON_GetObjectItem(root, "user");
	int num_user = cJSON_GetArraySize(user);
	Object->users = (struct user *)malloc(sizeof(struct user) * num_user);
	for(int i = 0; i < num_user; i++){
		memset(Object->users->name, 0, sizeof(NAME_SIZE));
		Object->users->score = 0;
		cJSON* user_array = cJSON_GetArrayItem(user,i);
	       	cJSON* base = cJSON_GetObjectItem(user_array,"base"); 
		cJSON* base_x = cJSON_GetArrayItem(base, 0);
		cJSON* base_y = cJSON_GetArrayItem(base, 1);
		Object->users[i].base_loc.x = base_x->valueint;
		Object->users[i].base_loc.y = base_y->valueint;
#ifdef DEBUG
		printf("name: %s\n",Object->users->name);
		printf("base x: %d\n",Object->users[i].base_loc.x);
		printf("base y: %d\n",Object->users[i].base_loc.y);
#endif
	}
	
	cJSON * item = cJSON_GetObjectItem(root, "item_location");
	int num_item = cJSON_GetArraySize(item);
	Object->item_locations = (struct location *)malloc(sizeof(struct location) * num_item); 
	for(int i = 0; i < num_item; i++){
		cJSON* item_array = cJSON_GetArrayItem(item,i);
		cJSON* item_x = cJSON_GetArrayItem(item_array, 0);
		cJSON* item_y = cJSON_GetArrayItem(item_array, 1);
		Object->item_locations[i].x = item_x->valueint;
		Object->item_locations[i].y = item_y->valueint;
#ifdef DEBUG
		printf("item x: %d\n",Object->item_locations[i].x);
		printf("item y: %d\n",Object->item_locations[i].y);
#endif
	}	

	cJSON * block = cJSON_GetObjectItem(root, "block_location");
	int num_block = cJSON_GetArraySize(block);
	Object->block_locations = (struct location *)malloc(sizeof(struct location) * num_block); 
	for(int i = 0; i < num_block; i++){
		cJSON* block_array = cJSON_GetArrayItem(block,i);
		cJSON* block_x = cJSON_GetArrayItem(block_array, 0);
		cJSON* block_y = cJSON_GetArrayItem(block_array, 1);
		Object->block_locations[i].x = block_x->valueint;
		Object->block_locations[i].y = block_y->valueint;
#ifdef DEBUG
		printf("block x: %d\n",Object->block_locations[i].x);
		printf("block y: %d\n",Object->block_locations[i].y);
#endif
	}	
		
	return 0;
}


int main(int argc, char *argv[]){
	// struct json_object * fs_json = json_object_from_file("examplejson.json") ; 
    	parseJson();
	free(Object->users);
	free(Object->item_locations);
	free(Object->block_locations);
	free(Object); 
  return 0;
}
