// Oz Browner 316482355

#ifndef FLIGHTSIMULATOR_ANOMALYDETECTOR_H
#define FLIGHTSIMULATOR_ANOMALYDETECTOR_H

#include <string>
#include <vector>
#include "timeseries.h"
#include "math.h"
using namespace std;


class AnomalyReport{
public:
    const string description;
    const long timeStep;
    AnomalyReport(string description, long timeStep):description(description),timeStep(timeStep){}
};

class TimeSeriesAnomalyDetector {
public:
    virtual void learnNormal(const TimeSeries& ts)=0;
    virtual vector<AnomalyReport> detect(const TimeSeries& ts)=0;
    virtual ~TimeSeriesAnomalyDetector(){}
};

#endif //FLIGHTSIMULATOR_ANOMALYDETECTOR_H