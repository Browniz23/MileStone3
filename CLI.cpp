#include "CLI.h"

CLI::CLI(DefaultIO* dio1) {
    dio = dio1;
}

void printMenu(DefaultIO* dio) {
    dio->write("Welcome to the Anomaly Detection Server.\n");
    dio->write("Please choose an option:\n");
    dio->write("1.upload a time series csv file\n");
    dio->write("2.algorithm settings\n");
    dio->write("3.detect anomalies\n");
    dio->write("4.display results\n");
    dio->write("5.upload anomalies and analyze results\n");
    dio->write("6.exit\n");
}
void CLI::start(){
    int option;
  //  DataCollection data(dio);
    PrintMenuCommand printMenu(dio);
    UploadCSVCommand c1(dio);
    ThresholdChangeCommand c2(dio, &ad);
    DetectCommand c3(dio,&ad);
    vector<AnomalyReport> ar;
    do {
        printMenu.execute();
        PrintAnomalyRepCommand c4(dio, ar);
        TPFPCommand c5(dio, c3.getTsTest(), c3.getAnomalyRep());
        dio->read(&option);
        switch (option) {
            case 1:
                c1.execute();
                break;
            case 2:
                c2.execute();
                break;
            case 3:
                c3.execute();
                ar = c3.getAnomalyRep();
                break;
            case 4:
                c4.execute();
                break;
            case 5:
                c5.execute();
                break;
            case 6:
                break;
            default:
                break;  //todo: wrong input?
        }
    } while (option != 6);
}


CLI::~CLI() {
}

