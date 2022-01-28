
#include<iostream>
#include<vector>
#include<fstream>
#include <ctime>
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
void PrintOutput(vector<int> arrivalTime,int burstTime[],int turnAroundTime[],int waitingTime[],int completionTime[],int temp[])
{
	double avgWaitTime = 0, avgTAT = 0; 
	cout<<endl;
	cout<<"Process ID  "<< "Burst Time  "<<"Arrival Time  " ;
	cout<<"Waiting Time  "<<"Turnaround Time  "<<"Completion Time  "<<endl;
	int n = arrivalTime.size();
    for(int i=0; i<n; i++)
    {
        cout<<"P"<<i+1<<"\t\t"<<temp[i]<<"\t\t"<<arrivalTime[i]<<"\t\t";
		cout<<waitingTime[i]<<"\t\t"<<turnAroundTime[i]<<"\t\t"<<completionTime[i]<<endl;
        avgWaitTime = avgWaitTime + waitingTime[i];
        avgTAT = avgTAT + turnAroundTime[i];
    }
    cout<<"\n\nAverage waiting time = "<<avgWaitTime / n;
    cout<<"\nAverage Turnaround time = "<<avgTAT / n<<endl;
}
void PremptiveSJF(vector<int> arrivalTime)
{

    int shortestJob = 0;
   
    cout<<"\nThe total number of Processes: ";  //input
    int n = arrivalTime.size();
    cout<<n;

    int a[n+1],burstTime[n+1],temp[n+1];
    int waitingTime[n+1],turnAroundTime[n+1],completionTime[n+1];
	
	vector<string>gantChart;
	srand(time(0));
	int ax[n+1] = {7,5,2,5,2,2,4,6,8,6};
	// assigning random burst time for all process
    for(int i = 0; i < n; i++)
    {
        //burstTime[i] = (rand() % 8) + 1; //burst time ranges from 1 to 8.
    	burstTime[i] = ax[i];
    }
    
	//copying burst time to temp array for future use
    for(int i = 0; i < n; i++)
        temp[i] = burstTime[i];

    burstTime[n] = 9;//to compare burst time of all processs to find minimum burst time
	int noOfProcessCompleted = 0, endTime = 0;

    for(int currTime = 0; noOfProcessCompleted != n; currTime++)
    {
        shortestJob = n;
       
        //findng the shortest job by comparing it with all other burst time and min arrival time
        for(int i = 0; i < n; i++)
        {
            if(arrivalTime[i] <= currTime && burstTime[i] < burstTime[shortestJob] && burstTime[i] > 0 )
                shortestJob = i;
        }
      
        burstTime[shortestJob] -= 1; // reduces burst time of process by 1 unit after each iteration of a particular process is executed
		string id = to_string(shortestJob + 1);
		string str = "P" + id;
		gantChart.push_back(str);
		
        if(burstTime[shortestJob] == 0) // if the process is exectued completely
        {
            endTime = currTime + 1;
            completionTime[shortestJob] = endTime;
            turnAroundTime[shortestJob] = endTime - arrivalTime[shortestJob];
            waitingTime[shortestJob] = turnAroundTime[shortestJob] - temp[shortestJob];
			noOfProcessCompleted++; // incrementing noOfProcessCompleted to keep track of number of process completed its execution
        	
		}
       
    }
    cout<<"\n\n";
    for(auto ele: gantChart)
    {
    	cout<<"|"<<ele;
	}
	cout<<"\n\n";
    PrintOutput(arrivalTime,burstTime,turnAroundTime,waitingTime,completionTime,temp);

}

int main()
{
    vector<vector<int>> arrival_time;

    arrival_time=getArrivalTime();

    print2DVector(arrival_time);

    for(int i=0;i<arrival_time.size()-1;i++)
    {
    	PremptiveSJF(arrival_time[i]);
		cout<<"\n\n";
    }
    return 0;
}
