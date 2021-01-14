// 316482355 Oz Browner

#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {}

HybridAnomalyDetector::~HybridAnomalyDetector() {}

float HybridAnomalyDetector::getPointDis(Point p, correlatedFeatures correlated) {
    if (correlated.corrlation < minForCorr) {
        return distance(Point(correlated.x,correlated.y), p);
    }
    return SimpleAnomalyDetector::getPointDis(p,correlated);
}

correlatedFeatures HybridAnomalyDetector::createCorrFeaFromPoints(Point** points, string fea, string matchedFea, int colSize, float corr) {
    if (corr < minForCorr) {
        Circle c = findMinCircle(points, colSize);
        // making threshold a bit bigger for negligible cases.
        float threshold = c.radius*1.1;
        Line l = Line();
        return correlatedFeatures{fea, matchedFea, corr, l, threshold, c.center.x, c.center.y};
    }
    return SimpleAnomalyDetector::createCorrFeaFromPoints(points, fea, matchedFea, colSize, corr);
}

bool HybridAnomalyDetector::isHighCorr(float corr) {
    // in hybridDetector circle always 0.5
    if (minForCorr > 0.5)
        return corr >= 0.5;
    return corr >= minForCorr;
}
