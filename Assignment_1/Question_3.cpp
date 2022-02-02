#include <iostream>
#include<bits/stdc++.h>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
using namespace std;

int main() 
{
	
	int n, m;
	ifstream infile;

    infile.open("input.txt");
    
    if (infile.fail()) // File NOT Found
    {
        cerr << "Could not find the input file" << endl;
        exit(0);
    }
    
    infile>>n;
    infile>>m;
    cout<<n<<" "<<m<<endl;
    
	vector<int> available(m);
	int x;
	
	for(int i = 0; i < m; i++)
	{
		infile>>x;
		available.push_back(x);
	}
	
	vector<vector<int>>allocated;
	vector<vector<int>>maxReq;
	
	for(int i = 0; i < n; i++)
	{
		vector<int>temp;
		for(int j = 0; j < m; j++)
		{
			infile>>x;
			temp.push_back(x);
		}
		allocated.push_back(temp);
		//cout<<allocated.size()<<" ";
	}
	cout<<"\n";
	
	for(int i = 0; i < n; i++)
	{
		vector<int>temp;
		for(int j = 0; j < m; j++)
		{
			infile>>x;
			temp.push_back(x);
		}
		maxReq.push_back(temp);
		//cout<<maxReq.size()<<" ";
	}
	
	return 0;
}
