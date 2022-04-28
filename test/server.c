#include "string.h"
#include "server.h"
#include <unistd.h>
#include <sys/types.h>
#include "stdio.h"
#include <stdlib.h>
#include "netdb.h"
#include "netinet/in.h"



void writeSock( int Sock, char *message )
{
    int m = write( Sock, message, strlen(message) );
    if (m < 0) 
      {
         perror("ERROR writing in to the socket");
         
      }
}

char **split_data(char *buff)
{
   int l = strlen(buff), i, tokens=1;
   for(i=0; i<l; i++)
      if(buff[i]=='\n')
         tokens++;
   
   char *tokken;
   char **result;
   result = (char **)malloc(tokens * sizeof(char *));
   tokken = strtok(buff, "\n");
   i=0;
   while(tokken!=NULL)
   {
      result[i] = (char *)malloc(sizeof(char)*strlen(tokken));
      strcpy(result[i], tokken);
      i++;
      tokken = strtok(NULL, "\n");
   }
   return result;
}

// 1 means user and pass both match // 2 means correct user, wrong pass // 3 means both incorrect
int auth_Valid( char *user, char *password, int *UserNum )
{
    FILE *fp = fopen("login.txt", "r");
    char buff[50];
    *UserNum=0;
    while( fgets(  buff, sizeof(buff), fp) != NULL )
    {
        *UserNum += 1;
        char *user1 = strtok(buff, ":");
        char *pass1 = strtok(NULL, "\n");
        if( strcmp( user, user1) == 0 )
        {
            if( strcmp(password, pass1)==0)
               return 1;
            else 
               return 2;
        }
    }
    return 3;
}

// compare requests
int llessthanr(struct request l, struct request r) {
   if(l.price < r.price)
   { 
   return 1;
   }
   if(l.price == r.price)
   {
      return (l.id < r.id);
   }
   return 0;
}

// swap requests
void Swap_reqs(struct request *first, struct request *second) 
{
   struct request temp;
   temp = *second;
   *second = *first;
   *first = temp;
}

// Insert request in sorted order into the rigth queue
void sort_Insert(struct request abc) 
{
  int *A1,*A2;
   struct request *array = ( (abc.type=='B')?(buyQueue[abc.itemNumber]):(sellQueue[abc.itemNumber]) );
   if((abc.type == 'B'))
{
A1 = (buyHead + abc.itemNumber);     
A2 = (buyTail + abc.itemNumber);
}
else
{
A1 =(sell_Head + abc.itemNumber);
A2 =(sell_Tail + abc.itemNumber);
}
   //int *A1 = ( (abc.type == 'B') ? (buyHead + abc.itemNumber) : (sell_Head + abc.itemNumber) );
   //int *A2 = ( (abc.type == 'B') ? (buyTail + abc.itemNumber) : (sell_Tail + abc.itemNumber) );
   //empty queue?
   if(*A1 == *A2) {
      array[*A1] = abc;
      *A2 = (*A2 + 1)%500;
   }
   else {
      int idx = *A1, i;
      while(!llessthanr(abc, array[idx]) && idx != *A2) {
         idx = (idx+1)%500;
      }
      // insert element at idx
      // shift all elements from idx forward
      struct request temp = abc;
      for(i = idx; i != *A2; i = (i+1)%500 ) {
         Swap_reqs(array+i, &temp);
      }
      array[*A2] = temp;
      *A2 = (*A2 + 1)%500;
   }
   
}

// check is sequence of bytes is present in memory buffer
int memsubstr(const char *buffer123, int Length1234, const char *sequence) {
   int i;
   for(i=0; i+strlen(sequence) < Length1234 ; i++) {
      if(memcmp(buffer123 + i, sequence, strlen(sequence)) == 0) {
         return 1;
      }
   }
   return 0;
}


