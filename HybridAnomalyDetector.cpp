
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {}

HybridAnomalyDetector::~HybridAnomalyDetector() {}

float HybridAnomalyDetector::getPointDis(Point p, correlatedFeatures correlated) {
    if (correlated.corrlation < 0.9) {   // todo: fix 1.2,0.9 to generic
        return distance(Point(correlated.x,correlated.y), p);
    }
    return SimpleAnomalyDetector::getPointDis(p,correlated);    //todo: going to father?
}

correlatedFeatures HybridAnomalyDetector::createCorrFeaFromPoints(Point** points, string fea, string matchedFea, int colSize, float corr) {
    if (corr < 0.9) {
        Circle c = findMinCircle(points, colSize);
        // making threshold a bit bigger for negligible cases.
        float threshold = c.radius*1.1;                 //todo: changed only here to 1.1!
        Line l = Line();
        return correlatedFeatures{fea, matchedFea, corr, l, threshold, c.center.x, c.center.y};
    }
    return SimpleAnomalyDetector::createCorrFeaFromPoints(points, fea, matchedFea, colSize, corr);
}

bool HybridAnomalyDetector::isHighCorr(float corr) {
    return corr >= 0.5;
}



/*
correlatedFeatures HybridAnomalyDetector::createCorrFea(string fea, string matchedFea, map<string, vector<float>> table,
                                         vector<string> features, int colSize, float corr) {
    cout << "3";
    return SimpleAnomalyDetector::createCorrFea(fea, matchedFea, table, features, colSize, corr);
}*//*
void HybridAnomalyDetector::fillCorrelated(correlatedFeatures* correlatedP, Point** points,
                                           int colSize, float corr) {
    cout << "2";
    if (corr < 0.9) {
        correlatedP->lin_reg = Line();                      //needed?
        Circle c = findMinCircle(points, colSize);
        // making threshold a bit bigger for negligible cases.
        correlatedP->threshold = c.radius*1.2;
        correlatedP->x = c.center.x;
        correlatedP->y = c.center.y;
    }
}*/