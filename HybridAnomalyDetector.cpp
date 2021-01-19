/**
 * @author : Dvir Asaf 313531113.
 */
#include "HybridAnomalyDetector.h"
HybridAnomalyDetector::HybridAnomalyDetector() {
    threshold = 0.9;
}

HybridAnomalyDetector::~HybridAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

void HybridAnomalyDetector::learnHelper(float pearson, Point **ps, int size, string s1, string s2, correlatedFeatures ct){
    if (pearson >= threshold){
        SimpleAnomalyDetector::learnHelper(pearson, ps, size, s1, s2, ct);
    }
    else if (0.5 < pearson && pearson < threshold && ct.corrlation < pearson){
        Circle c = findMinCircle(ps, size);
        ct.feature1 = s1;
        ct.feature2 = s2;
        ct.radius = c.radius;
        ct.center = c.center;
        ct.corrlation = pearson;
        ct.lin_reg = linear_reg(ps, size);
        if (ct.threshold < ct.radius) {
            ct.threshold =  ct.radius * 1.1;
        }
        cf.push_back(ct);
    }
    return;
}

bool HybridAnomalyDetector::pearsonResult(float pearson, float min) {
    if(0.5 < pearson && pearson < threshold){
        return true;
    }
    else if(pearson >= threshold) {
        return SimpleAnomalyDetector::pearsonResult(pearson, min);
    }
    return false;
}

bool HybridAnomalyDetector::isAnomalous(correlatedFeatures c, float x, float y) {
    if (c.corrlation >= threshold) {
        return SimpleAnomalyDetector::isAnomalous(c, x, y);
    }
    else if(0.5 < c.corrlation) {
        float powX = pow(x - c.center.x, 2);
        float powY = pow(y - c.center.y, 2);
        float dis = sqrt(powX + powY);
        return (dis > c.threshold);
    }
    return false;
}