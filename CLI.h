

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"
#include "HybridAnomalyDetector.h"

using namespace std;

class CLI {
	DefaultIO* dio;
	HybridAnomalyDetector ad;
    const char* csvTrain = "anomalyTrain.csv";
    const char* csvTest = "anomalyTest.csv";
	// you can add data members
public:
	CLI(DefaultIO* dio1);
	void start();
	virtual ~CLI();
};

#endif /* CLI_H_ */
