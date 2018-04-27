#include"headers.h"

#define TIME_OUT 2

static pthread_t r_thrd, timer_thrd; //receive and timer threads.
static int sockfd;
static struct sockaddr_in this, receiver;
static int cur_frame;
static sem_t w;

void* read_f(void*);
void* timer_f(void*);

//sending function (on main thread)
void send_f(){

	while(1){
		sleep(1); //just to see.
		printf("sending frame %d...\n", cur_frame);
		sendto(sockfd, &cur_frame, sizeof(cur_frame),
				0, (struct sockaddr*)&receiver, sizeof(receiver));
		printf("waiting for ack.\n");
		pthread_create(&r_thrd, NULL, read_f, NULL);   //create thread to get ack.
		pthread_create(&timer_thrd, NULL, timer_f, NULL); //create thread to maintain timer.

		sem_wait(&w); //wait for either ack or timeout.
		pthread_cancel(r_thrd);
		pthread_cancel(timer_thrd);
	}
}

void* timer_f(void* args){
	sleep(TIME_OUT); //sleep for the timeout duration.
	printf("timed out.\n");
	sem_post(&w); //wake up waiting sender w/o update current frame.
}

void* read_f(void* args){
	int ack;
	recvfrom(sockfd, &ack, sizeof(ack),
			0, NULL, NULL);
	if (ack == (cur_frame+1)%2){ //if ack is valid.
		printf("valid ack recvd for frame %d\n", cur_frame);
		cur_frame = (cur_frame+1)%2; //update next frame number.
		sem_post(&w); //wake up waiting sender.
	}
}

int main(){
	//init global vars.
	cur_frame = 0;
	if (sem_init(&w, 1, 0)<0){
		perror("sem_init");
		exit(EXIT_FAILURE);
	}

	//create this socket. (UDP)
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	//check for errors.
	if(sockfd<0){perror("socket");exit(EXIT_FAILURE);}

	//initialize the receviver's socket.
	receiver.sin_port = RECV_PORT;
	receiver.sin_family = AF_INET;
	inet_aton("0.0.0.0", &receiver.sin_addr);
	
	//initialize this socket.
	this.sin_port = SEND_PORT;
	this.sin_family = AF_INET;
	inet_aton("0.0.0.0", &this.sin_addr);

	//bind this socket.
	if (bind(sockfd, (struct sockaddr*)&this, sizeof(this))<0){
		perror("bind");
		exit(EXIT_FAILURE);
	}
	send_f();
	return 0;
}
