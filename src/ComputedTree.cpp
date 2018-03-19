#include "ComputedTree.h"
#include <iostream>

#include "ComputedTreeList.h"

ComputedTreeList chief_;

ComputedTree::ComputedTree():   input_index_(-1),name_(""),value_(0),
                                in1_(0),in2_(0),type_(NLNONE),me_(0)
{

}

ComputedTree::~ComputedTree()
{

}

ComputedTree::ComputedTree(const ComputedTree& other):input_index_(other.input_index_),name_(other.name_),value_(other.value_),
                                                        in1_(other.in1_),in2_(other.in2_),type_(other.type_),me_(other.me_)
{
//    std::cout<<"copy constructor other = "<< other <<std::endl;
//    std::cout<<"copy constructor *this = "<< *this <<std::endl;
}

ComputedTree::ComputedTree(const double& input):input_index_(-1),value_(input),
                                                in1_(0),in2_(0),type_(NLDOUBLE)
{
    me_ = chief_.add_intermediate(*this);
    name_ = to_string_with_precision(input);
}

std::string ComputedTree::get_tmp_name()
{
    if(type_==NLDOUBLE) return to_string_with_precision(value_);
    return "tmp[" + std::to_string(tmp_index_) + "]";
}
//AbstractGeneratedCode* ComputedTree::get_recompile_code(const std::string & libname)const
//{
//    return chief_.get_recompile_code(libname);
//}

//double ComputedTree::get_value() const
//{
//    if(is_double())
//    {
//        std::map<Monomial*,double>::const_iterator it = polynomial_.begin();
//        return it->second;
//    }
//
//    // else
//    std::cerr<<"Error in "<<__FILE__<<" at line "<< __LINE__<< " cannot get value of non double"<<std::endl;
//    exit(0);
//}
//
//bool ComputedTree::is_double() const
//{
//     if (polynomial_.size()!=1 )
//        return false;
//
//    std::map<Monomial*,double>::const_iterator it = polynomial_.begin();
//    if ( (it->first)->mono.size() != 0)
//        return false;
//
//    return true;
//}

void ComputedTree::prepare_file( const std::string & filename)
{
    chief_.prepare_file(filename);
}

void ComputedTree::set_and_sons(const std::string& tab)
{
    std::cout<<tab<<"set_and_sons : "<<*this <<std::endl;
    updated_ = true;
    if( in1_ && !in1_->is_set())
    {
        std::cout<<tab<<"in1_"<<std::endl;
        in1_->set_and_sons(tab+"\t");
    }

    if( in2_ && !in2_->is_set())
    {
        std::cout<<tab<<"in2_"<<std::endl;
        in2_->set_and_sons(tab+"\t");
    }

}


void ComputedTree::set_as_input(const std::string& name)
{
    name_ = name;
    type_ = NLIN;
    in1_ = chief_.add_input(*this);
    me_ = in1_;
    if(me_) me_->name_ = name;
}

void ComputedTree::show_all()const
{
    chief_.show_all();
}

void ComputedTree::set_as_output(   unsigned int index,
                                    unsigned int num_out,
                                    const std::string& name)
{
    name_ = name;
    //chief_.add_output(*this,index,num_out);
    chief_.add_output(me_,index,num_out);
}

