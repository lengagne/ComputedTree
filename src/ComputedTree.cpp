#include "ComputedTree.h"
#include <iostream>


ComputedTree::ComputedTree():value_(NULL)
{

}

ComputedTree::~ComputedTree()
{
    if(value_) delete value_;
}

ComputedTree::ComputedTree(const ComputedTree& other):value_(other.value_)
{

}

ComputedTree::ComputedTree(const long double& input)
{
    value_ = new  long double (input);
}

void ComputedTree::set_as_input()
{
    chief_.add_input(this);
}

std::ostream& operator<<(std::ostream& os, const ComputedTree& obj)
{
    if(obj.value_)
        os<<*(obj.value_);
    else
        os<<"not_defined";
    return os;
}
