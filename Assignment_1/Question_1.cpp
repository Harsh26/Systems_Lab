
#include<iostream>
#include<vector>
#include<fstream>
#include<ctime>

using namespace std;

vector<int> extractIntegersFromLine(string str) // Utility function for reading Arrival time from file and converting string to int
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

vector<vector<int>> getArrivalTime() // Returns File content of Arrival time in vector of vectors format
{
    vector<vector<int>> v;

    string line;

    ifstream inFile;

    string filename;

    cout<<"\n*****************************************************\n";

    cout<<"\n\nEnter name of the arrival file with .txt extension: ";
    cin>>filename;

    cout<<"\n\n";

    inFile.open(filename); // File present in same folder

    if (inFile.fail()) // File NOT Found
    {
        cerr << "Could not find the "<<filename<<".txt file\n" << endl;
        cerr << "HINT: You may have missed \" .txt \" extension while writing.\n\n";
        exit(0);
    }

    while (true) 
    {
        if(!inFile) // reads file till end of *file*
        break;

        vector<int> f; 

        getline(inFile,line);
        
        //cout<<line.length()<<"\n";

        f=extractIntegersFromLine(line);

        v.push_back(f);

        if(line.length()==0)
        break;
    }

    return v;
}

void print2DVector(vector<vector<int>> v) // Utility function For checking if File Read properly or not
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
    // This function Prints Turn Around, Completion and Waiting time in tabular format


	double avgWaitTime = 0, avgTAT = 0; 
	cout<<endl;
	cout<<"Process ID  "<< "Burst Time  "<<"Arrival Time  " ;
	cout<<"Waiting Time  "<<"Turnaround Time  "<<"Completion Time  "<<endl;
	int n = arrivalTime.size();
    for(int i=0; i<n; i++)
    {
        cout<<"P"<<i<<"\t\t"<<temp[i]<<"\t\t"<<arrivalTime[i]<<"\t\t";
		cout<<waitingTime[i]<<"\t\t"<<turnAroundTime[i]<<"\t\t"<<completionTime[i]<<endl;
        avgWaitTime = avgWaitTime + waitingTime[i];
        avgTAT = avgTAT + turnAroundTime[i];
    }
    cout<<"\n\nAverage waiting time = "<<avgWaitTime / n;
    cout<<"\nAverage Turnaround time = "<<avgTAT / n<<endl;
}

void PremptiveSJF(vector<int> arrivalTime,int tcnum)
{

    int shortestJob = 0;

    cout<<"Test Case Number: "<<tcnum+1;
   
    cout<<"\n\nThe total number of Processes: ";  //input
    int n = arrivalTime.size();
    cout<<n<<"\n";

    int a[n+1],burstTime[n+1],temp[n+1];
    int waitingTime[n+1],turnAroundTime[n+1],completionTime[n+1];
	
	vector<string>gantChart;
	
    // int bx[]={1,1,1};

	// assigning random burst time for all process
    for(int i = 0; i < n; i++)
    {
        burstTime[i] = (rand() % 8) + 1; //burst time ranges from 1 to 8.
    	//burstTime[i] = bx[i];
    }
    
	//copying burst time to temp array for future use
    for(int i = 0; i < n; i++)
        temp[i] = burstTime[i];

    burstTime[n] = 9;//to compare burst time of all processs to find minimum burst time
	int noOfProcessCompleted = 0, endTime = 0;

    for(int currTime = 0; noOfProcessCompleted != n; currTime++)
    {
        shortestJob = n;
       
        // Findng the shortest job by comparing with all other burst time and min arrival time
        for(int i = 0; i < n; i++)
        {
            if(arrivalTime[i] <= currTime && burstTime[i] < burstTime[shortestJob] && burstTime[i] > 0 )
                shortestJob = i;
        }
        if(burstTime[shortestJob] != 9)
        {
            burstTime[shortestJob] -= 1; // Reduces burst time of process by 1 unit after each iteration of a particular process is executed in a time Stamp
		
            string id = to_string(shortestJob);
		    string str = "P" + id;
		    gantChart.push_back(str);
        }

        else
        {
            gantChart.push_back("NULL"); // IDLE time of CPU for this timepoint
        }
		
        if(burstTime[shortestJob] == 0) // If the process is exectued completely then write Completion,Turn Around and Waiting time
        {
            endTime = currTime + 1;
            completionTime[shortestJob] = endTime;
            turnAroundTime[shortestJob] = endTime - arrivalTime[shortestJob];
            waitingTime[shortestJob] = turnAroundTime[shortestJob] - temp[shortestJob];
			noOfProcessCompleted++; // incrementing noOfProcessCompleted to keep track of number of process completed its execution
		}
       
    }
    
    PrintOutput(arrivalTime,burstTime,turnAroundTime,waitingTime,completionTime,temp);

    cout<<"\n\n";

    // Printing Gantt Chart

    cout<<"*** GANTT CHART of Above Table ***\n\n";
    for(auto ele: gantChart)
    {
        if(ele == "NULL")// IF IDLE time print Nothing
        cout<<" | ";

        else
    	cout<<" | "<<ele;
	}

	cout<<"\n\n";

    int count=0;

    string element="-";
    
    for(auto ele: gantChart)
    {
        if(ele == "NULL")
        {
            if(count != 0)
            cout<<count<<"\n";

            cout<<"No process Executed from "<<count++<<" to ";
            element=ele;
        }

        else if(element == ele)
        {
            count++;
            continue;
        }
        
        else 
    	{
            if(element!="-")
            cout<<count<<"\n";

            element=ele;
            cout<<ele<<" executed from "<<count++<<" to ";
        }
	}

    cout<<count;
	cout<<"\n\n";

}

int main()
{
    srand(time(0)); // Gives the random function a new seed

    vector<vector<int>> arrival_time; // Content to be fixed by File itself

    arrival_time=getArrivalTime();

    //print2DVector(arrival_time);

    cout<<"\n\n\n********* OUTPUT *********\n\n\n";
    cout<<"\n\n--------------------------------------------------------------------------------------------------------------------------\n\n";

    for(int i=0;i<arrival_time.size()-1;i++)
    {
    	PremptiveSJF(arrival_time[i],i);
        cout<<"\n\n--------------------------------------------------------------------------------------------------------------------------\n\n";
    }

    return 0;
}
