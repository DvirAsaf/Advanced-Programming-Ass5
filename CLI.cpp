/**
 * @author : Dvir Asaf 313531113.
 */
#include "CLI.h"

CLI::CLI(DefaultIO *dio) {
    this->dio = dio;
    CommandHelp commandHelp;
    this->help = commandHelp;
    this->comArr[0] = new uploadTimeSeriesCSVFile(dio);
    this->comArr[1] = new algoSetting(dio, &help);
    this->comArr[2] = new anomaliesDetection(dio, &help);
    this->comArr[3] = new displayResult(dio, &help);
    this->comArr[4] = new anomaliesAndAnalyze(dio, &help);
    this->comArr[5] = new exitFromCLI(dio);
}

void CLI::start() {
    int inputFromUser;
    bool exit = false;
    string comNumber;
    string menu =  "Welcome to the Anomaly Detection Server.\n"
                    "Please choose an option:\n"
                    "1. upload a time series csv file\n"
                    "2. algorithm settings\n"
                    "3. detect anomalies\n"
                    "4. display results\n"
                    "5. upload anomalies and analyze results\n"
                    "6. exit\n";
    while (exit != true) {
        this->dio->write(menu);
        comNumber = this->dio->read();
        inputFromUser = stoi(comNumber);
        inputFromUser -= 1;
        if (comNumber == "6") {
            exit = true;
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