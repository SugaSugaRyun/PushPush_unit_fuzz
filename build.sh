set -x

gcc -Wall -g mapJson.c cJSON.c -o jsonfs -D_FILE_OFFSET_BITS=64

chmod +x jsonfs
