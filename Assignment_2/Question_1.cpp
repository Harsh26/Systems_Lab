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
	int flags[noOfReq] = {0},misAlloc[3] = {0};
	
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
				
				misAlloc[0]++;
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

	
	for(int i = 0; i < noOfBlocks; i++)
	{
		allocation[i] = -1;
		
	}
	for(int i = 0; i < noOfReq; i++)
	{
		flags[i] = 0;
	}
	
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
			misAlloc[1]++;
            flags[i] = 1;
           	profit[1] += 5000;
			if(requestEntries[i] == 3)
				profit[1] += 7500;
			else if(requestEntries[i] == 2)
				profit[1] += 4000;
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
			misAlloc[2]++;
            flags[i] = 1;
            profit[2] += 5000;
			if(requestEntries[i] == 3)
				profit[2] += 7500;
			else if(requestEntries[i] == 2)
				profit[2] += 4000;
        }
    }
    
    for(int i = 0; i < 3; i++)
    {
    	misAlloc[i] = noOfReq - misAlloc[i];
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
	
	//writing output on file
	ofstream outfile;
	string fileName;
	outfile.open("output_Question_1.txt");
	outfile<<"Manager Name \t\t Revenue Generated (Rs)\t\tMoney Wasted because of Nature of Manager (Rs) \t\tNo. of Request rejected because of wrong allotment of rooms\n\n";
	for(int i = 0; i < 3; i++)
    {
    	if(i == 0)
    	{
    		outfile<<"\nAman\t\t\t\t"<<profit[i]<<"\t\t\t\t"<<max - profit[i]<<"\t\t\t\t\t\t"<<misAlloc[0] - misAlloc[1];
		}
		else if(i == 1)
    	{
    		outfile<<"\nRaj\t\t\t\t"<<profit[i+1]<<"\t\t\t\t"<<max - profit[i+1]<<"\t\t\t\t\t\t"<<misAlloc[2] - misAlloc[1];
		}
		else if(i == 2)
    	{
    		outfile<<"\nAlok\t\t\t\t"<<profit[i-1]<<"\t\t\t\t"<<max - profit[i-1]<<"\t\t\t\t\t\t"<<misAlloc[1] - misAlloc[1];
		}
    }
    for(int i = 0; i < 3; i++)
    {
    	if(profit[i] == max)
    	{
			switch(i)
    		{
    			case 0 : outfile<<"\n\nAman is best manager\n\n";
    					break;
    			case 1 : outfile<<"\nAlok is best manager\n\n";
    					break;
    			case 2 : outfile<<"\nRaj is best manager\n\n";
    					break;
			}
		}
	}
    
	outfile.close();
	
	//writing output on console
	cout<<"Manager Name \t\t Revenue Generated (Rs)\t\tMoney Wasted because of Nature of Manager (Rs) \t\tNo. of Request rejected because of wrong allotment of rooms\n\n";
	for(int i = 0; i < 3; i++)
    {
    	if(i == 0)
    	{
    		cout<<"\nAman\t\t\t\t"<<profit[i]<<"\t\t\t\t"<<max - profit[i]<<"\t\t\t\t\t\t"<<misAlloc[0] - misAlloc[1];
		}
		else if(i == 1)
    	{
    		cout<<"\nRaj\t\t\t\t"<<profit[i+1]<<"\t\t\t\t"<<max - profit[i+1]<<"\t\t\t\t\t\t"<<misAlloc[2] - misAlloc[1];
		}
		else if(i == 2)
    	{
    		cout<<"\nAlok\t\t\t\t"<<profit[i-1]<<"\t\t\t\t"<<max - profit[i-1]<<"\t\t\t\t\t\t"<<misAlloc[1] - misAlloc[1];
		}
    }
    
    for(int i = 0; i < 3; i++)
    {
    	if(profit[i] == max)
    	{
			switch(i)
    		{
    			case 0 : cout<<"\n\nAman is best manager\n\n";
    					break;
    			case 1 : cout<<"\nAlok is best manager\n\n";
    					break;
    			case 2 : cout<<"\nRaj is best manager\n\n";
    					break;
			}
		}
	}
    
	return 0;
}
