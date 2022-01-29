
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

int string_to_int(string str)
{
    // object from the class stringstream
    stringstream ss(str);

    int num = 0;
    ss >> num;

    return num;
}

int main()
{
    int n;

    cout<<"\n\n\nEnter Number of Friends: ";
    cin>>n;

    vector<int> couponNumbers(n);

    cout<<"\n\n";

    for(int person=0;person<n;person++)
    {
        string cno;
        vector<int> :: iterator itr;

        cout<<"Enter 3 Digit Coupon Number for person "<<person+1<<" : ";
        cin>>cno;

        if(cno.length() != 3)
        {
            cout<<"NOT a 3 Digit Number, Enter Again...\n\n";
            person--;
            continue;
        }

        int coupNum = string_to_int(cno);

        itr = std::find (couponNumbers.begin(), couponNumbers.end(), coupNum);

        if (itr != couponNumbers.end())
        {
            cout << "Duplicate Coupon Number Found !! ";

            person--;
            cout<<"Enter Again...\n\n";
            continue;
        }
        else
        {
            couponNumbers[person]=(coupNum);
            cout << "Successfully Noted person "<<person+1<<" Coupon Number...\n\n";
        }

    }

    //Uncomment below line to see Coupon Numbers Assigned by you
    /*
    cout<<"\n\nFollowing are Coupon Numbers :\n";

    for(int person=0;person<n;person++)
    {
        cout<<"Coupon Number of person "<<person+1<<" = "<<couponNumbers[person]<<"\n";
    }
    */

   priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;

   for(int i=0;i<n;i++)
   {
       pq.push({couponNumbers[i],i});
   }

   //Uncomment below line to see output of Priority queue
   /*cout<<"\n\nFollowing are sorted Coupon Numbers :\n";
   while(!pq.empty())
   {
       cout<<pq.top().first<<" assigned to person "<<pq.top().second+1<<"\n";
       pq.pop();
   }
   */

  
   return 0;
}