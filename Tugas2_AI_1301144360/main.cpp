#include "NodeDanGen.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <ctime>
#include <algorithm>
#include <fstream>
using namespace std;

#define UkuranPopulasi 500
#define GenMaximum 500
#define ProbMutasi 0.001
#define NilaiKonstan 1000
#define BanyaknyaCrossOver 4
#define BanyaknyaMutasi 12

int generasi;
int nNode;
int Kapasitas;
KOORDINAT_NODE LokasiDepot;
vector<Node> nodes;

void Inisialisasi();
void evaluasi();
void evaluasi(NodeDanGen &);
void InvididuTerbaik();
void Pilih();
void crossover();
void CrossOver(int, int);
void mutasi();
void Terpilih();
void report();


NodeDanGen populasi[UkuranPopulasi+1];
NodeDanGen newpopulasi[UkuranPopulasi+1];
ifstream inFile;
ofstream outFile;

void Inisialisasi()
{
    bool insertIn,insert;
    bool *visited = new bool[nNode+1];
    int processnNode;
    int optimal;
    int begin;
    int end;
    int newBegin;
    int bp;
    int up;
    int load;
    double length;
    double JarakLurusOptimal;
    double bpd;
    double tpd;
    for (int i = 0; i<UkuranPopulasi; ++i)
    {
        processnNode = 0;
        begin = 1;
        end = 1;
        memset(visited,0,sizeof(bool) * (nNode+1));
        while (processnNode<nNode)
        {
            insertIn = true;
            newBegin = (rand()%nNode)+1;
            while(visited[newBegin])
            {
                newBegin = (newBegin%nNode)+1;
            }
            processnNode++;
            load =  nodes[newBegin].demand;
            visited[newBegin] = true;
            populasi[i].gen[end] = newBegin;
            end++;
            length = 2 * LokasiDepot.JarakLurus(nodes[newBegin].lokasiXY);
            while (insertIn)
            {
                optimal = -1;
                JarakLurusOptimal = 1e10;
                insertIn = false;
                for (int j = 1; j<=nNode; ++j)
                {
                    insert = false;//
                    if ((!visited[j])&&(nodes[j].demand + load <= Kapasitas))
                    {
                        insertIn = true;
                        insert = true;
                        bpd = length - LokasiDepot.JarakLurus(nodes[populasi[i].gen[begin]].lokasiXY) + LokasiDepot.JarakLurus(nodes[j].lokasiXY) + nodes[j].lokasiXY.JarakLurus(nodes[populasi[i].gen[begin]].lokasiXY);
                        bp = begin;
                        for (int k = begin+1; k<end; ++k)
                        {
                            tpd = length - nodes[populasi[i].gen[k-1]].lokasiXY.JarakLurus(nodes[populasi[i].gen[k]].lokasiXY) + nodes[populasi[i].gen[k-1]].lokasiXY.JarakLurus(nodes[j].lokasiXY) + nodes[j].lokasiXY.JarakLurus(nodes[populasi[i].gen[k]].lokasiXY);
                            if (tpd < bpd)
                            {
                                bp = k;
                                bpd = tpd;
                            }
                        }
                        tpd = length -  nodes[populasi[i].gen[end-1]].lokasiXY.JarakLurus(LokasiDepot) + nodes[populasi[i].gen[end-1]].lokasiXY.JarakLurus(nodes[j].lokasiXY) + nodes[j].lokasiXY.JarakLurus(LokasiDepot);
                        if (bpd > tpd)
                        {
                            bp = end;
                            bpd = tpd;
                        }
                    }
                    if  ((JarakLurusOptimal>bpd)&&insert)
                    {
                        JarakLurusOptimal = bpd;
                        optimal = j;
                        up = bp;
                    }
                }
                if (insertIn)
                {
                    for (int j = end; j>up; j--)
                    {
                        populasi[i].gen[j] = populasi[i].gen[j-1];
                    }
                    length = JarakLurusOptimal;
                    if (visited[optimal])
                    {
                        optimal = optimal;
                    }
                    populasi[i].gen[up] = optimal;
                    visited[optimal] = true;
                    load += nodes[optimal].demand;
                    end++;
                    processnNode++;
                }
            }
            if (!insertIn)
            {
                begin = end;
            }
        }
    }
    delete [] visited;
}

