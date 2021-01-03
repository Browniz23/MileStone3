

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
	virtual ~DefaultIO(){};             //todo: why???????? if =0 problem. now diferent one.

	// you may add additional methods here

	// upload string to destFile (needs to be open). but we sent string!!!
    virtual void uploadFile(string line, ofstream& destFile) {
        destFile << line;
    }
    virtual void read(int* d) {
        float f;
        read(&f);
        *d = (int)f;
    }
};

/*
class StandardIO: public DefaultIO {        // add virtual? * 3?
public:
    StandardIO();
    ~StandardIO() override;
    string read() override {
        string text;
        cin >> text;
        return text;
    }
    void write(string text) override {
        cout << text;
    }
    void write(float f) override {
        cout << f;
    }
    void read(float* f) override {
        cin >> *f;
    }
};*/
// you may add here helper classes


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

class DataCollection {              // needed?
    DefaultIO* dio;
    HybridAnomalyDetector* had;
    vector<AnomalyReport> ar;
    vector<UnitedReports> unitedAr;
    vector<pair<int,int>> anomalies;
public:
    DataCollection() {}
    friend class UniteAnomalies;

    HybridAnomalyDetector* getHybridAnomalyDetector() {
        return had;
    }
    vector<AnomalyReport> getAnomalyReports() {
        return ar;
    }
    vector<UnitedReports> getUnitedReports() {
        return unitedAr;
    }
    vector<pair<int,int>> getAnomalies() {
        return anomalies;
    }
};

class DataCollection;
// you may edit this class
class Command{
protected:
	DefaultIO* dio;
	string description;
public:
	Command(DefaultIO* dio):dio(dio){}
	virtual void execute()=0;
	virtual ~Command(){}
    string getDescription() {
        return description;
    }
    friend class DataCollection;
};

// implement here your command classes

// gets reports and have united reports
class UniteAnomalies: public Command {
    //vector<AnomalyReport> ar;
    // int counts num of anomalies per description.
    //vector<UnitedReports> unitedAr;
    DataCollection data;
public:
    explicit UniteAnomalies(DefaultIO* dio/*, vector<AnomalyReport> reps*/, DataCollection dataC) : Command(dio)/*,ar(reps)*/, data(dataC) {}
    // base on ordered according description, and in same description according timestep.
    void execute() override {
        vector<AnomalyReport> ar = data.ar;
        vector<UnitedReports> unitedAr;
        for (AnomalyReport a : ar) {
            if (!unitedAr.empty() && unitedAr.back().description == a.description && unitedAr.back().last == a.timeStep-1) {
                unitedAr.back().last++;
            } else {
                unitedAr.push_back(UnitedReports(a.description, a.timeStep, a.timeStep));
            }
        }
        data.unitedAr = data.unitedAr;
    }

    vector<UnitedReports> getUnitedReports() {
     //   return unitedAr;
    }
};

