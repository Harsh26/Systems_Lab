#include <iostream>
#include<vector>
#include<fstream>
#include<unordered_map>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
using namespace std;
int n1,n2, n3,noOfReq;
vector<int>requestEntries;

void readInput()
{
    ifstream infile;

	string filename;

	cout<<"\n\nEnter name of the file with .txt extension: ";
    cin>>filename;

    cout<<"\n\n";

    infile.open(filename); // File present in same folder

    if (infile.fail()) // File NOT Found
    {
        cerr << "Could not find the "<<filename<<".txt file\n" << endl;
        cerr << "HINT: You may have missed \" .txt \" extension while writing.\n\n";
        exit(0);
    }

    // Reading input...


int main() 
{
	
	int n1,n2, n3,noOfReq;
	cout<<"Enter number of Single Occupancy rooms : ";
	cin>>n1;
	cout<<"Enter number of Double Occupancy rooms : ";
	cin>>n2;
	cout<<"Enter number of Triple Occupancy rooms : ";
	cin>>n3;
	
	
	
    infile>>n1;
    infile>>n2;
    infile>>n3;
    infile>>noOfReq;

   
	char x;
	for(int i = 0; i < noOfReq; i++)
	{
		infile>>x;
		if(x == 'T')
		requestEntries.push_back(3);
		else if(x == 'D')
		requestEntries.push_back(2);
		else
		requestEntries.push_back(1);
		
	}
	infile.close();
}
int main(int argc, char** argv) {
	
	
	readInput();
	
	vector<int>blockSize;
	int profit[3] = {0},allocation[noOfReq] ;
	
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
	
	//first fit
	for(int i = 0; i < noOfReq; i++)
	{
		for(int j = 0; j < noOfBlocks; j++)
		{
			//finding first block size >= required size
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
	
	//checking number of rooms misallocated
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
	//best fit
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
	//checking number of misallocated rooms
	for(int i = 0; i < noOfReq; i++)
	{
		if(flags[i] != 1)
		{
			if(misAlloc[requestEntries[i] - 2] > 0)
			{
			misAlloc[requestEntries[i] - 2]--;
			//count[1]++;
			}
			
		} 
	}
	
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
	
	//worst fit
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
    
    //checking for misallotted rooms
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
	//finding maximum profit
    int max = INT_MIN,maxInd;
    for(int i = 0; i < 3; i++)
    {
    	if(profit[i] > max)
    	{
			max = profit[i];
    		maxInd = i;
		}
	}
	ofstream outfile;
	string fileName;
	outfile.open("output.txt");
	outfile<<"Manager Name \t\t Revenue Generated (Rs)\t\tMoney Wasted because of Nature of Manager (Rs) \t\tNo. of Request rejected because of wrong allotment of rooms\n\n";
	for(int i = 0; i < 3; i++)
    {
    	if(i == 0)
    	{
    		outfile<<"\nAman\t\t\t\t"<<profit[i]<<"\t\t\t\t"<<max - profit[i]<<"\t\t\t\t\t\t"<<count[i];
		}
		else if(i == 1)
    	{
    		outfile<<"\nRaj\t\t\t\t"<<profit[i+1]<<"\t\t\t\t"<<max - profit[i+1]<<"\t\t\t\t\t\t"<<count[i+1];
		}
		else if(i == 2)
    	{
    		outfile<<"\nAlok\t\t\t\t"<<profit[i-1]<<"\t\t\t\t"<<max - profit[i-1]<<"\t\t\t\t\t\t"<<count[i-1];
		}
    }
    switch(maxInd)
    {
    	case 0 : outfile<<"\nAman is best manager\n\n";
    			break;
    	case 1 : outfile<<"\nAlok is best manager\n\n";
    			break;
    	case 2 : outfile<<"\nRaj is best manager\n\n";
    			break;
	}
	outfile.close();
	cout<<"Manager Name \t\t Revenue Generated (Rs)\t\tMoney Wasted because of Nature of Manager (Rs) \t\tNo. of Request rejected because of wrong allotment of rooms\n\n";
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
    switch(maxInd)
    {
    	case 0 : cout<<"\nAman is best manager\n\n";
    			break;
    	case 1 : cout<<"\nAlok is best manager\n\n";
    			break;
    	case 2 : cout<<"\nRaj is best manager\n\n";
    			break;
	}
	return 0;
}
