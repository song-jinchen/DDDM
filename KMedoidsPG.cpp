#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <sstream>
#include <math.h>
#include <stdio.h>
#include <iomanip>
#include <cmath>
using namespace std;
//Create the cluseter paket for later implementation.
class packet{
public:
    string flowKey;
    int ArrivalTime=arrivalTime;
    int arrivalTime;
    int PacketLength=packetLength;
    int packetLength;
};

//Create the class cluster for later implementation
class cluster{
public:
    int ModiodIdx=modiodIdx;
    int modiodIdx;
    vector<int> nesrestPoints;
    void addPoint(int pointIdx)
    {
        int PointIdx=pointIdx;
        nesrestPoints.push_back(PointIdx);
    }
    void clearPoints()
    {
        nesrestPoints.clear();
    }

};

//Calculate the distance with manhatan distance return the sum of the two results
double Distance(vector<vector<double>> flowResult, int point1, int point2)
{
    int Point1=point1;
    double r1 = abs(flowResult[Point1][0]-flowResult[point2][0]);
    int Point2=point2;
    double r2 = abs(flowResult[point1][1]-flowResult[Point2][1]);
    return r1 + r2;
    //return abs(flowResult[point1][0]-flowResult[point2][0])+abs(flowResult[point1][1]-flowResult[point2][1]);
}

//Calculate the distance with E distance return the sum of the two results
double EDistance(vector<vector<double>> flowResult, int point1, int point2)
{
    int Point1=point1;
    double r1 = pow(flowResult[Point1][0]-flowResult[point2][0],2);
    int Point2=point2;
    double r2 = pow(flowResult[point1][1]-flowResult[Point2][1],2);
    return pow(r1 + r2,0.5);
    //return abs(flowResult[point1][0]-flowResult[point2][0])+abs(flowResult[point1][1]-flowResult[point2][1]);
}



//Find the nearest points
int DetermineNearest(int pointIdx, vector<vector<double>> map, vector<int> modoidIdx)
{
    int minIdx;
    int MinIdx= minIdx;
    double min_dis=9999999.0;
    double dis;

    int k=0;
    while (k<modoidIdx.size())
    {
        dis=map[pointIdx][modoidIdx[k]];
        if (dis<min_dis)
        {
            min_dis=dis;
            minIdx=modoidIdx[k];
        }
        k++;
    }

    return minIdx;
}
//determine wether it is a medoid or not
bool isMedoid(vector<int> medoid_idx, int key)
{
    int Key = key;
    int i =0;
    while (i<medoid_idx.size())
    {
        if(key!=medoid_idx[i])
        {
            return false;
        }
        i++;
    }
    return true;
}
//build the mao by adding the results
vector<vector<double>>  buildMap(vector<vector<double>> flowResult)
{
    vector<vector<double>> map;
    int x=0;
    while (x<flowResult.size())
    {
        vector<double> entity;
        int y=0;
        while (y<flowResult.size())
        {
            entity.push_back(Distance(flowResult, x, y));
            y++;
        }
        map.push_back(entity);
        x++;
    }
    return map;
}

vector<vector<double>>  EbuildMap(vector<vector<double>> flowResult)
{
    vector<vector<double>> map;
    int x=0;
    while (x<flowResult.size())
    {
        vector<double> entity;
        int y=0;
        while (y<flowResult.size())
        {
            entity.push_back(EDistance(flowResult, x, y));
            y++;
        }
        map.push_back(entity);
        x++;
    }
    return map;
}

void assignPoints(vector<vector<double>>& flowResult, vector<int>& modoidIdx, vector<cluster>& clusters)
{
    for (int l = 0; l<clusters.size(); l++)
    {
        clusters[l].clearPoints();
        clusters[l].modiodIdx=modoidIdx[l];
    }

    int i=0;
    while (i<flowResult.size())
    {
        int minIdx;
        int MinIdx=minIdx;
        minIdx= DetermineNearest(i, flowResult, modoidIdx);
        if (minIdx!=-1)
        {
            int h=0;
            while (h<clusters.size())
            {
                if(clusters[h].modiodIdx==minIdx)
                {
                    clusters[h].addPoint(i);
                    break;
                }
                h++;
            }
        }
        i++;
    }
}
//adding the flow with push_bakc to the result
vector<int> findFlow(string key,vector<packet> packets)
{
    vector<int> resultIdx;
    int i =0;
    while (i<packets.size())
    {
        string Key= key;
        if(packets[i].flowKey == Key)
        {
            resultIdx.push_back(i);
        }
        i++;
    }
    return resultIdx;
}
//remove the flow fron the vector
void removeFlow(vector<int> resultIdx, vector<packet>& packets)
{
    vector <packet>::iterator Iter;
    //remove from end to first
    int i=resultIdx.size()-1;
    while (i>=0)
    {
        Iter = packets.begin()+resultIdx[i];
        packets.erase(Iter);
        Iter = packets.begin();
        i--;
    }   
 }

