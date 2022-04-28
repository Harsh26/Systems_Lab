// This server runs forever 
#include <stdio.h>  
#include <string.h>  
#include <unistd.h>   
#include <arpa/inet.h>    
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> // For FD_SET, FD_ISSET, FD_ZERO macros
#include <stdlib.h> 
#include <errno.h> 


#define max_clients 5

//-------------------------------------------------------------------------------------------------------------------------------------------------------
struct Item // Item will contain info. about who is trader, qhat he's selling, quantity and price
{
	int trader_id;
	int quantity;
	int price;
	struct Item* next;
};

struct request // Request will contain demand details like item num/code, buy/sell 
{
	int item_number;
	struct Item* buy_request;
	struct Item* sell_request; 
};

struct request trade_array[10];
struct sockaddr_in server_address, client_address;

//-------------------------------------------------------------------------------------------------------------------------------------------------------
//All global variables

char* trade_files[] = {"trader1.txt","trader2.txt","trader3.txt","trader4.txt","trader5.txt"};
int trader_id_sd[5] = {-1,-1,-1,-1,-1}; 
int portno;
int client_length;
int master_socket; 
int serv_len;
int opt;
//-------------------------------------------------------------------------------------------------------------------------------------------------------
struct Item* getNode(int quantity, int price,int trader_id) // Makes a new node to insert in the list
{
	struct Item * new_node = (struct Item*) malloc(sizeof(struct Item));
	new_node->next = NULL;
	new_node->trader_id = trader_id;
	new_node->quantity = quantity;
	new_node->price = price;
	return new_node;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void print_list(struct Item* head) // Prints the list containing quantity and price
{
	while(head != NULL)
	{
		printf("%d,%d ",head->quantity, head->price);
		head = head->next;
	}
	printf("\n");
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void view_order_status(int sd,int trader_id) // Function to see the order status.
{
	struct Item* head;
	int n;
	char buffer[30];
	char order_str[1024];
	strcpy(order_str,"");
	int quantity,price;
	for(int i=0;i<10;i++)
	{
		sprintf(buffer,"\n%d:\n",i+1);
		strcat(order_str,buffer);
		head = trade_array[i].buy_request;
		if(head != NULL)
		{
			quantity = head->quantity;
			price = head->price;
			sprintf(buffer,"buy: ");
			strcat(order_str,buffer);
			while(head->next )
			{
			    sprintf(buffer," ( %d -> %d ) ",head->quantity,head->price);
			    strcat(order_str,buffer);
			    head = head->next;
			}
			sprintf(buffer," ( %d -> %d ) \n",head->quantity,head->price);
			strcat(order_str,buffer);
		}
		else
		{
			sprintf(buffer,"buy: \n");
			strcat(order_str,buffer);
		}

		head = trade_array[i].sell_request;
		if(head != NULL)
		{
			quantity = head->quantity;
			price = head->price;
			sprintf(buffer,"sell: ");
			strcat(order_str,buffer);
			while(head->next )
			{
				sprintf(buffer," ( %d -> %d ) ",head->quantity,head->price);
			    strcat(order_str,buffer);
			    head = head->next;
			}
			sprintf(buffer," ( %d -> %d ) \n",head->quantity,head->price);
			strcat(order_str,buffer);
		}
		else
		{
			sprintf(buffer,"sell: \n");
			strcat(order_str,buffer);
		}
	}
	n = write(sd,order_str,strlen(order_str));
	if(n < 0)
	{
		perror("Error writing to socket");
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void buy_queue_insert(int item_number,int quantity,int price,int trader_id) // Insert in buy Queue. the item code,qty, rate and trader who is buying 
{
	struct Item* newnode = getNode(quantity,price,trader_id);
	struct Item *temp = trade_array[item_number-1].buy_request;
	if(temp == NULL){
		trade_array[item_number-1].buy_request = newnode;
	}
	else{
		if(temp->price < price){
			newnode->next = temp;
			trade_array[item_number-1].buy_request = newnode;
			return;
		}
		while(temp->next!=NULL){	
			if(temp->next->price < price){
				newnode->next = temp->next;
				temp->next = newnode;
				break;
			}
			temp = temp->next;
		}
		if(temp->next == NULL)
		{
			temp->next = newnode;
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void sell_queue_insert(int item_number,int quantity,int price,int trader_id)// Insert in sell Queue. the item code,qty, rate and trader who is selling
{
	struct Item* newnode = getNode(quantity,price,trader_id);	
	struct Item *temp = trade_array[item_number-1].sell_request;
	if(temp == NULL){
		trade_array[item_number-1].sell_request = newnode;
	}
	else{
		if(temp->price > price){
			newnode->next = temp;
			trade_array[item_number-1].sell_request = newnode;
			return;
		}
		while(temp->next!=NULL){
			
			if(temp->next->price > price){
				newnode->next = temp->next;
				temp->next = newnode;
				break;
			}
			temp = temp->next;
		}
		if(temp->next == NULL){
			temp->next = newnode;
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void view_trade_status(int sd,int trader_id) // Display the trade status
{
	//int n;
	FILE* fp = fopen(trade_files[trader_id],"r");
	char buffer[40];
	char order_str[1024];
	strcpy(order_str,"\ntrade-type(item_number) trader_id ,qty ,price\n");
	while(fgets(buffer,39,fp) != NULL)
	{
		strcat(order_str,buffer);
	}
	int n = write(sd,order_str,strlen(order_str));
	if(n < 0)
		perror("Error writing to socket");
	fclose(fp);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void handle_buy_request(int item_number,int quantity,int price,int trader_id) // Driver function for buy request
{
	int trade_qt, trade_price;
	FILE* gp;
	struct Item* temp = trade_array[item_number-1].sell_request;
	FILE* fp = fopen(trade_files[trader_id],"a");
	while(quantity > 0)
	{
		temp = trade_array[item_number-1].sell_request;
		if(temp != NULL)
		{
			if(temp->price <= price)
			{
				gp = fopen(trade_files[temp->trader_id],"a");
				if(temp->quantity <= quantity)
				{
					trade_qt = (quantity < temp->quantity) ? quantity : temp->quantity;
					trade_price = (price < temp->price) ? price : temp->price;
					fprintf(fp,"buy%d:  %d,%d,%d\n",item_number,temp->trader_id+1,trade_qt,trade_price);
					fprintf(gp,"sell%d: %d,%d,%d\n",item_number,trader_id+1,trade_qt, trade_price);	
					quantity = quantity - temp->quantity;
					trade_array[item_number-1].sell_request = temp->next;
					free(temp);
				}
				else
				{
					trade_qt = (quantity < temp->quantity) ? quantity : temp->quantity;
					trade_price = (price < temp->price) ? price : temp->price;
					fprintf(fp,"buy%d:  %d,%d,%d\n",item_number,temp->trader_id+1,trade_qt,trade_price);
					fprintf(gp,"sell%d: %d,%d,%d\n",item_number,trader_id+1,trade_qt, trade_price);
					temp->quantity = temp->quantity - quantity;
					quantity = 0;
				}
				fclose(gp);
				continue;
			}
			else
			{
				buy_queue_insert(item_number,quantity,price,trader_id);
				break;
			}
		}
		else
		{
			buy_queue_insert(item_number,quantity,price,trader_id);
			break;
		}
	}
	fclose(fp);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void handle_sell_request(int item_number,int quantity,int price,int trader_id)// Driver function for sell request
{
	FILE* gp;
	int trade_qt, trade_price;
	struct Item* temp = trade_array[item_number-1].buy_request;
	FILE* fp = fopen(trade_files[trader_id],"a");
	while(quantity > 0)
	{
		temp = trade_array[item_number-1].buy_request;
		if(temp != NULL)
		{
			if(temp->price >= price)
			{
				gp = fopen(trade_files[temp->trader_id],"a");

				if(temp->quantity <= quantity)
				{
					trade_qt = (quantity < temp->quantity) ? quantity : temp->quantity;
					trade_price = (price < temp->price) ? price : temp->price;
					fprintf(fp,"sell%d: %d,%d,%d\n",item_number,temp->trader_id+1,trade_qt, trade_price);
					fprintf(gp,"buy%d: %d,%d,%d\n",item_number,trader_id+1,trade_qt, trade_price);
					quantity = quantity - temp->quantity;
					trade_array[item_number-1].buy_request = temp->next;
					free(temp);
				}
				else
				{
					trade_qt = (quantity < temp->quantity) ? quantity : temp->quantity;
					trade_price = (price < temp->price) ? price : temp->price;
					fprintf(fp,"sell%d: %d,%d,%d\n",item_number,temp->trader_id+1,trade_qt, trade_price);
					fprintf(gp,"buy%d:  %d,%d,%d\n",item_number,trader_id+1,trade_qt,trade_price);
					temp->quantity = temp->quantity - quantity;
					quantity = 0;
				}
				fclose(gp);
				continue;
			}
			else
			{
				sell_queue_insert(item_number,quantity,price,trader_id);
				break;
			}
		}
		else
		{
			sell_queue_insert(item_number,quantity,price,trader_id);
			break;
		}
	}
	fclose(fp);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------
int verifyLogin(int newsockfd,char* LoginDetails[10]) // Function to verify login details
{
	int n,i;
	char buffer[256];
	bzero(buffer,256);
	char id[64];
	char password[64];
	int flag=0;

	n = read(newsockfd,buffer,255);
	if (n < 0)
	{
		perror("ERROR reading from socket");
		exit(1);
	}

	strcpy(id,buffer);
	for(i=0;i<10;i+=2)
	{
		if(strcmp(id,LoginDetails[i]) == 0)
		{
			if(trader_id_sd[i/2] != -1)
			{
				n = write(newsockfd,"3",1);
				if(n < 0)
					perror("error writing to socket");
				return 0;
			}
			else
			{
				flag = 1;
				strcpy(password,LoginDetails[i+1]);
				break;
			}
		}
	}
	if(flag == 0)
	{
		n = write(newsockfd,"0",1);
		if (n < 0)
		{
			perror("ERROR writing to socket");
			exit(1);
		}	
		return 0;
	}	
	else
	{
		n = write(newsockfd,"1",1);
		if (n < 0)
		{
			perror("ERROR writing to socket");
			exit(1);
		}	
		bzero(buffer,256);
		n = read(newsockfd,buffer,255);
		if (n < 0)
		{
			perror("ERROR reading from socket");
			exit(1);
		}
		
		if(strcmp(buffer,password) != 0)
		{
			n = write(newsockfd,"0",1);
			if (n < 0)
			{
				perror("ERROR writing to socket");
				exit(1);
			}	
			return 0; 
		}
		else
		{
			n = write(newsockfd,"1",1);
			if (n < 0)
			{
				perror("ERROR writing to socket");
				exit(1);
			}
			trader_id_sd[i/2] = newsockfd;
			return 1;	
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void getLoginDetails(char *LoginDetails[10]) // Read login.txt file to see who are traders and their corresponding ID, passwords
{
	FILE *fp;
	int i=0;
	fp = fopen("login.txt","r");
	char id[64]; 
	char password[64];
	if(!fp)
	{
		printf("Password file does not exist\n");
		return;
	}

	while(fscanf(fp,"%[^,],%s\n",id,password) != EOF)
	{
		strcpy(LoginDetails[i++],id);
		strcpy(LoginDetails[i++],password);
	}
	fclose(fp);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void create_files() // Creating all empty trader files
{
	FILE *f;
	char files[20];
	for(int i=0;i<5;i++)
	{
		sprintf(files,"trader%d.txt",i+1);
		f=fopen(files,"w");
		fclose(f);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void socket_connection() // Socket function to check if socket made or not
{
	master_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (master_socket < 0)
	{
		perror("ERROR opening socket");
		exit(1);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void setsockopt_connection()
{
	//set master socket to allow multiple connections.
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )  
	{  
		perror("setsockopt");    
		exit(1);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void bind_connection() // Bind the IP address to socket
{
	if (bind(master_socket, (struct sockaddr *) &server_address,sizeof(server_address)) < 0)
	{
		perror("ERROR on binding");
		exit(1);
	}
	printf("Listener on port %d \n", portno);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void connections()
{
	int client_socket[5]; 
	int max_sd, activity,sd, new_socket;
	int login_done = 0;
	char buffer[256];
	int n,i,j;
	fd_set readfds;
	opt = 1;
	for (i = 0; i < max_clients; i++)  
    	{  
       	client_socket[i] = 0;  
    	}	

	socket_connection();
	setsockopt_connection();  

	bzero((char *) &server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(portno);

	bind_connection();

	char* LoginDetails[10];
	
	for (i = 0; i < 10; ++i)
	{
		LoginDetails[i] = (char *) malloc(64*sizeof(char));
	}
	
	getLoginDetails(LoginDetails);
	if (listen(master_socket, 3) < 0)  
   	 {  
       	perror("listen"); 
        	exit(1); 
    	}

   	//accept the incoming connection 
    	serv_len = sizeof(server_address);  
    	puts("Waiting for connections ...");

    	while(1)
    	{
    		//clear the socket set 
        	FD_ZERO(&readfds);  
    
		//add master socket to set 
		FD_SET(master_socket, &readfds);  
		max_sd = master_socket;  
		    
		//add child sockets to set 
		for ( i = 0 ; i < max_clients ; i++)  
		{  
		    //socket descriptor 
		    sd = client_socket[i];  
		        
		    //if valid socket descriptor then add to read list 
		    if(sd > 0)  
		        FD_SET( sd , &readfds);  
		        
		    //highest file descriptor number, need it for the select function 
		    if(sd > max_sd)  
		        max_sd = sd;  
		}

		//wait for an activity on one of the sockets , timeout is NULL since we need to wait indefinitely
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
	      
		if ((activity < 0) && (errno!=EINTR))  
		{  
		    printf("select error");  
		}  

		//If something happened on the master socket , then it is an incoming connection 

		if (FD_ISSET(master_socket, &readfds))  
		{  
		    if ((new_socket = accept(master_socket,(struct sockaddr *)&client_address, (socklen_t*)&client_length))<0)  
		    {  
		        perror("accept");  
		        exit(1);
		    }  
		    //inform user of socket number - used in send and receive commands 
		    printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(client_address.sin_addr) , ntohs(client_address.sin_port));  
		                
		    //add new socket to array of sockets 
		    for (i = 0; i < max_clients; i++)  
		    {  
		        //if position is empty 
		        if( client_socket[i] == 0)  
		        {  
		        	n = write(new_socket,"1",1);
		        	if(n < 0)
		        		perror("error in writing\n");

		        	login_done = verifyLogin(new_socket,LoginDetails);
						if(!login_done)
						{
							n = close(new_socket);
							if (n < 0) 
							perror("socket cannot be closed");
						}
						else
						{
							printf("Login Done\n");
							client_socket[i] = new_socket;  
		            	printf("Adding to list of sockets as %d\n" , i);
						}    
		            break;  
		        }  
		    } 
		    if(i == 5)
		    {
		    	n = write(new_socket,"2",1);
		    	if(n < 0)
		    		perror("Error writing\n");
		    	n = close(new_socket);
					if (n < 0) 
					perror("socket cannot be closed");
		    }
		}

		//else its some IO operation on some other socket
		for (i = 0; i < max_clients; i++)  
		{  
		    sd = client_socket[i];  
		        
		    if (FD_ISSET( sd , &readfds))  
		    {  
		        //Check if it was for closing , and also read the incoming message 
					bzero(buffer,256);
		        if ((n = read( sd , buffer, 256)) == 0)  
		        {  
		            //Somebody disconnected , get his details and print 
		            getpeername(sd , (struct sockaddr*)&client_address,(socklen_t*)&client_address);  
		           
		            printf("Host disconnected , ip %s , port %d \n",inet_ntoa(client_address.sin_addr) , ntohs(client_address.sin_port));  
		                
		            //Close the socket and mark as 0 in list for reuse 
		            close(sd);  
		            for(j = 0;j < 5;j++)
		            {
		            	if(trader_id_sd[j] == sd)
		            		trader_id_sd[j] = -1;
		            }
		            client_socket[i] = 0;  
		        }  
		            
		        //Echo back the message that came in 
		        else
		        {  
		            //set the string terminating NULL byte on the end of the data read 
		            int trader_id;
		            for(j = 0;j < 5;j++)
		            {
		            	if(trader_id_sd[j] == sd){
		            		trader_id = j;
		            		break;
		            	}
		            }
		            int option,item_number,quantity,price;
		            buffer[n] = '\0';  
		            if(buffer[0] == '1')
		            {
		            	sscanf(buffer,"%d %d %d %d",&option,&item_number,&quantity,&price);
		            	handle_buy_request(item_number,quantity,price,trader_id);
		            }
		            else  if(buffer[0] == '2')
		            {
		            	sscanf(buffer,"%d %d %d %d",&option,&item_number,&quantity,&price);
		            	handle_sell_request(item_number,quantity,price,trader_id);		
		            }
		            else  if(buffer[0] == '3')
		            {
		            	view_order_status(sd,trader_id);
		            }
		            else  if(buffer[0] == '4')
		            {
		            	view_trade_status(sd,trader_id);
		            }
		        }  
		    }
		} 
	  }
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	create_files();
	if (argc < 2) // Check if number of arguments is less than 2
	{
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}
	portno = atoi(argv[1]);
	connections();
	return 0;
}
