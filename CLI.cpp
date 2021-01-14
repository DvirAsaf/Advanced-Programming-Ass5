#include "CLI.h"

CLI::CLI(DefaultIO *dio) {
    this->dio = dio;
    CommandHelp commandHelp;
    this->help = commandHelp;
    this->comArr[0] = new uploadCSV(dio);
    this->comArr[1] = new algorithmSetting(dio, &help);
    this->comArr[2] = new detectAnomalies(dio, &help);
    this->comArr[3] = new displayResult(dio, &help);
    this->comArr[4] = new uploadAnomaliesAndAnalyze(dio, &help);
    this->comArr[5] = new exitCLI(dio);
}

void CLI::start() {
    int inputFromUser;
    bool exitCLI = false;
    string comNumber;
    string menu =  "Welcome to the Anomaly Detection Server.\n"
                    "Please choose an option:\n"
                    "1.upload a time series csv file\n"
                    "2.algorithm settings\n"
                    "3.detect anomalies\n"
                    "4.display results\n"
                    "5.upload anomalies and analyze results\n"
                    "6.exit\n";
    while (exitCLI != true) {
        this->dio->write(menu);
        comNumber = this->dio->read();
        inputFromUser = stoi(comNumber);
        inputFromUser -= 1;
        if (comNumber == "6") {
            exitCLI = true;
        }
        else if (inputFromUser >= 0 && inputFromUser <=4) {
            this->comArr[inputFromUser]->execute();
        }
    }
}

CLI::~CLI() {
    for (auto & i : comArr){
        delete i;
    }
}