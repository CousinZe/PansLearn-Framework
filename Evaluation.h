#ifndef __EVALUATION__
#define __EVALUATION__

#include "iostream"
#include "string"
#include "vector"
#include "map"
#include "set"
#include "cmath"
#include "Data.h"

//You have to inlude the h file with your modle.
#include "NaiveBayes.h"

template<typename data>
class Evaluation
{
public:
    enum class ValidationMethod
    {
        NaiveBayes
    };

    //to initial this object: "n to set how many part to divide".
    Evaluation(const std::vector<std::vector<data>>& X, const std::vector<data>& Y, unsigned int n);

    //use cross validation to give evaluation.
    double crossValidation(ValidationMethod method);

private:
    //to save the divide X.
    std::vector<std::vector<std::vector<data>>> XList;

    //to save the divide Y.
    std::vector<std::vector<data>> YList;

    //to save the N.
    unsigned int N;

    //to save the threholdValue.
    unsigned int threholdValue;

    //crossValidationNaiveBayesFunction.
    double crossValidationNaiveBayes();

    //evaluateMethod.(Privided for user to difine special evaluate function).
    double evaluateMethod(std::vector<data> realLable, std::vector<data> predictedLable);

    Data<data> basicData;
};


template<typename data>
Evaluation<data>::Evaluation(const std::vector<std::vector<data>> &X,
                             const std::vector<data> &Y,
                             unsigned int n)
{
    std::vector<std::vector<std::vector<data>>> resultX;
    std::vector<std::vector<data>> resultY;

    N = n;
    this->threholdValue = threholdValue;

    std::vector<std::vector<data>> tempX;
    std::vector<data> tempY;
    for (unsigned int count = 0; count != basicData.dataSize; count++)
    {
        if (count % threholdValue != 0)
        {
            tempX.push_back(X[count]);
            tempY.push_back(Y[count]);
        }
        else
        {
            resultX.push_back(tempX);
            resultY.push_back(tempY);
            tempX.clear();
            tempY.clear();
            tempX.push_back(X[count]);
            tempY.push_back(Y[count]);
        }
    }

    resultX.push_back(tempX);
    resultY.push_back(tempY);

    XList = resultX;
    YList = resultY;
}

template<typename data>
double Evaluation<data>::crossValidation(ValidationMethod method)
{
    switch (method)
    {
    case ValidationMethod::NaiveBayes:
        return crossValidationNaiveBayes();

    default:
        // Fixme : add some error-report here.
        ;
    }
}

template<typename data>
double Evaluation<data>::crossValidationNaiveBayes()
{
    double result = 0;
    for (unsigned int count = 0; count != Evaluation<data>::N; count++)
    {
        std::vector<std::vector<data>> test = XList[count];
        std::vector<data> lable = YList[count];
        std::vector<std::vector<data>> trainX;
        std::vector<data> trainY;
        for (unsigned int i = 0; i != N; i++)
        {
            if (i != count)
            {
                trainX.insert(trainX.end(), XList[i].begin(), XList[i].end());
                trainY.insert(trainY.end(), YList[i].begin(), YList[i].end());
            }
        }

        NaiveBayes<data> modle;
        modle.fit(trainX, trainY);
        std::vector<data> priList = modle.predict(test);

        result += Evaluation::evaluateMethod(lable, priList);
    }

    return result / N;
}

#endif // !__EVALUATION__
