#include <iostream>
#include <cstring>
#include <stdio.h>
#include <math.h>
#include <string>
#include <math.h>
#include <fstream>
#include <vector>
#include <stdlib.h>
using namespace std;

//class emp which is stored in a vector
class emp {
  public:
    int id;
    string name;
    string bio;
    int man_id;
};

/*
reads the csv file corresponding to the commas
then stores it into a vector class with it's data
*/
void readCSV(vector<emp> &emp_data) {
    ifstream data;
    string lines;
    char* ch;
    data.open("Employees.csv");

    if(!data) {
        cout << "Error: could not open file [make sure your Employees.csv file exist] (please check README.txt)" << endl;
        exit(1);
    }
    else {
        while(getline(data, lines)) { //read csv file
            emp emp_content;
            char raw_arr[lines.size()+1];
            strcpy(raw_arr, lines.c_str());
            ch = strtok(raw_arr, ",");
            int id = atoi(ch);
            ch = strtok(NULL, ",");
            string name = ch;
            ch = strtok(NULL, ",");
            string bio = ch;
            ch = strtok(NULL, ",");
            int man_id = atoi(ch);
            emp_content.id = id;
            emp_content.name = name;
            emp_content.bio = bio;
            emp_content.man_id = man_id;
            emp_data.push_back(emp_content); //save data into vector
        }
    }
}

/*
this creates a file called main_bucket.txt which
refereences child files like 1.txt 2.txt etc...
the file will follow the id then by a command 
then the child file(bucket) it belongs to
*/
int readMainBucket(int id) {
    ifstream main;
    string lines;
    char* ch;
    vector<int> uid;
    vector<int> b_location;
    int index_location = -1; //not a valid index if it's still -1
    main.open("main_bucket.txt");

    if(!main) {
        cout << "Error: could not open file [make sure you ran ./a.out C] (please check README.txt)" << endl;
        exit(1);
    }
    else {
        while(getline(main, lines)) { //read main file
            emp emp_content;
            char raw_arr[lines.size()+1];
            strcpy(raw_arr, lines.c_str());
            ch = strtok(raw_arr, ",");
            uid.push_back(atoi(ch));
            ch = strtok(NULL, ",");
            b_location.push_back(atoi(ch));
        }
    }
    for (size_t i = 0; i < uid.size(); i++) {
        if(id == uid[i]) {
            return b_location[i];
        }
    }

    if(index_location != -1) { //if -1 then we didnt find it
        return index_location;
    }
    else {
        cout << "Could not find index" << endl;
        exit(1);
    }
}

/*
reads the specific txt child file if we
cannot find the user in the child file
then the user does not exist.
*/
void readLocation(int location, int id) {
    ifstream loc;
    string lines;
    char* ch;
    vector<emp> content;
    loc.open(to_string(location) + ".txt"); //open child file

    if(!loc) {
        cout << "Error: could not open file [make sure you ran ./a.out C] (please check README.txt)" << endl;
        exit(1);
    }
    else {
        while(getline(loc, lines)) { //parse txt child file
            emp emp_content;
            char raw_arr[lines.size()+1];
            strcpy(raw_arr, lines.c_str());
            ch = strtok(raw_arr, ",");
            int id = atoi(ch);
            ch = strtok(NULL, ",");
            string name = ch;
            ch = strtok(NULL, ",");
            string bio = ch;
            ch = strtok(NULL, ",");
            int man_id = atoi(ch);
            emp_content.id = id;
            emp_content.name = name;
            emp_content.bio = bio;
            emp_content.man_id = man_id;
            content.push_back(emp_content); //save data into a vector for now
        }
    }

    for (size_t i = 0; i < content.size(); i++) { //loop through vector to find requested id
        if(id == content[i].id) {
            cout << content[i].id << endl;
            cout << content[i].name << endl;
            cout << content[i].bio << endl;
            cout << content[0].man_id;
        }
    }
}

/*
saves the buckets into txt child files
ex) 1.txt 2.txt etc... where the number
corresponds to the bucket
*/
void saveBuckets(vector<vector<emp>> &blocks) {
    ofstream mainfile("main_bucket.txt");

    for (int i = 0; i < blocks.size(); i++) {
        ofstream childfile;
        childfile.open(to_string(i) + ".txt");
        for (int j = 0; j < blocks[i].size(); j++) {
            mainfile << blocks[i][j].id << "," << i << endl;
            childfile << blocks[i][j].id << "," << blocks[i][j].name << "," << blocks[i][j].bio << "," << blocks[i][j].man_id << endl;
        }
        childfile.close();
    }
    mainfile.close();
}

/*
pop data populates the data into buckets/blocks
they get reallocated when the policy is 80%
*/
void popData(vector<emp> &emp_data, vector<vector<emp>> &blocks, int &n, int &buc, int &buc_cap) {
    blocks.resize(buc);
    float item_count = 0;

    for (size_t i = 0; i < emp_data.size(); i++) {
        if(((float)item_count / ((float)buc * (float)buc_cap)) > 0.80){ //bucket exeeds 80%
            buc++; //# of buckets inceased
            n++; //increment NEXT
            vector<vector<emp>> tmp = blocks;
            blocks.clear(); //delete our previous solution
            for (int i = 0; i < tmp.size(); i++) {
                for (int j = 0; j < tmp[i].size(); j++) {
                    blocks.resize(buc); //allocate new bucket
                    blocks[tmp[i][j].id % buc].push_back(tmp[i][j]); //change where buckets should be located
                }
            }
        }
        blocks[emp_data[i].id % buc].push_back(emp_data[i]); //add a bucket
        item_count++; //increment bucket so we can check if policy goes above 80%
    }
}


/*
get the command L or C and check if argc is right amount
*/
int main(int argc, char *argv[]) {
    //DATA
    vector<emp> emp_data; //all data is stored here
    vector<vector<emp>> blocks; //data gets stored into blocks/buckets
    int n = 0; //which buckets to split
    int buc = 2; //# of buckets
    int buc_cap = 2; //bucket capacity

    //error handling
    if(argc < 2) {
        cout << "Error: incorrect or invalid cmd line arg (please check README.txt)" << endl;
        exit(1);
    }

    //find switch C
    if(strcmp(argv[1], "C") == 0) {
        readCSV(emp_data); //we read the file and store it to vector emp_data
        popData(emp_data, blocks, n, buc, buc_cap); //we populate the data using vector named block
        saveBuckets(blocks); //save the buckets in a file
    }

    //find switch L
    if(strcmp(argv[1], "L") == 0) {
        int file_location = readMainBucket(atoi(argv[2])); //find the bucket
        readLocation(file_location, atoi(argv[2])); //find the file it resides in and print data requested
    }
    
    return 0;
}