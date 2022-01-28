// Write Code Here for Solution to Question 1... From Harsh's VS Studio Code 

#include<bits/stdc++.h>

using namespace std;

vector<int> extractIntegersFromLine(string str)
{
    vector<int> v;

    char *p = &str[0];

    while (*p) 
    { // While there are more characters to process...

        if ( isdigit(*p) || ( (*p=='-'||*p=='+') && isdigit(*(p+1)) )) 
        {
            // Found a number
            long val = strtol(p, &p, 10); // Read number
            //printf("%ld\n", val);        and print it.

            v.push_back(val);
        } 

        else 
        {
            // Otherwise, move on to the next character.
            p++;
        }

    }

    return v;
}

vector<vector<int>> getArrivalTime()
{
    vector<vector<int>> v;

    string line;

    ifstream inFile;

    inFile.open("arrival.txt"); // File present in same folder

    if (inFile.fail()) // File NOT Found
    {
        cerr << "Could not find the ARRIVAL file" << endl;
        exit(0);
    }


    while (true) 
    {
        if(!inFile) // reads file till end of *file*
        break;

        vector<int> f; 

        getline(inFile,line);

        f=extractIntegersFromLine(line);

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