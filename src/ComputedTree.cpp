#include "ComputedTree.h"
#include <iostream>

#include "ComputedTreeList.h"

ComputedTreeList chief_;

void update_name(Monomial* m)
{
    m->name="";
    for (std::list<ComputedTree*>::iterator it=m->mono.begin(); it!=m->mono.end(); it++)
    {
        m->name += (*it)->get_name();
        std::cout<<*(*it)<<std::endl;
    }
    std::cout<<"update_name(Monomial*m) name = "<< m->name<<std::endl;
}

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

void ComputedTree::prepare_file( const std::string & filename)
{
    chief_.prepare_file(filename);
}

void ComputedTree::set_as_input(const std::string& name)
{
    name_ = name;
    Monomial* mono = chief_.add_input(this);
    polynomial_[mono] = 1.0;
}

void ComputedTree::set_as_output(   unsigned int index,
                                    unsigned int num_out,
                                    const std::string& name)
{
    name_ = name;
    chief_.add_output(this,index,num_out);
}

std::ostream& operator<<(std::ostream& os, const ComputedTree& obj)
{
    os<<"name:"<<obj.name_<<" ";
    if(obj.value_)
        os<<*(obj.value_);
    else
        os<<"not_defined";
    return os;
}

ComputedTree ComputedTree::operator* (const long double & d) const
{
    ComputedTree out;
    std::map<Monomial*,long double>::const_iterator iter;
    for (iter = polynomial_.begin(); iter != polynomial_.end(); ++iter)
        out.polynomial_[iter->first] = iter->second * d;

    return out;
}