int main( int thearg, char *a12[] ) {
   
   char buffer[65536];
 char ip[] = "127.0.0.1";    
 struct sockaddr_in c_add, s_add;
   int  n;
  
   int s_1,portno, clilen, s_1new ;
   if(thearg<2)
   {
      fprintf(stderr,"usage %s port\n", a12[0]);
      exit(0);
   }

   /* First call to socket() function */
   s_1 = socket(AF_INET, SOCK_STREAM, 0);
   
   if (s_1 < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   
   /* Initialize socket structure */
   bzero((char *) &s_add, sizeof(s_add));
   portno = atoi(a12[1]);
   
   s_add.sin_family = AF_INET;
   s_add.sin_addr.s_addr = INADDR_ANY;
   s_add.sin_port = htons(portno);
   
   /* Now bind the host address using bind() call.*/
   if (bind(s_1, (struct sockaddr *) &s_add, sizeof(s_add)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
      
   /* Now start listening for the clients, here process will
      * go in sleep mode and will wait for the incoming connection
   */
   
   listen(s_1,5);
   clilen = sizeof(c_add);
   
   while(1)
   {
        /* Accept actual connection from the client */\
        s_1new = accept(s_1, (struct sockaddr *)&c_add, &clilen);
        if (s_1new < 0) {
            perror("ERROR on accept");
            continue;
        }
      
        /* If connection is established then start communicating */
        char buff[256];
        int offset = 0;
        
        while(1) {
            memset(buff, '\0', 256);
            int sz = read( s_1new, buff, 256);
            if(sz < 0) {
                printf("Error reading from socket\n");
                exit(-1);
        }
        memcpy(buffer+offset, buff, sz);
        offset += sz;
        if(memsubstr(buff, 256, "$$")) break;
    }
    buffer[offset] = '\0';
      
      
      
      
    printf("Here is the message:\n%s\n",buffer);
    char **msg = split_data(buffer);
      
      
    if(strcmp(msg[2], "LOGIN")==0)
    {
        
        int userNum;
        int res = auth_Valid(msg[0], msg[1], &userNum);
        switch( res )
        {
            case 1:     
                writeSock( s_1new, "SUCCESS\nLOG IN Successful!\n") ;
                break;
            case 2:     
                writeSock(s_1new, "FAIL\nWrong Password!!!\nTry again\n");
                break;
            case 3: 
                writeSock(s_1new, "FAIL\nWrong username!!!\nTry again\n");
                break;
        }
    }
    else if(strcmp(msg[2], "BUY")==0)
    {
        int userNum;
        int res = auth_Valid(msg[0], msg[1], &userNum);
        if(res != 1)
        {
            writeSock( s_1new, "FAIL\nLOG IN UnSuccessful!\n") ;
            close(s_1new);
            continue;
        }
        /*construct the buy request structure*/
        int i;
        int item = atoi( msg[3] );
        int qty =  atoi( msg[4] );
        int unitPrice = atoi( msg[5] );
        struct request buy;
        strcpy(buy.user, msg[0]);
        buy.itemNumber = item;
        buy.qty = qty;
        buy.price = unitPrice;
        buy.id = requestID++;
        buy.type = 'B';
        //add to the initial user request log
        userRequests[userNum][ nRequest[ userNum ]++ ] = buy;
        //check if you get at least one seller
        int check =0;
        for(i=sell_Head[item]; i != sell_Tail[item]; i = (i+1)%500)           //check for each pending sell request
        {
            if( sellQueue[item][i].price <= unitPrice )                     //if price is compatible
            {
                if( sellQueue[item][i].qty >= buy.qty )                     //if the item can be fully purchased
                {
                    check = 1;
                    sellQueue[item][i].qty -= buy.qty;                      //update the initial sell request
                    if(sellQueue[item][i].qty == 0)                         //if sell qty is zero remove the request
                        sell_Head[item] = (sell_Head[item]+1)%500;
                    
                    /*update the transaction log*/
                    
                    struct userLog tempLog; strcpy(tempLog.buyer, msg[0]); strcpy(tempLog.seller, sellQueue[item][i].user); tempLog.itemNumber = item; 
                    tempLog.price = sellQueue[item][i].price; tempLog.qty = buy.qty; tempLog.buyRequestID = buy.id; 
                    tempLog.sellRequestID = sellQueue[item][i].id; tra_Log[nTradeLog++] = tempLog;
                    break;
                }
                else                                                        //the sell qty is less than that of buy
                {
                    sell_Head[item] = (sell_Head[item]+1)%500;                //remove from sell queue
                    buy.qty -= sellQueue[item][i].qty;                      //update initial request
                    /*update the transaction log*/
                    struct userLog tempLog; strcpy(tempLog.buyer, msg[0]); strcpy(tempLog.seller, sellQueue[item][i].user); tempLog.itemNumber = item; 
                    tempLog.price = sellQueue[item][i].price; tempLog.qty = sellQueue[item][i].qty; tempLog.buyRequestID = buy.id; 
                    tempLog.sellRequestID = sellQueue[item][i].id; tra_Log[nTradeLog++] = tempLog;
                }
            }
        }
        
        //if no seller found, insert in buy queue
        if(check == 0)
            sort_Insert(buy);
        
        writeSock(s_1new, "SUCCESS\n");
    }
    else if(strcmp(msg[2], "SELL")==0)
    {
        int usernumber;
        int res = auth_Valid(msg[0], msg[1], &usernumber);
        if(res != 1)
        {
            writeSock( s_1new, "FAIL\nLOG IN UnSuccessful!\n") ;
            close(s_1new);
            continue;
        }
        //construct initial sell request
        int i;
        int item = atoi( msg[3] );
        int qty =  atoi( msg[4] );
        int unitPrice = atoi( msg[5] );
        struct request sell;
        strcpy(sell.user, msg[0]);
        sell.itemNumber = item;
        sell.qty = qty;
        sell.price = unitPrice;
        sell.id = requestID++;
        sell.type = 'S';
        //add to user request log
        userRequests[usernumber][ nRequest[ usernumber ]++ ] = sell;
        
        while( sell.qty > 0 )
        {
            if(buyHead[item] == buyTail[item])          //if no buyer available, add to sell queue
            {
                sort_Insert(sell);
                break;
            }
            int bestSell = buyHead[item];
            //find index in buy queue with highest price in FCFS manner
            for(i=buyHead[item]+1; i != buyTail[item]; i = (i+1)%500)       //check for each sell request pending
            {
                if( (buyQueue[item][i].price > buyQueue[item][bestSell].price) || ( (buyQueue[item][i].price == buyQueue[item][bestSell].price) && (buyQueue[item][i].id < buyQueue[item][bestSell].id) ) )
                    bestSell = i;
            }
            if( buyQueue[item][bestSell].price >= sell.price )              //if the price is compatible
            {
                if( buyQueue[item][bestSell].qty > sell.qty )               //sell whole amount in request
                {
                    buyQueue[item][bestSell].qty -= sell.qty;               //update the buy queue qty
                    /*construct the transaction and update*/
                    struct userLog tempLog; strcpy(tempLog.seller, msg[0]); strcpy(tempLog.buyer, buyQueue[item][bestSell].user); tempLog.itemNumber = item; 
                    tempLog.price = sell.price; tempLog.qty = sell.qty; tempLog.buyRequestID = buyQueue[item][bestSell].id; 
                    tempLog.sellRequestID = sell.id; tra_Log[nTradeLog++] = tempLog;
                    
                    sell.qty = 0;
                    break;
                }
                else                                                        //sell request still remains
                {
                    sell.qty -= buyQueue[item][bestSell].qty;               //update sell request qty
                    /*update the trransaction log*/
                    struct userLog tempLog; strcpy(tempLog.seller, msg[0]); strcpy(tempLog.buyer, buyQueue[item][bestSell].user); tempLog.itemNumber = item; 
                    tempLog.price = sell.price; tempLog.qty = buyQueue[item][bestSell].qty; tempLog.buyRequestID = buyQueue[item][bestSell].id; 
                    tempLog.sellRequestID = sell.id; tra_Log[nTradeLog++] = tempLog;
                    //remove the entry form buy queue
                    for( i=bestSell; i!=buyTail[item]; i=(i+1)%500 )
                        buyQueue[item][i] = buyQueue[item][(i+1)%500];
                    buyTail[item] = (500+buyTail[item] - 1)%500;
                }
            }
            else                                                            //price not compatible
            {
                sort_Insert(sell);                                           //add to sell queue
                break;
            }
        }
        writeSock(s_1new, "SUCCESS\n");
    }
    /*display best sell and best buy for each item*/
    else if(strcmp(msg[2], "VIEW_ORDERS")==0)
    {
        int i;
        char mesg[500] = "\0";
        for(i=0; i<10; i++)
        {
            sprintf(mesg+strlen(mesg), "Item: %d\n", i);
            strcpy(mesg+strlen(mesg), "   best sell: ");
            if( sell_Head[i] != sell_Tail[i] )
            {
                sprintf(mesg+strlen(mesg), "qty- %d",sellQueue[i][sell_Head[i]].qty );
                strcpy(mesg+strlen(mesg), ", ");
                sprintf(mesg+strlen(mesg), "price- %d", sellQueue[i][sell_Head[i]].price);
            }
            else
                strcpy(mesg+strlen(mesg), "NA");
            strcpy(mesg+strlen(mesg), "\n");
              
            strcpy(mesg+strlen(mesg), "   best Buy: ");
            if( buyHead[i] != buyTail[i] )
            {  
                sprintf(mesg+strlen(mesg), "qty- %d", buyQueue[i][(500+buyTail[i]-1)%500].qty);
                strcpy(mesg+strlen(mesg), ",");
                sprintf(mesg+strlen(mesg), "price- %d", buyQueue[i][(500 + buyTail[i]-1)%500].price);
            }
            else
                strcpy(mesg+strlen(mesg), "NA");
            strcpy(mesg+strlen(mesg), "\n");
        }
        writeSock(s_1new, "SUCCESS\n");
        writeSock(s_1new, mesg);
    }  
    else if(strcmp(msg[2], "VIEW_TRADES")==0)
    {
        int userNum;
        int result = auth_Valid(msg[0], msg[1], &userNum);
        printf("%s: %d\n", msg[0], nRequest[userNum]);
        if(result != 1)
        {
            writeSock( s_1new, "FAIL\nLOG IN UnSuccessful!\n") ;
            close(s_1new);
            continue;
        }
        char sendMsg[500];
        writeSock(s_1new, "SUCCESS\n");
        int i, j;
        for(j=0; j<nRequest[userNum]; j++)                                  //iterate in all initial requests of a given user
        {
            int getBuyID = userRequests[userNum][j].id;                     //get the buy id
            //print the initial request
            sprintf(sendMsg, "INITIAL REQUEST\n");
            writeSock(s_1new, sendMsg);
            sprintf(sendMsg, "%s %d %d %d %c %d\n", userRequests[userNum][j].user, userRequests[userNum][j].itemNumber, userRequests[userNum][j].qty, userRequests[userNum][j].price, userRequests[userNum][j].type, userRequests[userNum][j].id);
            writeSock(s_1new, sendMsg);
            sprintf(sendMsg, "(BUYER/SELLER, PRICE, ITEM, QTY, BUY_REQUEST_ID/SELL_REQUEST_ID)\n");
            writeSock(s_1new, sendMsg);
            for(i=0; i<nTradeLog; i++)                                      //check each transaction log
            {
                if(tra_Log[i].buyRequestID == getBuyID)                    //user in question is buyer
                {
                    sprintf(sendMsg, "%s %s %d %d %d %d %d\n", tra_Log[i].buyer, tra_Log[i].seller, tra_Log[i].itemNumber, tra_Log[i].qty, tra_Log[i].price, tra_Log[i].buyRequestID, tra_Log[i].sellRequestID);
                    writeSock(s_1new, sendMsg);
                }
                else if(tra_Log[i].sellRequestID == getBuyID)              //user in question is seller
                {
                    sprintf(sendMsg, "%s %s %d %d %d %d %d\n", tra_Log[i].buyer, tra_Log[i].seller, tra_Log[i].itemNumber, tra_Log[i].qty, tra_Log[i].price, tra_Log[i].buyRequestID, tra_Log[i].sellRequestID);
                    writeSock(s_1new, sendMsg);
                }
            }
        }
    }  
    close(s_1new);
   }
   return 0;