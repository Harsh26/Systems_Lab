#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>

int portno;
//-----------------------------------------------------------------------------------------------------------------------
void send_request(int x, int sockfd){ // Sends the request to server
	int item_code, qty, price;
	printf("\nEnter item code : ");
	scanf("%d",&item_code);
	printf("Enter quantity : ");
	scanf("%d",&qty);
	printf("Enter price : ");
	scanf("%d",&price);
	if(item_code < 1 || item_code > 10){
		printf("invalid item code\n");
	}
	else if(qty <= 0 || price < 0){
		printf("invalid qty and price\n");
	}
	else{
		char send_msg[20];
		sprintf(send_msg,"%d %d %d %d",x,item_code,qty,price);
		int n = write(sockfd,send_msg, strlen(send_msg));
		if(n < 0){
			perror("error in writing to socket");
			exit(0);
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------------
void view_status(int x,int sockfd) // Function for viewing the status of each queue
{
	int n;
	char buffer[1024];
	if(x == 3)
		n = write(sockfd,"3",1);
	else
		n = write(sockfd,"4",1);
	if(n < 0)
	{
		perror("error in writing to socket");
		exit(0);
	}

	bzero(buffer,1024);
	n = read(sockfd,buffer,1023);
	if(n < 0)
	{
		perror("error reading from socket");
		exit(0);
	}
	printf("%s",buffer);
} 
//-----------------------------------------------------------------------------------------------------------------------
void login(int sockfd) // Login function
{
	char buffer[256];
	int n;

	n = read(sockfd,buffer,255);
	if(n < 0)
	{
		perror("ERROR reading from socket");
		exit(0);
	}
		

	if(buffer[0] == '2')
	{
		perror("Max limit for client requests reached\n");
		exit(0);
	}

	printf("Enter your username: ");
	bzero(buffer,256);
	fgets(buffer,255,stdin);
	buffer[strlen(buffer) - 1] = '\0';

	n = write(sockfd,buffer,strlen(buffer));

	if (n < 0)
	{
		perror("ERROR writing to socket");
		exit(0);
	}
	bzero(buffer,256);

	n = read(sockfd,buffer,255);
	
	if (n < 0)
	{
		perror("ERROR reading from socket");
		exit(0);
	}

	if(buffer[0] == '0')
	{
		printf("invalid username\n");
		exit(0);
	}
	else if(buffer[0] == '3')
	{
		perror("This trader already logged in\n");	
		exit(0);
	}	
	else
	{
		printf("Enter the Password: ");
		bzero(buffer,256);
		fgets(buffer,255,stdin);

		buffer[strlen(buffer) - 1] = '\0';
		n = write(sockfd,buffer,strlen(buffer));

		if (n < 0)
		{
			perror("ERROR writing to socket");
			exit(0);
		}

		bzero(buffer,256);
		n = read(sockfd,buffer,255);
		if (n < 0)
		{
			perror("ERROR reading from socket");
			exit(0);
		}

		if(buffer[0] == '0')
		{
			perror("invalid password\n");
			exit(0);
		}
			
	}
}
//-----------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
	int sockfd, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];

	if (argc < 3)
	{
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}

	portno = atoi(argv[2]);
	server = gethostbyname(argv[1]);
	if(server == NULL)
	{
		fprintf(stderr, "ERROR, no such host\n");
	}
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		perror("ERROR opening socket");
		exit(0);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);

	if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0)
	{
		perror("ERROR connecting");
		exit(0);
	} 
	login(sockfd);
	printf("Logged in\n\n");
	int c;
	while(1)
	{	
		printf("\n1. To send a buy request\n");
		printf("2. To send a sell request\n");
		printf("3. To view order status\n");
		printf("4. To view your trade status\n");
		printf("Enter your option: ");
		//c = getchar();
		//getchar();
		
		scanf("%d", &c);
		
		if(c == 1) // A buy request
		{
			send_request(1,sockfd);
		}
		else if(c == 2) // A sell request
		{
			send_request(2,sockfd);
		}
		else if(c == 3) // A View status request
		{
			view_status(3,sockfd);
		}
		else if(c == 4) // A Trade status request
		{
			view_status(4,sockfd);
		}
		else
		{
			printf("enter valid option press ctrl-c to exit\n");
		}
	}
	return 0;
}
