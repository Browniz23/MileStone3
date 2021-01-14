

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
    // new methods
    virtual void setMinForCorr(float min) {
        minForCorr = min;
    }
    virtual float getMinForCorr() {
        return minForCorr;
    }
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
