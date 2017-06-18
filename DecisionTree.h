#ifndef __DECISIONTREE__
#define __DECISIONTREE__

#include"iostream"
#include"string"
#include"vector"
#include"map"
#include"set"
#include"cmath"
#include"Data.h"
using namespace std;

template<typename data> class DecisionTree : public Data<data>
{
public:
	//to fit a modle: "X: featureSets", "Y: lableSets"
	void fit(const vector<vector<data>> &X, const vector<data> &Y);

	//to use the modle to predict. Can get a result vector.
	vector<data> predict(const vector<vector<data>> &test);

private:




};



#endif // !__DECISIONTREE__