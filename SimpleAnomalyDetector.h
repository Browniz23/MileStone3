// Oz Browner 316482355

#ifndef FLIGHTSIMULATOR_SIMPLEANOMALYDETECTOR_H
#define FLIGHTSIMULATOR_SIMPLEANOMALYDETECTOR_H

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
    string feature1,feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
    //todo @@@@@@@@@@@@@@@@@@@@@@@@@@@@
    float x,y;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
    vector<correlatedFeatures> cf;
public:
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();

    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);

    vector<correlatedFeatures> getNormalModel(){
        return cf;
    }
    // todo new methods
    virtual float getPointDis(Point p, correlatedFeatures correlated);
    correlatedFeatures createCorrFeaFromScratch(string fea, string matchedFea, map<string, vector<float>> table,
                                     vector<string> features, int colSize, float corr);
    virtual correlatedFeatures createCorrFeaFromPoints(Point** points, string fea, string matchedFea, int colSize, float corr);
    virtual bool isHighCorr(float corr);


    //virtual void fillCorrelated(correlatedFeatures* correlatedP, Point** points, int colSize, float corr);
};

#endif //FLIGHTSIMULATOR_SIMPLEANOMALYDETECTOR_H