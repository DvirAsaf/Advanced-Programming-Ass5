
/**
 * @author : Dvir Asaf 313531113.
 */
#ifndef COMMANDS_H_
#define COMMANDS_H_
#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <memory>
#include "HybridAnomalyDetector.h"
class DefaultIO {
public:
    virtual string read() = 0;
    virtual void write(string text) = 0;
    virtual void write(float f) = 0;
    virtual void read(float *f) = 0;
    virtual ~DefaultIO() {}
};
class CommandHelp {
public:
    double newThreshold;
    HybridAnomalyDetector *detector;
    CommandHelp(){
        detector = new HybridAnomalyDetector;
    };
};

class Command {
protected:
    DefaultIO *dio;
    CommandHelp *help;
public:
    Command(DefaultIO *dio) : dio(dio) {}
    virtual void execute() = 0;
    virtual ~Command() {}
};

class uploadTimeSeriesCSVFile : public Command {
public:
    explicit uploadTimeSeriesCSVFile(DefaultIO *dio) : Command(dio) {
    };

    void execute() override {
        ofstream anomalyTrainCSV;
        ofstream anomalyTestCSV;
        anomalyTrainCSV.open("anomalyTrain.csv");
        dio->write("Please upload your local train CSV file.\n");
        string input = dio->read();
        while (input != "done") {
            anomalyTrainCSV << input << endl;
            input = dio->read();
        }
        dio->write("Upload complete.\n");
        anomalyTestCSV.open("anomalyTest.csv");
        dio->write("Please upload your local test CSV file.\n");
        input = dio->read();
        while (input != "done") {
            anomalyTestCSV << input << endl;
            input = dio->read();
        }
        dio->write("Upload complete.\n");
        anomalyTrainCSV.close();
        anomalyTestCSV.close();
    }
};

class algoSetting : public Command {
public:
    algoSetting(DefaultIO *dio, CommandHelp *help1) : Command(dio) {
        this->help = help1;
    };
    void execute() override {
        //show the correlactiom and ask for new correlaction
        dio->write("The current correlation threshold is 0.9\n"
                   "Type a new threshold\n");
        help->newThreshold = stof(dio->read());
        while (help->newThreshold <= 0 || help->newThreshold >= 1) {
            dio->write("please choose a value between 0 and 1.\n");
            help->newThreshold = stof(dio->read());
        }
    }
};

class anomaliesDetection : public Command {
public:
    anomaliesDetection(DefaultIO *dio, CommandHelp *help1) : Command(dio) {
        this->help = help1;
    };
    void execute() override {
        help->detector->learnNormal(TimeSeries("anomalyTrain.csv"));
        help->detector->detect(TimeSeries("anomalyTest.csv"));
        dio->write("anomaly detection complete.\n");
    }
};

class displayResult : public Command {
public:
    displayResult(DefaultIO *dio, CommandHelp *help1) : Command(dio) {
        this->help = help1;
    };
    void execute() override {
        int size = help->detector->anomalyReport.size();
        vector<AnomalyReport> an = help->detector->anomalyReport;
        for(int i = 0; i < size; i++) {
            dio->write(to_string(an[i].timeStep) + "\t " + an[i].description + "\n");
        }
        dio->write("Done.\n");
    }
};

class anomaliesAndAnalyze : public Command {
public:
    anomaliesAndAnalyze(DefaultIO *dio, CommandHelp *help1) : Command(dio) {
        this->help = help1;
    };
    void execute() override {
        //TP = true positive, FP = false positive, FA = false alarm, fix = for printing 3 digit after the point
        int fix = 1000;
        string input, outputTP, outputFP;
        long firstMark;
        stringstream  TPss, FPss;
        float SizeOfTP, sizeOfFA;
        //reset the value of the counters.
        int posi = 0, FP = 0, TP = 0;
        //2 vectors for help the check between the reports from anomaly and the user anomaly.
        vector<pair<int, int>> timeMarks;
        vector<pair<int, int>> rangeOfAnomaly;
        int neg = TimeSeries("anomalyTest.csv").allData.begin()->second.size();
        dio->write("Please upload your local anomalies file.\n");
        int size = help->detector->anomalyReport.size();
        vector<AnomalyReport> an = help->detector->anomalyReport;
        for (int i = 0; i < size; i++) {
            firstMark = an[i].timeStep;
            while (an[i].description == an[i + 1].description && (an[i].timeStep + 1) == an[i + 1].timeStep) {
                i++;
            }
            pair<int, int> p(firstMark, an[i].timeStep);
            rangeOfAnomaly.push_back(p);
        }
        input = dio->read();
        while (input != "done") {
            //separate timeMarks by using ','.
            int separator = input.find(',');
            string string1 = input.substr(0, separator);
            string string2 = input.substr(separator + 1);
            //insert to timeMarks the valuesin range string1 to string 2.
            // s2i = string to int.
            int s2i1 = stoi(string1);
            int s2i2 = stoi(string2);
            pair<int, int> p(s2i1, s2i2);
            timeMarks.push_back(p);
            neg -= (s2i2 - s2i1);
            input = dio->read();
            //increase posi by 1;
            posi++;
        }
        dio->write("Upload complete.\n");
        //checking our anomaly report vs. user report
        int range = rangeOfAnomaly.size();
        int marks = timeMarks.size();
        for (int i = 0; i < range; i++) {
            bool cut = false;
            for (int j = 0; j < marks; j++) {
                //in this condition we check if the user range has coincidence with 1 of the reports from anomaly.
                if ((timeMarks[j].second >= rangeOfAnomaly[i].first && rangeOfAnomaly[i].second >= timeMarks[j].second) ||
                    (timeMarks[j].first >= rangeOfAnomaly[i].first && rangeOfAnomaly[i].second >= timeMarks[j].first) ||
                    (timeMarks[j].first <= rangeOfAnomaly[i].first && rangeOfAnomaly[i].second <= timeMarks[j].second)) {
                    //increase TP
                    TP++;
                    cut = true;
                }
            }
            //FP report, increase FP
            if (!cut) {
                FP++;
            }
        }
        //negative
        sizeOfFA = (FP * fix / neg);
        sizeOfFA = floor(sizeOfFA);
        sizeOfFA = sizeOfFA / fix;
        FPss << sizeOfFA;
        outputFP = FPss.str();
        //positive
        SizeOfTP = (TP * fix / posi);
        SizeOfTP = floor(SizeOfTP);
        SizeOfTP = SizeOfTP / fix;
        TPss << SizeOfTP;
        outputTP = TPss.str();
        //printing
        dio->write("True Positive Rate: " + outputTP +"\n");
        dio->write("False Positive Rate: " + outputFP +"\n");
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