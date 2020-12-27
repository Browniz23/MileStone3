// Oz Browner 316482355

#ifndef FLIGHTSIMULATOR_TIMESERIES_H
#define FLIGHTSIMULATOR_TIMESERIES_H

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

class TimeSeries{
private:
    // colLength - the length of each col (must be equal for every each). m_featuresNum - features amount.
    int colLength = 0;
    int m_featuresNum = 0;
    // m_orderedFeatures - vector of features in order ts got them.
    // m_features - a map which each pair is a feature and its col in ts.
    vector<string> m_orderedFeatures;
    map<string, vector<float>> m_features;
public:
    /**
     * consturctor.
     * @param CSVfileName
     */
    TimeSeries(const char *CSVfileName);

    vector<float> featureValues(string feature);

    /**
     * @return the features vector.
     */
    vector<string> getFeatures() const;

    void addRowData(float *rowData, int size);

    /**
     * gets a line and add it to ts as a new row. must be a value for each feature.
     * @param line
     */
    void addRowData(string line);

    /**
     * @return the table - which is the map of features and their cols.
     */
    map<string, vector<float>> getTable() const;

    /**
     * @return col length of ts cols.
     */
    int getColLength() const;
};

#endif //FLIGHTSIMULATOR_TIMESERIES_H
