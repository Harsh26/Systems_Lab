#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <utility>
#include <map>
#include <queue>

using namespace std;

vector<int> NumSubject(3); // Store number of subject 0 --> B.tech, 1 --> M.tech, 2 --> P.hd 
vector<int> BookPerTray(3); // Stores Tray Size 0 --> B.tech, 1 --> M.tech, 2 --> P.hd
vector<string> seq; // Stores Sequence of book to be accessed

void print_input()
{
    cout<<"Following is content of file:-\n\n";

    cout<<"Number of subject B.tech - M.tech - P.hd: ";

    for(int i=0;i<NumSubject.size();i++)
    {
        cout<<NumSubject[i]<<" ";
    }
    cout<<"\n\n";

    cout<<"Number of books/tray B.tech - M.tech - P.hd: ";
    for(int i=0;i<BookPerTray.size();i++)
    {
        cout<<BookPerTray[i]<<" ";
    }
    cout<<"\n\n";

    cout<<"Sequence: ";
    for(int i=0;i<seq.size();i++)
    {
        cout<<seq[i]<<" ";
    }
    cout<<"\n\n";
}

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

    // Reading No. of Subjects...

    int nb,nm,np;

    infile>>nb;
    infile>>nm;
    infile>>np;

    NumSubject[0]=(nb); // No. of Subjects B.Tech
    NumSubject[1]=(nm); // No. of Subjects M.Tech
    NumSubject[2]=(np); // No. of Subjects P.hd

    // Reading No. of books/tray...

    int tb,tm,tp;

    infile>>tb;
    infile>>tm;
    infile>>tp;

    BookPerTray[0]=(tb); // Max no. of book/tray B.Tech
    BookPerTray[1]=(tm); // Max no. of book/tray M.Tech
    BookPerTray[2]=(tp); // Max no. of book/tray P.hd

    while(infile)
    {
        string x;

        infile>>x;

        if(x.length() > 0)
        seq.push_back(x);
    }
    
    print_input();
}

void validateInput()
{
    for(int i=0;i<3;i++)
    {
        if(NumSubject[i] < BookPerTray[i])
        {
            cout<<"\nInvalid Input, Number of Book per tray more than Number of subjects, check your file... Exiting...\n";
            exit(0);
        }
    }

    vector<string> possibleseq;

    int btech_subjects = NumSubject[0];

    int mtech_subjects = NumSubject[1];

    int phd_subjects = NumSubject[2];

    for(int i=0;i<btech_subjects;i++)
    {
        string x = "B" + to_string((i+1));

        possibleseq.push_back(x);
    }

    for(int i=0;i<mtech_subjects;i++)
    {
        string x = "M" + to_string((i+1));

        possibleseq.push_back(x);
    }

    for(int i=0;i<phd_subjects;i++)
    {
        string x = "P" + to_string((i+1));

        possibleseq.push_back(x);
    }

    for(int i=0;i<seq.size();i++)
    {
        vector<string> :: iterator itr;
        itr = std :: find(possibleseq.begin(),possibleseq.end(),seq[i]);

        if(itr == possibleseq.end())
        {
            cout<<"\nSubject Id: '"<<seq[i]<<"' is invalid as per format 'first_letter_of_course' followed by 'Subject number'\n\n";
            
            if(seq[i][0]=='B')
            cout<<"Valid subject number is from: "<<"1 to "<<btech_subjects<<" only";

            else if(seq[i][0]=='M')
            cout<<"Valid subject number is from: "<<"1 to "<<mtech_subjects<<" only";

            else if(seq[i][0]=='P')
            cout<<"Valid subject number is from: "<<"1 to "<<phd_subjects<<" only";

            else
            cout<<"Starting of subject id does NOT meet the required criteria";

            cout<<"\n\nExiting...\n";
            exit(0);
        }
        
    }

    cout<<"\nAll inputs validated...\n";
}

void B_tech(string subject,vector<string>& tray, int &fifo)
{
    vector<string> :: iterator itr;

    itr = std :: find(tray.begin(),tray.end(),subject);

    if(itr != tray.end()) // If Book found in tray, return
    return;

    for(int i=0;i<tray.size();i++) // Else, see if any vacant position
    {
        if(tray[i] == "NULL")
        {
            tray[i]=subject;
            return;
        }
    }

    for(int i=0;i<tray.size();i++) // Replace the book pointed to, by fifo
    {
        tray[fifo]=subject;

        fifo=(fifo+1)%tray.size();

        return;
    }    
}

