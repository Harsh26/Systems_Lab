#include <iostream>
#include <vector>
#include <cstring>
#include <stdio.h>
#include <math.h>
#include <bits/stdc++.h>
using namespace std;
int contentStartInd = 0;
class Disk{
  private:
  // data structure to store Inode information
    struct Inode{
        //string file_name;
       // int size;
        int number;
        unordered_map<int,string> block;
    }In;

  // data structure to store SuperBlock information
    /*struct SuperBlock{
        bool usedBlocks[128];
        struct Inode inodes[16];
    }superBlock;
    */
    
    unordered_map<string,struct Inode> virtualFileSys;


    //char dataBlocks[127][1024]; // 127 data blocks of size 1kB each

    // function to check whether a file is present is or not
   bool exists(string fileName){
   		cout<<"in ";
        for(auto it : virtualFileSys)
        {
        	cout<<it.first<<" ";
		
            if(it.first == fileName)
			 return true;
		}
        return false;
    }

    // function to check whether the specified number of data blocks are availble
   /* bool available(int size){
        int freeBlockCount = 0;
        for(auto block : superBlock.usedBlocks)
            if(!block) freeBlockCount++;
        return freeBlockCount >= size;
    }

    // function to find unused Inode
    int getInode(){
        for(int i=0; i<16; ++i)
            if(!superBlock.inodes[i].used) return i;
        return -1;
    }*/

  public:
   /* Disk(){
        superBlock.usedBlocks[0] = true;
        memset(dataBlocks, '\0', sizeof(dataBlocks));
    }*/

    // function to create new file
    void create(string fileName, string content){
        // refuse to create the file if size of greater than 8kB
       /* if(size > 8){
            cout << "File too large!" << endl;
            return;
        }
*/
        // check if a file with same name already exists
        if(exists(fileName))
		{
            cout << "File already exists!" << endl;
            return;
        }

        // check if specified number of data blocks are available
      /*  if(!available(size)){
            cout << "Could not create file. Specified number of data blocks are not available!" << endl;
            return;
        }*/

		int noOfBlocks = ceil(content.length() * 1.0 / 4.0);
		int inodeNum,size = content.length();
		unordered_set<long int>set;

        // get a free Inode if available
       /* int curINode = getInode();
        if(curINode == -1){
            cout << "Maximum number of supported files already exist!" << endl;
            return;
        }
*/
        // setting Inode properties
       /* strcpy(superBlock.inodes[curINode].file_name, file_name);
        superBlock.inodes[curINode].used = 1;
        superBlock.inodes[curINode].size = size;*/

        // allocate data blocks to the file
        Inode inode;
        do
        {
        	inodeNum =rand() % (rand() % 1000000);
        	
        }while(set.find(inodeNum) != set.end());
        
        
        struct Inode t;
        set.insert(inodeNum);
        t.number = inodeNum;
        
       /* for(int i=1,j=0; i<128 && j<size; ++i){
            if(!superBlock.usedBlocks[i]){
                superBlock.inodes[curINode].blockPointers[j] = i;
                superBlock.usedBlocks[i] = true;
                ++j;
            }
        }*/
        int k = 0;
        for(int i = 0; i < noOfBlocks; i++)
        {
        	string temp ="";
        	for(int j = 0; j < 4,k < size; j++)
        	{
        		
        		temp += content[k++];
			}
			t.block[i] = temp;
		}
        
		virtualFileSys[fileName] = t;
        cout << "Created new file: " << fileName << endl<<virtualFileSys[fileName].number<<endl;
    for(auto it : virtualFileSys)
        {
        	cout<<it.first<<" "<<it.second.number<<"      ";
		
		}
	}


    void deleteFile(string fileName){
        // check if the file exists
        if(!exists(fileName)){
            cout << "File does not exist!" << endl;
            return;
        }
		virtualFileSys.erase(fileName);
        // free the data blocks used by the file
       /* for(int i=0; i<16; ++i){
            // get the Inode for the file and free corresponding data blocks
            if(strcmp(file_name,superBlock.inodes[i].file_name) == 0){
                superBlock.inodes[i].used = 0;
                for(int dataBlock : superBlock.inodes[i].blockPointers){
                    memset(dataBlocks[dataBlock], '\0', 1024);  // clearing the data block
                    superBlock.usedBlocks[dataBlock] = false;  // adding to the free block lists
                }
                break;
            }
        }*/

        cout << "Deleted file " << fileName << endl;
    }

