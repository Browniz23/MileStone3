#include "CLI.h"

CLI::CLI(DefaultIO* dio1) {
    dio = dio1;
}

void CLI::start(){
    int option;
    DataCollection d(csvTrain, csvTest);
    vector<Command*> macroCommands;
    UploadCSVCommand c1(dio, &d);
    macroCommands.push_back(&c1);
    ThresholdChangeCommand c2(dio, &d);
    macroCommands.push_back(&c2);
    DetectCommand c3(dio, &d);
    macroCommands.push_back(&c3);
    PrintAnomalyRepCommand c4(dio, &d);
    macroCommands.push_back(&c4);
    TPFPCommand c5(dio,&d);
    macroCommands.push_back(&c5);
    ExitCommand c6(dio,&d);
    macroCommands.push_back(&c6);
    PrintMenuCommand printMenu(dio,&d, macroCommands);
    do {
        printMenu.execute();
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
                break;
            case 4:
                c4.execute();
                break;
            case 5:
                c5.execute();
                break;
            case 6:
                c6.execute();
                break;
            default:
                break;
        }
    } while (option != 6);
}

CLI::~CLI() {}
