#ifndef __EVALUATION__
#define __EVALUATION__

#include"iostream"
#include"string"
#include"vector"
#include"map"
#include"set"
#include"cmath"
using namespace std;

template<typename data> class Evaluation
{
public:
	//to initial this object: "n to set how many part to divide".
	Evaluation(vector<vector<data>> X, unsigned int n);

	//use cross validation to give evaluation
	double crossValidation();

private:
	//to save the divide
	vector<vector<vector<data>>> XList;

};


#endif // !__EVALUATION__