/************************************

NEED TO FIX:
1.forgetting to check if the demensition of X and Y is same.
2.need to do more debug.

*************************************/



#ifndef __NAIVEBAYES__
#define __NAIVEBAYES__

#include"iostream"
#include"string"
#include"vector"
#include"map"
#include"set"
#include"cmath"
#include"Data.h"
using namespace std;

//declaration:
template<typename data> class NaiveBayes : public Data<data>
{
public:
	//to fit a modle: "X: featureSets", "Y: lableSets"
	void fit(const vector<vector<data>> &X, const vector<data> &Y);

	//to use the modle to predict. Can get a result vector.
	vector<data> predict(const vector<vector<data>> &test);

private:

	//to get the amount of one special attribute value.
	unsigned int checkAttributeValueAmount(unsigned int attributeType, data attributeValue);

	//to get the amount of lable.
	unsigned int checkLableValueAmount(data lableValue);

	//to get the amount of special attribute in one special lable
	unsigned int checkAttributeValueAmountInLable(unsigned int attributeType, data attributeValue, data lableValue);

	//to calculate the probability: "P((attribute = a)|(lable = b))".
	double probabilityCalculate(unsigned int attributeAmount, unsigned int lableAmount);

	//to get and save the probability of all attributes.(MAIN FIT PROCESS)
	void getProbability();

	//save the probability of different lable (Y probability)
	map<data, double> lableProbabilities;

	//save the probability of all attributes(X probability): "outside map: key=lableValue, value=attributeMap", "inside map: key=attributePair, value=probability"
	map<data, map<pair<unsigned int, data>, double>> probabilities;

};


//definition
template<typename data>
void NaiveBayes<data>::fit(const vector<vector<data>> &X, const vector<data> &Y)
{
	//Initial the dataSize.
	NaiveBayes<data>::dataSize = X.size();

	//Initial the propertySize.
	NaiveBayes<data>::propertySize = X[0].size();

	//Initial the data.
	NaiveBayes<data>::X = X;
	NaiveBayes<data>::Y = Y;

	//Initial the feature value vector.
	NaiveBayes<data>::attributeValues.resize(propertySize);

	//Get the feature value vector and lable value vector.
	for (auto line = X.begin(); line != X.end(); line++)
	{
		for (auto column = line->begin(); column != line->end(); column++)
		{
			int columnIndex = column - line->begin();
			NaiveBayes<data>::attributeValues[columnIndex].insert(*column);
		}
	}

	for (auto line = Y.begin(); line != Y.end(); line++)
	{
		NaiveBayes<data>::lableValues.insert(*line);
	}

	//Get the ability. (MAIN FIT PROCESS)
	NaiveBayes<data>::getProbability();
}

template<typename data>
vector<data> NaiveBayes<data>::predict(const vector<vector<data>> &test)
{
	vector<data> result;
	for (auto it = test.begin(); it != test.end(); it++)
	{
		map<data, double> resultProbability;
		for (auto lableIt = NaiveBayes<data>::lableValues.begin(); lableIt != NaiveBayes<data>::lableValues.end(); lableIt++)
		{
			double probalibity = 1.0;
			probalibity *= NaiveBayes<data>::lableProbabilities[*lableIt];
			for (auto lineIt = it->begin(); lineIt != it->end(); lineIt++)
			{
				unsigned int index = lineIt - it->begin();
				probalibity *= NaiveBayes<data>::probabilities[*lableIt][make_pair(index, *lineIt)];
			}
			resultProbability.insert(make_pair(*lableIt, probalibity));
		}

		pair<data, double> bestType = make_pair(0, 0.0);
		for (auto resultIt = resultProbability.begin(); resultIt != resultProbability.end(); resultIt++)
		{
			if (resultIt->second >= bestType.second)
			{
				bestType = *resultIt;
			}
		}

		result.push_back(bestType.first);
	}

	return result;
}

template<typename data>
void NaiveBayes<data>::getProbability()
{
	// temp Key and Value pair and Amount of attribute.
	unsigned int attributeType;
	data attributeValue;
	unsigned int attributeValueAmount;

	// temp Value and Amount of lable.
	data lableValue;
	unsigned int lableValueAmount;

	//temp probability.
	double probability;

	//temp lable probability.
	double lableProbability;

	//initial the probability data. (MAIN FIT PROCESS)
	for (auto lableIt = NaiveBayes<data>::lableValues.begin(); lableIt != NaiveBayes<data>::lableValues.end(); lableIt++)
	{
		lableValue = *lableIt;
		lableValueAmount = NaiveBayes<data>::checkLableValueAmount(lableValue);
		lableProbability = static_cast<double>(lableValueAmount) / static_cast<double>(NaiveBayes<data>::dataSize);
		NaiveBayes<data>::lableProbabilities.insert(make_pair(lableValue, lableProbability));

		map<pair<unsigned int, data>, double> tempInsideMap;
		for (auto attrTypeIt = NaiveBayes<data>::attributeValues.begin(); attrTypeIt != NaiveBayes<data>::attributeValues.end(); attrTypeIt++)
		{
			attributeType = attrTypeIt - NaiveBayes<data>::attributeValues.begin();

			auto attris = attributeValues[attributeType];
			for (auto attrIt = attris.begin(); attrIt != attris.end(); attrIt++)
			{
				attributeValue = *attrIt;
				attributeValueAmount = NaiveBayes<data>::checkAttributeValueAmountInLable(attributeType, attributeValue, lableValue);
				pair<unsigned int, data> tempAttrPair(attributeType, attributeValue);
				probability = NaiveBayes<data>::probabilityCalculate(attributeValueAmount, lableValueAmount);
				tempInsideMap.insert(make_pair(tempAttrPair, probability));
			}
		}
		NaiveBayes<data>::probabilities.insert(make_pair(lableValue, tempInsideMap));
	}
}

template<typename data>
unsigned int NaiveBayes<data>::checkLableValueAmount(data lableValue)
{
	unsigned int count = 0;
	for (auto it = NaiveBayes<data>::Y.begin(); it != NaiveBayes<data>::Y.end(); it++)
	{
		if (*it == lableValue)
		{
			count++;
		}
	}
	return count;
}

template<typename data>
unsigned int NaiveBayes<data>::checkAttributeValueAmount(unsigned int attributeType, data attributeValue)
{
	unsigned int count = 0;
	for (auto it = NaiveBayes<data>::X.begin(); it != NaiveBayes<data>::X.end(); it++)
	{
		auto line = *it;
		if (line[attributeType] == attributeValue)
		{
			count++;
		}
	}
	return count;
}

template<typename data>
unsigned int NaiveBayes<data>::checkAttributeValueAmountInLable(unsigned int attributeType, data attributeValue, data lableValue)
{
	unsigned int count = 0;
	for (auto it = NaiveBayes<data>::X.begin(); it != NaiveBayes<data>::X.end(); it++)
	{
		auto line = *it;
		unsigned int index = it - X.begin();

		if (line[attributeType] == attributeValue && Y[index] == lableValue)
		{
			count++;
		}
	}
	return count;
}

template<typename data>
double NaiveBayes<data>::probabilityCalculate(unsigned int attributeAmount, unsigned int lableAmount)
{
	return static_cast<double>(attributeAmount) / static_cast<double>(lableAmount);
}



#endif // !__NAIVEBAYES__
