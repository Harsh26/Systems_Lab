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

void SRTF(vector<int> arrival)
{

    int i,j,smallest,count=0,time,n;
    double avg=0,tt=0,end;

    cout<<"\nThe number of Processes: ";  //input
    n=arrival.size();
    cout<<n;

    int a[n+1],b[n+1],x[n+1];
    int waiting[n+1],turnaround[n+1],completion[n+1];

    for(i=0; i<n; i++)
    {
        a[i]=arrival[i];
    }

    for(i=0; i<n; i++)
    {
        b[i]=rand()%8+1;
    }

    for(i=0; i<n; i++)
        x[i]=b[i];

    b[n]=9999;

    /*cout<<"\nhere\n";

    for(int i=0;i<n;i++)
    {
        cout<<a[i]<<"\t"<<b[i]<<"\t"<<x[i];
        cout<<"\n";
    }*/
    
    for(time=0; count!=n; time++)
    {
        smallest=n;
        for(i=0; i<n; i++)
        {
            if(a[i]<=time && b[i]<b[smallest] && b[i]>0 )
                smallest=i;
        }
        b[smallest]--;

        if(b[smallest]==0)
        {
            count++;
            end=time+1;
            completion[smallest] = end;
            waiting[smallest] = end - a[smallest] - x[smallest];
            turnaround[smallest] = end - a[smallest];
        }
    }
    cout<<"Process"<<"\t"<< "burst-time"<<"\t"<<"arrival-time" <<"\t"<<"waiting-time" <<"\t"<<"turnaround-time"<< "\t"<<"completion-time"<<endl;
    for(i=0; i<n; i++)
    {
        cout<<"p"<<i+1<<"\t\t"<<x[i]<<"\t\t"<<a[i]<<"\t\t"<<waiting[i]<<"\t\t"<<turnaround[i]<<"\t\t"<<completion[i]<<endl;
        avg = avg + waiting[i];
        tt = tt + turnaround[i];
    }
    cout<<"\n\nAverage waiting time ="<<avg/n;
    cout<<"  Average Turnaround time ="<<tt/n<<endl;

}

int main()
{
    vector<vector<int>> arrival_time;

    arrival_time=getArrivalTime();

    print2DVector(arrival_time);

    for(int i=0;i<arrival_time.size()-1;i++)
    SRTF(arrival_time[i]), cout<<"\n\n";

    return 0;
}
