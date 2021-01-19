/**
 * @author : Dvir Asaf 313531113.
 */
#include "timeseries.h"
using namespace std;
//read from the given file csv and return pair of string and vector.
vector<pair<string, vector<float>>> TimeSeries::read_csv(string fileName) const{
// Reads a CSV file into a vector of <string, vector<int>> pairs where
// each pair represents <column name, column values>
// Create a vector of <string, int vector> pairs to store the result
    vector<pair<string, vector<float>>> result;
// Create an input filestream
    ifstream myFile(fileName);
// Make sure the file is open
    if(!myFile.is_open()) throw runtime_error("Could not open file");
// Helper vars
    string line, colName;
    float val = 0;
// Read the column names
    ReadCols(result, colName, myFile, line);
// Read data, line by line
    myFile = dynamic_cast<basic_ifstream<char> &&>(readTextLineByLine(result, myFile, line, val));
// Close file
    myFile.close();
    return result;
}
//read from the given file line by line.
ifstream &
TimeSeries::readTextLineByLine(vector<pair<string, vector<float>>> &result, ifstream &myFile, string &line,
                               float val) const {
    while(getline(myFile, line))
    {
// Create a stringStream of the current line
        stringstream ss(line);
// Keep track of the current column index
        int colIdx = 0;
// Extract each integer
        while(ss >> val){
// Add the current integer to the 'colIdx' column's values vector
            result.at(colIdx).second.push_back(val);
// If the next token is a comma, ignore it and move on
            if(ss.peek() == ',') ss.ignore();
// Increment the column index
            colIdx++;
        }
    }
    return myFile;
}
//read from the given file according to the correct column.
void TimeSeries::ReadCols(vector<pair<string, vector<float>>> &result, string &colName, ifstream &myFile, string &line) const{
    if(myFile.good())
    {
// Extract the first line in the file
        getline(myFile, line);
// Create a stringStream from line
        stringstream ss(line);
// Extract each column name
        while(getline(ss, colName, ',')){
// Initialize and add <colName, int vector> pairs to result
            result.push_back({colName, vector<float> {}});
        }
    }
}
//convert pair of vector and string to map of string and vector.
map<string , vector<float>> TimeSeries::ConvertToMap(vector<pair<string, vector<float>>> vector_map)const{
    map<string , vector<float>> map;
    for (auto  &x : vector_map){
        string col_name = x.first;
        auto vec = x.second;
        map.insert({col_name, vec});
    }
    return map;
}
//check if a given name is feature in the time series.
bool TimeSeries::isVectorNameInMap(string Name) const{
    map<string, vector<float>>::iterator it;
    auto x = this->allData.find(Name);
    if (it == this->allData.end()) {
        return false;
    }
    return true;
}
//return the correct vector according to the name feature.
vector<float> TimeSeries::getVectorByName(string Name) const{
    if (isVectorNameInMap(Name)) {
        auto x = this->allData.find(Name);
        return x->second;
    }
    return {};
}
