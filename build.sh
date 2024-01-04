set -x

gcc -Wall chat_serv.c cJSON.c -o mapJson -pthread

chmod +x mapJson
