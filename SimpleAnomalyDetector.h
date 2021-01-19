/**
 * @author : Dvir Asaf 313531113.
 */
#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_
#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "minCircle.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>
struct correlatedFeatures{
    string feature1,feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
    Point center;
    float radius;
};
class SimpleAnomalyDetector : public TimeSeriesAnomalyDetector{
protected:
    vector<correlatedFeatures> cf;

    float threshold;
public:
    vector<AnomalyReport> anomalyReport;
    //constructor
    SimpleAnomalyDetector();
    //destructor
    virtual ~SimpleAnomalyDetector();
    //check which columns have the best correlation update feature1,feature2 and corrlation, and create array of points
    // according to the columns vector, and update lin_reg.
    //according to the line and points array send them to dev and update the struct newThreshold.
    virtual void learnNormal(const TimeSeries& ts);
    //according to the values of the struct correlatedFeatures, create points are given in time series and check if the
    // newThreshold of each point is higher the the value of cf.newThreshold, if yes return anomaly report.
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    virtual void learnHelper(float pearson, Point **ps, int size, string s1, string s2, correlatedFeatures c) ;
    virtual bool pearsonResult(float pearson, float min);
    virtual float getThreshold(Point **ps, correlatedFeatures c, int size, Line l);
    virtual bool isAnomalous(correlatedFeatures c, float x, float y);
    virtual void deleteArray(Point **p, int size);
    vector<correlatedFeatures> getNormalModel() {
        return cf;
    }
};
#endif /* SIMPLEANOMALYDETECTOR_H_ */