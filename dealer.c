#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include "common.h"
#include <pthread.h>
#include <time.h>

struct cards{
	char name[40];
	int number;
};

struct cards pc[52];

int random_array[52];

//This variable keeps a track of the cards pulled out from the deck
int global_track = 0;

void Initialize_random_array(){
	int i;
	for(i=0;i<52;i++){
		random_array[i] = i;
	}

	srand(time(NULL));

	//Shuffling the array
	for(i=0;i<52;i++){
		int j = rand() % (i+1);

		int temp;
		temp = random_array[i];
		random_array[i] = random_array[j];
		random_array[j] = temp;	
		
	}







}



void Initialize_card(){
	int i;
	int j;
	j = 1;
	for(i=0;i<52;i++){
		if(i>=0 && i<=12){
			strcpy(pc[i].name,"Spades");
			pc[i].number = j;
			j++;
			if(i==12){
				j=1;
			}
		}

		else if(i>=13 && i<=25){
			strcpy(pc[i].name,"Hearts");
			pc[i].number = j;
			j++;
			if(i==25){
				j=1;
			}
		}

		else if(i>=26 && i<=38){
			strcpy(pc[i].name,"Diamonds");
			pc[i].number = j;
			j++;
			if(i==38){
				j=1;
			}
		}

		else if(i>=39 && i<=51){
			strcpy(pc[i].name,"Clubs");
			pc[i].number = j;
			j++;
			if(i==51){
				j=1;
			}
		}

	}
}





int clientFd[MAX_PLAYERS];


