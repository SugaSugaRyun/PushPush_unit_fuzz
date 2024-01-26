#include "../client.h"

int main(){
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


	int id, x, y;
	printf("id: ");
	/*
	item id: item_idxToId(0~num_item)
	user id: (0~Model.max_user)+1
	*/
	fscanf(stdin, "%d", &id);
	if(!((id > 0 && id <= Model.max_user) || (id >= item_idxToId(0) && id <= item_idxToId(num_item)))) id = 1;
	printf("x y: ");
	fscanf(stdin, "%d %d", &x, &y);
	if(x < 0 || x > Model.map_width) x = 0;
	if(y < 0 || y > Model.map_height) y = 0;
	update_model(id, x, y);

}