//menghitung semua populasi individu
void evaluasi()
{
    for (int i = 0; i<UkuranPopulasi; ++i)
    {
        evaluasi(populasi[i]);
        NodeDanGen &individual=populasi[i];
        double total = 0;
        KOORDINAT_NODE now = KOORDINAT_NODE(0,0);
        int cap = 0;
        for (int j = 1; j<=nNode; ++j)
        {
            if (cap + nodes[individual.gen[j]].demand > Kapasitas)
            {
                cap = 0;
                total += now.JarakLurus(LokasiDepot);
                now = LokasiDepot;
            }
            cap += nodes[individual.gen[j]].demand;
            total += now.JarakLurus(nodes[individual.gen[j]].lokasiXY);
            now = nodes[individual.gen[j]].lokasiXY;
        }
        total += now.JarakLurus(LokasiDepot);
        individual.fitness = NilaiKonstan / total;
        individual.totalJarak = total;
    }
}

//menghitung populasi individu
void evaluasi(NodeDanGen &individual)
{
    double total = 0;
    KOORDINAT_NODE now = KOORDINAT_NODE(0,0);
    int cap = 0;
    for (int j = 1; j<=nNode; ++j)
    {
        if (cap + nodes[individual.gen[j]].demand > Kapasitas)
        {
            cap = 0;
            total += now.JarakLurus(LokasiDepot);
            now = LokasiDepot;
        }
        cap += nodes[individual.gen[j]].demand;
        total += now.JarakLurus(nodes[individual.gen[j]].lokasiXY);
        now = nodes[individual.gen[j]].lokasiXY;
    }
    total += now.JarakLurus(LokasiDepot);
    individual.fitness = NilaiKonstan / total;
    individual.totalJarak = total;
}

void Pilih()
{
    double sumFitness = 0;
    double randomNum;

    for (int i = 0; i<UkuranPopulasi; ++i)
    {
        sumFitness+=populasi[i].fitness;
    }

    for (int i = 0; i<UkuranPopulasi; ++i)
    {
        populasi[i].rfitness = populasi[i].fitness/sumFitness;
    }
    populasi[0].cfitness = populasi[0].rfitness;


    for (int i = 1; i<UkuranPopulasi; ++i)
    {
        populasi[i].cfitness = populasi[i-1].cfitness + populasi[i].rfitness;
    }

    for (int i = 0; i < UkuranPopulasi; i++)
    {
        randomNum = rand()%1000/1000.0;
        if (randomNum < populasi[0].cfitness)
            newpopulasi[i] = populasi[0];
        else
        {
            for (int j = 0; j < UkuranPopulasi; j++)
                if (randomNum >= populasi[j].cfitness && randomNum<populasi[j+1].cfitness)
                    newpopulasi[i] = populasi[j+1];
        }
    }

    for (int i = 0; i < UkuranPopulasi; i++)
        populasi[i] = newpopulasi[i];
}