void M_tech(string subject,vector<string>& tray,map<string,int>& mp,int c)
{
    // Find if element is present in memory or not
    auto it = find(tray.begin(), tray.end(), subject);
  
    // If element is not present
    if (it == tray.end()) 
    {
        // If memory is full
        if (tray.size() == c) 
        {
  
            // Decrease the frequency
            mp[tray[0]]--;
  
            // Remove the first element as
            // It is least frequently used
            tray.erase(tray.begin());
        }
  
        // Add the element at the end of memory
        tray.push_back(subject);
        // Increase its frequency
        mp[subject]++;
  
        }
        else 
        {
  
            // If element is present
            // Remove the element
            // And add it at the end
            // Increase its frequency
            mp[subject]++;
            tray.erase(it);
            tray.push_back(subject);
        }
  
        // Compare frequency with other pages
        // starting from the 2nd last page                 
        int k = tray.size() - 2;
  
        // Sort the pages based on their frequency 
        // And time at which they arrive
        // if frequency is same
        // then, the page arriving first must be placed first
        while (mp[tray[k]] > mp[tray[k + 1]] && k > -1) 
        {
            swap(tray[k + 1], tray[k]);
            k--;
        }
     
}

void P_hd(string subject,vector<string>& tray,vector<string> &prev,int ref)
{
    //vector<pair<string,int>> :: iterator itr;

    //itr = std :: find(tray.begin(),tray.end(),subject);

    for(int i=0;i<tray.size();i++) // If Book found in tray, return
    {
        if(tray[i] == subject)
        return;
    }
    
    for(int i=0;i<tray.size();i++) // Else, see if any vacant position
    {
        if(tray[i] == "NULL")
        {
            tray[i]=subject;
            return;
        }
    }

    for(int i=0;i<tray.size();i++) // Replace the book since tray is full
    {
        if(tray[i] == prev[ref])
        {
            tray[i]=subject;
            return;  
        }
    } 
     
}

void print_tray(vector<string> tray,char course)
{
    if(course == 'B')
    cout<<"\nCondition of B.tech tray: \n";

    else if(course == 'M')
    cout<<"\nCondition of M.tech tray: \n";

    else if(course == 'P')
    cout<<"\nCondition of P.hd tray: \n";

    for(int i=0;i<tray.size();i++)
    {
        cout<<"\t "<<" --------"<<"\n";
        cout<<"\t"<<i<<" |  "<<tray[i]<<"  |\n";
        cout<<"\t "<<" --------"<<"\n";
    }

    cout<<"\n\n";
}

void Process()
{
    vector<string> tb(BookPerTray[0],"NULL");
    vector<string> tm;//(BookPerTray[1],"NULL");
    vector<string> tp(BookPerTray[2],"NULL");

    int fifo = 0;  // For maintaining to-be-replaced book in case of B.tech

    map<string,int> lfreq; // For maintaining to-be-replaced book in case of M.tech

    for(int i=0;i<seq.size();i++)
    {
        if(seq[i][0] == 'B')
        {
            B_tech(seq[i],tb,fifo);

            cout<<"\n\nInserting: "<<seq[i];
            print_tray(tb,seq[i][0]);
        }

        else if(seq[i][0] == 'M')
        {

            M_tech(seq[i],tm,lfreq,BookPerTray[1]);

            cout<<"\n\nInserting: "<<seq[i];
            print_tray(tm,seq[i][0]);

        }

        else if(seq[i][0] == 'P')
        {
            vector<string> prev_seq(BookPerTray[2],"NULL");   // For maintaining to-be-replaced book in case of P.hd

            if(i >= BookPerTray[2])
            {
                int k=i-1;

                for(int p=0;p<prev_seq.size();p++)
                {
                    vector<string> :: iterator itr;
                    itr = std :: find(prev_seq.begin(),prev_seq.end(),seq[k]);

                    if(itr == prev_seq.end())
                    {
                        prev_seq[p]=seq[k];
                    }

                    else
                    p--;

                    k--;

                    if(k<0)
                    break;
                }

                /*cout<<"\nprev_seq: ";
                for(int p=0;p<prev_seq.size();p++)
                cout<<prev_seq[p]<<" ";
                cout<<"\n";*/

            }

            P_hd(seq[i],tp,prev_seq,prev_seq.size()-1);

            cout<<"\n\nInserting: "<<seq[i];
            print_tray(tp,seq[i][0]);
        }

    }
}


int main()
{
    readInput();

    validateInput();

    Process();

    return 0;
}