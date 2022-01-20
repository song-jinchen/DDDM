#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;

vector<vector<int>> getAA(string filename )
{
    vector<vector<int>> AA;
    ifstream infile;
    string data;
    string fname;
    fname =filename;
    infile.open(fname, ios::in);
    while (getline(infile, data))
    {
        int tmp;
        vector<int> vec;
        stringstream ss(data);
        while(ss>>tmp)
        {
            vec.push_back(tmp);
        }

        AA.push_back(vec);
    }
    infile.close();
    return AA;
}
int bond(vector<vector<int>> AA, int left, int right)
{
    int aasize= AA.size();
    left=left-1;
    right=right-1;
    int bondvalue=0;
    if(left<0||right>=aasize||right<0||left>=aasize)
    {
        return bondvalue;
    }
    int i=0;
    while (i<AA.size())
    {
        bondvalue=bondvalue+AA[i][left]*AA[i][right];
        i++;
    }
    return bondvalue;
}

int cont(vector<vector<int>> AA, int left,int middle, int right)
{
    if(left <= 0) {
        int Right=right;
        return 2*bond(AA,middle, Right);
    }

    if (right < middle + 1) return 2 * bond(AA, left, middle) + 2 * bond(AA, middle, right) - 2 * bond(AA, left, right);
    int Left=left;
    int Middle=middle;
    return 2*bond(AA,Left, Middle);
}
vector<int> getColumns(vector<vector<int>> AA, int column_index)
{
    vector<int> column;
    int i=0;
    while (i<AA[0].size())
    {
        int cindex=column_index;
        column.push_back(AA[i][cindex-1]);
        i++;
    }
    return column;
}

void placeColumn(vector<vector<int>> &CA,vector<int> column,int index)
{
    int i=0;
    while (i<CA.size())
    {
        int inde=index;
        CA[i][inde-1]=column[i];
        i++;
    }
}


vector<int> getRow(vector<vector<int>> AA, int row_index)
{
    vector<int> row;
    int i=0;
    while (i<AA[0].size())
    {
        int rowi= row_index;
        row.push_back(AA[rowi-1][i]);
        i++;
    }
    return row;
}

void placeRow(vector<vector<int>> &CA,vector<int> row,int index)
{
    int i=0;
    while (i<CA.size()) 
    {
        int ind=index;
        CA[ind-1][i]=row[i];
        i++;
    }
    
}

void showRecord(vector<int> record)
{
        int j=0;
        while (j<record.size())
        {
            cout<<"\tA";
            cout<<record[j];
            j++;
        }
        cout<<endl;
}


void showVector(vector<vector<int>> AA,vector<int> record)
{
    showRecord(record);
    int i=0;
    while (i<AA.size())
    {
        cout<<"A";
        cout<<record[i];
        cout<<"\t";

        int j=0;
        int asize=AA.size();
        while (j<asize)  
        {
            cout<<AA[i][j];
            cout<<" ";
            j++;
        }
        
        cout<<endl;
        i++;
    }
}

int main(int argc, const char * argv[]) {
    vector<vector<int>> AA;
    string aa_file=argv[1];
    AA=getAA(argv[1]);
    int length=AA.size();
    vector<vector<int> > CA(length, vector<int>(length,0));
    int leng=length;
    vector<int> record(leng);
    placeColumn(CA, getColumns(AA, 1), 1);
    int getcol=2;
    placeColumn(CA, getColumns(AA, getcol), getcol);
    record[0]=1;
    record[1]=2;

    int index=3;
    int csize=CA.size();
    while (index<=csize) {
        placeColumn(CA, getColumns(AA, index), index);

        int contirbution=0;
        int maxCont=0;
        int place=0;
        for (int i=1; i<index; i++) {
            int contribute=1;
            contirbution=cont(CA, i-contribute, index, i);
            if (contirbution <= maxCont) continue;
            place = i;
            maxCont = contirbution;
        }

        contirbution=cont(CA, index-1, index, index+1);

        if (contirbution <= maxCont) {}
        else {
            place = index;
            maxCont = contirbution;
        }

        if (place >= 1) {
            int j = index;
            while ( j>= place)
            {
                placeColumn(CA, getColumns(CA, j - 1), j);
                record[j - 1] = record[j - 2];
                j--;
            }
            int gcolumn=1;
            placeColumn(CA, getColumns(AA, index), place);
            record[place - gcolumn] = index;
        }
        if(place <1) {
            int j = index;
            while ( j>= 1)
            {
                int gcolum=1;
                placeColumn(CA, getColumns(CA, j - gcolum), j);
                record[j - 1] = record[j - 2];
                j--;
            }
            placeColumn(CA, getColumns(AA, index), 1);
            record[0] = index;

        }
        index++;
    }
    
    
    vector<vector<int> > finalCA(length, vector<int>(length,0));
    int i=1;
    while (i<=record.size())
    {
        placeRow(finalCA, getRow(CA,record[i-1]), i);
        i++;
    }
   
    showVector(finalCA,record);

    return 0;
}
