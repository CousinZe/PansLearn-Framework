#ifndef __DATAPREPROCESSING__
#define __DATAPREPROCESSING__

#include"iostream"
#include"string"
#include"vector"
#include"map"
#include"set"
#include"cmath"
#include"Data.h"
#include <cassert>

template<typename data>
class DataPreprocessing
{
public:
    enum class FixMethod
    {
        AverageFix,
        MedianFix,
        ModeFix
    };

    //initial function. (MAIN PROCESS)
    DataPreprocessing(const std::vector<std::vector<data>> &dataSet);

    //to do discretization for dataSet.
    void discretization(unsigned int splitPointAmount);

    //to fix the missing value in dataSet: "method: average/median/mode/custom".
    void fixMissingValue(FixMethod method);
    void fixMissingValue(FixMethod, const data& fixValue);

    //to standardize the dataSet's value. "type: "
    void standardize();

    //to kill the missing value sample.
    void killMissingValueSample();

private:
    //save the max value of dataset.
    std::vector<data> maxValues;

    //save the min value of dataset.
    std::vector<data> minValues;

    //save the average value of dataset.
    std::vector<data> averageValues;

    //save the median value of dataset.
    std::vector<data> medianValues;

    //save the mode value of dataset.
    std::vector<data> modeValues;

    Data<data> basicData;
};

template<typename data>
DataPreprocessing<data>::DataPreprocessing(const std::vector<std::vector<data>> &dataSet)
{
    assert(!dataSet.empty());

    basicData.X = dataSet;
    basicData.dataSize = dataSet.size();
    basicData.propertySize = dataSet[0].size();

    std::vector<data> max(basicData.propertySize);
    std::vector<data> min(basicData.propertySize);
    std::vector<data> average(basicData.propertySize);
    std::vector<data> mode(basicData.propertySize);

    //The occurrence number of each feature.
    std::vector<std::map<data, unsigned int>> modeMap(basicData.propertySize);

    for (auto it = basicData.X.begin()->begin();
         it != basicData.X.begin()->end();
         it++)
    {
        unsigned int index = it - basicData.X.begin()->begin();
        max[index] = min[index] = average[index] = *it;
        modeMap[index][*it]++;
    }

    for (auto row = basicData.X.begin() + 1;
         row != basicData.X.end();
         row++)
    {
        for (auto column = row->begin(); column != row->end(); column++)
        {
            unsigned int index = column - row->begin();
            data tempData = *column;

            if (max[index] < tempData)
            {
                max[index] = tempData;
            }

            if (min[index] > tempData)
            {
                min[index] = tempData;
            }

            average[index] += tempData;

            modeMap[index][tempData]++;
        }
    }

    for (auto it = average.begin(); it != average.end(); it++)
    {
        *it /= basicData.dataSize;
    }

    for (auto it = modeMap.begin(); it != modeMap.end(); it++)
    {
        unsigned int index = it - modeMap.begin();
        unsigned int count = 0;
        data tempdata;

        for (auto insideIt = it->begin(); insideIt != it->end(); insideIt++)
        {
            if (insideIt->second > count)
            {
                tempdata = insideIt->first;
                count = insideIt->second;
            }
        }

        mode[index] = tempdata;
    }

    maxValues = max;
    minValues = min;
    averageValues = average;
    modeValues = mode;
}

template<typename data>
void DataPreprocessing<data>::discretization(unsigned int splitPointAmount)
{
    std::vector<data> thresholdValues(basicData.propertySize);
    for (unsigned int i = 0; i != basicData.propertySize - 1; i++)
    {
        thresholdValues[i] =
                (basicData.maxValues[i]
                 - basicData.minValues[i]) / splitPointAmount;
    }

    for (auto row = basicData.X.begin();
         row != basicData.X.end(); row++)
    {
        for (auto column = row->begin(); column != row->end(); column++)
        {
            unsigned int index = column - row->begin();
            for (unsigned int count = 0; count != splitPointAmount; count++)
            {
                if (count * thresholdValues[index] < *column
                    && *column < (count + 1) * thresholdValues[index])
                {
                    *column = count;
                    break;
                }
            }
        }
    }
}

template<typename data>
void DataPreprocessing<data>::fixMissingValue(FixMethod method)
{
    std::vector<data> fixValues(basicData.propertySize);

    //method: average/median/mode/custom

    switch (method)
    {
    case FixMethod::AverageFix:
        fixValues = averageValues;
        break;

    case FixMethod::MedianFix:
        assert(0);
        break;

    case FixMethod::ModeFix:
        fixValues = modeValues;
        break;

    default:
        assert(0);
    }

    for (auto row = basicData.X.begin();
         row != basicData.X.end();
         row++)
    {
        for (auto column = row->begin(); column != row->end(); column++)
        {
            unsigned int index = column - row->begin();
            if (*column == 0 || *column == '\0')
            {
                *column = fixValues[index];
            }
        }
    }
}

template<typename data>
void DataPreprocessing<data>::fixMissingValue(FixMethod,
                                              const data &fixValue)
{
    for (auto row = basicData.X.begin();
         row != basicData.X.end();
         row++)
    {
        for (auto column = row->begin();
             column != row->end();
             column++)
        {
            if (*column == 0 || *column == '\0')
            {
                *column = fixValue;
            }
        }
    }
}


template<typename data>
void DataPreprocessing<data>::standardize()
{
    std::vector<double> ratio(basicData.propertySize);
    for (unsigned int i = 0; i != basicData.propertySize; i++)
    {
        ratio.push_back(
                    1 / static_cast<double>(basicData.maxValues[i]
                                            - basicData.minValues[i]));
    }

    for (auto row = basicData.X.begin();
         row != basicData.X.end();
         row++)
    {
        for (auto column = row->begin(); column != row->end(); column++)
        {
            unsigned int index = column - row->begin();
            *column *= ratio[index];
        }
    }
}

template<typename data>
void DataPreprocessing<data>::killMissingValueSample()
{
    for (auto row = basicData.X.begin();
         row != basicData.X.end();
         row++)
    {
        for (auto column = row->begin();
             column != row->end();
             column++)
        {
            if (*column == 0 || *column == '\0')
            {
                basicData.X.erase(row);
                break;
            }
        }
    }
}

#endif // !__DATAPREPROCESSING__
