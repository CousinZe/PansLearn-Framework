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

//declaration:
template <typename data>
class NaiveBayes
{
public:
    //to fit a modle: "X: featureSets", "Y: lableSets"
    void fit(const std::vector<std::vector<data>> &X,
             const std::vector<data> &Y);

    //to use the modle to predict. Can get a result std::vector.
    std::vector<data> predict(const std::vector<std::vector<data>> &test);

private:

    //to get the amount of one special attribute value.
    unsigned int checkAttributeValueAmount(unsigned int attributeType,
                                           const data &attributeValue);

    //to get the amount of lable.
    unsigned int checkLableValueAmount(const data &lableValue);

    //to get the amount of special attribute in one special lable
    unsigned int checkAttributeValueAmountInLable(unsigned int attributeType,
                                                  const data &attributeValue,
                                                  const data &lableValue);

    //to calculate the probability: "P((attribute = a)|(lable = b))".
    double probabilityCalculate(unsigned int attributeAmount,
                                unsigned int lableAmount);

    //to get and save the probability of all attributes.(MAIN FIT PROCESS)
    void getProbability();

    //save the probability of different lable (Y probability)
    std::map<data, double> lableProbabilities;

    //save the probability of all attributes(X probability):
    //     "outside std::map: key=lableValue, value=attributeMap",
    //     "inside std::map: key=attributePair, value=probability"
    std::map<data, std::map<std::pair<unsigned int, data>, double>>
        probabilities;

    Data<data> basicData;

};


//definition
template<typename data>
void NaiveBayes<data>::fit(const std::vector<std::vector<data>> &X,
                           const std::vector<data> &Y)
{
    //Initial the dataSize.
    basicData.dataSize = X.size();

    //Initial the propertySize.
    basicData.propertySize = X[0].size();

    //Initial the data.
    basicData.X = X;
    basicData.Y = Y;

    //Initial the feature value std::vector.
    basicData.attributeValues.resize(basicData.propertySize);

    //Get the feature value std::vector and lable value std::vector.
    for (auto line = X.begin(); line != X.end(); line++)
    {
        for (auto column = line->begin(); column != line->end(); column++)
        {
            int columnIndex = column - line->begin();
            basicData.attributeValues[columnIndex].insert(*column);
        }
    }

    for (auto line = Y.begin(); line != Y.end(); line++)
    {
        basicData.lableValues.insert(*line);
    }

    //Get the ability. (MAIN FIT PROCESS)
    basicData.getProbability();
}

template<typename data>
std::vector<data> NaiveBayes<data>::predict(
        const std::vector<std::vector<data>> &test)
{
    std::vector<data> result;
    for (auto it = test.begin(); it != test.end(); it++)
    {
        std::map<data, double> resultProbability;
        for (auto lableIt = basicData.lableValues.begin();
             lableIt != basicData.lableValues.end();
             lableIt++)
        {
            double probalibity = 1.0;
            probalibity *= basicData.lableProbabilities[*lableIt];
            for (auto lineIt = it->begin(); lineIt != it->end(); lineIt++)
            {
                unsigned int index = lineIt - it->begin();
                probalibity *=
                        basicData.probabilities
                        [*lableIt][std::make_pair(index, *lineIt)];
            }
            resultProbability.insert(std::make_pair(*lableIt, probalibity));
        }

        std::pair<data, double> bestType = std::make_pair(0, 0.0);
        for (auto resultIt = resultProbability.begin();
             resultIt != resultProbability.end();
             resultIt++)
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
    for (auto lableIt = basicData.lableValues.begin();
         lableIt != basicData.lableValues.end();
         lableIt++)
    {
        lableValue = *lableIt;
        lableValueAmount = basicData.checkLableValueAmount(lableValue);
        lableProbability =
                static_cast<double>(lableValueAmount)
                / static_cast<double>(basicData.dataSize);
        basicData.lableProbabilities.insert(
                    std::make_pair(lableValue, lableProbability));

        std::map<std::pair<unsigned int, data>, double> tempInsideMap;
        for (auto attrTypeIt = basicData.attributeValues.begin();
             attrTypeIt != basicData.attributeValues.end();
             attrTypeIt++)
        {
            attributeType = attrTypeIt - basicData.attributeValues.begin();

            auto attris = basicData.attributeValues[attributeType];
            for (auto attrIt = attris.begin(); attrIt != attris.end(); attrIt++)
            {
                attributeValue = *attrIt;
                attributeValueAmount =
                        basicData.checkAttributeValueAmountInLable(
                            attributeType, attributeValue, lableValue);
                std::pair<unsigned int, data> tempAttrPair(
                            attributeType, attributeValue);
                probability = basicData.probabilityCalculate(
                                  attributeValueAmount, lableValueAmount);
                tempInsideMap.insert(std::make_pair(tempAttrPair, probability));
            }
        }
        basicData.probabilities.insert(
                    std::make_pair(lableValue, tempInsideMap));
    }
}

template<typename data>
unsigned int NaiveBayes<data>::checkLableValueAmount(const data& lableValue)
{
    unsigned int count = 0;
    for (auto it = basicData.Y.begin(); it != basicData.Y.end(); it++)
    {
        if (*it == lableValue)
        {
            count++;
        }
    }
    return count;
}

template<typename data>
unsigned int NaiveBayes<data>::checkAttributeValueAmount(
        unsigned int attributeType, const data& attributeValue)
{
    unsigned int count = 0;
    for (auto it = basicData.X.begin(); it != basicData.X.end(); it++)
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
unsigned int NaiveBayes<data>::checkAttributeValueAmountInLable(
        unsigned int attributeType,
        const data& attributeValue,
        const data& lableValue)
{
    unsigned int count = 0;
    for (auto it = basicData.X.begin(); it != basicData.X.end(); it++)
    {
        auto line = *it;
        unsigned int index = it - basicData.X.begin();

        if (line[attributeType] == attributeValue
            && basicData.Y[index] == lableValue)
        {
            count++;
        }
    }
    return count;
}

template<typename data>
double NaiveBayes<data>::probabilityCalculate(unsigned int attributeAmount,
                                              unsigned int lableAmount)
{
    return static_cast<double>(attributeAmount)
            / static_cast<double>(lableAmount);
}


#endif // !__NAIVEBAYES__
