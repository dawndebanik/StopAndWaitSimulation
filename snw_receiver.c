#include"headers.h"

#define TIME_OUT 2

static int sockfd;
static struct sockaddr_in this, sender;
static int val_frame;

void recv_f(void){
	val_frame = 0;
	int frame;
	while(1){
		int ack;
		socklen_t ssize = sizeof(sender);
		recvfrom(sockfd, &frame, sizeof(frame),
			0,(struct sockaddr*)&sender, &ssize);  //read frame from sender.
		
		printf("received frame %d.\n", frame);
		if (frame!=val_frame){ //discard frame.
		}	
		ack = (frame+1)%2;  //calculating ack.

		printf("sending ack %d.\n", ack);
		sendto(sockfd, &ack, sizeof(ack),
				0, (struct sockaddr*)&sender, ssize);
	}
}
int main(){
	//create this socket. (UDP)
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	//check for errors.
	if(sockfd<0){perror("socket");exit(EXIT_FAILURE);}

	//initialize this socket.
	this.sin_port = RECV_PORT;
	this.sin_family = AF_INET;
	inet_aton("0.0.0.0", &this.sin_addr);

	//initialize sender's socket.
	sender.sin_port = SEND_PORT;
	sender.sin_family = AF_INET;
	inet_aton("0.0.0.0", &sender.sin_addr);

	//bind this socket.
	if (bind(sockfd, (struct sockaddr*)&this, sizeof(this))<0){
		perror("bind");
		exit(EXIT_FAILURE);
	}
	
	recv_f();
	return 0;
}
