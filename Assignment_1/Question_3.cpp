#include <iostream>
#include<fstream>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <cstdint>
#include <stdio.h>
#include <time.h>
#include <sched.h>

using namespace std;

pthread_mutex_t mutex;
pthread_cond_t cond;

int n, m,processDone;
vector<int> availRes;                //stores available resources
vector<vector<int>> allocatedRes;    //stores resource allocated to each process
vector<vector<int>> maxRes ;         //stores maximum need of resources for each process
vector<vector<int>> needRes;         //stores the amount of resource needed for each process to execute
vector<int> seq;


//reading input file
void readInput()
{
	ifstream infile;

	string filename;

	cout<<"\n\nEnter name of the arrival file with .txt extension: ";
    cin>>filename;

    cout<<"\n\n";

    infile.open(filename); // File present in same folder

    if (infile.fail()) // File NOT Found
    {
        cerr << "Could not find the "<<filename<<".txt file\n" << endl;
        cerr << "HINT: You may have missed \" .txt \" extension while writing.\n\n";
        exit(0);
    }
    
    infile>>n;
    infile>>m;
	int x;
	
	for(int i = 0; i < m; i++)
	{
		infile>>x;
		availRes.push_back(x);
	}
	
	for(int i = 0; i < n; i++)
	{
		vector<int>temp;
		for(int j = 0; j < m; j++)
		{
			infile>>x;
			temp.push_back(x);
		}
		allocatedRes.push_back(temp);
	}
	
	for(int i = 0; i < n; i++)
	{
		vector<int>temp;
		for(int j = 0; j < m; j++)
		{
			infile>>x;
			temp.push_back(x);
		}
		maxRes.push_back(temp);
		
	}

	for(int i = 0; i < n; ++i)
    {
    	vector<int>temp;
        for(int j = 0; j < m; ++j)
        {
            temp.push_back(maxRes[i][j] - allocatedRes[i][j]);
        }
        needRes.push_back(temp);
    }
}
void *procs(void* processID)
{     
	int pID;
	pthread_mutex_lock(&mutex);  //locking the thread
	
	for(pID = ((intptr_t)processID);pID != seq[processDone];)
	{
		pthread_mutex_unlock(&mutex);
 	        pthread_cond_wait(&cond, &mutex);  //conditional wait ie a thread waits until the previous thread which entered the critical section is executued
 	}
	

    	cout<<"\n--> Process: "<<pID+1;
    	cout<<"\n\tAllocated :  ";
    	for(int i=0; i<m; i++)
        	cout<< allocatedRes[pID][i]<<"  ";

    	cout<<"\n\t"<<"Needed    :  ";
    	for(int i=0; i<m; i++)
    		cout<<needRes[pID][i]<<"  ";

    	cout<<"\n\tAvailable :  ";
    	for(int i=0; i<m; i++)
       		cout<<availRes[i]<<"  ";

    	cout<<endl;
	sleep(1);
    	cout<<"\tResource Allocated!";
    	cout<<endl; 
	sleep(1);
    	cout<<"\tResource Released!";

	for(int i=0; i<m; i++)
        	availRes[i] += allocatedRes[pID][i];

    	cout<<"\n\tNow Available : ";
    	for(int i=0; i<m; i++)
        	cout<< availRes[i]<<"  ";
    	cout<<endl;
    	cout<<endl;
	
    	processDone++;
    	pthread_cond_broadcast(&cond);
    	pthread_mutex_unlock(&mutex);//releasing lock
	pthread_exit(NULL);
    	return NULL;
}
int checkSafe()
{
    vector<bool>completed(n,false);    //boolean array to store whether a process is completed or not
    vector<int>tempRes(availRes.begin(),availRes.end()); // temp array which stores available resources
    for(int i = 0; i < n; i++)
    {
        if (completed[i] == false)
        {
            for(int j = 0; j < m; j++)
            {
                if(needRes[i][j] <= tempRes[j]) //if available resource is sufficient to need of a process then we allocate res to it
                { 
			if(j == m-1)  
			{ 
                        	for (int k = 0; k < m; ++k)
                        	{
                            		tempRes[k] += allocatedRes[i][k];
                    		}
                    		completed[i] = true;
                    		seq.push_back(i);
				i = -1;
				break;
     			  }
		}
		else
		     break;
	    }
	}
    }
           
    for(int i = 0; i < n; i++)
    {
        if (completed[i] == false) // if any of the process is not completed then return -1
            return -1;
    }        
    return 0;
}
int main()
{   
	readInput();
	if(checkSafe() != 0)
	{
		cout<<"Safe sequence not Found\n";
		exit(0);
	}
	cout<<"\n\nSafe sequence Found : ";
	
	for(int i = 0; i < n; i++)
	{
		cout<<seq[i] + 1<<"  ";
	}
	//creating threads to allocate resources according to safe sequence found
    pthread_attr_t attrDefault;
    pthread_attr_init(&attrDefault);
    pthread_t tid[n];
    int pid[n];
    
    for(int i = 0; i < n; i++)
    {
	    pid[i] = i;//allocating process id to every threads
    }

    cout<<"\n";

    for(int i = 0; i < n; i++)
    {
        pthread_create(&tid[i], NULL, procs, (void *) (intptr_t) pid[i]);  //creating threads by passing process id to allocate resources
    }
	
    for(int i = 0; i < n; i++)
    {
       pthread_join(tid[i],NULL);  //it makes thread wait until it terminates
    }
    printf("\nAll Processes Finished\n");
    return 0;
}
