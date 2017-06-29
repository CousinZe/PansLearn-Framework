#ifndef __DATA__
#define __DATA__

#include"iostream"
#include"string"
#include"vector"
#include"map"
#include"set"
#include"cmath"

using namespace std;

template<typename data>
struct Data
{
    //save the amount of samples.
    unsigned int dataSize;

    //save the amount of propertise.
    unsigned int propertySize;

    //save the X stringSet.
    vector<vector<data>> X;

    //save the Y stringSet.
    vector<data> Y;

    //save the value types of lable.
    set<data> lableValues;

    //save the value types of all attributes.
    vector<set<data>> attributeValues;
};

#endif //  !__DATA__