//proses mutasi secara random
void mutasi()
{
    int temp,randomI,randomJ,len;
    NodeDanGen tmpIndividual;
    for (int i = 0; i<UkuranPopulasi; ++i)
    {
        tmpIndividual = populasi[i];
        for (int j = 0; j<BanyaknyaMutasi; ++j)
        {
            randomI = rand()%(nNode-1)+1;
            randomJ = rand()%(nNode-1)+1;
            temp = tmpIndividual.gen[randomJ];
            tmpIndividual.gen[randomJ] = tmpIndividual.gen[randomI];
            tmpIndividual.gen[randomI] = temp;
            evaluasi(tmpIndividual);
            if (tmpIndividual.fitness > populasi[i].fitness)
            {
                populasi[i] = tmpIndividual;
            }
        }

        tmpIndividual = populasi[i];
        for (int j = 0; j<BanyaknyaMutasi; ++j)
        {
            randomI = rand()%(nNode-1)+1;
            randomJ = rand()%(nNode-1)+1;
            if (randomI>randomJ)
            {
                temp = randomI;
                randomI = randomJ;
                randomJ = temp;
            }
            for (int j = 0; j<(randomJ-randomI)/2; ++j)
            {
                temp = tmpIndividual.gen[randomI+j];
                tmpIndividual.gen[randomI+j] = tmpIndividual.gen[randomJ-j];
                tmpIndividual.gen[randomJ-j] = temp;
            }
            evaluasi(tmpIndividual);
            if (tmpIndividual.fitness > populasi[i].fitness)
            {
                populasi[i] = tmpIndividual;
            }
        }


        tmpIndividual = populasi[i];
        for (int j = 0; j<BanyaknyaMutasi; ++j)
        {
            randomI = rand()%(nNode-1)+1;
            randomJ = rand()%(nNode-1)+1;
            if (randomI>randomJ)
            {
                temp = randomI;
                randomI = randomJ;
                randomJ = temp;
            }
            if ((nNode-1)-randomJ > 0)
            {
                len = rand()%((nNode-1)-randomJ)+1;
            }
            else
            {
                len = 1;
            }
            for (int j = 0; j<len; ++j)
            {
                temp = tmpIndividual.gen[randomI+j];
                tmpIndividual.gen[randomI+j] = tmpIndividual.gen[randomJ+j];
                tmpIndividual.gen[randomJ+j] = temp;
            }
            evaluasi(tmpIndividual);
            if (tmpIndividual.fitness > populasi[i].fitness)
            {
                populasi[i] = tmpIndividual;
            }
        }
    }
}

void Terpilih()
{
    double bestIndividual, worstIndividual;
    int bestIndex, worstIndex;
    bestIndividual = populasi[0].fitness;
    bestIndex = 0;
    worstIndividual = populasi[0].fitness;
    worstIndex = 0;
    for (int i = 0; i < UkuranPopulasi - 1; ++i)
    {
        if (populasi[i].fitness>bestIndividual)
        {
            bestIndividual = populasi[i].fitness;
            bestIndex = i;
        }
        if (populasi[i].fitness<worstIndividual)
        {
            worstIndividual = populasi[i].fitness;
            worstIndex = i;
        }
    }

    if (bestIndividual >= populasi[UkuranPopulasi].fitness)
    {
        for (int i = 1; i <= nNode; i++)
            populasi[UkuranPopulasi].gen[i] = populasi[bestIndex].gen[i];
        populasi[UkuranPopulasi].fitness = populasi[bestIndex].fitness;
        populasi[UkuranPopulasi].totalJarak = populasi[bestIndex].totalJarak;
    }
    else
    {
        for (int i = 1; i <= nNode; i++)
            populasi[worstIndex].gen[i] = populasi[UkuranPopulasi].gen[i];
        populasi[worstIndex].fitness = populasi[UkuranPopulasi].fitness;
        populasi[worstIndex].totalJarak = populasi[UkuranPopulasi].totalJarak;
    }
}

//kawin silang dua individu yang dipilih secara acak
void crossover()
{
    int firstChosen,secondChosen;
    int chosenNum  =  0;
    double randomNum,firstFitness,secondFitness;

    for (int i = 0; i < UkuranPopulasi; ++i)
    {
        randomNum = rand()%1000/1000.0;
        if (randomNum < ProbMutasi)
        {
            ++chosenNum;
            if (chosenNum % 2 == 0)
            {
                for (int j = 0; j<BanyaknyaCrossOver; ++j)
                {
                    secondChosen=i;
                    firstFitness = populasi[firstChosen].fitness;
                    secondFitness = populasi[secondChosen].fitness;
                    CrossOver(firstChosen, secondChosen);
                    if ((firstFitness<populasi[firstChosen].fitness)&&(secondFitness<populasi[secondChosen].fitness))
                        break;
                }
            }
            else
                firstChosen = i;
        }
    }
}

