#include "ComputedTreeList.h"
#include <iostream>

ComputedTreeList::~ComputedTreeList()
{
    inputs_.clear();
}

void ComputedTreeList::add_input(ComputedTree* in)
{
//    std::cout<<"ComputedTreeListadd_input " << *in <<std::endl;
    // if does not already exist
    if(std::find(inputs_.begin(), inputs_.end(), in) == inputs_.end())
    {
        inputs_.push_back(in);
    }
//    else
//    {
//        std::cerr<<"We do not add this input since it already exists"<<std::endl;
//    }
}
