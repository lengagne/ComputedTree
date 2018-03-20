#ifndef COMPUTEDTREE_H
#define COMPUTEDTREE_H
#include <list>
#include <map>
#include <algorithm>
#include "AbstractGeneratedCode.h"

class ComputedTree;

typedef enum {NLNONE, NLDOUBLE, NLIN, NLCOS, NLSIN, NLOPP, NLADD, NLSUB, NLMUL }NLType;

class ComputedTree
{
    public:
        ComputedTree();
        virtual ~ComputedTree();
        ComputedTree(const ComputedTree& other);

        ComputedTree(const double& input);

        std::string get_name() const
        {
            return name_;
        }

        unsigned int get_tmp_index()const
        {
            return tmp_index_;
        }

        std::string get_tmp_name();

//
//        // get the value : assume it is double
//        double get_value() const;

        AbstractGeneratedCode* get_recompile_code(const std::string & libname ="")const;

//        bool is_double() const;
//
        void prepare_file( const std::string & filename="ComputedTreeGenerated.cpp");

        void set_as_input(const std::string& name="undefined");
//
        void set_as_output( unsigned int index,
                            unsigned int num_out=0,
                            const std::string& name="undefined");

        void set_input_vector(double d, std::vector<double>& v)
        {
            v[input_index_] = d;
        }

        void set_input_id( unsigned int id)
        {
            input_index_ = id;
        }

        void set_name(const std::string & n)
        {
            name_ = n;
        }

        void show_all()const;


//        void operator= (const double & d);
////        void operator= (const ComputedTree& in);
//
        void operator*= (const double & d)
        {
            *this = *this * d;
        }

        void operator+= (const ComputedTree& in);
        void operator-= (const ComputedTree& in);
        void operator*= (const ComputedTree& in);

        ComputedTree operator- () const;

        inline ComputedTree operator* (const double & d) const
        {
            return (ComputedTree(d)) *  (*this);
        }
//        ComputedTree operator/ (const double & d) const;
        ComputedTree operator+ (const ComputedTree& in) const;
        ComputedTree operator- (const ComputedTree& in) const;
        ComputedTree operator* (const ComputedTree& in) const;

        bool is_set()const
        {
            return updated_;
        }

        void set()
        {
            updated_ = true;
        }

        void set_and_sons(const std::string& tab="");

        void set_id(unsigned int id)
        {
            tmp_index_ = id;
        }

        void reset()
        {
            updated_ = false;
        }

        friend std::ostream& operator<<(std::ostream& os, const ComputedTree& obj);
        friend ComputedTree cos(ComputedTree& in);
        friend ComputedTree sin(ComputedTree& in);
        friend class ComputedTreeList;
    private:
        int input_index_ = -1;
        std::string name_;
        double value_;
        ComputedTree * in1_;
        ComputedTree * in2_;
        ComputedTree * me_;
        NLType type_;

        bool updated_;
        unsigned int tmp_index_;
};

// cos and sin function can be used only for alone inputs



inline ComputedTree operator * (const double b, const ComputedTree & a)
{
    return a*b;
}

#endif // COMPUTEDTREE_H
