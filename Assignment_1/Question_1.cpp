// Write Code Here for Solution to Question 1... From Harsh's VS Studio Code 

#include<bits/stdc++.h>

using namespace std;

vector<vector<int>> getArrivalTime()
{
    const int nCol = 10; // read from file

    vector<vector<int>> v;

    ifstream inFile;
    inFile.open("arrival.txt");

    if (inFile.fail()) 
    {
        cerr << "Could not find the ARRIVAL file" << endl;
        exit(0);
    }

    // reads file to end of *file*, not line 

    while (!inFile.eof()) 
    {
        vector<int> f; 

        for (int i = 0; i < nCol; i++) 
        {
            int x;
            inFile >> x;
            f.push_back(x); 
        }

        v.push_back(f);
        
    }

    return v;
}

void print2DVector(vector<vector<int>> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v[i].size(); j++)
        {   
            cout << v[i][j]<<" ";
        }
        cout<<"\n";
    }

}
int main()
{
    vector<vector<int>> arrival_time;

    arrival_time=getArrivalTime();

    print2DVector(arrival_time);

    return 0;
}