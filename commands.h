/**
 * @author : Dvir Asaf 313531113.
 */
#ifndef COMMANDS_H_
#define COMMANDS_H_
#include<iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <memory>
#include "HybridAnomalyDetector.h"
using namespace std;

class DefaultIO{
public:
    virtual string read()=0;
    virtual void write(string text)=0;
    virtual void write(float f)=0;
    virtual void read(float* f)=0;
    virtual ~DefaultIO()= default;
};

class CommandHelp {
public:
    double threshold;
    shared_ptr<HybridAnomalyDetector> detector;
};

class Command{
protected:
    DefaultIO* dio;
    CommandHelp *help{};
public:
    explicit Command(DefaultIO* dio):dio(dio){}
    virtual void execute()=0;
    virtual ~Command()= default;
};

class uploadTimeSeriesCSVFile : public Command {
public:
    explicit uploadTimeSeriesCSVFile(DefaultIO *dio) : Command(dio) {

    };
    void execute() override {
        ofstream anomalyTrainCSV;
        ofstream anomalyTestCSV;
        string inputString;
        string testCSVFile = "Please upload your local test CSV file.\n";
        string trainCSVFile = "Please upload your local train CSV file.\n";
        string complete = "Upload complete.\n ";
        anomalyTrainCSV.open("anomalyTrain.csv");
        dio->write(trainCSVFile);
        inputString = dio->read();
        while (inputString != "done") {
            anomalyTrainCSV << inputString << endl;
            inputString = dio->read();
        }
        dio->write(complete);
        anomalyTrainCSV.close();
        anomalyTestCSV.close();
    }
};

 class algoSetting : public Command {
 public:
     algoSetting(DefaultIO *dio, CommandHelp *help1) : Command(dio) {
         this->help = help1;
     }
     void execute() override {
         string correlation = "The current correlation threshold is 0.9\n"
                              "Enter a new value of threshold:\n";
         string selectedValue = "please choose a value between 0 to 1.\n";
         dio->write(correlation);
         help->threshold = stof(dio->read());
         while (help->threshold <= 0 || help->threshold >= 1) {
             dio->write(selectedValue);
             help->threshold = stof(dio->read());
         }
     }
 };

 class anomaliesDetection : public Command {
 public:
     anomaliesDetection(DefaultIO *dio, CommandHelp *help1) : Command(dio) {
         this->help = help1;
     };
     void execute() override {
         string  complete = "anomaly detection complete.\n";
         shared_ptr<HybridAnomalyDetector> ptrDetector(new HybridAnomalyDetector());
         TimeSeries anomalyTrainCSV("anomalyTrain.csv");
         TimeSeries anomalyTestCSV("anomalyTest.csv");
         ptrDetector->learnNormal(anomalyTrainCSV);
         ptrDetector->detect(anomalyTestCSV);
         help->detector = ptrDetector;
         dio->write(complete);
     }
 };

 class displayResult : public Command {
 public:
     displayResult(DefaultIO *dio, CommandHelp *help1) : Command(dio) {
         this->help = help1;
     };
     void execute() override {
         string done = "done\n";
         int size = help->detector->anomalyReport.size();
         for(int i = 0; i < size; i++) {
             string descriptionI = help->detector->anomalyReport[i].description;
             long timeStepI = help->detector->anomalyReport[i].timeStep;
             string s = to_string(timeStepI) + " \t" + descriptionI + "\n";
             dio->write(s);
         }
         dio->write(done);
     }
 };

 class anomaliesAndAnalyze : public Command {
 public:
     anomaliesAndAnalyze(DefaultIO *dio, CommandHelp*help1) : Command(dio) {
         this->help = help1;
     };
     void execute() override {
         string inputString;
         string truePstring;
         string falsePstring;
         stringstream  truePs;
         stringstream  falsePs;
         long firstMark;
         float truePositiveRate;
         float falseAlarmRate;
         string anomaliesFile = "Please upload your local anomalies file.\n";
         string complete = "Upload complete.\n";
         vector<pair<int, int> > timeMarks;
         vector<pair<int, int> > anomaly;
         int pos = 0;
         int neg = TimeSeries("anomalyTest.csv").allData.begin()->second.size();
         int time = 1000;
         int FP = 0;
         int TP = 0;
         dio->write(anomaliesFile);
         int size = help->detector->anomalyReport.size() - 1;
         for (int i = 0; i < size; i++) {
             firstMark = help->detector->anomalyReport[i].timeStep;
             string des1 = help->detector->anomalyReport[i].description;
             string des2 = help->detector->anomalyReport[i + 1].description;
             long time1 = help->detector->anomalyReport[i + 1].timeStep;
             long time2 = help->detector->anomalyReport[i].timeStep + 1;
             while (des1 == des2 && time1 == time2) {
                 i++;
             }
             pair<int, int> pair1(firstMark, help->detector->anomalyReport[i].timeStep);
             anomaly.push_back(pair1);
         }
         inputString = dio->read();
         while (inputString != "done") {
             int position = inputString.find(',');
             string string1 = inputString.substr(0, position);
             string string2 = inputString.substr(position + 1);
             pair<int, int> pair2(stoi(string1), stoi(string2));
             timeMarks.push_back(pair2);
             neg -= (stoi(string2) - stoi(string1));
             inputString= dio->read();
             pos++;
         }
         dio->write(complete);
         for (auto & aR : anomaly) {
             bool piece = false;
             for (auto & tM : timeMarks) {
                 if ((tM.second >= aR.first && aR.second >= tM.second) ||
                     (tM.first >= aR.first && aR.second >= tM.first) ||
                     (tM.first <= aR.first && aR.second <= tM.second)) {
                     TP++;
                     piece = true;
                 }
             }
             if (piece != true) {
                 FP++;
             }
         }
         //false alarm rate
         falseAlarmRate = (FP * time / neg) ;
         falseAlarmRate = floor(falseAlarmRate);
         falseAlarmRate = falseAlarmRate/time;
         falsePs << falseAlarmRate;
         falsePstring = falsePs.str();
         string printFalseRate = "False Positive Rate: " + falsePstring +"\n";
         dio->write(printFalseRate);
         // true positive rate
         truePositiveRate = (TP * time / pos) ;
         truePositiveRate = floor(truePositiveRate);
         truePositiveRate = truePositiveRate/time;
         truePs << truePositiveRate;
         truePstring = truePs.str();
         string printTrueRate = "True Positive Rate: " + truePstring +"\n";
         dio->write(printTrueRate);
     }

 };

class exitFromCLI : public Command {
public:
    explicit exitFromCLI(DefaultIO *dio) : Command(dio) {
    };

    void execute() override {
    }
};

#endif /* COMMANDS_H_ */