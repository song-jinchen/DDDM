#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <sstream>
#include <math.h>

using namespace std;

vector <string> getAtt(string filename) {
    vector <string> Att;
    ifstream infile;
    string data;
    string fname;
    fname =filename;
    infile.open(fname, ios::in);
    getline(infile, data);
    while (getline(infile, data)) {
        string tmp;
        int pos=data.find(" ");
        int dsize=data.size();
        data=data.substr(pos + 1, dsize);
        stringstream ss(data);
         while (ss >> tmp) {
            Att.push_back(tmp);
        }
    }
    infile.close();
    return Att;
}

vector <string> getQuery(string filename) {
    string filen;
    vector <string> Query;
    ifstream infile;
    string data;

    filen=filename;
    infile.open(filen, ios::in);
    while (getline(infile, data)) {
        int pos=data.find(" ");
        int datas=data.size();
        data=data.substr(pos + 1, datas);
        Query.push_back(data);
    }
    infile.close();
    
    return Query;
}


vector <vector<int>> getAcc(string filename) {
    string fileName;
    vector <vector<int>> Acc;
    ifstream infile;
    string data;
    
    fileName=filename;
    infile.open(fileName, ios::in);
    getline(infile, data);
    while (getline(infile, data)) {
        int pos=data.find(" ");
        int dataSize=data.size();
        data=data.substr(pos + 1, dataSize);
        int tmp;
        vector<int> vec;
        stringstream ss(data);
        int ag;
        while (ss >> tmp) {
            vec.push_back(tmp);
        }
        Acc.push_back(vec);
    }
    infile.close();
    return Acc;
}


int isFeatureUsed(string feature, string query) {
    vector <string> vec;
    string tmp;
    int pos;
    
    while (query.find('(') != string::npos) {
        pos=query.find('(');
        int queryf =1;
        query.replace(pos, queryf, " ");
    }
    
    while (query.find(')') != string::npos) {
        pos=query.find(')');
        int qfind =1;
        query.replace(pos, qfind, " ");
    }
    
    while (query.find('=') != string::npos) {
        pos=query.find('=');
        int queryr =1;
        query.replace(query.find('='), queryr, " ");
    }
    
    while (query.find(',') != string::npos) {
        pos=query.find(',');
        int qreplace =1;
        query.replace(pos, qreplace, " ");
    }

    stringstream ss(query);

    while (ss >> tmp) {
        vec.push_back(tmp);
    }
    int i=0;
    while (i<vec.size())
    {
        if (!!(feature == vec[i])) return 1;
        i++;
    }
    
    return 0;
}

int A_ik(int isUsed, vector<int> acc_k) {
    int acc_sum=0;
    int a_ik=0;
    int i=0;
    while (i < acc_k.size( ))
    {
        acc_sum = acc_sum + acc_k[i];
        i++;
    }
    a_ik=isUsed * acc_sum;
    return a_ik;
}

int main(int argc, const char *argv[]) {
    string att_file=argv[1];
    string query_file=argv[2];
    string acc_file=argv[3];
    vector <string> att=getAtt(argv[1]);
    vector <string> queries=getQuery(argv[2]);
    vector <vector<int>> acc=getAcc(argv[3]);
    int qyerySize = queries . size( );
    int attSize = att . size( );
    int usageMatrix[qyerySize][attSize];
    int AA[attSize][attSize];

    int i=0;
    while (i<qyerySize)
    {
        string query=queries[i];

        int j=0;
        while (j<attSize)
        {
            string feature=att[j];

            usageMatrix[i][j]=isFeatureUsed(feature, query);
            j++;
        }
        i++;
    }


    for (int i=0;i<attSize;i++){
        int j =0;
        while (j<attSize)
        {
            long int sum_Aik_Ajk=0;
            long int sum_Aik=0;
            long int sum_Ajk=0;

            int k=0;
            while (k<qyerySize)
            {
                sum_Aik_Ajk=
                        sum_Aik_Ajk + A_ik(usageMatrix[k][i], acc[k]) * A_ik(usageMatrix[k][j], acc[k]);
                sum_Aik=sum_Aik + A_ik(usageMatrix[k][i], acc[k]);
                sum_Ajk=sum_Ajk + A_ik(usageMatrix[k][j], acc[k]);
                k++;
            }
            if (sum_Aik * sum_Ajk != 0) {
                AA[i][j] = ceil(sum_Aik_Ajk / sqrt(sum_Aik * sum_Ajk));
            } else {
                AA[i][j] = 0;
            }
            if(j==0){
                cout << " ";}
            cout << AA[i][j];
            cout << " ";
            j++;
        }        
        cout << endl;
    }


    return 0;
}

// g++ -std=c++11 AAGenerator.cpp -o runAA
//  ./runAA att_1.txt query_1.txt acc_1.txt > aa_1.txt

//  g++ -std=c++11 CAGenerator.cpp -o runCA
//  ./runCA aa_1.txt > ca_1.txt