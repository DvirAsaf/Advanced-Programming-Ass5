/**
 * @author : Dvir Asaf 313531113.
 */
#include "SimpleAnomalyDetector.h"
// constructor
SimpleAnomalyDetector::SimpleAnomalyDetector() {
    threshold = 0.9;
}
// destructor
SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

void SimpleAnomalyDetector::deleteArray(Point **p, int size){
    for(int k = 0; k < size; k++) {
        delete(p[k]);
    }
}

float SimpleAnomalyDetector::getThreshold(Point **ps, correlatedFeatures c, int size, Line l) {
    for(int j = 0; j < size; j++) {
        float help = dev(*ps[j], l);
        if (c.threshold < help){
            c.threshold = help;
        }
    }
    return (c.threshold * 1.1);
}

void SimpleAnomalyDetector::learnHelper(float pearson, Point **ps, int size, string s1, string s2, correlatedFeatures c) {
    if(pearson >= threshold && c.corrlation < pearson) {
//        correlatedFeatures c;
        c.feature1 = s1;
        c.feature2 = s2;
        c.corrlation = pearson;
        c.lin_reg = linear_reg(ps, size);
        c.threshold = getThreshold(ps, c, size, c.lin_reg);
        cf.push_back(c);
    }
}

// check which columns have the best correlation update feature1,feature2 and
// corrlation, and create array of points
// according to the columns vector, and update lin_reg.
// according to the line and points array send them to dev and update the struct
// newThreshold.
void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    map<string, vector<float>> time = ts.allData;
    // size of how much values in each vector.
    int vectors_size = time.begin()->second.size();
    Point *ps[vectors_size];
    for (auto col_1 = time.begin(); col_1 != time.end(); col_1++) {
        struct correlatedFeatures ct;
        ct.corrlation = 0.0;
        ct.threshold = 0.0;
        ct.radius = 0.0;
        for (auto col_2 = next(col_1); col_2 != time.end(); col_2++) {
            if (col_1->first == col_2->first) {
                continue;
            }
            float *vector_ptr_A = col_1->second.data();
            float *vector_ptr_B = col_2->second.data();
            float result = fabs(pearson(vector_ptr_A, vector_ptr_B, vectors_size));
            if (pearsonResult(result, ct.corrlation)) {
                // make array of points with the best correlation in the map.
                for (int i = 0; i < vectors_size; i++) {
                    ps[i] = new Point(vector_ptr_A[i], vector_ptr_B[i]);
                }
                learnHelper(result, ps, vectors_size, col_1->first, col_2->first, ct);
            }
        }
    }
    deleteArray(ps, vectors_size);
}

bool SimpleAnomalyDetector::pearsonResult(float pearson, float min) {
    if(pearson >= threshold && min < pearson){
        return true;
    }
    return false;
}

bool SimpleAnomalyDetector::isAnomalous(correlatedFeatures c, float x, float  y) {
    return (dev(Point(x, y), c.lin_reg) > c.threshold);
}

// according to the values of the struct correlatedFeatures, create points are
// given in time series and check if the
// newThreshold of each point is higher the the value of cf.newThreshold, if yes
// return anomaly report.
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    int size = ts.allData.begin()->second.size();
    vector<AnomalyReport> vector_Anomaly_Report;
    string description;
    long timeStep = 0;
//    map<string, vector<float>> map = ts.allData;
//    int vector_size = ts.allData.begin()->second.size();
    for (correlatedFeatures const &c : cf) {
        vector<float> x = ts.getVectorByName(c.feature1);
        vector<float> y = ts.getVectorByName(c.feature2);
        for (int k = 0; k < size; k++) {
            if (isAnomalous(c, x[k], y[k])) {
                description = c.feature1 + "-" + c.feature2;
                timeStep = k + 1;
                AnomalyReport an(description, timeStep);
                anomalyReport.push_back(an);
            }
        }
    }
    return anomalyReport;
}