    void list(){
        cout << "---------------------------------" << endl;
        cout << "|  File Name\t|\tSize\t|" << endl;
        cout << "---------------------------------" << endl;

        // check for every Inode which is in use
        for(auto it : virtualFileSys)
             cout << it.first<<"  |    " <<it.second.number<<  endl;
        cout << "---------------------------------" << endl;
    }

    void read(string fileName){
        // check if the file exists or not
        /*if(!exists(file_name)){
            cout << "File does not exist!" << endl;
            return;
        }*/

        // finding the Inode for the file
        for(auto it : virtualFileSys)
		{
            if(fileName == it.first)
			{
                /*if(blockNum > inode.size){
                    cout << "Invalid block number!" << endl;
                    return;
                }*/
				struct Inode toPrint = it.second;
				cout<<"inode number : "<<toPrint.number;
				for(auto it1 : toPrint.block)
				{
					cout<<it1.first<<" "<<it1.second<<" ";
				}
                // copy data in the specified block to the buffer
                //strcpy(buf, virtaulFileSys[fileName]);
                break;
            }
        }

    }
    void rename(string fileName,string newFileName)
    {
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
string extractCommand(string s)
{
	string command ="";
    command += s[0] ;
    command += s[1];
	return command;
}
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
    int choice = 0;
    cout<<"enter choice: ";
    cin>>choice;
    while(choice != 9)
	{
    	contentStartInd = 0;
       /* cout << "Enter the operation that you want to perform." << endl;
        cout << " 1. Create a new file" << endl;
        cout << " 2. Delete an existing file" << endl;
        cout << " 3. Read data from a file" << endl;
        cout << " 4. Write data to a file" << endl;
        cout << " 5. View all files" << endl;
        cout << " 9. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cout << endl;
	*/

		//	char file_name[8];  // name of the file
        // memset(file_name, '\0', 8);
        //int blockNum;  // bolck number on which the read/write operation is to be performed
        //char buffer[1024];  // buffer array to read or write the data
        //memset(buffer, '\0', 1024);
        Disk disk;
		string str;
		int size;
		cout << "Enter the command : ";
		getline(cin >>ws, str);
        string command, contents;
        command = extractCommand(str);
        
        if(command == "mf")
        {
        	string fileName,contents;
        	fileName = extractFileName(str);
        	contents = extractContents(str);
        	size = contents.length();
        	disk.create(fileName,contents);
        	
		}
        else if(command == "df")
        {
        	string fileName;
        	fileName = extractFileName(str);
        	disk.deleteFile(fileName);
		}
		else if(command == "rf")
        {
        	string fileName,newFileName;
        	fileName = extractFileName(str);
        	newFileName = extractContents(str);
        	disk.rename(fileName,newFileName);
		}
		else if(command == "pf")
        {
        	string fileName;
        	fileName = extractFileName(str);
        	disk.read(fileName);
		}
		else if(command == "ls")
        {
        	disk.list();
		}
		
        
        cout<<"enter choice: ";
    	cin>>choice;
        if(choice >=9)continue;
        
        
        
        
        
        
        
        
        
       /* 
        switch(choice){
            case 1:
            	
                disk.create(file_name, contents);
                break;

            case 2:
                cout << "Enter the file name: ";
                cin >> file_name;
                disk.Delete(file_name);
                break;
            
            case 3:
                cout << "Enter the file name: ";
                cin >> file_name;
                cout << "Enter block number to read: ";
                cin >> blockNum;
                disk.read(file_name, blockNum, buffer);
                cout << "Contents of the file are: " << buffer << endl;
                break;

            case 4:
                cout << "Enter the file name: ";
                cin >> file_name;
                cout << "Enter block number to write: ";
                cin >> blockNum;
                cout << "Enter the data to be written: ";
                cin.ignore(256, '\n');
                cin.getline(buffer, 1024);
                disk.write(file_name, blockNum, buffer);
                break;

            case 5:
                disk.ls();
                break;
            
            case 9: break;

            default:
                cout << "Invalid choice!" << endl;
                break;
        }

        cout << endl;*/
    }

    return 0;
}
