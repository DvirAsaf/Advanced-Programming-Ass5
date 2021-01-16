/**
 * @author : Dvir Asaf 313531113.
 */
#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringStream
#include "anomaly_detection_util.h"
using namespace std;

class TimeSeries {
public:
    // constructor for this class.
    map<string , vector<float>> allData;
    //create time series from given string.
    explicit TimeSeries(const char* CSVfileName)
    {
        this->allData = ConvertToMap(read_csv(CSVfileName));
    }
    //read from the given file csv and return pair of string and vector.
    vector<pair<string, vector<float>>> read_csv(string filename)const;
    //read from the given file according to the correct column.
    void ReadCols(vector<pair<string, vector<float>>> &result, string &colname, ifstream &myFile,string &line)const;
    //read from the given file line by line.
    ifstream &
    readTextLineByLine(vector<pair<string, vector<float>>> &result, ifstream &myFile, string &line, float val) const;
    //convert pair of vector and string to map of string and vector.
    map<string , vector<float>> ConvertToMap(vector<pair<string, vector<float>>> three_cols)const;
    //check if a given name is feature in the time series.
    bool isVectorNameInMap(string name)const;
    //return the correct vector according to the name feature.
    vector<float> getVectorByName(string name) const;
};
#endif /* TIMESERIES_H_ */
