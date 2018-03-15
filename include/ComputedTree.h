#ifndef COMPUTEDTREE_H
#define COMPUTEDTREE_H
#include <list>
#include <map>
#include <algorithm>
#include "AbstractGeneratedCode.h"

class ComputedTree;

typedef enum {NLCOS, NLSIN }NLType;

typedef struct Monomial{
    std::list<ComputedTree* > mono;
//    double value;      // used to estimate the value
    bool update = false;    // used to create the file
    std::string name;
}Monomial;

Monomial merge_monomial(const Monomial& m1, const Monomial& m2);

bool operator==(const Monomial& a, const Monomial&b);
std::ostream& operator<<(std::ostream& os, const Monomial& obj);

void update_name(Monomial* m);

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

        // get the value : assume it is double
        double get_value() const;
//        {
//            return value_;
//        }


        AbstractGeneratedCode* get_recompile_code(const std::string & libname ="")const;

        bool is_double() const;

        void prepare_file( const std::string & filename="ComputedTreeGenerated.cpp");

        void set_as_input(const std::string& name="undefined");

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

        friend std::ostream& operator<<(std::ostream& os, const ComputedTree& obj);

        void operator= (const double & d);
//        void operator= (const ComputedTree& in);

        void operator*= (const double & d);
        void operator+= (const ComputedTree& in);
        void operator-= (const ComputedTree& in);
        void operator*= (const ComputedTree& in);

        ComputedTree operator* (const double & d) const;
        ComputedTree operator/ (const double & d) const;
        ComputedTree operator+ (const ComputedTree& in) const;
        ComputedTree operator- (const ComputedTree& in) const;
        ComputedTree operator* (const ComputedTree& in) const;

//    protected:
//    private:
        std::map<Monomial*,double> polynomial_;
//        double value_;
        std::string name_;

        int input_index_ = -1;

        std::map<ComputedTree*, NLType > nonlinear_sons_;

        std::pair<ComputedTree*, NLType > nonlinear_dad_;
};

// cos and sin function can be used only for alone inputs
ComputedTree cos(ComputedTree& in);
ComputedTree sin(ComputedTree& in);

inline ComputedTree operator * (const double b, const ComputedTree & a)
{
    return a*b;
}

#endif // COMPUTEDTREE_H