void start_game(int players){
	int j;

	//Initialize cards and let the dealer choose two cards
	Initialize_card();
	int i;

	for(i=0;i<MAX_PLAYERS;i++){
		if(clientFd[i]>0){
			char buffer[BUFFER_SIZE];
			int nwritten;
			strcpy(buffer,"Welcome to the Game Player, Dealer is choosing his cards ....... \n");
			if (BUFFER_SIZE != (nwritten = write(clientFd[i], buffer, BUFFER_SIZE)))
			{
				printf("Error! Couldn't write to player \n");
				//close(clientFd[i]);
				return;
			}


		}
	}

	/* The dealer chooses his cards over here */
	Initialize_random_array();

	//Choosing two cards
	int total = 0 ;
	int n1 = random_array[global_track];
	global_track++;
	int n2 = random_array[global_track];
	global_track++;
	total = total + pc[n1].number + pc[n2].number;

	//Passing the number of the Cards
	char name1[100];
	char name2[100];

	//Passing the Name of the Cards
	strcpy(name1,pc[n1].name);
	strcpy(name2,pc[n2].name);

	printf("%d %s\n",pc[n1].number,name1);
	printf("%d %s\n",pc[n2].number,name2);
	char buffer[BUFFER_SIZE];
	
	//strcat(buffer,name1);
	int nwritten;

	int temp;
	temp = global_track;

	
    /////////////////////// For the first client /////////////////////////

    int n_first = 0;
    strcpy(buffer,"You are the first player, press HIT/STAND \n");
    strcat(buffer,pc[temp].name);
	if (BUFFER_SIZE != (nwritten = write(clientFd[0], buffer, BUFFER_SIZE)))
			{
				printf("Error! Couldn't write to player \n");
				//close(clientFd[i]);
				return;
	}

	 if (0 > (nwritten = read(clientFd[0], buffer, BUFFER_SIZE))){
            /* error("Error reading from client"); */
            printf("Response from socket  timed out\n");
            
     }
     else{
     	while(1){
	     	if(strcmp(buffer,"HIT")==0){
	     		n_first++;
	     		printf("\nUser1 has chosen HIT, you have another chance!\n");
	     		 if (0 > (nwritten = read(clientFd[0], buffer, BUFFER_SIZE))){
                  /* error("Error reading from client"); */
                  printf("Response from socket  timed out\n");
            
            }

	     	}
	     	else{
	     		//Player has chosen STAND and decided not to continue
	     		strcpy(buffer,"Your turn over, now wait for the results\n");
	     		int nwritten;
	     		if (BUFFER_SIZE != (nwritten = write(clientFd[0], buffer, BUFFER_SIZE))){
					printf("Error! Couldn't write to player \n");
					//close(clientFd[i]);
					return;
	            }

	     		break;
	     	}


        }
     }

     printf("First player has chosen %d  cards\n",n_first);











	//////////////////////// FOR THE second client ///////////////////////



    strcpy(buffer,"Player 1 has already chosen cards, your turn press HIT/STAND!\n");
	int n_second = 0;
    strcat(buffer,"You are the Second player, press HIT/STAND \n");
	if (BUFFER_SIZE != (nwritten = write(clientFd[1], buffer, BUFFER_SIZE)))
			{
				printf("Error! Couldn't write to player \n");
				//close(clientFd[i]);
				return;
	}

	 if (0 > (nwritten = read(clientFd[1], buffer, BUFFER_SIZE))){
            /* error("Error reading from client"); */
            printf("Response from socket  timed out\n");
            
     }
     else{
     	while(1){
	     	if(strcmp(buffer,"HIT")==0){
	     		n_second++;
	     		printf("\nUser 2 has chosen HIT, you have another chance!\n");
	     		 if (0 > (nwritten = read(clientFd[1], buffer, BUFFER_SIZE))){
                  /* error("Error reading from client"); */
                  printf("Response from socket  timed out\n");
            
            }

	     	}
	     	else{
	     		//Player has chosen STAND and decided not to continue
	     		strcpy(buffer,"Your turn over, now wait for the results\n");
	     		int nwritten;
	     		if (BUFFER_SIZE != (nwritten = write(clientFd[1], buffer, BUFFER_SIZE))){
					printf("Error! Couldn't write to player \n");
					//close(clientFd[i]);
					return;
	            }

	     		break;
	     	}


        }
     }

     printf("Second player has chosen %d  cards\n",n_first);




     //////////////////////// For the third client /////////////////////////////

    strcpy(buffer,"Player 2 has already chosen cards, your turn press HIT/STAND!\n");
	int n_third = 0;
    strcat(buffer,"You are the Third player, press HIT/STAND \n");
	if (BUFFER_SIZE != (nwritten = write(clientFd[2], buffer, BUFFER_SIZE)))
			{
				printf("Error! Couldn't write to player \n");
				//close(clientFd[i]);
				return;
	}

	 if (0 > (nwritten = read(clientFd[2], buffer, BUFFER_SIZE))){
            /* error("Error reading from client"); */
            printf("Response from socket  timed out\n");
            
     }
     else{
     	while(1){
	     	if(strcmp(buffer,"HIT")==0){
	     		n_third++;
	     		printf("\nUser 3 has chosen HIT, you have another chance!\n");
	     		 if (0 > (nwritten = read(clientFd[2], buffer, BUFFER_SIZE))){
                  /* error("Error reading from client"); */
                  printf("Response from socket  timed out\n");
            
            }

	     	}
	     	else{
	     		//Player has chosen STAND and decided not to continue
	     		strcpy(buffer,"Your turn over, now wait for the results\n");
	     		int nwritten;
	     		if (BUFFER_SIZE != (nwritten = write(clientFd[2], buffer, BUFFER_SIZE))){
					printf("Error! Couldn't write to player \n");
					//close(clientFd[i]);
					return;
	            }

	     		break;
	     	}


        }
     }

     printf("Third player has chosen %d  cards\n",n_first);


     /// Calculate score for each of the player and pass the score to them
     int total1 = 0;
     int total2 = 0;
     int total3 = 0;

     int m;
     int t;
     for(m=global_track;m<global_track+n_first;m++){
     	total1 = total1 + pc[m].number;
     }

     t = m;
     for(m=t;m<global_track + n_first+n_second;m++){
     	total2 = total2 + pc[m].number;
     }

     t = m;
     for(m=t;m< global_track + n_first+n_second+n_third;m++){
     	total3 = total3 + pc[m].number;
     }

     ///////////////////  Conditions for Scoring  ////////////////////

     int arr[3];
     arr[0] = 0;
     arr[1] = 0;
     arr[2] = 0;

     if(total1 > 21){
     	//printf("User 1 loses \n");
     	arr[0] = 1;
     }

     if(total2 > 21){
     	//printf("User 2 loses \n");
     	arr[1] = 1;
     }

     if(total3 >21){
     	//printf("User 3 loses \n");
     	arr[2] = 1;
     }

     int b;
     for(b=0;b<3;b++){
     	if(arr[b]==1){
     		printf("Player %d loses because of total > 21 ! ",b+1);
     	}
     }

     int diff1, diff2, diff3;
     int min;

     int win[3];
     win[0] = 0;
     win[1] = 0;
     win[2] = 0;


     if(total1>=21 && total>=21 && total>=21){
     	win[0] = 0;
     	win[1] = 0;
     	win[2] = 0;

     }

     if(total1<21 || total2<21 || total3<21){
     	//The minimum difference has to be calculated
     	 

     	diff1 = abs(total1 - 12);
     	diff2 = abs(total2 - 12);
     	diff3 = abs(total3 - 12);

     	//Find the minimum amongst the three
     	
     	printf("First player score : %d\n",diff1);
     	printf("Second player score : %d\n",diff2);
     	printf("Third player score : %d\n",diff3);

     	if(diff1<=diff2 && diff1<=diff3){
     		printf("Player1 Wins!");
     		if(diff1<=9){
     			win[0] = 1;
     		}    		
     		
     	
     	}

     	if(diff2<=diff1 && diff2<=diff3){
     		printf("Player2 Wins!\n");
     		if(diff2<=9){
     			win[1] = 1;
     		}
     	}
     	if(diff3<=diff1 && diff3<=diff2){
     		printf("Player3 Wins!\n");
     		if(diff3<=9){
     			win[2] = 1;
     		}
     	}


     	for(b=0;b<3;b++){
     		if(win[b]==1){
     			//This player wins
     			strcpy(buffer,"You win!\n");
     			int nwritten;
     			if (BUFFER_SIZE != (nwritten = write(clientFd[b], buffer, BUFFER_SIZE))){
					printf("Error! Couldn't write to player \n");
					//close(clientFd[i]);
					return;
	            }


     		}
     		else{
     			//This player loses
     			strcpy(buffer,"You lose!\n");
     			int nwritten;
     			if (BUFFER_SIZE != (nwritten = write(clientFd[b], buffer, BUFFER_SIZE))){
					printf("Error! Couldn't write to player \n");
					//close(clientFd[i]);
					return;
	            }
     		}
     	}





     }




     close(clientFd[0]);
     close(clientFd[1]);
     close(clientFd[2]);





     ///////////////////////////////////////////////////////////////////////////////////////////////
     ///////////////////////////////////////////////////////////////////////////////////////////////



     //Close each connection -- Both Client and Server Connection -- 


    

	



}

int main(int argc, char *argv[]){

	int i;
	for(i=0;i<argc-1;i++){
		//Initializing the socket numbers
		clientFd[i] = atoi(argv[i+1]);		
	}

	start_game(MAX_PLAYERS);


    
    return 0;
}