#include "../client.h"

int main(int argc, char **argv){
    int read_len = 0;
    int rd = 0
    char buf[256]
    char* json;
    while((rd = read(STDIN_FILENO, buf, 256)) > 0){
        read_len += rd;
        char* temp = (char*)malloc(sizeof(char) * read_len);
        for(int i=0; i < read_len - rd; i++){
            temp[i] = json[i];
        }
    }
    return 0;
}