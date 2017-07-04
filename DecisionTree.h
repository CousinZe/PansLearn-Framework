#ifndef __DECISIONTREE__
#define __DECISIONTREE__

#include "iostream"
#include "string"
#include "vector"
#include "map"
#include "set"
#include "cmath"
#include "Data.h"

template<typename data>
class DecisionTree
{
public:
	//to fit a modle: "X: featureSets", "Y: lableSets"
    void fit(const std::vector<std::vector<data>> &X,
             const std::vector<data> &Y);

    //to use the modle to predict. Can get a result std::vector.
    std::vector<data> predict(const std::vector<std::vector<data>> &test);

private:
    Data<data> basicData;
};


#endif // !__DECISIONTREE__