//void ComputedTree::operator= (const double & d)
//{
////    value_ = d;
//    polynomial_.clear();
//    Monomial* mono = new Monomial; //chief_.add_input(this);
//    mono->name = "fake_double";
//    polynomial_[mono] = d;
//}
//
//void ComputedTree::operator= (const ComputedTree& in)
//{
//    std::cout<<"operator= other = "<< in <<std::endl;
//
//    value_ = in.value_;
//    polynomial_ = in.polynomial_;
//    name_ = in.name_;
//    std::cout<<"operator= *this = "<< *this <<std::endl;
//}
//
//void ComputedTree::operator*= (const double & d)
//{
//    for (std::map<Monomial*,double>::iterator iter = polynomial_.begin(); iter != polynomial_.end(); ++iter)
//        iter->second *= d;
//}
//
//void ComputedTree::operator+= (const ComputedTree& in)
//{
//    std::map<Monomial*,double>::const_iterator iter;
//    for (iter = in.polynomial_.begin(); iter != in.polynomial_.end(); ++iter)
//            polynomial_[iter->first] += iter->second;
//}
//
//void ComputedTree::operator-= (const ComputedTree& in)
//{
//    std::map<Monomial*,double>::const_iterator iter;
//    for (iter = in.polynomial_.begin(); iter != in.polynomial_.end(); ++iter)
//            polynomial_[iter->first] -= iter->second;
//}
//
//void ComputedTree::operator*= (const ComputedTree& in)
//{
//    *this = *this * in;
//}
//
//ComputedTree ComputedTree::operator* (const double & d) const
//{
//    ComputedTree cd(d);
//    ComputedTree out;
//    out.in2_ = in1_;
//    out.in1_ = chief_.add_intermediate(cd);
//    out.type_ = NLMUL;
//    out.me_ = chief_.add_intermediate(out);
//    return out;
//}
//
//ComputedTree ComputedTree::operator/ (const double & d) const
//{
//    ComputedTree out;
//    std::map<Monomial*,double>::const_iterator iter;
//    for (iter = polynomial_.begin(); iter != polynomial_.end(); ++iter)
//        out.polynomial_[iter->first] = iter->second / d;
//
//    return out;
//}
//
ComputedTree ComputedTree::operator+ (const ComputedTree& in) const
{
    ComputedTree out;
    if (me_<in.me_)
    {
        out.in1_ = me_;
        out.in2_ = in.me_;
    }else
    {
        out.in1_ = in.me_;
        out.in2_ = me_;
    }
    out.type_ = NLADD;
    out.name_ = "("+get_name() + "+" +in.get_name()+")";
    out.me_ = chief_.add_intermediate(out);
    return out;
}
//
ComputedTree ComputedTree::operator- (const ComputedTree& in) const
{
    ComputedTree out;
    out.in1_ = me_;
    out.in2_ = in.me_;
    out.type_ = NLSUB;
    out.name_ = "("+get_name() + "-" +in.get_name()+")";
    out.me_ = chief_.add_intermediate(out);
    return out;
}

ComputedTree ComputedTree::operator* (const ComputedTree& in) const
{
//    std::cout<<"this = "<< *this <<std::endl;
//    std::cout<<"in = "<<  in<<std::endl;
//    std::cout<<"me = "<<  *me_ <<std::endl;
//    std::cout<<"in.me = "<<  (in.me_) <<std::endl;
//    std::cout<<"*in.me = "<<  *(in.me_) <<std::endl;
    ComputedTree out;
    out.in1_ = me_;
    out.in2_ = in.me_;
    out.type_ = NLMUL;
    out.name_ = get_name() + "*" +in.get_name();
    out.me_ = chief_.add_intermediate(out);
//    std::cout<<"out = "<<  out <<std::endl;
    return out;
}

ComputedTree cos(ComputedTree& in)
{
    ComputedTree out;
    out.in1_ = in.me_;
    out.type_ = NLCOS;
    out.name_ = "cos("+in.name_+")";
    out.me_ = chief_.add_intermediate(out);
    return out;
}

ComputedTree sin(ComputedTree& in)
{
    ComputedTree out;
    out.in1_ = in.me_;
    out.type_ = NLSIN;
    out.name_ = "sin("+in.name_+")";
    out.me_ = chief_.add_intermediate(out);
    return out;
}

std::ostream& operator<<(std::ostream& os, const ComputedTree& obj)
{
   switch(obj.type_)
    {
        case(NLMUL):
            os<<"("<<*(obj.in1_)<<"*"<<*(obj.in2_)<<")";
            break;
        case(NLADD):
            os<<"("<<*(obj.in1_)<<"+"<<*(obj.in2_)<<")";
            break;
        case(NLSUB):
            os<<"("<<*(obj.in1_)<<"-"<<*(obj.in2_)<<")";
            break;
        case(NLCOS):
            os<<"cos("<<*(obj.in1_)<<")";
            break;
        case(NLSIN):
            os<<"sin("<<*(obj.in1_)<<")";
            break;
        case(NLIN):
            os<<obj.name_;
            break;
        case(NLDOUBLE):
            os<<obj.value_;
            break;
        case(NLNONE):
            os<<"NLNONE";
            break;
        default: os<<"TYPE "<< obj.type_ <<" undefined";
    }
    return os;
}
