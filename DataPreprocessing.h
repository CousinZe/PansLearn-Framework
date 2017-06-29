#ifndef __DATAPREPROCESSING__
#define __DATAPREPROCESSING__

#include"iostream"
#include"string"
#include"vector"
#include"map"
#include"set"
#include"cmath"
#include"Data.h"
using namespace std;

template<typename data> class DataPreprocessing : public Data<data>
{
public:
	//initial function. (MAIN PROCESS)
	DataPreprocessing(vector<vector<data>> &dataSet);

	//to do discretization for dataSet.
	void discretization(unsigned int splitPointAmount);

	//to fix the missing value in dataSet: "method: average/median/mode/custom".
	void fixMissingValue(string method);
	void fixMissingValue(string method, data fixValue);

	//to standardize the dataSet's value. "type: "
	void standardize();

	//to kill the missing value sample.
	void killMissingValueSample();

private:
	//save the max value of dataset.
	vector<data> maxValues;

	//save the min value of dataset.
	vector<data> minValues;

	//save the average value of dataset.
	vector<data> averageValues;

	//save the median value of dataset.
	vector<data> medianValues;

	//save the mode value of dataset.
	vector<data> modeValues;
};

template<typename data>
DataPreprocessing<data>::DataPreprocessing(vector<vector<data>> &dataSet)
{
	DataPreprocessing::X = dataSet;
	DataPreprocessing::dataSize = dataSet.size();
	DataPreprocessing::propertySize = dataSet[0].size();

	vector<data> max(DataPreprocessing::propertySize);
	vector<data> min(DataPreprocessing::propertySize);
	vector<data> average(DataPreprocessing::propertySize);
	vector<data> madian(DataPreprocessing::propertySize);
	vector<data> mode(DataPreprocessing::propertySize);

	//The occurrence number of each feature.
	vector<vector<data>> medianList;
	vector<map<data, unsigned int>> modeMap(DataPreprocessing::propertySize);
	
	for (auto it = X.begin()->begin(); it != X.begin()->end(); it++)
	{
		unsigned int index = it - X.begin()->begin();
		max[index] = min[index] = average[index] = *it;
		modeMap[index][*it]++;
	}

	for (auto line = X.begin() + 1; line != X.end(); line++)
	{
		for (auto column = line->begin(); column != line->end(); column++)
		{
			unsigned int index = column - line->begin();
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

	for (auto &it = average.begin(); it != average.end(); it++)
	{
		*it /= dataSize;
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

	DataPreprocessing::maxValues = max;
	DataPreprocessing::minValues = min;
	DataPreprocessing::averageValues = average;
	DataPreprocessing::modeValues = mode;
}

template<typename data>
void DataPreprocessing<data>::discretization(unsigned int splitPointAmount)
{
	vector<data> thresholdValues(DataPreprocessing::propertySize);
	for (unsigned int i = 0; i != DataPreprocessing::propertySize - 1; i++)
	{
		thresholdValues[i] = (DataPreprocessing::maxValues[i] - DataPreprocessing::minValues[i]) / splitPointAmount;
	}

	for (auto &line = X.begin(); line != X.end(); line++)
	{
		for (auto &column = line->begin(); column != line->end(); column++)
		{
			unsigned int index = column - line->begin();
			for (unsigned int count = 0; count != splitPointAmount; count++)
			{
				if (count * thresholdValues[index] < *column && *column < (count + 1) * thresholdValues[index])
				{
					*column = count;
					break;
				}
			}
		}
	}
}

template<typename data>
void DataPreprocessing<data>::fixMissingValue(string method)
{
	vector<data> fixValues(DataPreprocessing::propertySize);

	//method: average/median/mode/custom
	if (method == "average")
	{
		fixValues = DataPreprocessing::averageValues;
	}

	if (method == "mode")
	{
		fixValues = DataPreprocessing::modeValues;
	}

	for (auto &line = DataPreprocessing::X.begin(); line != DataPreprocessing::X.end(); line++)
	{
		for (auto &column = line->begin(); column != line->end(); column++)
		{
			unsigned int index = column - line->begin();
			if (*column == 0 || *column == '\0')
			{
				*column = fixValues[index];
			}
		}
	}
}

template<typename data>
void DataPreprocessing<data>::fixMissingValue(string method, data fixValue)
{
	for (auto &line = DataPreprocessing::X.begin(); line != DataPreprocessing::X.end(); line++)
	{
		for (auto &column = line->begin(); column != line->end(); column++)
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
	vector<double> ratio(DataPreprocessing::propertySize);
	for (unsigned int i = 0; i != DataPreprocessing::propertySize; i++)
	{
		ratio[i] = 1 / static_cast<double>(DataPreprocessing::maxValues[i] - DataPreprocessing::minValues[i]);
	}

	for (auto &line = DataPreprocessing::X.begin(); line != DataPreprocessing::X.end(); line++)
	{
		for (auto &column = line->begin(); column != line->end(); column++)
		{
			unsigned int index = column - line->begin();
			*column *= ratio[index];
		}
	}
}

template<typename data>
void DataPreprocessing<data>::killMissingValueSample()
{
	for (auto &line = DataPreprocessing::X.begin(); line != DataPreprocessing::X.end(); line++)
	{
		for (auto &column = line->begin(); column != line->end(); column++)
		{
			if (*column == 0 || *column == '\0')
			{
				X.erase(line);
				break;
			}
		}
	}
}


#endif // !__DATAPREPROCESSING__