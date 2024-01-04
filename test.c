#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include <sys/stat.h>
#include <errno.h>

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
FILE *file;

int main() {
    // 파일에서 JSON 데이터 읽어오기
    FILE *json_file = fopen("test.json", "r");
    if (json_file == NULL) {
        perror("Error opening JSON file");
        return EXIT_FAILURE;
    }

    fseek(json_file, 0, SEEK_END);
    long file_size = ftell(json_file);
    fseek(json_file, 0, SEEK_SET);

    char *json_data = (char *)malloc(file_size + 1);
    fread(json_data, 1, file_size, json_file);
    json_data[file_size] = '\0';

    fclose(json_file);

    // JSON 데이터를 파싱하여 cJSON 구조체 생성
    cJSON *root = cJSON_Parse(json_data);

    // cJSON_Print를 사용하여 cJSON 구조체를 JSON 문자열로 출력
    char *json_output = cJSON_Print(root);
    printf("JSON Output:\n%s\n", json_output);

    // 메모리 해제
    free(json_data);
    free(json_output);
    cJSON_Delete(root);

    return 0;
}

