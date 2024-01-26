#include "../client.h"

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
    update_cell();
    check_validation(15);
}