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
vector<int> availRes;
vector<vector<int>> allocatedRes; 
vector<vector<int>> maxRes ;
vector<vector<int>> needRes;
vector<int> seq;

void readInput()
{
	ifstream infile;
    infile.open("myip4.txt");
    
    if (infile.fail()) // File NOT Found
    {
        cerr << "Could not find the input file" << endl;
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
	int pID= ((intptr_t)processID); 
	pthread_mutex_lock(&mutex);
	
	while(pID != seq[processDone])
	{
		pthread_mutex_unlock(&mutex);
 	    pthread_cond_wait(&cond, &mutex);
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
    pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
    return NULL;
}
int checkSafe()
{
    vector<bool>completed(n,false); 
    vector<int>tempRes(availRes.begin(),availRes.end());
    for(int i = 0; i < n; i++)
    {
        if (completed[i] == false)
        {
            for(int j = 0; j < m; j++)
            {
                if(needRes[i][j] <= tempRes[j])
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
        if (completed[i] == false)
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

    pthread_attr_t attrDefault;
    pthread_attr_init(&attrDefault);
    pthread_t tid[n];
	int pid[n];
    
    for(int i = 0; i < n; i++)
    {
	    pid[i] = i;
	}

	cout<<"\n";

	for(int i = 0; i < n; i++)
    {
        pthread_create(&tid[i], NULL, procs, (void *) (intptr_t) pid[i]);
	}
	
	for(int i = 0; i < n; i++)
    {
	    pthread_join(tid[i],NULL);
	}
	printf("\nAll Processes Finished\n");
    return 0;
}
