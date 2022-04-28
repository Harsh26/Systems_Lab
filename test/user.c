#include <stdio.h>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>


static struct sockaddr_in server;


void request(char *x,const char *f);




static char tid[201];



void new_add( const char *field,char *req) {
    strcat(req, field);
    strcat(req, "\n");
}
static char pass[201];

void path(char *req) {
    new_add(tid,req);
   new_add(pass,req);
}

void view_req(int y);

void end_req(char *req) {
    strcat(req, "$$");
}


int success(const char *res) {
    if(memcmp(res, "SUCCESS", 7) == 0) {
        return 1;
    }
    else {
        return 0;
    }
}


char *seek(char *res) {
    char *pointer = res;
    while(*pointer != '\n') pointer++;
    pointer++;
    return pointer;
}

void socket_add(int ula, const char *gh);

int buy_sell( int type, int unit_price,int item_code, int quantity) {
     char fld[32];
   
    char res[65536];
    char abc[65536];
    abc[0] = '\0';
    path(abc);
    new_add( ((type==0)?("BUY"):("SELL")),abc );
    sprintf(fld, "%d", item_code);
    new_add(fld,abc);
    sprintf(fld, "%d", quantity);
    new_add(fld,abc);
    sprintf(fld, "%d", unit_price);
    new_add(fld,abc);
    end_req(abc);

  
    request(res, abc);
   
    if(success(res)) return 1;
    else return 0;
}



int atct();
int main(int argc, char const *argv[])
{
    socket_add(atoi(argv[2]),argv[1] );

  
    while( 1 ) {
        char xyz[65536];
        printf("Trader id: ");
        scanf("%s", tid);
        printf("Password: ");
        scanf("%s", pass);

        if(atct(xyz)) {
            printf("%s", seek(xyz));
            break;
        }
        else {
            printf("%s", seek(xyz)); 
        }
    }

    printf("List of commands:\n");
    printf("orders\n");
    printf("buy\n");
    printf("sell\n");
    printf("trades\n");
    printf("help\n");

   
    while( 1 ) {
        char argument[32];
       
        printf("TRADING SYSTEM ");
        scanf("%s", argument);
        int quantity, unit_price,item_code ;
        if(strcmp(argument, "") == 0) continue;
      
else if(strcmp(argument, "sell") == 0) {
           
            printf("Enter Item code: ");
            scanf("%d", &item_code);
 	 
	    printf("Enter Unit Price: ");
            scanf("%d", &unit_price);          
 	
            printf("Enter Quantity: ");
            scanf("%d", &quantity);
           
            buy_sell(1,unit_price,item_code, quantity);
        }
  
      else if(strcmp(argument, "buy") == 0) {
            printf("Item code: ");
            scanf("%d", &item_code);
            printf("Unit Price: ");
            scanf("%d", &unit_price);            

            printf("Quantity: ");
            scanf("%d", &quantity);
            
            buy_sell(0,unit_price,item_code, quantity);
        }
        
	
	   else if(strcmp(argument, "trades") == 0) {
            view_req(1);
        }  
     
   else if(strcmp(argument, "orders") == 0) {
            view_req(0);
        }
     
        else if(strcmp(argument, "help") == 0) {
            printf("List of commands:\n");
    printf("orders\n");
    printf("buy\n");
    printf("sell\n");
    printf("trades\n");
    printf("help\n");
        }
        else {
            printf("Invalid command\n");
        }
    }

    
        
    return 0;
}



void view_req(int type) {
    char abc[65536];
    char xyz[65536];
    abc[0] = '\0';
    path(abc);
    new_add(((type==0)?("VIEW_ORDERS"):("VIEW_TRADES")),abc );
    end_req(abc);

    
    request(xyz, abc);
    
    if(!success(xyz)) {
        printf("Request Failed\n");
        return;
    }

    printf("%s", seek(xyz)); 
}





void socket_add( int port, const char *ip) {
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
}


void request(char *xyz,const char *abc) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        printf("Unable to create socket\n");
        exit(-1);
    }

    if( connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0 ) {
        printf("Unable to connect to server\n");
        exit(-1);
    }

   
    if( send(sock, abc, strlen(abc), 0) < 0 ) {
        printf("Unable to send() to server\n");
        exit(-1);
    }

   
   
    int offset = 0;
 char *buffer[2048];   
 while( 1 ) {
        int size = recv(sock, buffer, 2048, 0);
        if(size > 0) {
            memcpy(xyz + offset, buffer, size);
            offset += size;
        }
        else {
            break;
        }
    }

    xyz[offset] = '\0';

}


int atct(char *xyz) {
    char abc[65536];
    abc[0] = '\0';
    path(abc);
    new_add( "LOGIN",abc);
    end_req(abc);

    // send request
    request(xyz, abc);
    // check response
    if(success(xyz)) return 1;
    else return 0;