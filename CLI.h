

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"
#include "HybridAnomalyDetector.h"
#include "StandardIO.h"

using namespace std;

class CLI {
	DefaultIO* dio;
	HybridAnomalyDetector ad;
//	float corrThreshold = 0.9;
//    const char* csvTrain = R"(C:\Users\oziyo\CLionProjects\MileStone3)";
//   const char* csvTest = R"(C:\Users\oziyo\CLionProjects\MileStone3)";
	// you can add data members
public:
	CLI(DefaultIO* dio1);
	void start();
	virtual ~CLI();
};

#endif /* CLI_H_ */
