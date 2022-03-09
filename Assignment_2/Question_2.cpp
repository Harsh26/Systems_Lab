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

int num_miss_btech;
int num_miss_mtech;
int num_miss_phd;

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

void B_tech(string subject,vector<string>& tray, int &fifo,map<string,int>& bookid)
{
    /*vector<string> :: iterator itr;

    itr = std :: find(tray.begin(),tray.end(),subject);

    if(itr != tray.end()) // If Book found in tray, return
    {}
        return;
    }*/
    for(int i=0;i<tray.size();i++) 
    {
        if(tray[i] == subject) // If Book found in tray
        {
            bookid[subject]++; // Issue already present book, add next book id at later point in time
            return;
        }
    }

    for(int i=0;i<tray.size();i++) // Else, see if any vacant position
    {
        if(tray[i] == "NULL")
        {
            num_miss_btech++;
            tray[i]=subject;
            bookid[subject]=2; // Since tray is empty, 1st book id is brought and issued to student, 2nd book id is placed in tray
            return;
        }
    }

    for(int i=0;i<tray.size();i++) // Replace the book pointed to, by fifo
    {
        tray[fifo]=subject;

        map<string,int> :: iterator iter;
        iter = bookid.find(subject);

        if(iter == bookid.end()) // Book NOT referenced before
        {
            bookid[subject]=2; // Since book id 1 is issued to student, 2nd book id is placed in tray
        }

        else // Book WAS referenced before
        {
            bookid[subject] += 1; // Since next book id is issued to student, next to next book id is placed in tray
        }

        fifo=(fifo+1)%tray.size();

        num_miss_btech++;
        return;
    }    
}

