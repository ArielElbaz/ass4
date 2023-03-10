#include "Database.h"
#include <algorithm>

#include <map>

using std::string;
using std::vector;

/**
 * Constructor.
 * @param s file name
 * @param k k nearest
 */
Database::Database() {
    initDistances();
    // Create string vector for class parameter
    this->distance = this->map["AUC"];
    this->distanceName = "AUC";
    this->k = 5;
    this ->classifications = "";
    this->trainVectors = vector<Vector>();
    this->testVectors = vector<Vector>();
}


void Database::print() {
    if (!this->trainVectors.empty()) {
        for (Vector v: this->trainVectors) {
            v.print();
        }
    } else {
        std::cout << "Database is empty" << std::endl;
    }
}

/**
 * @brief initVectors the trainVectors distances by the correct function.
 * @param funcIdent - function name
 * @param v - arg vector.
 */
void Database::initVectors(Vector v) {
    for (Vector &vec: this->trainVectors) {
        try {
            (*distance)(vec, v);
        } catch (...) {
            throw;
        }
    }
}

/**
 * @brief find the k nearest neighbors
 * @param v - argument vector
 * @param code - function name
 * @return string - classification
 */
string Database::knn() {
    string toServer = "";
    int i = 1;
//    vector<std::pair<double, string>> lineAndClassifications;
    string classification;
    Comparator comparator;
    // Create a vector of pairs, where the first element is the distance,
    // and the second is the index of the vector

    for (Vector &vec: this->testVectors) {
        std::map<string, int> classificationCount;
        vector<std::pair<double, string>> classifyKnearest;
        try{
            Database::initVectors(vec);
        } catch (...) {
            throw;
        }
        // Sort the vector by distance function
        std::sort(this->trainVectors.begin(), this->trainVectors.end(), comparator);
        for (int j = 0; j < this->k; j++) {
            classifyKnearest.emplace_back(this->trainVectors[j].getDistFromArg(),
                                          this->trainVectors[j].getClassification());
        }

        for (int j = 0; j <this->k; j++) {
            classificationCount[classifyKnearest[j].second]++;
        }
        // Find the classification with the most votes
        int max = 0;
        for (auto &j: classificationCount) {
            if (j.second > max) {
                max = j.second;
                classification = j.first;
            }
        }
        toServer +=  std::to_string(i) + " " + classification + "\n";
        i++;
    }
    this->classifications = toServer;
    return toServer;
}

/**
 * @brief get the size of a vector in the database
 * @return size of vector
 */
unsigned long Database::getVectorSize() {
    return this->trainVectors[0].size();
}

/**
 * Get the size of the database
 * @return size of database
 */
int Database::size() {
    return this->trainVectors.size();
}

/**
 * Create corresponding distance functor from string input and initialize the map of distances.
 * @param s - distance function name
 */
void Database::initDistances() {
    this->map["AUC"] = new AUC();
    this->map["CHB"] = new CHB();
    this->map["CAN"] = new CAN();
    this->map["MIN"] = new MIN();
    this->map["MAN"] = new MAN();
}

void Database::setK(int k) {
    this->k = k;
}

bool Database::isKValid(int k) {
    return k <= Database::size();
}

void Database::setDistanceFunction(string s) {
    this->distanceName = s;
    this->distance = this->map[s];
    if (this->distance == nullptr) {
        throw "Invalid distance function";
    }
}

Database::~Database() {
    // Delete the distances map
    for (auto &i: this->map) {
        delete i.second;
    }
    this->distance = nullptr;
}
/**
 * Initialize test vectors from file
 * @param fileTestVectors content of test file in string format
 */
void Database::initTestVectors(string fileTestVectors) {
    std::istringstream stream(fileTestVectors);
    char del = '\n';
    string line;
    // Read one line at a time
    while (std::getline(stream, line, del)) {
        Vector v;
        // Init vector from the line, then push it into member Vector list
        try {
            v.initFromString(line);
        }
        catch (std::ios_base::failure const &ex) {
            throw;
        }
        this->testVectors.push_back(v);
    }
    if (this->testVectors.empty()) { // Exit if database contains no trainVectors
        throw std::ios_base::failure("File contains no valid trainVectors. Exiting program");
    } else if (this->testVectors.size() > 1) { // Exit if database contains
        for (Vector vec: this->testVectors) {
            if (vec.size() != this->testVectors[2].size()) {
                throw std::ios_base::failure(
                        "File contains trainVectors of differing sizes. Exiting program");
            }
        }
    }

}
/**
 * Init training vectors from file
 * @param fileTrainVectors content of training file in string format
 */
void Database::initTrainVectors(string fileTrainVectors) {
    // Read the file
    // Iterate over member file list
    std::istringstream sstream(fileTrainVectors);
    char del = '\n';
    string line;
    // Read one line at a time
    while (std::getline(sstream, line, del)) {
        Vector v;
        // Init vector from the line, then push it into member Vector list
        try {
            v.initFromString(line);
        }
        catch (std::ios_base::failure const &ex) {
            throw;
        }
        trainVectors.push_back(v);
    }
    if (trainVectors.empty()) { // Exit if database contains no trainVectors
        throw std::ios_base::failure("File contains no valid trainVectors. Exiting program");
    } else if (trainVectors.size() > 1) { // Exit if database contains
        for (Vector vec: trainVectors) {
            if (vec.size() != trainVectors[0].size()) {
                throw std::ios_base::failure(
                        "File contains trainVectors of differing sizes. Exiting program");
            }
        }
    }

}

std::string Database::getK() {
    return std::to_string(this->k);
}

int Database::getKInt() {
    return this->k;
}

std::string Database::getDistanceFunction() {
    return this->distanceName;
}

/**
 * Check whether either dataset is unloaded (test or train)
 * @return
 */
bool Database::isFilesUnloaded() {
    if (this->trainVectors.empty() || this->testVectors.empty()) {
        return true;
    }
    return false;
}



std::string Database::getClassfications() {
    return this->classifications;
}

void Database::setClassfications(const char *string) {
    this->classifications = string;

}

void Database::resetFiles() {
    this->classifications = "";
    this->trainVectors.clear();
    this->testVectors.clear();
}









