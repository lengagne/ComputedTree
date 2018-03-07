#include "ComputedTree.h"
#include <iostream>

#include "ComputedTreeList.h"

ComputedTreeList chief_;


Monomial merge_monomial(const Monomial& m1, const Monomial& m2)
{
    Monomial tmp;
    for (std::list<ComputedTree*>::const_iterator it=m1.mono.begin(); it!=m1.mono.end(); it++)
        tmp.mono.push_back(*it);
    for (std::list<ComputedTree*>::const_iterator it=m2.mono.begin(); it!=m2.mono.end(); it++)
        tmp.mono.push_back(*it);
    tmp.mono.sort();
    for (std::list<ComputedTree*>::const_iterator it=tmp.mono.begin(); it!=tmp.mono.end(); it++)
        tmp.name += "_" + (*it)->get_name();
    return tmp;
}

bool operator==(const Monomial& a, const Monomial&b)
{
    // a voir si c'est bon (il faut que le nom soit mis à jour
    if(a.mono.size() != b.mono.size())
        return false;
    std::list<ComputedTree*>::const_iterator ita =a.mono.begin();
    std::list<ComputedTree*>::const_iterator itb =b.mono.begin();
    for (; ita!=a.mono.end(); ita++,itb++)
    {
        if(*ita != *itb)    return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& os, const Monomial& obj)
{
    for (std::list<ComputedTree*>::const_iterator it =obj.mono.begin(); it!=obj.mono.end(); it++)
    {
        os<<"*"<< (*it)->get_name();
    }
    return os;
}

void update_name(Monomial* m)
{
    m->name="";
    for (std::list<ComputedTree*>::iterator it=m->mono.begin(); it!=m->mono.end(); it++)
    {
        m->name += (*it)->get_name();
    }
}

ComputedTree::ComputedTree():value_(0)
{

}

ComputedTree::~ComputedTree()
{

}

ComputedTree::ComputedTree(const ComputedTree& other):value_(other.value_), polynomial_(other.polynomial_),name_(other.name_)
{

}

ComputedTree::ComputedTree(const double& input)
{
    value_ = input;
}

AbstractGeneratedCode* ComputedTree::get_recompile_code(const std::string & libname)const
{
    return chief_.get_recompile_code(libname);
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
    chief_.add_output(*this,index,num_out);
}

void ComputedTree::operator*= (const double & d)
{
    for (std::map<Monomial*,double>::const_iterator iter = polynomial_.begin(); iter != polynomial_.end(); ++iter)
        (iter->first)->value *= d;
}

void ComputedTree::operator+= (const ComputedTree& in)
{
    std::map<Monomial*,double>::const_iterator iter;
    for (iter = in.polynomial_.begin(); iter != in.polynomial_.end(); ++iter)
            polynomial_[iter->first] += iter->second;
}

void ComputedTree::operator*= (const ComputedTree& in)
{
    *this = *this * in;
}

ComputedTree ComputedTree::operator* (const double & d) const
{
    ComputedTree out;
    std::map<Monomial*,double>::const_iterator iter;
    for (iter = polynomial_.begin(); iter != polynomial_.end(); ++iter)
        out.polynomial_[iter->first] = iter->second * d;

    return out;
}

ComputedTree ComputedTree::operator+ (const ComputedTree& in) const
{
    ComputedTree out = *this;
    std::map<Monomial*,double>::const_iterator iter;
    for (iter = in.polynomial_.begin(); iter != in.polynomial_.end(); ++iter)
            out.polynomial_[iter->first] += iter->second;
    return out;
}

ComputedTree ComputedTree::operator- (const ComputedTree& in) const
{
    ComputedTree out = *this;
    std::map<Monomial*,double>::const_iterator iter;
    for (iter = in.polynomial_.begin(); iter != in.polynomial_.end(); ++iter)
            out.polynomial_[iter->first] -= iter->second;
    return out;
}

ComputedTree ComputedTree::operator* (const ComputedTree& in) const
{
    ComputedTree out;
    for (std::map<Monomial*,double>::const_iterator iterthis = polynomial_.begin(); iterthis != polynomial_.end(); ++iterthis)
    {
        const Monomial& m1 = *(iterthis->first);
        for (std::map<Monomial*,double>::const_iterator iterin = in.polynomial_.begin(); iterin != in.polynomial_.end(); ++iterin)
        {
            const Monomial& m2 = *(iterin->first);
            Monomial tmp = merge_monomial(m1,m2);
            Monomial* good = chief_.add_intermediate(tmp);
            out.polynomial_[good] += iterthis->second * iterin->second ;
        }
    }
    return out;
}

ComputedTree cos(ComputedTree& in)
{
    return chief_.add_non_linear_input(&in,NLCOS);
}

ComputedTree sin(ComputedTree& in)
{
    return chief_.add_non_linear_input(&in,NLSIN);
}


std::ostream& operator<<(std::ostream& os, const ComputedTree& obj)
{
    os<<"name:"<<obj.name_<<" "<< obj.value_<":";
    for (std::map<Monomial*,double>::const_iterator it = obj.polynomial_.begin(); it != obj.polynomial_.end();it++)
    {
        os<<"+"<< it->second<< *(it->first) ;
    }
    return os;
}



