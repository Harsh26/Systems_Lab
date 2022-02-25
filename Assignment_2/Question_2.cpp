#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

vector<int> NumSubject(3);
vector<int> BookPerTray(3);
vector<string> seq;

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
            cout<<"\nSubject Id: '"<<seq[i]<<"' is invalid as per format 'first_letter_of_course' followed by 'Unique subject number'\n\n";
            
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

int main()
{
    readInput();

    validateInput();

    return 0;
}