
/**
 * @author : Dvir Asaf 313531113.
 */
#include "CLI.h"
#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <memory>
#include "HybridAnomalyDetector.h"

CLI::CLI(DefaultIO *dio) {
    this->dio = dio;
    help = new CommandHelp;
    this->comArr[0] = new uploadTimeSeriesCSVFile(dio);
    this->comArr[1] = new algoSetting(dio, help);
    this->comArr[2] = new anomaliesDetection(dio, help);
    this->comArr[3] = new displayResult(dio, help);
    this->comArr[4] = new anomaliesAndAnalyze(dio, help);
    this->comArr[5] = new exitFromCLI(dio);
}

void CLI::start() {
    dio->write("Welcome to the Anomaly Detection Server.\n"
               "Please choose an option:\n"
               "1.upload a time series csv file\n"
               "2.algorithm settings\n"
               "3.detect anomalies\n"
               "4.display results\n"
               "5.upload anomalies and analyze results\n"
               "6.exit\n");
    string chooseNumber = dio->read();
    int input = stoi(chooseNumber);
    -- input;
    comArr[input]->execute();
    if (chooseNumber == "6") {
        return;
    }
    return CLI::start();
}

CLI::~CLI() {
    for (auto & i : comArr){
        delete i;
    }
}