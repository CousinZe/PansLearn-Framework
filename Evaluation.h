#ifndef __EVALUATION__
#define __EVALUATION__

#include"iostream"
#include"string"
#include"vector"
#include"map"
#include"set"
#include"cmath"
#include"Data.h"

//You have to inlude the h file with your modle.
#include"NaiveBayes.h"
using namespace std;

template<typename data> class Evaluation : public Data<data>
{
public:
	//to initial this object: "n to set how many part to divide".
	Evaluation(vector<vector<data>> X, vector<data> Y, unsigned int n);

	//use cross validation to give evaluation.
	double crossValidation(string method);

private:
	//to save the divide X.
	vector<vector<vector<data>>> XList;

	//to save the divide Y.
	vector<vector<data>> Ylist;

	//to save the N.
	unsigned int N;

	//to save the threholdValue.
	unsigned int threholdValue;

	//crossValidationNaiveBayesFunction.
	double crossValidationNaiveBayes();

	//evaluateMethod.(Privided for user to difine special evaluate function).
	double evaluateMethod(vector<data> realLable, vector<data> predictedLable);

};


template<typename data>
Evaluation<data>::Evaluation(vector<vector<data>> X, vector<data> Y, unsigned int n)
{
	vector<vector<vector<data>>> resultX;
	vector<vector<data>> resultY;

	unsigned int dataSize = X.size();
	unsigned int thresholdValue = X.size() / n;

	Evaluation::N = n;
	Evaluation::threholdValue = threholdValue;

	vector<vector<data>> tempX;
	vector<data> tempY;
	for (unsigned int count = 0; count != Evaluation::dataSize; count++)
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

	Evaluation::XList = resultX;
	Evaluation::Ylist = resultY;
}

template<typename data>
double Evaluation<data>::crossValidation(string method)
{
	if (method = "NaiveBayes")
	{
		return crossValidationNaiveBayes();
	}
}

template<typename data>
double Evaluation<data>::crossValidationNaiveBayes()
{
	double result = 0;
	for (unsigned int count = 0; count != Evaluation<data>::N; count++)
	{
		vector<vector<data>> test = Evaluation::XList[i];
		vector<data> lable = Evaluation::Ylist[i];
		vector<vector<data>> trainX;
		vector<data> trainY;
		for (unsigned int i = 0; i != Evaluation::N; i++)
		{
			if (i != count)
			{
				trainX.insert(trainX.end(), Evaluation::XList[i].begin(), Evaluation::XList[i].end());
				trainY.insert(trainY.end(), Evaluation::YList[i].begin(), Evaluation::YList[i].end());
			}
		}

		NaiveBayes<data> modle;
		modle.fit(trainX, trainY);
		vector<data> priList = modle.predict(test);

		result += Evaluation::evaluateMethod(lable, priList);
	}

	return result / Evaluation::N;
}

#endif // !__EVALUATION__