//kawin silang dari dua populasi
void CrossOver(int first, int second)
{
    int randomLeft,randomRight,tmp;
    randomLeft = (rand() % (nNode - 1)) + 1;
    randomRight = (rand() % (nNode - 1)) + 1;
    if (randomLeft>randomRight)
    {
        tmp = randomLeft;
        randomLeft = randomRight;
        randomRight = tmp;
    }
    bool *firstFlag = new bool[nNode+1];
    bool *secondFlag = new bool[nNode+1];
    int *firstMap = new int[nNode+1];
    int *secondMap = new int[nNode+1];
    NodeDanGen firstChild,secondChild;

    for (int i = 0; i<nNode+1; ++i)
    {
        secondFlag[i] = false;
        firstFlag[i] = false;
        secondMap[i] = 0;
        firstMap[i] = 0;
    }

    for (int i = randomLeft; i<randomRight; ++i)
    {
        secondChild.gen[i] = populasi[first].gen[i];
        firstChild.gen[i] = populasi[second].gen[i];
        firstFlag[firstChild.gen[i]] = true;
        secondFlag[secondChild.gen[i]] = true;
        firstMap[firstChild.gen[i]] = secondChild.gen[i];
        secondMap[secondChild.gen[i]] = firstChild.gen[i];
    }

    int p;
    for (int i = 1; i<=nNode; ++i)
    {
        if (i<randomLeft || i>=randomRight)
        {
            p = firstMap[populasi[first].gen[i]];
            if (p != 0)
            {
                while (firstFlag[p])
                {
                    p = firstMap[p];
                }
                if (p!=0)
                {
                    firstChild.gen[i] = p;
                    firstFlag[p] = true;
                }
                else
                {
                    firstChild.gen[i] = populasi[first].gen[i];
                    firstFlag[firstChild.gen[i]] = true;
                }
            }
            else
            {
                firstChild.gen[i] = populasi[first].gen[i];
                firstFlag[firstChild.gen[i]] = true;
            }
        }
    }
    for (int i = 1; i<=nNode; ++i)
    {
        if (i<randomLeft || i>=randomRight)
        {
            p = secondMap[populasi[second].gen[i]];
            if (p != 0)
            {
                while (secondFlag[p])
                {
                    p = secondMap[p];
                }
                if (p!=0)
                {
                    secondChild.gen[i] = p;
                    secondFlag[p] = true;
                }
                else
                {
                    secondChild.gen[i] = populasi[second].gen[i];
                    secondFlag[populasi[second].gen[i]] = true;
                }
            }
            else
            {
                secondChild.gen[i] = populasi[second].gen[i];
                secondFlag[populasi[second].gen[i]] = true;
            }
        }
    }

    evaluasi(firstChild);
    evaluasi(secondChild);

    if (firstChild.fitness > populasi[first].fitness)
    {
        populasi[first] = firstChild;
    }
    else if (firstChild.fitness > populasi[second].fitness)
    {
        populasi[second] = firstChild;
    }

    if (secondChild.fitness > populasi[first].fitness)
    {
        populasi[first] = secondChild;
    }
    else if (secondChild.fitness > populasi[second].fitness)
    {
        populasi[second] = secondChild;
    }

    delete [] firstMap;
    delete [] secondMap;
    delete [] firstFlag;
    delete [] secondFlag;
}

//ambil individu terbaik
void InvididuTerbaik()
{
    int bestIndividualIndex = 0;
    populasi[UkuranPopulasi].fitness = populasi[0].fitness;
    for (int i = 0; i<UkuranPopulasi; ++i)
    {
        if (populasi[i].fitness > populasi[UkuranPopulasi].fitness)
        {
            bestIndividualIndex = i;
            populasi[UkuranPopulasi].fitness = populasi[i].fitness;
        }
    }

    populasi[UkuranPopulasi].totalJarak = populasi[bestIndividualIndex].totalJarak;
    for (int i = 1; i<=nNode; ++i)
    {
        populasi[UkuranPopulasi].gen[i] = populasi[bestIndividualIndex].gen[i];
    }
}

