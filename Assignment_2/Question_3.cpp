#include <iostream>
#include <vector>
#include <cstring>
#include <stdio.h>
#include <cstdio>
#include <math.h>
#include <bits/stdc++.h>
using namespace std;
int contentStartInd = 0;
class Disk
{
  private:
  // data structure to store Inode information
    struct Inode
	{
        int number;
        map<int,string> block;
    };
    
    unordered_map<string,struct Inode> virtualFileSys;
    //to check whether the file exists or not
    bool exists(string fileName)
	{
        for(auto it : virtualFileSys)
        {
            if(it.first == fileName)
			 return true;
		}
        return false;
    }

  public:
   
   //creating a file
    void mf(string fileName, string content)
	{
        if(exists(fileName))
		{
            cout << "File already exists!" << endl;
            return;
        }
        string fName = fileName + ".txt";
		ofstream out;
		out.open(fName);
		

		int noOfBlocks = ceil(content.length() * 1.0 / 4.0);
		int inodeNum,size = content.length();
		unordered_set<long int>set;

        Inode inode;
        
        //generate unique random number for inode;
        do
        {
        	inodeNum =rand() % (rand() % 1000000);
        	
        }while(set.find(inodeNum) != set.end());
        
        
        struct Inode t;
        set.insert(inodeNum);
        t.number = inodeNum;
        
       
        int k = 0;
        for(int i = 0; i < noOfBlocks; i++)
        {
        	//creating diskBlocks
        	ofstream out1;
        	string blockName = fileName + to_string(i) + ".txt";
        	out1.open(blockName);
        	out<<blockName<<endl;
        	
        	string temp ="";
        	for(int j = 0; j < 4 && k < size; j++)
        	{
        		
        		temp += content[k++];
			}
			t.block[i] = temp;
			out1<<temp;
			out1.close();
		}
        
		virtualFileSys[fileName] = t;
        cout << "New file Created " << fileName << endl;
      	out.close();	
	}
	//deleting a file
    void df(string fileName)
	{
        if(!exists(fileName))
		{
            cout << "File does not exist!" << endl;
            return;
        }
        //traverse through map to find the file
        for(auto it : virtualFileSys)
        {
            if(it.first == fileName)
			{
				for(auto it1 : it.second.block)
       			{
       				string blockName = fileName + to_string(it1.first)+ ".txt";
       				const char *bName = blockName.c_str();
       				int res = remove(bName);
       				if(res!=0)
        				cout<<"Unsuccessful delete"<<bName<<"\n\n";
			 		it1.second="\0";
			 	}
			 	break;
				
			}
		}
		string delName = fileName + ".txt";
		const char *fName = delName.c_str();
		
		int res = remove(fName);
        if(res!=0)
        	cout<<"Delete unsuccessful \n\n";
		virtualFileSys.erase(fileName);
        cout << "\n\nFile Deleted\n\n " << fileName << endl;
    }

    void ls()
	{
    	 
        cout << "*********************************" << endl;
        cout << "|  File Name\t|\tInode Number\t|" << endl;
        cout << "******************0***************" << endl;

        // check for every Inode which is in use
        for(auto it : virtualFileSys)
             cout << it.first<<"  \t|\t    " <<it.second.number<<  endl;
        cout << "---------------------------------" << endl;
        
    }

    void pf(string fileName)
	{
        // check if the file exists or not
        if(!exists(fileName))
		{
            cout << "File does not exist!" << endl;
            return;
        }
		
        // finding the Inode for the file
        for(auto it : virtualFileSys)
		{
            if(fileName == it.first)
			{
				struct Inode toPrint = it.second;
				int noOfBlocks = toPrint.block.size();
				//printing contents of all files
				for(int i = 0; i < noOfBlocks; i++)
				{
					string blockName = fileName + to_string(i) + ".txt";
					ifstream infile;
					infile.open(blockName);
					if(infile.is_open())
					cout<<infile.rdbuf();
					
				}
				cout<<endl;
				
                break;
            }
        }

    }
    void rf(string fileName,string newFileName)
    {
    	
    	if(!exists(fileName))
		{
            cout << "File does not exist!" << endl;
            return;
        }
    	//renaming index file
    	string fullName = fileName + ".txt";
    	string newFullName = newFileName + ".txt";
    	const char *fName = fullName.c_str();
    	const char *fNameNew = newFullName.c_str();
    	
    	
    	if(rename(fName, fNameNew) == 1)
		cout<<"Error renaming file";
		else
		cout << "File renamed successfully";
		
		
		//renaming diskBlocks presnt in index file
		for(auto it : virtualFileSys)
        {
        	
            if(it.first == fileName)
			{
				for(auto it1 : it.second.block)
       			{
       				string blockName = fileName + to_string(it1.first)+ ".txt";
       				const char *bName = blockName.c_str();
       				string newBlockName = newFileName + to_string(it1.first)+ ".txt";
       				const char *bNameNew = newBlockName.c_str();
			 	}
			 	break;
				
			}
		}
		
    	for(auto it : virtualFileSys)
		{
            if(fileName == it.first)
			{
				struct Inode newStruct = it.second;
				virtualFileSys.erase(fileName);
				virtualFileSys[newFileName] = newStruct;
                break;
            }
        }
    	
	}
};
//extracting command
string extractCommand(string s)
{
	string command ="";
    command += s[0] ;
    command += s[1];
	return command;
}
//extracting file name
string extractFileName(string s)
{
	string fileName = "";
	int i;
	for( i = 3; i < s.length(); i++)
	{
		if(s[i] != ' ')
		{
			fileName += s[i];
		}
		else
		break;
	}
	contentStartInd = i + 1;
	return fileName;
}
//extracting contents
string extractContents(string s)
{
	string contents = "";
	for(int i = contentStartInd; i < s.length(); i++)
	{
		contents += s[i];	
	}
	return contents;
}



int main()
{
    Disk disk;
    srand(time(0));
    cout<<"Enter 0 to exit the program\n\n ";
    
    while(1)
	{
    	contentStartInd = 0;
		string str;
		int size;
		cout << "\nEnter the command : ";
		getline(cin >>ws, str);
        string command, contents;
        command = extractCommand(str);
        
        if(command == "mf")
        {
        	string fileName,contents;
        	fileName = extractFileName(str);
        	contents = extractContents(str);
        	size = contents.length();
        	if(size<=0)
        	{
        		cout<<"\nInvalid input\n\n";
        		continue;
			}
        	disk.mf(fileName,contents);
        	
		}
        else if(command == "df")
        {
        	string fileName;
        	fileName = extractFileName(str);
        	disk.df(fileName);
		}
		else if(command == "rf")
        {
        	string fileName,newFileName;
        	fileName = extractFileName(str);
        	newFileName = extractContents(str);
        	if(fileName.length()<=0 || newFileName.length()<=0)
        	{
        		cout<<"\nInvalid input\n\n";
        		continue;
			}
        	disk.rf(fileName,newFileName);
		}
		else if(command == "pf")
        {
        	string fileName;
        	fileName = extractFileName(str);
        	if(fileName.length() <= 0)
        	{
        		cout<<"\nInvalid input\n\n";
        		continue;
			}
        	disk.pf(fileName);
		}
		else if(str == "ls")
        {
        	disk.ls();
		}
		else if(str =="0")
		{
			exit(0);
		}
		else
		{
			cout<<"\n\nWrong Command\n\n";
			continue;
		}
		
    }

    return 0;
}
