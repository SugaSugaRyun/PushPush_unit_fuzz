#include <stdio.h>
#include <stdlib.h>


int clnt_socks[5] = {1, 10, 15, 8, 2};
int clnt_cnt = 5;

//disconnected: check if any user is disconnected, handel clnt_cnt $$$$
void disconnected(int sock){
//	pthread_mutex_lock(&mutx);

	for (int i = 0; i < clnt_cnt; i++){   // remove disconnected client
		if (sock == clnt_socks[i]){
			while (i < clnt_cnt-1){
				clnt_socks[i] = clnt_socks[i+1];
				i++;
			}
			break;
		}
	}
	clnt_cnt--;

	if(clnt_cnt == 0){
//		game_start = 0;
//		usr_cnt = 0;
		exit(EXIT_SUCCESS);
	}
//	pthread_mutex_unlock(&mutx);
//	close(sock);

}

int main(){
	
	for(int i = 0; i < clnt_cnt; i++) printf("%d ", clnt_socks[i]);
	printf("remove %d...\n", 10);
	disconnected(10);
	for(int i = 0; i < clnt_cnt; i++) printf("%d ", clnt_socks[i]);
	printf("\n");

}
