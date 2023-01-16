#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Vector.h"
#include "Distances/Distances.h"
#include "Distances/CHB.h"
#include "Distances/AUC.h"
#include "Distances/MIN.h"
#include "Distances/MAN.h"
#include "Distances/CAN.h"
#include "Comparator.h"
#include "map"

using std::vector;
using std::istream;

class Database {

private:
    std::map<string, Distances *> map;
    vector<Vector> trainVectors;
    vector<Vector> testVectors;
    Distances *distance;
    string distanceName;
    int k;

public:
    explicit Database();
    ~Database();

    void initVectors(Vector v);

    void initDistances();

    unsigned long getVectorSize();

    void setK(int k);
    void setDistanceFunction(string s);
    std::string getK();

    std::string getDistanceFunction();

    int size();

    string knn(Vector &v,int k);

    void print();

    void initTrainVectors(string fileTrainVectors);
    void initTestVectors(string fileTestVectors);
};

#endif