void report()
{
    double bestFitness;
    double average;
    double STDDev;
    double sumSquare;
    double squareSum;
    double sum;
    double besttotalJarak;

    sum = 0.0;
    sumSquare = 0.0;

    for (int i = 0; i < UkuranPopulasi; i++)
    {
        sum += populasi[i].fitness;
        sumSquare += populasi[i].fitness * populasi[i].fitness;
    }

    average = sum/(double)UkuranPopulasi;
    squareSum = average * average * UkuranPopulasi;
    STDDev = sqrt((sumSquare - squareSum)/(UkuranPopulasi - 1));
    bestFitness = populasi[UkuranPopulasi].fitness;
    besttotalJarak = populasi[UkuranPopulasi].totalJarak;

    outFile<<generasi<<" "<<besttotalJarak<<" "<<bestFitness<<" "<<average<<" "<<STDDev<<endl;
    cout<<generasi<<" "<<besttotalJarak<<" "<<bestFitness<<" "<<average<<" "<<STDDev<<endl;
}



int main()
{
    double BEST;
    srand(time(0));
    inFile.open("input.dat");
    outFile.open("output.dat");

    inFile>>nNode>>BEST>>Kapasitas>>LokasiDepot.x>>LokasiDepot.y;
    cout<<"Banyaknya Node Tujuan  : "<< nNode << endl;
    cout<<"Best Fitness           : " << BEST <<endl;
    cout<<"Kapasitas Kendaraan    : " <<Kapasitas << endl;
    cout<<"Lokasi Koordinat Depot : " << LokasiDepot.x << "," << LokasiDepot.y << endl;
    cout<<endl;
    system("pause");
    Node node(0,0,0,0);
    nodes.push_back(node);

    int index,x,y,d;
    double bestNow;
    for (int i = 0; i<nNode; ++i)
    {
        inFile>>index>>x>>y>>d;
        Node node(index,x,y,d);
        nodes.push_back(node);
    }

    generasi = 0;
    Inisialisasi();
    evaluasi();
    InvididuTerbaik();
    while (generasi<GenMaximum)
    {
        cout<<endl<<endl;
        generasi++;
        Pilih();
        crossover();
        mutasi();
        evaluasi();
        Terpilih();
        report();
        outFile<<"Generasi Ke-"<<generasi<<endl<<"Jarak Total: "<<populasi[generasi].totalJarak<<endl<<"Solusi Optimal: "<<BEST<<endl<<"Best member:"<<endl;
        cout<<"Generasi Ke-"<<generasi<<endl<<"Jarak Total: "<<populasi[generasi].totalJarak<<endl<<"Solusi Optimal: "<<BEST<<endl<<"Best member:"<<endl;
        for(int i=1; i<=nNode; ++i)
        {
            outFile<<populasi[generasi].gen[i]<<" ";
            cout<<populasi[generasi].gen[i]<<" ";
        }
        outFile<<endl;
        cout<<endl;

    }
    cout<<endl<<endl;
    cout<<endl<<endl;
    outFile<<"SELESAI!!!"<<endl<<"Jarak Total: "<<populasi[UkuranPopulasi].totalJarak<<endl<<"Solusi Optimal: "<<BEST<<endl<<"Best member:"<<endl;
    cout<<"SELESAI!!!"<<endl<<"Jarak Total: "<<populasi[UkuranPopulasi].totalJarak<<endl<<"Solusi Optimal: "<<BEST<<endl<<"Best member:"<<endl;
    for(int i=1; i<=nNode; ++i)
    {
        outFile<<populasi[UkuranPopulasi].gen[i]<<" ";
        cout<<populasi[UkuranPopulasi].gen[i]<<" ";
    }
    outFile<<endl;
    cout<<endl;
    inFile.close();
    outFile.close();
    system("pause");
    return 0;
}
