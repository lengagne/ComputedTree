#ifndef COMPUTEDTREELIST_H
#define COMPUTEDTREELIST_H
#include "ComputedTree.h"
#include <sstream>
#include <iomanip>
#include <limits>

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

        /// add output
        /// index is the number of the output
        /// out is to create new independant value (no redundant computation are perform for the same output)
        void add_output(ComputedTree* in,
                        unsigned int index,
                        unsigned int out=0);

        void prepare_file( const std::string & filename="ComputedTreeGenerated.cpp");
    private:

    std::list<ComputedTree* > inputs_;

    std::list<Monomial*> monomials_;

    std::vector<ComputedTree* > outputs_;
    std::vector<unsigned int > output_num_;
    std::vector<unsigned int > output_index_;
    long double updated_value_;
};

#endif // COMPUTEDTREELIST_H