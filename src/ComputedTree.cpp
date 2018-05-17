#include "ComputedTree.h"
#include <iostream>

#include "ComputedTreeList.h"

ComputedTreeList chief_;

ComputedTree::ComputedTree():   input_index_(-1),name_(""),value_(0),
                                in1_(0),in2_(0),type_(NLNULL),me_(0)
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
                                                in1_(0),in2_(0)
{
    if (input !=0)
    {
        name_ = to_string_with_precision(input);
        type_ = NLDOUBLE;
        me_ = chief_.add_intermediate(*this);

    }else
    {
        type_ = NLNULL;
    }

}

void ComputedTree::TreeList_clear_all()
{
    chief_.clear_all();
}

double ComputedTree::eval()
{
    if(updated_)
    {
        return value_;
    }

    updated_ = true;
    switch(type_)
    {
        case(NLMUL):
            value_ = in1_->eval() * in2_->eval();
            break;
        case(NLADD):
            value_ = in1_->eval() + in2_->eval();
            break;
        case(NLSUB):
            value_ = in1_->eval() - in2_->eval();
            break;
        case(NLOPP):
            value_ = -in1_->eval();
        case(NLCOS):
            value_ = cos(in1_->eval());
            break;
        case(NLSIN):
            value_ = sin(in1_->eval());
            break;
//        default: ;
    }
//    std::cout<<name_<<" = "<< value_ <<std::endl;
    return value_;

}

std::string ComputedTree::ComputedTree::get_tmp_name() const
{
    if(type_==NLDOUBLE) return to_string_with_precision(value_);
    return "t[" + std::to_string(tmp_index_) + "]";
}

unsigned int ComputedTree::get_nb_tmp()const
{
    return chief_.get_nb_tmp();
}

AbstractGeneratedCode* ComputedTree::get_recompile_code(const std::string & libname)const
{
    return chief_.get_recompile_code(libname);
}

bool ComputedTree::is_not_null()const
{
    if(type_ == NLNULL) return false;
    return true;
}


void ComputedTree::prepare_file( const std::string & filename)
{
    chief_.prepare_file(filename);
}

void ComputedTree::reset_all()
{
    reset();
    chief_.reset();
}

void ComputedTree::set_and_sons(const std::string& tab)
{
    updated_ = true;
    if( in1_ && !in1_->is_set())
        in1_->set_and_sons(tab+"\t");
    if( in2_ && !in2_->is_set())
        in2_->set_and_sons(tab+"\t");
}


void ComputedTree::set_as_input(const std::string& name)
{
    name_ = name;
    type_ = NLIN;
    input_index_ = chief_.get_nb_inputs();
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
    chief_.add_output(me_,index,num_out);
}

void ComputedTree::operator+= (const ComputedTree& in)
{
    if(type_ == NLNULL)
        *this = in;
    else if(in.type_ == NLNULL)
    {

    }else
    {
        *this = *this + in;
    }
}

void ComputedTree::operator-= (const ComputedTree& in)
{
    if(type_ == NLNULL)
        *this = -in;
    else if(in.type_ == NLNULL)
    {

    }else
    {
        *this = *this - in;
    }
}

void ComputedTree::operator*= (const ComputedTree& in)
{
    if(type_ == NLNULL)
        *this = 0;
    else if(in.type_ == NLNULL)
    {
        *this = 0;
    }else
    {
        *this = *this * in;
    }
}

ComputedTree ComputedTree::operator- () const
{
    ComputedTree out;
    out.type_ = NLOPP;
    out.in1_ = me_;
    out.name_ = "-" +get_name();
    out.me_ = chief_.add_intermediate(out);
    return out;
}

ComputedTree ComputedTree::operator+ (const ComputedTree& in) const
{
    if(type_ == NLNULL)
        return in;
    if(in.type_ == NLNULL)
        return *this;

    if(in.me_->type_ == NLOPP)
        return *this - *(in.me_->in1_);

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
    if(type_ == NLNULL)
        return -in;

    if(in.type_ == NLNULL)
        return *this;

    if(in.me_->type_ == NLOPP)
        return *this + *(in.me_->in1_);


    ComputedTree out;
    if(me_ == in.me_)   // we do x-x
        return out;

    out.in1_ = me_;
    out.in2_ = in.me_;
    out.type_ = NLSUB;
    out.name_ = "("+get_name() + "-" +in.get_name()+")";
    out.me_ = chief_.add_intermediate(out);
    return out;
}

ComputedTree ComputedTree::operator* (const ComputedTree& in) const
{
    if(type_ == NLNULL || in.type_ == NLNULL)
        return ComputedTree();

    if(me_->type_ == NLDOUBLE && me_->value_ == 1)
        return in;

    if(in.me_->type_ == NLDOUBLE && in.me_->value_ == 1)
        return *this;

    if(me_->type_ == NLDOUBLE && me_->value_ == -1)
        return -in;

    if(in.me_->type_ == NLDOUBLE && in.me_->value_ == -1)
        return -*this;

    if(me_->type_ == NLDOUBLE && me_->value_ == 0)
        return 0;

    if(in.me_->type_ == NLDOUBLE && in.me_->value_ == 0)
        return 0;

    ComputedTree out;
    out.in1_ = me_;
    out.in2_ = in.me_;
    out.type_ = NLMUL;
    out.name_ = get_name() + "*" +in.get_name();
    out.me_ = chief_.add_intermediate(out);
    return out;
}

ComputedTree cos(const ComputedTree& in)
{
    ComputedTree out;
    out.in1_ = in.me_;
    out.type_ = NLCOS;
    out.name_ = "cos("+in.name_+")";
    out.me_ = chief_.add_intermediate(out);
    return out;
}

ComputedTree sin(const ComputedTree& in)
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
        case(NLOPP):
            os<<"-"<<*(obj.in1_);
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
        case(NLNULL):
            os<<"NLNULL";
            break;
        default: os<<"TYPE "<< obj.type_ <<" undefined";
            exit(0);
    }
    return os;
}
