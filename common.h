#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

#define MAX_PLAYERS 3
#define BUFFER_SIZE 4000

void error(const char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(1);
}