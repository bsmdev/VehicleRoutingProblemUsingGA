#ifndef NODEDANGEN_H_INCLUDED
#define NODEDANGEN_H_INCLUDED

#include <cmath>
#define NILAIMAXIMAL 500

class NodeDanGen
{
public:
    int gen[NILAIMAXIMAL];
    double fitness;
    double rfitness;
    double cfitness;
    double totalJarak;

    const NodeDanGen operator=(const NodeDanGen xy)
    {
        fitness = xy.fitness;
        totalJarak = xy.totalJarak;
        rfitness = xy.rfitness;
        cfitness = xy.cfitness;

        for (int i = 0; i<NILAIMAXIMAL; ++i)
        {
            gen[i] = xy.gen[i];
        }

        return *this;
    }
};

class KOORDINAT_NODE
{
public:
    int x;
    int y;

    KOORDINAT_NODE()
    {
        x = y = 0;
    }

    KOORDINAT_NODE(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    const KOORDINAT_NODE operator=(const KOORDINAT_NODE xy2)
    {
        x = xy2.x;
        y = xy2.y;
        return *this;
    }

    double JarakLurus(KOORDINAT_NODE xy2)
    {
        return sqrt(((x - xy2.x)*(x - xy2.x)) + ((y - xy2.y)*(y - xy2.y)));
    }
};

class Node
{
public:
    KOORDINAT_NODE lokasiXY;
    int demand;
    int NodeKe;
    Node(int i, int x, int y, int d)
    {
        this->NodeKe=i;
        this->lokasiXY=KOORDINAT_NODE(x,y);
        this->demand=d;
    }
};


#endif // NODEDANGEN_H_INCLUDED
