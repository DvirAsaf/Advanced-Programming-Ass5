/**
 * @author : Dvir Asaf 313531113.
 */
#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_
#include "SimpleAnomalyDetector.h"
#include "minCircle.h"
class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
    HybridAnomalyDetector();
    virtual ~HybridAnomalyDetector();
    virtual void learnHelper(float pearson, Point **ps, int size, string s1, string s2, correlatedFeatures ct);
    virtual bool pearsonResult(float pearson, float min);
    virtual bool isAnomalous(correlatedFeatures c, float x, float y);
};
#endif /* HYBRIDANOMALYDETECTOR_H_ */