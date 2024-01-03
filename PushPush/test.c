#include <stdio.h>
#include <time.h>
#include <stdlib.h>
// #include <gtk/gtk.h>

#define image_size 16
   
#define BACKGROUND 0
#define PLAYER 1
#define HOME 2
#define TARGET 3
#define OBSTACLE 4


#define TARGET_NUM 10
#define OBSTACLE_NUM 20
#define MAP_SIZE 16
// 16x16
// 0,0은 플레이어 위치로 고정
// 타켓은 각 열에 하나씩 먼저 생성 되게끔

int map[MAP_SIZE][MAP_SIZE/8];
// short map_cell[MAP_SIZE][MAP_SIZE];

void make_map()
{
    //player start
    map[0][0] |= 0x10000000;
    
    int x, y, z;

    srand(time(NULL));

    for(int i=0; i<TARGET_NUM;)
    {
        
        int check_target[MAP_SIZE] = {0};
    

        do
        {
            y = rand() % MAP_SIZE;
            if(check_target[y] <= i/(int)MAP_SIZE)
            {
                check_target[y]++;
                // fprintf(stderr,"%d\n",check_target[y]);

            }
                
        }
        while(check_target[y] < i/(int)MAP_SIZE);

        x = x % (MAP_SIZE/8);
        z = rand() % 8;

        if( (map[y][x] >> (4*z) & 0xF) == 0)
        {
            map[y][x] |= (0x3 << 4*z);
            i++;
        }
    }
}


int main(int argc, char *argv[]) {

    // make_map();

     int *dynamicData = (int *)malloc(sizeof(int) * 2);


    dynamicData[0] = 1;
    dynamicData[0] <<= 4 * 7;
    // 첫 번째 원소의 0번째 비트를 1로 설정
    printf("%x %x\n",dynamicData[0],dynamicData[1] );

 
    return 0;
}
