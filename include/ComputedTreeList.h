#ifndef COMPUTEDTREELIST_H
#define COMPUTEDTREELIST_H
#include "ComputedTree.h"
#include "config_ComputedTree.h"
#include <sstream>
#include <iomanip>
#include <limits>
#include <dlfcn.h>

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = std::numeric_limits<long double>::digits10)
{
    std::ostringstream out;
    out << std::setprecision(n) << a_value;
    return out.str();
}

typedef struct output_index{
    unsigned int index1;
    unsigned int index2;
} output_index;

class ComputedTreeList
{
    public:
        ComputedTreeList(){}
        ~ComputedTreeList();

        Monomial* add_input(ComputedTree* in);

        Monomial* add_intermediate(Monomial& in);

        ComputedTree add_non_linear_input( ComputedTree*in,
                                        const NLType& t);

        /// add output
        /// index is the number of the output
        /// out is to create new independant value (no redundant computation are perform for the same output)
        void add_output(const ComputedTree& in,
                        unsigned int index,
                        unsigned int out=0);

        std::vector<Monomial*> get_monomial_update_list( const ComputedTree* in) const;

        AbstractGeneratedCode* get_recompile_code(const std::string & libname="");

        void prepare_file( const std::string & filename="ComputedTreeGenerated.cpp");
    private:

    std::list<ComputedTree* > inputs_;
    std::list<ComputedTree* > nonlinear_inputs_;

    std::list<Monomial> monomials_;

    std::vector<ComputedTree > outputs_;
    std::vector<unsigned int > output_num_;
    std::vector<unsigned int > output_index_;
    long double updated_value_;

    std::string class_name_;

    create_code* creator_;
    destroy_code* destructor_;
};

#endif // COMPUTEDTREELIST_H