//Calculate the total cost(Manhaton Distance)
double calculateTC(int previousModoidIdx, int potientalModoidIdx, vector<cluster>& clusters,  vector<vector<double>>& map, vector<int>& newModiodIdx,vector<int>& oldModiodIdx)
{
    int PreviousModoidIdx= previousModoidIdx;
    int PotientalModoidIdx= potientalModoidIdx;
    double TCih=0;
    int j=0;
    while (j<map.size())
    {
        bool belongPrevious =false;
        bool belongPotential=false;
        int newClusterIdx;
        int NewClusterIdx= newClusterIdx;
        int previousCluster;
        int PreviousCluster= previousCluster;
        double cost=0;
        previousCluster= DetermineNearest(j, map, oldModiodIdx);
        if (previousCluster != previousModoidIdx) {}
        else {
            belongPrevious = true;
        }
        newClusterIdx= DetermineNearest(j, map, newModiodIdx);
        if (newClusterIdx != potientalModoidIdx) {
            belongPotential = false;
        } else {
            belongPotential = true;
        }
        if (!belongPrevious || !belongPotential) {
            if (!belongPrevious || belongPotential) {
                if (belongPrevious || !belongPotential) {
                    cost = 0;
                } else {
                    cost = map[j][potientalModoidIdx] - map[j][previousCluster];
                }
            } else {
                cost = map[j][newClusterIdx] - map[j][previousModoidIdx];
            }
        } else {
            cost = map[j][potientalModoidIdx] - map[j][previousModoidIdx];
        }
        TCih+=cost;
        j++;
    }
    return TCih;
}

//calculate the error
double calculateError(vector<cluster>& clusters, vector<vector<double>>& map)
{
    int a=0;
    double error=0;
    int i=0;
    while (i<clusters.size())
    {
        int j=0;
        while (j<clusters[i].nesrestPoints.size())
        {
            error=error+map[clusters[i].modiodIdx][ clusters[i].nesrestPoints[j]];
            j++;
        }
        i++;
    }
    return error;
}

