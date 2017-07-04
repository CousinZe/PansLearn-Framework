#ifndef __DATA__
#define __DATA__

#include"iostream"
#include"string"
#include"vector"
#include"map"
#include"set"
#include"cmath"

template<typename data>
struct Data
{
    //save the amount of samples.
    unsigned int dataSize;

    //save the amount of propertise.
    unsigned int propertySize;

    //save the X stringSet.
    std::vector<std::vector<data>> X;

    //save the Y stringSet.
    std::vector<data> Y;

    //save the value types of lable.
    std::set<data> lableValues;

    //save the value types of all attributes.
    std::vector<std::set<data>> attributeValues;
};

#endif //  !__DATA__
