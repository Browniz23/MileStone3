

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <utility>
#include <vector>
#include "HybridAnomalyDetector.h"


using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){};

	// you may add additional methods here

	// upload string to destFile (must be open).
    virtual void uploadFile(string line, ofstream& destFile) {
        destFile << line;
    }
    virtual void read(int* d) {
        float f;
        read(&f);
        *d = (int)f;
    }
};

// you may add here helper classes

// class UnitedReports, contains reports: description, start of anomaly, end of anomaly.
class UnitedReports{
public:
    string description;
    int first;
    int last;
    UnitedReports(string d, int f, int l) {
        description = d;
        first = f;
        last = l;
    }
};

class DataCollection {
protected:
    HybridAnomalyDetector had;
    vector<AnomalyReport> ar;
    vector<UnitedReports> unitedAr;
    vector<pair<int,int>> anomalies;
    int colLength = 0;
    const char* trainFile;
    const char* testFile;
public:
    DataCollection() {}
    DataCollection(const char* file1, const char* file2) {
        trainFile = file1;
        testFile = file2;
    }
    friend class UniteAnomalies;
    friend class ReportsAnalysis;
    friend class ThresholdChangeCommand;
    friend class SendAnomalies;
    friend class TPFPCommand;
    friend class PrintAnomalyRepCommand;
    friend class DetectCommand;
    friend class UploadCSVCommand;
};

// you may edit this class
class Command{
protected:
	DefaultIO* dio;
	string description;
	DataCollection* data;
public:
	Command(DefaultIO* dio, DataCollection* d):dio(dio),data(d){}
    friend class DataCollection;
	virtual void execute()=0;
	virtual ~Command(){}
    string getDescription() {
        return description;
    }
};

// implement here your command classes

class PrintMenuCommand:public Command {
    vector<Command*> commands;
public:
    explicit PrintMenuCommand(DefaultIO* dio, DataCollection* d, vector<Command*> c) : Command(dio,d),commands(c) {}
    void execute() override {
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        for (Command* c : commands) {
            dio->write(c->getDescription());
        }
    }
};

// read file (got its name from description).
class TransferCSVFile: public Command {
public:
    explicit TransferCSVFile(DefaultIO* dio, DataCollection* d, const char* fileName) : Command(dio,d) {
        description = fileName;
    }
    void execute() override {
        ofstream destFile;
        destFile.open(description);
        string line = dio->read();              // here
        while (line != "done") {
            line.append("\n");
            dio->uploadFile(line, destFile);
            line = dio->read();
        }
        destFile.close();
    }
};

// MACRO 1. transfer 2 csv files.
class UploadCSVCommand: public Command {
public:
    explicit UploadCSVCommand(DefaultIO* dio, DataCollection* d) : Command(dio, d) {
        description = "1.upload a time series csv file\n";
    }
    void execute() override {
        // maybe can change PrintMessage so can set the message (and create one appearence).
        dio->write("Please upload your local train CSV file.\n");
        TransferCSVFile two(dio, data, data->trainFile);
        two.execute();
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
        TransferCSVFile five(dio, data, data->testFile);
        five.execute();
        dio->write("Upload complete.\n");
    }
};

// MACRO 2. changing threshold
class ThresholdChangeCommand: public Command {
public:
    ThresholdChangeCommand(DefaultIO *dio, DataCollection* d) : Command(dio, d){
        description = "2.algorithm settings\n";
    }
    void execute() override {
        dio->write("The current correlation threshold is ");
        dio->write(data->had.getMinForCorr());
        dio->write("\nType a new threshold\n");
        float newThreshold;
        dio->read(&newThreshold);
        while (newThreshold < 0 || newThreshold > 1) {
            dio->write("please choose a value between 0 and 1.");
            dio->read(&newThreshold);
        }
        data->had.setMinForCorr(newThreshold);
    }
};

// MACRO 3. learnNormal and detect anomalies.
class DetectCommand: public Command {
public:
    DetectCommand(DefaultIO *dio,DataCollection* d) : Command(dio,d){
        description = "3.detect anomalies\n";
    }
    void execute() override {
        TimeSeries tsTrain(data->trainFile);
        TimeSeries tsTest(data->testFile);
        data->had.learnNormal(tsTrain);
        data->ar = data->had.detect(tsTest);
        data->colLength = tsTest.getColLength();
        dio->write("anomaly detection complete.\n");
    }
};

