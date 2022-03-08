#include <iostream>
#include<vector>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
using namespace std;
int main(int argc, char** argv) {
	
	int n1,n2, n3,noOfReq;
	cout<<"Enter number of Single Occupancy rooms : ";
	cin>>n1;
	cout<<"Enter number of Double Occupancy rooms : ";
	cin>>n2;
	cout<<"Enter number of Triple Occupancy rooms : ";
	cin>>n3;
	
	
	cout<<"\nEnter number of Booking requests : ";
	cin>>noOfReq;
	cout<<endl;
	
	vector<int>requestEntries;
	vector<int>blockSize;

	int profit[3] = {0},allocation[noOfReq] ;
	char x;
	
	for(int i = 0; i < noOfReq; i++)
	{
		cout<<"Enter request entry "<<i + 1<<" : ";
		cin>>x;
		if(x == 'T' || x ==  't')
		requestEntries.push_back(3);
		else if(x == 'D' || x == 'd')
		requestEntries.push_back(2);
		else
		requestEntries.push_back(1);
		
	}
	for(int i = 0; i < n3; i++)
	{
		blockSize.push_back(3);
	}
	for(int i = 0; i < n1; i++)
	{
		blockSize.push_back(1);
	}
	for(int i = 0; i < n2; i++)
	{
		blockSize.push_back(2);
	}

	
	int noOfBlocks = n1 + n2 + n3;
	int flags[noOfReq] = {0},misAlloc[2];
	misAlloc[0] = 0;
	misAlloc[1] = 0;
	
	for(int i = 0; i < noOfBlocks; i++)
	{
		allocation[i] = -1;
	}
	for(int i = 0; i < noOfReq; i++)
	{
		for(int j = 0; j < noOfBlocks; j++)
		{
			if(allocation[j] == -1 && blockSize[j] >= requestEntries[i])
			{
				if(blockSize[j] != requestEntries[i])
					misAlloc[blockSize[j] - 2]++;
				
				allocation[j] = i;
				flags[i] = 1;
				profit[0] += 5000;
				if(requestEntries[i] == 3)
					profit[0] += 7500;
				else if(requestEntries[i] == 2)
					profit[0] += 4000;
				break;
			}
		}
	}
	int count[3];
	for(int i = 0; i <= 2; i++)
	count[i] = 0;
	
	for(int i = 0; i < noOfReq; i++)
	{
		if(flags[i] != 1)
		{
			if(misAlloc[requestEntries[i] - 2] > 0)
			{
				misAlloc[requestEntries[i] - 2]--;
				count[0]++;
			}
			
			
		} 
	}
	
/*	
	printf("\nBlock no.\tsize\t\tprocess no.\t\t");
	for(int i = 0; i < noOfBlocks; i++)
	{
		printf("\n%d\t\t%d\t\t", i+1, blockSize[i]);
		if(flags[i] == 1)
			printf("%d\t\t\t",allocation[i]+1);
		else
			printf("Not allocated");
	}
	cout<<"\nProfit by Aman  "<<profit[0]<<endl;
	cout<<"\n\n\n";*/
	
	for(int i = 0; i < noOfBlocks; i++)
	{
		allocation[i] = -1;
		
	}
	for(int i = 0; i < noOfReq; i++)
	{
		flags[i] = 0;
	}
	misAlloc[0] = 0;
	misAlloc[1] = 0;
	for(int i = 0; i < noOfReq; i++)
	{
		int bestIdx = -1;
		for(int j = 0; j < noOfBlocks; j++)
        {
            if (allocation[j] == -1 && blockSize[j] >= requestEntries[i])
            {
                if (bestIdx == -1)
                    bestIdx = j;
                else if (blockSize[bestIdx] > blockSize[j])
                    bestIdx = j;
            }

        }
        if (bestIdx != -1)
        {
            allocation[bestIdx] = i;
            if(blockSize[bestIdx] != requestEntries[i])
					misAlloc[blockSize[bestIdx] - 2]++;
            flags[i] = 1;
           	profit[1] += 5000;
			if(requestEntries[i] == 3)
				profit[1] += 7500;
			else if(requestEntries[i] == 2)
				profit[1] += 4000;
        }
	}
	for(int i = 0; i < noOfReq; i++)
	{
		if(flags[i] != 1)
		{
			if(misAlloc[requestEntries[i] - 2] > 0)
			{
			misAlloc[requestEntries[i] - 2]--;
			count[1]++;
			}
			
		} 
	}
/*	printf("\nBlock no.\tsize\t\tprocess no.");
	for(int i = 0; i < noOfBlocks; i++)
	{
		printf("\n%d\t\t%d\t\t", i+1, blockSize[i]);
		if(flags[i] == 1)
			printf("%d\t\t\t",allocation[i]+1);
		else
			printf("Not allocated");
	}
	cout<<"\nProfit by Alok  "<<profit[1]<<endl;*/
	
	for(int i = 0; i < noOfBlocks; i++)
	{
		allocation[i] = -1;
		//flags[i] = 0;
	}
	for(int i = 0; i < noOfReq; i++)
	{
		flags[i] = 0;
	}
	misAlloc[0] = 0;
	misAlloc[1] = 0;
	
	for (int i = 0; i < noOfReq; i++)
    {
        // Find the best fit block for current process
        int wstIdx = -1;
        for (int j = 0; j < noOfBlocks; j++)
        {
            if (allocation[j] == -1 && blockSize[j] >= requestEntries[i])
            {
                if (wstIdx == -1)
                    wstIdx = j;
                else if (blockSize[wstIdx] < blockSize[j])
                    wstIdx = j;
            }
        }
 
        if (wstIdx != -1)
        {
            allocation[wstIdx] = i;
            if(blockSize[wstIdx] != requestEntries[i])
					misAlloc[blockSize[wstIdx] - 2]++;
            flags[i] = 1;
            profit[2] += 5000;
			if(requestEntries[i] == 3)
				profit[2] += 7500;
			else if(requestEntries[i] == 2)
				profit[2] += 4000;
        }
    }
    for(int i = 0; i < noOfReq; i++)
	{
		if(flags[i] != 1)
		{
			if(misAlloc[requestEntries[i] - 2] > 0)
			{
			
			misAlloc[requestEntries[i] - 2]--;
			count[2]++;
			}
			
		} 
	}
	/*printf("\nBlock no.\tsize\t\tprocess no.");
	for(int i = 0; i < noOfBlocks; i++)
	{
		printf("\n%d\t\t%d\t\t", i+1, blockSize[i]);
		if(flags[i] == 1)
			printf("%d\t\t\t",allocation[i]+1);
		else
			printf("Not allocated");
	}
	cout<<"\nProfit by Raj  "<<profit[2]<<endl;*/
	
    int max = INT_MIN,maxInd;
    for(int i = 0; i < 3; i++)
    {
    	if(profit[i] > max)
    	{
			max = profit[i];
    		maxInd = i;
		}
	}
	string managers[3] = {"Aman","Alok","Raj"};
	cout<<"\n\nManager Name \t\t Revenue Generated (Rs)\t\tMoney Wasted because of Nature of Manager (Rs) \t\tNo. of Request rejected because of wrong allotment of rooms";
	for(int i = 0; i < 3; i++)
    {
    	if(i == 0)
    	{
    		cout<<"\nAman\t\t\t\t"<<profit[i]<<"\t\t\t\t"<<max - profit[i]<<"\t\t\t\t\t\t"<<count[i];
		}
		else if(i == 1)
    	{
    		cout<<"\nRaj\t\t\t\t"<<profit[i+1]<<"\t\t\t\t"<<max - profit[i+1]<<"\t\t\t\t\t\t"<<count[i+1];
		}
		else if(i == 2)
    	{
    		cout<<"\nAlok\t\t\t\t"<<profit[i-1]<<"\t\t\t\t"<<max - profit[i-1]<<"\t\t\t\t\t\t"<<count[i-1];
		}
    }
    cout<<"\n"<<managers[maxInd]<<" is the best manager\n";
	return 0;
}
