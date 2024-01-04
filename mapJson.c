/* taken from https://github.com/fntlnz/fuse-example, and modified */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include "cJSON.h"


typedef struct location {
    int x;
    int y;
}location;

typedef struct object_data{	
	int map_width;
	int map_height;
	int timeout;
	int num_user;
	location* base_locations; 
	location* user_locations;
	int num_item;
	location* item_locations;
	int num_block;
	location* block_locations;
}object_data;

int arraySize;
cJSON* item;
object_data *Object;

void parseEntries(cJSON* entriesArray, location* entries, int* numEntries) {
	if (entriesArray == NULL || !cJSON_IsArray(entriesArray)) {
			entries = NULL;
			*numEntries = 0;
			return;
	}

	int element_cnt = *numEntries;

	for (int i = 0; i < element_cnt; i++) {
		cJSON* entry = cJSON_GetArrayItem(entriesArray, i);
		cJSON* inode = cJSON_GetObjectItem(entry, "x");
		entries[i].x = inode->valueint;
		inode = cJSON_GetObjectItem(entry, "y");
		entries[i].y = inode->valueint;

	}
}

int parseJson() {
	char filepath[256] = "test.json";
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
	
	item = cJSON_Parse(jsonfile);
	if (item == NULL) { 
			const char *error_ptr = cJSON_GetErrorPtr(); 
			if (error_ptr != NULL) { 
					printf("Error: %s\n", error_ptr); 
			} 
			cJSON_Delete(item); 
			return 1; 
	} 
	Object = (object_data *)malloc(sizeof(object_data));

  cJSON* inode = cJSON_GetObjectItemCaseSensitive(item, "map_width");
	Object->map_width = inode->valueint;
	inode = cJSON_GetObjectItemCaseSensitive(item, "map_height");
	Object->map_height = inode->valueint;
	inode = cJSON_GetObjectItemCaseSensitive(item, "timeout");
	Object->timeout = inode->valueint;
	inode = cJSON_GetObjectItemCaseSensitive(item, "num_user");
	Object->num_user = inode->valueint;
	//printf("%d %d %d\n",Object->map_size, Object->timeout, Object->num_user);
	cJSON* entries = cJSON_GetObjectItemCaseSensitive(item, "base");
	Object->base_locations = (location *)malloc(sizeof(location)*Object->num_user);
	parseEntries(entries, Object->base_locations, &(Object->num_user));
	
	entries = cJSON_GetObjectItemCaseSensitive(item, "user_location");
	Object->user_locations = (location *)malloc(sizeof(location)*Object->num_user);
	parseEntries(entries, Object->user_locations, &(Object->num_user));

	inode = cJSON_GetObjectItemCaseSensitive(item, "num_item");
	Object->num_item = inode->valueint;

	entries = cJSON_GetObjectItemCaseSensitive(item, "item_location");
	Object->item_locations = (location *)malloc(sizeof(location)*Object->num_item);
	parseEntries(entries, Object->item_locations, &(Object->num_item));
	
	inode = cJSON_GetObjectItemCaseSensitive(item, "num_block");
	Object->num_block = inode->valueint;

	entries = cJSON_GetObjectItemCaseSensitive(item, "block_location");
	Object->block_locations = (location *)malloc(sizeof(location)*Object->num_block);
	parseEntries(entries, Object->block_locations, &(Object->num_block));

  
	return 0;
}

int main(int argc, char *argv[]){
	// struct json_object * fs_json = json_object_from_file("examplejson.json") ; 
	
  if(parseJson()){
		exit(1);
	}
	printf("map_width = %d\n",Object->map_width);
	printf("map_height = %d\n",Object->map_height);
	printf("timeout = %d\n", Object->timeout);
	printf("num_user = %d\n", Object->num_user);
	printf("user 1 = %d %d\n", Object->user_locations[0].x, Object->user_locations[0].y);
	printf("user 2 = %d %d\n", Object->user_locations[1].x, Object->user_locations[1].y);
	printf("base 1 = %d %d\n", Object->base_locations[0].x, Object->base_locations[0].y);
	printf("base 2 = %d %d\n", Object->base_locations[1].x, Object->base_locations[1].y);
	printf("num_item = %d\n", Object->num_item);
	for(int i = 0; i < Object->num_item; i++){
		printf("item[%d] x: %d, y: %d\n", i, Object->item_locations[i].x, Object->item_locations[i].y);
	}
	printf("num_block = %d\n", Object->num_block);
	for(int i = 0; i < Object->num_block; i++){
		printf("block[%d] x: %d, y: %d\n", i, Object->block_locations[i].x, Object->block_locations[i].y);
	}

	free(Object);
    
  return 0;
}
