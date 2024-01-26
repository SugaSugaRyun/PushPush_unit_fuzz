#include "../client.h"

int main(){

	//int event = event_arry[front]; //0~15
	//int movement = check_validation(event); //0, 1, (10)*num_item, (-10)*num_item
	/*
	0 if invalid movement
	1 if valid movement
	item id if item-and-valid(include other's base)
	-item id if item-and-scoreup(is it base id?) a.... 
	*/
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

	int event, movement;
	printf("event: ");
	fscanf(stdin, "%d", &event);
	if(event < 0 || event >= Model.max_user*4) event = 0;
	printf("movement: ");
	fscanf(stdin, "%d", &movement);
	if(!(movement == 1 || movement == 0 || ((movement < item_idxToId(0) && (movement > item_idxToId(num_item)))) || ((movement < (0 - item_idxToId(num_item)) && movement > (0 - item_idxToId(0)))))) movement = 0;

	move(event, movement);



}




