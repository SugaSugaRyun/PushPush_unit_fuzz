#include "../client.h"

int main(int argc, char **argv){
    int read_len = 0;
    int rd = 0;
    char buf;
    char* json = NULL;
    while((rd = read(STDIN_FILENO, &buf, 1)) > 0){
        read_len += rd;
        char* temp = (char*)malloc(sizeof(char) * read_len);
        for(int i=0; i < read_len - rd; i++){
            temp[i] = json[i];
        }
        temp[read_len-1] = buf;
        if(json != NULL) free(json);
        json = temp;
    }
    printf("%s", json);
    parseJson(json);

    return 0;
}