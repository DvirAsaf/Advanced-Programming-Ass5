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
    virtual ~DefaultIO(){}
};

class CommandHelp {
public:
    double newThreshold;
    shared_ptr<HybridAnomalyDetector> had;
};

class Command{
protected:
    DefaultIO* dio;
    CommandHelp *help{};
public:
    Command(DefaultIO* dio):dio(dio){}
    virtual void execute()=0;
    virtual ~Command(){}
};

class uploadCSV : public Command {
public:
    uploadCSV(DefaultIO *dio) : Command(dio) {

    };
    void execute() override {
        ofstream trainCSV, testCSV;
        string uploadTestCSV = "Please upload your local test CSV file.\n";
        string inputData;
        string uploadTrainCSV = "Please upload your local train CSV file.\n";
        string complete = "Upload complete.\n ";

        trainCSV.open("anomalyTrain.csv");

        dio->write(uploadTrainCSV);
        inputData = dio->read();

        while (inputData != "done") {

            trainCSV << inputData << endl;
            inputData = dio->read();
        }
        dio->write(complete);
        trainCSV.close();
        testCSV.close();

    }
};

 class algorithmSetting : public Command {
 public:
     algorithmSetting(DefaultIO *dio, CommandHelp *help1) : Command(dio) {
         this->help = help1;
     }
     void execute() override {
         string currentCorrelation = "The current correlation threshold is 0.9\n"
                                     "Type a new threshold\n";
         string chooseValue = "please choose a value between 0 to 1.\n";
         dio->write(currentCorrelation);
         help->newThreshold = stof(dio->read());
         while (help->newThreshold <= 0 || help->newThreshold >= 1) {
             dio->write(chooseValue);
             help->newThreshold = stof(dio->read());
         }
     }
 };

 class detectAnomalies : public Command {
 public:
     detectAnomalies(DefaultIO *dio, CommandHelp *help1) : Command(dio) {
         this->help = help1;
     };
     void execute() override {
         string  detectComp1 = "anomaly detection complete.\n";
         shared_ptr<HybridAnomalyDetector> had(new HybridAnomalyDetector());
         TimeSeries trainCSV("anomalyTrain.csv"), testCSV("anomalyTest.csv");
         had->learnNormal(trainCSV);
         had->detect(testCSV);
         help->had = had;
         dio->write(detectComp1);
     }
 };

 class displayResult : public Command {
 public:
     displayResult(DefaultIO *dio, CommandHelp *help1) : Command(dio) {
         this->help = help1;
     };
     void execute() override {
         string done = "done\n";
         int size = help->had->anomalyReport.size();
         for(int i = 0; i < size; i++) {
             string s = to_string(help->had->anomalyReport[i].timeStep) + " \t" + help->had->anomalyReport[i].description + "\n";
             dio->write(s);
         }
         dio->write(done);
     }
 };

 class uploadAnomaliesAndAnalyze : public Command {
 public:
     uploadAnomaliesAndAnalyze(DefaultIO *dio, CommandHelp*help1) : Command(dio) {
         this->help = help1;
     };
     void execute() override {
         string inputData, TPstring, FPstring;
         string uploadAnomalies = "Please up;oad your local anomalies file.\n";
         string complete = "Upload complete.\n";
         vector<pair<int, int> > timeStamps, anomalyRange;
         stringstream  TPs, FPs;
         long firstTimeStamp;
         float truePositiveRate, falseAlarmRate;
         int positive = 0;
         int negative = TimeSeries("anomalyTest.csv").allData.begin()->second.size();
         int FP = 0;
         int TP = 0;
         dio->write(uploadAnomalies);
         int size = help->had->anomalyReport.size() - 1;
         for (int i = 0; i < size; i++) {
             firstTimeStamp = help->had->anomalyReport[i].timeStep;
             string des1 = help->had->anomalyReport[i].description;
             string des2 = help->had->anomalyReport[i + 1].description;
             long time1 = help->had->anomalyReport[i + 1].timeStep;
             long time2 = help->had->anomalyReport[i].timeStep + 1;
             while (des1 == des2 && time1 == time2) {
                 i++;
             }
             pair<int, int> pair(firstTimeStamp, help->had->anomalyReport[i].timeStep);
             anomalyRange.push_back(pair);
         }
         inputData = dio->read();
         while (inputData != "done") {
             int pos = inputData.find(",");
             string string1 = inputData.substr(0, pos);
             string string2 = inputData.substr(pos + 1);
             pair<int, int> pair(stoi(string1), stoi(string2));
             timeStamps.push_back(pair);
             negative -= (stoi(string2) - stoi(string1));
             inputData= dio->read();
             positive++;
         }
         dio->write(complete);
         for (int i = 0; i < anomalyRange.size(); i++) {
             bool cut = false;
             for (int j = 0; j < timeStamps.size(); j++) {
                 if ((timeStamps[j].second >= anomalyRange[i].first && anomalyRange[i].second >= timeStamps[j].second) ||
                     (timeStamps[j].first >= anomalyRange[i].first && anomalyRange[i].second >= timeStamps[j].first) ||
                     (timeStamps[j].first <= anomalyRange[i].first && anomalyRange[i].second <= timeStamps[j].second)) {
                     TP++;
                     cut = true;
                 }
             }
             if (!cut) {
                 FP++;
             }
         }
         truePositiveRate = (TP * 1000 / positive) ;
         falseAlarmRate = (FP * 1000 / negative) ;
         truePositiveRate = floor(truePositiveRate);
         truePositiveRate = truePositiveRate/1000;
         TPs << truePositiveRate;
         TPstring = TPs.str();
         falseAlarmRate = floor(falseAlarmRate);
         falseAlarmRate = falseAlarmRate/1000;
         FPs << falseAlarmRate;
         FPstring = FPs.str();
         string truePositive = "True Positive Rate: " + TPstring +"\n";
         string falsePositive = "False Positive Rate: " + FPstring +"\n";
         dio->write(truePositive);
         dio->write(falsePositive);
     }

 };

class exitCLI : public Command {
public:
    explicit exitCLI(DefaultIO *dio) : Command(dio) {
    };

    void execute() override {
    }
};

#endif /* COMMANDS_H_ */