// print AnomalyReports (description and time step).
class PrintAnomalyRepCommand: public Command {
public:
    PrintAnomalyRepCommand(DefaultIO *dio, DataCollection* d) : Command(dio, d){
        description = "4.display results\n";
    }
    void execute() override {
        for (AnomalyReport report : data->ar) {
            dio->write(report.timeStep);
            dio->write("\t ");
            dio->write(report.description);
            dio->write("\n");
        }
        dio->write("Done.\n");
    }
};

// create united reports from reports vector.
class UniteAnomalies: public Command {
public:
    explicit UniteAnomalies(DefaultIO* dio, DataCollection* dataC) : Command(dio,dataC) {}
    // base on ordered according description, and in same description according timestep.
    friend class DataCollection;
    void execute() override {
        vector<UnitedReports> unitedAr;
        for (AnomalyReport a : data->ar) {
            if (!unitedAr.empty() && unitedAr.back().description == a.description && unitedAr.back().last == a.timeStep-1) {
                unitedAr.back().last++;
            } else {
                unitedAr.push_back(UnitedReports(a.description, a.timeStep, a.timeStep));
            }
        }
        data->unitedAr = unitedAr;
    }
};

// create TP and FP rate, using anomalies vector and united reports.
class ReportsAnalysis: public Command {
public:
    explicit ReportsAnalysis(DefaultIO* dio, DataCollection* d) :Command(dio, d) {}
    void execute() override {
        float P = data->anomalies.size(), N = data->colLength;
        float FP =0, TP = 0;
        for (pair<int,int> foundAno : data->anomalies) {
            N -= foundAno.second - foundAno.first + 1;
        }
        for (UnitedReports reported : data->unitedAr) {
            bool isFalse = true;
            for (pair<int,int> foundAno : data->anomalies) {
                if ((reported.first >= foundAno.first && reported.first <= foundAno.second) ||
                    (reported.last >= foundAno.first && reported.last <= foundAno.second) ||
                    (reported.first <= foundAno.first && reported.last >= foundAno.second)) {
                    TP++;
                    isFalse = false;
                    break;
                }
            }
            if (isFalse)
                FP++;
        }
        float TPR = TP/P;
        TPR = floor(TPR*1000);
        TPR /= 1000;
        float FPR = FP/N;
        FPR = floor(FPR*1000);
        FPR /=1000;
        dio->write("True Positive Rate: ");
        dio->write(TPR);
        dio->write("\nFalse Positive Rate: ");
        dio->write(FPR);
        dio->write("\n");
    }
};

// gets lines of anomalies and create vector of anomalies (int start int end).
class SendAnomalies: public Command {
public:
    explicit SendAnomalies(DefaultIO* dio, DataCollection* d) : Command(dio, d) {}
    void execute() override {
        string line = dio->read();
        while (line != "done") {
            pair<int, int> anomaly;
            int pos = line.find(',');
            stringstream first(line.substr(0, pos));
            stringstream second(line.substr(pos+1));
            first >> anomaly.first;
            second >> anomaly.second;
            data->anomalies.push_back(anomaly);
            line = dio->read();
        }
    }
};

// MACRO 5. call unitedAnomalies, SendAnomalies and ReportAnalysis.
class TPFPCommand: public Command {
public:
    explicit TPFPCommand(DefaultIO* dio, DataCollection* d): Command(dio,d){
        description = "5.upload anomalies and analyze results\n";
    }
    void execute() override {
        UniteAnomalies unitedC(dio,data);
        unitedC.execute();
        dio->write("Please upload your local anomalies file.\n");
        data->anomalies.clear();
        SendAnomalies c1(dio,data);
        c1.execute();
        dio->write("Upload complete.\n");
        ReportsAnalysis re(dio,data);
        re.execute();
    }
};

class ExitCommand:public Command {
public:
    explicit ExitCommand(DefaultIO* dio, DataCollection* d): Command(dio,d) {
        description = "6.exit\n";
    }
    void execute() override {
        // free memory if needed
    }
};

#endif /* COMMANDS_H_ */
