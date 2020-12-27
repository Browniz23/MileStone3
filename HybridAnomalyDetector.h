

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"
#include "anomaly_detection_util.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
    virtual float getPointDis(Point p, correlatedFeatures correlated); // todo: virtual?
    virtual correlatedFeatures createCorrFeaFromPoints(Point** points, string fea, string matchedFea, int colSize, float corr);
    virtual bool isHighCorr(float corr);


    //virtual correlatedFeatures createCorrFea(string fea, string matchedFea, map<string, vector<float>> table,
    //vector<string> features, int colSize, float corr);
    //virtual void fillCorrelated(correlatedFeatures* correlatedP, Point** points, int colSize, float corr);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
