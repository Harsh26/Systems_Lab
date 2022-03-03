#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <utility>

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

void M_tech(string subject,vector<string>& tray)
{
    for(int i=0;i<tray.size();i++)
    {
        
    }   
}

void P_hd(string subject,vector<pair<string,int>>& tray,int &lru)
{
    //vector<pair<string,int>> :: iterator itr;

    //itr = std :: find(tray.begin(),tray.end(),subject);

    for(int i=0;i<tray.size();i++)
    {
        if(tray[i].first == subject)
        {
            tray[i].second += 1;
            return;
        }
    }
    
    for(int i=0;i<tray.size();i++) // Else, see if any vacant position
    {
        if(tray[i].first == "NULL")
        {
            tray[i].first=subject;
            tray[i].second=0;
            return;
        }
    }

    int min_index;
    int min_val=INT_MAX;

    for(int i=0;i<tray.size();i++) // Replace the book since tray is full
    {
        if(min_val > tray[i].second)
        {
            min_val = tray[i].second;
            min_index = i;
        }
    } 

    tray[min_index]={subject,tray[min_index].second+1};
    return;    
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
    int fifo = 0;  // For maintaining to-be-replaced book in case of B.tech

    int lfreq = 0; // For maintaining to-be-replaced book in case of M.tech

    int lru = 0;   // For maintaining to-be-replaced book in case of P.hd

    vector<string> tb(BookPerTray[0],"NULL");
    vector<string> tm(BookPerTray[1],"NULL");
    vector<pair<string,int>> tp(BookPerTray[2],make_pair("NULL",lru));

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
            M_tech(seq[i],tm);
        }

        else if(seq[i][0] == 'P')
        {
            P_hd(seq[i],tp,lru);

            vector<string> phd_tray(BookPerTray[2]);

            int tray_index=0;
            for(auto itr : tp)
            phd_tray[tray_index++]=itr.first+" "+to_string(itr.second);

            cout<<"\n\nInserting: "<<seq[i];
            print_tray(phd_tray,seq[i][0]);
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