int main(int argc, const char * argv[]) {

    int Argc=argc;
    vector<packet> packets;
    vector<vector<double>> flow_result;
    vector<int> flow_idx;
    int medoid_num;
    int Medoid_num=medoid_num;
    vector<int> medoid_idx;
    ifstream infile;
    string data;
    string Data=data;
    //Read the first file and do the calculation
    infile.open(argv[1],ios::in);
    getline(infile,data);
    while (getline(infile, data)) {
        string tmp;
        string Tmp=tmp;
        vector<string> vec;
        stringstream ss(data);
        while (ss>>Tmp) {
            vec.push_back(Tmp);
        }
        packet pac;
        packet Pac= pac;
        pac.flowKey= vec[0] + vec[1] + vec[2] + vec[3] + vec[4];
        pac.arrivalTime=std::stoi(vec[5]);
        pac.packetLength=std::stoi(vec[6]);
        packets.push_back(pac);
    }
    infile.close();

    while (packets.size()!=0) {
        flow_idx=findFlow(packets[0].flowKey, packets);
        if (!(flow_idx.size() <= 1)) {
            double time = 0;
            double length = 0;
            vector<double> result;
            int j = flow_idx.size() - 1;
            while (j > 0) {
                length = length + packets[flow_idx[j]].packetLength;
                int Time=1;
                time = time + packets[flow_idx[j]].arrivalTime - packets[flow_idx[j - Time]].arrivalTime;
                j--;
            }
            length = length + packets[0].packetLength;
            int TIME=1;
            time = time / (flow_idx.size() - TIME);
            length = length / flow_idx.size();
            result.push_back(time);
            result.push_back(length);
            flow_result.push_back(result);
            removeFlow(flow_idx, packets);

        } else {
            removeFlow(flow_idx, packets);
        }
    }
    //output the result to "Flow.txt"
    ofstream fout;
    fout.open ("Flow.txt", ios::out | ios::trunc);
    fout.flags(ios::fixed);
    int k=0;
    while (k<flow_result.size())
    {
        fout<<k<<" ";
        fout<<setprecision(2)<<flow_result[k][0];
        fout<<" ";
        fout<<setprecision(2)<<flow_result[k][1]<<endl;
        k++;
    }
    fout.close();

    ifstream infile1;
//open the second file
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
    infile1.open(argv[2],ios::in);
    string DAta =data;
    getline(infile1,data);
    medoid_num=stoi(data);
    while (getline(infile1, data)) {
        int num;
        int Num= num;
        stringstream ss(data);
        while (ss>>num) {
            medoid_idx.push_back(num);
        }
    }
    infile1.close();

    vector<cluster> clusters;
    int i =0;
    while (i<medoid_num)
    {
        cluster clust;
        clusters.push_back(clust);
        i++;
    }

    vector<vector<double>> map=buildMap(flow_result);

    bool change=true;
    double error;

    while(change)
    {
        assignPoints(map, medoid_idx, clusters);
        bool isChange=false;
        int m=0;
        for (int m=0; m<flow_result.size(); m++) {
            if (isMedoid(medoid_idx, m)) continue;
            bool isExchange=false;
            int g=0;
            while (g<medoid_num)
            {
                vector<int> newModiodIdx(medoid_idx);
                newModiodIdx[g]=m;
                double TC;
                TC=calculateTC(medoid_idx[g], newModiodIdx[g], clusters, map,newModiodIdx,medoid_idx);
                if (TC >= 0) {}
                else {
                    isExchange = true;
                    medoid_idx = newModiodIdx;
                    isChange = true;

                    break;
                }
                g++;
            }
            if (!isExchange)
                continue;
            break;

        }
        if (!isChange) {
            change = false;
        } else {
            change = true;
        }
    }
    error=calculateError(clusters, map);
    ofstream ccc;

    ////////////////////////
    ccc.open ("KMedoids.txt", ios::out | ios::trunc);
    ccc.flags(ios::fixed);
    ccc << setprecision(2) << error << endl;
    int a =0;
    while (a<medoid_idx.size())
    {
        ccc << setprecision(2) << medoid_idx[i] << " ";
        a++;
    }

    ccc << endl;

    int b =0;
    while (b<clusters.size())
    {
        int j=0;
    // cout<<213<<endl;

        while ( j<clusters[b].nesrestPoints.size())
        {
            ccc << setprecision(2) << clusters[b].nesrestPoints[j] << " ";
            j++;
        }
        ccc << endl;
        b++;
    }


    ccc.close();

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

//     ifstream infile2;
// //open the third file
//     infile2.open(argv[2],ios::in);
//     string DATA =data;
//     getline(infile2,data);
//     medoid_num=stoi(data);
//     while (getline(infile2, data)) {
//         int num;
//         int Num= num;
//         stringstream ss(data);
//         while (ss>>num) {
//             medoid_idx.push_back(num);
//         }
//     }
//     infile2.close();

    // cout<<DATA<<endl;

    vector<cluster> Clusters;
    int e =0;
    while (e<medoid_num)
    {
        cluster clust;
        Clusters.push_back(clust);
        e++;
    }

    vector<vector<double>> Map=EbuildMap(flow_result);

    bool Change=true;
    double Error;

    while(Change)
    {
        assignPoints(Map, medoid_idx, Clusters);
        bool isChange=false;
        int m=0;
        for (int m=0; m<flow_result.size(); m++) {
            if (isMedoid(medoid_idx, m)) continue;
            bool isExchange=false;
            int g=0;
            while (g<medoid_num)
            {
                vector<int> newModiodIdx(medoid_idx);
                newModiodIdx[g]=m;
                double TC;
                TC=calculateTC(medoid_idx[g], newModiodIdx[g], clusters, Map,newModiodIdx,medoid_idx);
                if (TC >= 0) {}
                else {
                    isExchange = true;
                    medoid_idx = newModiodIdx;
                    isChange = true;

                    break;
                }
                g++;
            }
            if (!isExchange)
                continue;
            break;

        }
        if (!isChange) {
            Change = false;
        } else {
            Change = true;
        }
    }
    Error=calculateError(Clusters, Map);
    ccc.open ("KMedoidsE.txt", ios::out | ios::trunc);
    ccc.flags(ios::fixed);
    ccc << setprecision(2) << Error << endl;
    int c =0;
    while (c<medoid_idx.size())
    {
        ccc << setprecision(2) << medoid_idx[i] << " ";
        c++;
    }

    ccc << endl;
    int d =0;
    while (d<clusters.size())
    {
        int j=0;
        while ( j<Clusters[d].nesrestPoints.size())
        {
            ccc << setprecision(2) << Clusters[d].nesrestPoints[j] << " ";
            j++;
        }
        ccc << endl;
        d++;
    }
    
    ccc.close();

    return 0;
}