void M_tech(string subject,vector<string>& tray,map<string,int>& mp,int c,map<string,int>& bookid)
{
    // Find if element is present in memory or not
    auto it = find(tray.begin(), tray.end(), subject);
  
    // If element is not present
    if (it == tray.end()) 
    {
        num_miss_mtech++;

        bool flag=false;
        // If memory is full
        if (tray.size() == c) 
        {
            // Decrease the frequency
            mp[tray[0]]--;
  
            // Remove the first element as
            // It is least frequently used
            tray.erase(tray.begin());

            flag=true;
        }
  
        // Add the element at the end of memory
        tray.push_back(subject);
        // Increase its frequency
        mp[subject]++;

        if(!flag)// First time reference [Compulsory Miss]
        {
            bookid[subject]=2; // Since tray is empty, 1st book id is brought and issued to student, 2nd book id is placed in tray
        } 

        else
        {
            map<string,int> :: iterator iter;
            iter = bookid.find(subject);

            if(iter == bookid.end()) // Book NOT referenced before
            {
                bookid[subject]=2; // Since book id 1 is issued to student, 2nd book id is placed in tray
            }

            else // Book WAS referenced before
            {
                bookid[subject] += 1; // Since next book id is issued to student, next to next book id is placed in tray
            }
            
        }
  
    }
    else 
    {
        // If element is present
        bookid[subject]++; // Issue already present book, add next book id at later point in time

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
    while (k > -1 && mp[tray[k]] > mp[tray[k + 1]]) 
    {
        swap(tray[k + 1], tray[k]);
        k--;
    }
    
}

void P_hd(string subject,vector<string>& tray,vector<string> &prev,int ref,map<string,int>& bookid)
{
    //vector<pair<string,int>> :: iterator itr;

    //itr = std :: find(tray.begin(),tray.end(),subject);

    for(int i=0;i<tray.size();i++) // If Book found in tray, return
    {
        if(tray[i] == subject)
        {
            bookid[subject]++; // Issue already present book, add next book id at later point in time
            return;
        }
    }
    
    for(int i=0;i<tray.size();i++) // Else, see if any vacant position
    {
        if(tray[i] == "NULL")
        {
            num_miss_phd++;
            tray[i]=subject;
            bookid[subject]=2; // Since tray is empty, 1st book id is brought and issued to student, 2nd book id is placed in tray
            return;
        }
    }

    for(int i=0;i<tray.size();i++) // Replace the book since tray is full
    {
        if(tray[i] == prev[ref])
        {
            tray[i]=subject;

            map<string,int> :: iterator iter;
            iter = bookid.find(subject);

            if(iter == bookid.end()) // Book NOT referenced before
            {
                bookid[subject]=2; // Since book id 1 is issued to student, 2nd book id is placed in tray
            }

            else // Book WAS referenced before
            {
                bookid[subject] += 1; // Since next book id is issued to student, next to next book id is placed in tray
            }
            num_miss_phd++;
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

void print_tray_bookid(vector<string> tray,char course,map<string,int> bookid)
{
    if(tray.size()==0)
    return;

    if(tray[0]=="NULL")
    return;

    ofstream outfile;

    outfile.open("output.txt", std::ios_base::app); // append instead of overwrite 

    if(course == 'B')
    {
        cout<<"\nBook-ID, Subject-ID at end of simulation for B.tech: \n";
        outfile << "\nBook-ID, Subject-ID at end of simulation for B.tech: \n";
    }

    else if(course == 'M')
    {
        cout<<"\nBook-ID, Subject-ID at end of simulation for M.tech: \n";
        outfile << "\nBook-ID, Subject-ID at end of simulation for M.tech: \n";
    }

    else if(course == 'P')
    {
        cout<<"\nBook-ID, Subject-ID at end of simulation for P.hd: \n";
        outfile << "\nBook-ID, Subject-ID at end of simulation for P.hd: \n";
    }

    for(int i=0;i<tray.size();i++)
    {
        cout<<"\t "<<" --------"<<"\n";
        outfile << "\t "<<" --------"<<"\n";

        cout<<"\t"<<i<<" |  "<<tray[i]<<"  |";
        outfile << "\t"<<i<<" |  "<<tray[i]<<"  |";

        auto itr=bookid.find(tray[i]);
        cout<<" Book id: "<<(*itr).second<<"\n";

        outfile <<" Book id: "<<(*itr).second<<"\n";
    
        //cout<<"\t "<<" --------"<<"\n";
    }

    cout<<"\n\n";
    outfile << "\n\n";

    outfile.close();
}

vector<string> mostFrequent(vector<string> arr, int n)
{
    vector<string> mfq;

    // Insert all elements in hash.
    unordered_map<string, int> hash;
    for (int i = 0; i < n; i++)
        hash[arr[i]]++;
 
    // find the max frequency
    int max_count = 0;

    vector<string> res;

    for (auto i : hash) 
    {
        if (max_count < i.second) 
        {
            max_count = i.second;
        }
    }

    for(auto i : hash)
    {
        if(max_count == i.second)
        res.push_back(i.first);
    }
 
    return res;
}

void Process()
{
    vector<string> seq_btech;
    vector<string> seq_mtech;
    vector<string> seq_phd;

    num_miss_btech = 0;
    num_miss_mtech = 0;
    num_miss_phd = 0;

    vector<string> tb(BookPerTray[0],"NULL");
    vector<string> tm;//(BookPerTray[1],"NULL");
    vector<string> tp(BookPerTray[2],"NULL");

    int fifo = 0;  // For maintaining to-be-replaced book in case of B.tech

    map<string,int> lfreq; // For maintaining to-be-replaced book in case of M.tech

    map<string,int> bookid_btech;
    map<string,int> bookid_mtech;
    map<string,int> bookid_phd;

    for(int i=0;i<seq.size();i++)
    {
        if(seq[i][0] == 'B')
        {
            seq_btech.push_back(seq[i]);

            B_tech(seq[i],tb,fifo,bookid_btech);

            cout<<"\n\nInserting: "<<seq[i];
            print_tray(tb,seq[i][0]);
        }

        else if(seq[i][0] == 'M')
        {
            seq_mtech.push_back(seq[i]);

            M_tech(seq[i],tm,lfreq,BookPerTray[1],bookid_mtech);

            cout<<"\n\nInserting: "<<seq[i];
            print_tray(tm,seq[i][0]);

        }

        else if(seq[i][0] == 'P')
        {
            seq_phd.push_back(seq[i]);

            vector<string> prev_seq(BookPerTray[2],"NULL");   // For maintaining to-be-replaced book in case of P.hd

            if(i >= BookPerTray[2])
            {
                int k=i-1;

                for(int p=0;p<prev_seq.size();p++)
                {
                    vector<string> :: iterator itr;
                    itr = std :: find(prev_seq.begin(),prev_seq.end(),seq[k]);

                    if(itr == prev_seq.end() && seq[k][0] == 'P')
                    {
                        prev_seq[p]=seq[k];
                    }

                    else
                    p--;

                    k--;

                    if(k<0)
                    break;
                }

                cout<<"\nprev_seq: ";
                for(int p=0;p<prev_seq.size();p++)
                cout<<prev_seq[p]<<" ";
                cout<<"\n";

            }

            P_hd(seq[i],tp,prev_seq,prev_seq.size()-1,bookid_phd);

            cout<<"\n\nInserting: "<<seq[i];
            print_tray(tp,seq[i][0]);
        }

    }

    vector<string> id_max_num_of_book_btech = mostFrequent(seq_btech,seq_btech.size());
    vector<string> id_max_num_of_book_mtech = mostFrequent(seq_mtech,seq_mtech.size());
    vector<string> id_max_num_of_book_phd = mostFrequent(seq_phd,seq_phd.size());

    cout<<"\n\n*****            Output              *****\n\n";

    ofstream MyFile("output.txt");

    cout<<"\nNumber of times Librarian has to search entire Library depo: "<<num_miss_btech+num_miss_mtech+num_miss_phd<<endl;
    MyFile << "\nNumber of times Librarian has to search entire Library depo: "<<num_miss_btech+num_miss_mtech+num_miss_phd<<endl;
    
    cout<<"\nNum miss btech="<<num_miss_btech;
    cout<<"\nNum miss mtech="<<num_miss_mtech;
    cout<<"\nNum miss phd="<<num_miss_phd;

    cout<<"\n\n";
    MyFile << "\n\n";

    cout<<"\nSubject id for which max no. of book issued B.tech: ";
    MyFile << "\nSubject id for which max no. of book issued B.tech: ";

    for(auto iter : id_max_num_of_book_btech)
    {
        cout<<iter<<" ";
        MyFile << iter <<" ";
    }

    cout<<"\nSubject id for which max no. of book issued M.tech: ";
    MyFile << "\nSubject id for which max no. of book issued M.tech: ";

    for(auto iter : id_max_num_of_book_mtech)
    {
        cout<<iter<<" ";
        MyFile << iter <<" ";
    }

    cout<<"\nSubject id for which max no. of book issued P.hd: ";
    MyFile << "\nSubject id for which max no. of book issued P.hd: ";

    for(auto iter : id_max_num_of_book_phd)
    {
        cout<<iter<<" ";
        MyFile << iter <<" ";
    }

    cout<<"\n\n";
    MyFile<<"\n\n";

    // Close the file
    MyFile.close();
    
    print_tray_bookid(tb,'B',bookid_btech);

    print_tray_bookid(tm,'M',bookid_mtech);

    print_tray_bookid(tp,'P',bookid_phd);

}


int main()
{
    readInput();

    validateInput();

    Process();

    return 0;
}