class ReportsAnalysis: public Command {              // needed?
    //TimeSeries ts;
    //vector<UnitedReports> unitedAr;
    //vector<pair<int,int>> anomalies;
    TimeSeries ts;
    DataCollection data;
public:
    explicit ReportsAnalysis(DefaultIO* dio, DataCollection d, TimeSeries rows) :Command(dio), data(d),ts(rows)/*, vector<UnitedReports> united,
                             vector<pair<int,int>> known, TimeSeries rows) :Command(dio),ts(rows) */{
        //unitedAr = united;
        //anomalies = known;
    }
    void execute() override {
        vector<UnitedReports> unitedAr = data.getUnitedReports();
        vector<pair<int,int>> anomalies = data.getAnomalies();
        float P = anomalies.size(), N = ts.getColLength();
        float FP =0, TP = 0;
        for (pair<int,int> foundAno : anomalies) {
            N -= foundAno.second - foundAno.first + 1;
        }
        for (UnitedReports reported : unitedAr) {
            bool isFalse = true;
            for (pair<int,int> foundAno : anomalies) {
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

// gets lines of anomalies and have vector of anomalise (int int).
class SendAnomalies: public Command {              // needed?
    HybridAnomalyDetector* had;
    vector<AnomalyReport> ar;
    vector<pair<int,int>> anomalies;
public:
    explicit SendAnomalies(DefaultIO* dio) : Command(dio) {}
    void execute() override {
        string line = dio->read();
        while (line != "done") {
            pair<int, int> anomaly;
            int pos = line.find(',');
            stringstream first(line.substr(0, pos));
            stringstream second(line.substr(pos+1));
            first >> anomaly.first;
            second >> anomaly.second;
            anomalies.push_back(anomaly);
            line = dio->read();
        }
    }

    vector<pair<int,int>> getAnomalies() {
        return anomalies;
    }
};

class TPFPCommand: public Command {
    vector<UnitedReports> united;
    vector<pair<int,int>> anomalies;
    TimeSeries ts;
    DataCollection d;
public:
    explicit TPFPCommand(DefaultIO* dio, TimeSeries rows, vector<AnomalyReport> reports, DataCollection d)
            : Command(dio),ts(rows),d(d) {
        UniteAnomalies unitedC(dio,d);//,reports);
        unitedC.execute();
        united = unitedC.getUnitedReports();
    }

    void execute() override {
        dio->write("Please upload your local anomalies file.\n");
        SendAnomalies c1(dio);
        c1.execute();
        dio->write("Upload complete.\n");
        anomalies = c1.getAnomalies();
        ReportsAnalysis re(dio,d,ts);//, united, anomalies, ts);
        re.execute();
    }
};

class PrintMenuCommand:public Command {
public:
    explicit PrintMenuCommand(DefaultIO* dio) : Command(dio) {}
    void execute() override {
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        dio->write("1.upload a time series csv file\n");
        dio->write("2.algorithm settings\n");
        dio->write("3.detect anomalies\n");
        dio->write("4.display results\n");
        dio->write("5.upload anomalies and analyze results\n");
        dio->write("6.exit\n");
    }
};

class PrintMessage: public Command {
public:
    explicit PrintMessage(DefaultIO* dio, string des) : Command(dio) {
            description = des;
    }
    void execute() override {
        dio->write(description);
    }
};

// maybe need to split to 2 commands
class TransferCSVFile: public Command {  // i think we ignore the "open file" and considering its open.
public:
    explicit TransferCSVFile(DefaultIO* dio, string fileName) : Command(dio) {
        description = fileName;   // todo ?
    }
    void execute() override {           // todo: maybe line includes\need to includes \n.
        ofstream destFile;
        //destFile.open(description); // needs to open in server (in folder?)
        destFile.open(description);
        string line = dio->read();
        while (line != "done") {                    // todo: \n?
            line.append("\n");
            dio->uploadFile(line, destFile);            // hope its fine!     //@@@@@@@@@@@@@@@@@@@@@@@@@@22
           // destFile << line;
            line = dio->read();
        }
        destFile.close();
    }
};

// now class for reading file and class for sending file?
class UploadCSVCommand: public Command {
public:
    explicit UploadCSVCommand(DefaultIO* dio) : Command(dio) {
        description = "1. upload a time series csv file\n";
    }
    void execute() override {
        // maybe can change PrintMessage so can set the message (and create one appearence).
        PrintMessage one(dio, "Please upload your local train CSV file.\n");  // need to be to client not dio!
        TransferCSVFile two(dio,"anomalyTrain.csv");    // need to be to client not dio!
        PrintMessage three(dio, "Upload complete.\n");
        PrintMessage four(dio, "Please upload your local test CSV file.\n");  // need to be to client not dio!
        TransferCSVFile five(dio, "anomalyTest.csv");    // need to be to client not dio!
        PrintMessage six(dio, "Upload complete.\n");
        one.execute();
        two.execute();
        three.execute();
        four.execute();
        five.execute();
        six.execute();
    }
};


class ThresholdChangeCommand: public Command {
    HybridAnomalyDetector* ad;          //  changed to pointer
public:
    ThresholdChangeCommand(DefaultIO *dio, HybridAnomalyDetector* had) : Command(dio), ad(had){};
    void execute() override {
        dio->write("The current correlation threshold is ");
        dio->write(ad->getMinForCorr());
        dio->write("\nType a new threshold\n");
        float newThreshold;
        dio->read(&newThreshold);
        while (newThreshold < 0 || newThreshold > 1) {
            dio->write("please choose a value between 0 and 1.");
            dio->read(&newThreshold);
        }
        ad->setMinForCorr(newThreshold);
    }
};

class DetectCommand: public Command {
    HybridAnomalyDetector* ad;              // chenged o pointer
    vector<AnomalyReport> ar;
    TimeSeries tsTrain = TimeSeries("anomalyTrain.csv");        // todo: how do i move the line from the server(output) to ts???
    TimeSeries tsTest = TimeSeries("anomalyTest.csv");
public:
    DetectCommand(DefaultIO *dio, HybridAnomalyDetector* had) : Command(dio), ad(had){};
    void execute() override {

        ad->learnNormal(tsTrain);// todo: maybe send every line (in first) to file we create on server? with fileIO
        ar = ad->detect(tsTest);
        dio->write("anomaly detection complete.\n");
    }
    vector<AnomalyReport> getAnomalyRep() {
        return ar;
    }

    TimeSeries getTsTrain() {
        return tsTrain;
    }
    TimeSeries getTsTest() {
        return tsTest;
    }
};

class PrintAnomalyRepCommand: public Command {
    vector<AnomalyReport> ar;
public:
    PrintAnomalyRepCommand(DefaultIO *dio, vector<AnomalyReport> reports) : Command(dio), ar(reports){};
    void execute() override {
        for (AnomalyReport report : ar) {
            dio->write(report.timeStep);
            dio->write("\t ");
            dio->write(report.description);
            dio->write("\n");
        }
        dio->write("Done.\n");      // \n?
    }
};

#endif /* COMMANDS_H_ */
