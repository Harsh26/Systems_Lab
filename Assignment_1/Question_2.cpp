
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <semaphore.h>
#include <cstdint>
#include <stdio.h>

using namespace std;

sem_t mutex; // mutex Sempahore is binary Semaphore which controls who reads the card.

sem_t writeblock; // writeblock Sempahore is binary Semaphore which controls who writes the card.

int data_val = 0;

int rcount = 0; // Counting Semaphore for Reader Count

int track;

int numOfFriends; // No. of friends who can read/write


int string_to_int(string str) // Convert the string to integer
{
    // object from the class stringstream
    stringstream ss(str);

    int num = 0;
    ss >> num;

    return num;
}

void* write_to_card(void *arg) // Card Write [Critical Section] 
{
    int num=((intptr_t) arg); // Extracting thread number from argument

    sem_wait(&writeblock); // write block Sempahore is binary Semaphore which controls who writes.

    data_val++; // Value wrote to the card

    cout<<"\nPerson "<<num+1<<" is writing to the card.....";

    cout<<"(Only 1 writer inside Critical Section....)\n";

    sleep(3); // Sleep for some time

    cout<<"Person "<<num+1<<" Done with writing, so Exiting Critical Section.....\n";

    sem_post(&writeblock);  

    pthread_exit(NULL);

    return NULL;
}

void* read_from_card(void *arg)
{
    int num= ((intptr_t)arg); // Extracting Thread number from arguments

    sem_wait(&mutex);

    rcount++;

    if(rcount==1)
    sem_wait(&writeblock);

    //printf("\nPerson %d is reading...",track);
    cout<<"\nPerson "<<num+1<<" is reading...";
    cout<<"("<<rcount<<" Readers are inside the Critical Section...)\n";

    sem_post(&mutex);
    
    //printf("(%d Readers are inside the Critical Section.....)\n",numOfFriends - rcount); // Using printf because cout is thread unsafe function
    
    sleep(2); // Sleep for some amount of time

    sem_wait(&mutex);

    rcount--;

    if(rcount==0) // Once All Readers are out then only allow writer to write
    {
        printf("\nAll readers are done reading....\n");
        sem_post(&writeblock);
    }

    sem_post(&mutex);

    pthread_exit(NULL);

    return NULL;
}

void reader_writer(priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq,int n)
{
    pthread_t friends_reader[n];   // All friends Can read 
    pthread_t friends_writer[n];   // and write. So creating a thread for each one of them

    sem_init(&mutex,0,1);          // Setting Initial Values of 
    sem_init(&writeblock,0,1);     //    Binary Semaphore       

    int my_priority=1000; // Variable to Assign highest priority

    while(!pq.empty())
    {
        pthread_attr_t tattr;
        pthread_t tid;
        int ret;
        int newprio = my_priority--; // Decrese priority for subsequent Threads
        sched_param param;
        
        /* initialized with default attributes */
        ret = pthread_attr_init (&tattr);

        /* safe to get existing scheduling param. Gets the scheduling policy and associated scheduling parameters of thread*/
        ret = pthread_attr_getschedparam (&tattr, &param);

        /* set the priority; others are unchanged */
        param.sched_priority = newprio;

        /* setting the new scheduling param. Sets the thread scheduling parameters attribute in the thread attribute object attr to param. */
        ret = pthread_attr_setschedparam (&tattr, &param);

       
        /* with new priority specified. intptr_t  casts a pointer to int type*/
        pthread_create(&friends_writer[pq.top().second], &tattr, write_to_card, (void *) (intptr_t) pq.top().second);
        sleep(rand()%3+1);
        pthread_create (&friends_reader[pq.top().second], &tattr, read_from_card, (void *) (intptr_t) pq.top().second); 
        sleep(rand()%3+1);

        pq.pop();
    }

    for(int i=0;i<n;i++)
    {
        pthread_join(friends_writer[i],NULL);
        //pthread_join(friends_reader[i],NULL);
    }
    for(int i=0;i<n;i++)
    {
        //pthread_join(friends_writer[i],NULL);
        pthread_join(friends_reader[i],NULL);
    }

}

int main()
{
    srand(0);
    int n;

    cout<<"\n\n\nEnter Number of Friends: ";
    cin>>n;

    vector<int> couponNumbers(n);

    numOfFriends=n;

    cout<<"\n\n";
	
	int menu;
	cout<<"\nMENU";
	cout<<"\nChoose 1 for random generation of coupon code ";
	cout<<"\nChoose 2 for manual input of coupon code ";
	cout<<"\nEnter the option(1/2) : ";
	cin>>menu;
	switch(menu)
	{
	case 1:
			
			for(int i = 0; i < n; i++)
			{
	
				int x = (rand() % (999-100+1))+100;
				cout<<x<<" ";
				 vector<int> :: iterator itr;
				itr = std::find (couponNumbers.begin(), couponNumbers.end(), x);
				if(itr == couponNumbers.end())
					couponNumbers[i] = x;
				else
					i--;
			}
			break;
	
	case 2:
    for(int person = 0; person < n; person++)
    {
        string cno;
        vector<int> :: iterator itr;
		
        cout<<"Enter 3 Digit Coupon Number for person "<<person+1<<" : ";
        cin>>cno;

        if(cno.length() != 3)
        {
            cout<<"NOT a 3 Digit Number, Enter Again...\n\n";
            person--;
            continue;
        }

        int coupNum = string_to_int(cno);

        itr = std::find (couponNumbers.begin(), couponNumbers.end(), coupNum);

        if (itr != couponNumbers.end())
        {
            cout << "Duplicate Coupon Number Found !! ";

            person--;
            cout<<"Enter Again...\n\n";
            continue;
        }
        else
        {
            couponNumbers[person]=(coupNum);
            cout << "Successfully Noted person "<<person+1<<" Coupon Number...\n\n";
        }

    }
    break;

    default:
            cout<<"\n\nWrong option. Exiting...\n";
            exit(0);
}


    //Uncomment below line to see Coupon Numbers Assigned by you
    
    cout<<"\n\nFollowing are Coupon Numbers :\n";
    for(int person=0;person<n;person++)
    {
        cout<<"Coupon Number of person "<<person+1<<" = "<<couponNumbers[person]<<"\n";
    }
    

   priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;

   for(int i=0;i<n;i++)
   {
       pq.push({couponNumbers[i],i});
   }

   //Uncomment below line to see output of Priority queue
   /*cout<<"\n\nFollowing are sorted Coupon Numbers :\n";
   while(!pq.empty())
   {
       cout<<pq.top().first<<" assigned to person "<<pq.top().second+1<<"\n";
       pq.pop();
   }
   */

   reader_writer(pq,n);

   cout<<"\n\n\n"; 
   return 0;
}
