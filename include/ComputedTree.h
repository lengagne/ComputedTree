#ifndef COMPUTEDTREE_H
#define COMPUTEDTREE_H
#include <list>
#include <map>
#include <algorithm>

class ComputedTree;

typedef enum {COS, SIN }NLType;

typedef struct Monomial{
    std::list<ComputedTree* > mono;
    long double value;      // used to estimate the value
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

        ComputedTree(const long double& input);

        std::string get_name() const
        {
            return name_;
        }

        void prepare_file( const std::string & filename="ComputedTreeGenerated.cpp");

        void set_as_input(const std::string& name="undefined");

        void set_as_output( unsigned int index,
                            unsigned int num_out=0,
                            const std::string& name="undefined");

        friend std::ostream& operator<<(std::ostream& os, const ComputedTree& obj);

        ComputedTree operator* (const long double & d) const;

        ComputedTree operator+ (const ComputedTree& in) const;
        ComputedTree operator- (const ComputedTree& in) const;
        ComputedTree operator* (const ComputedTree& in) const;

//    protected:
//    private:
        std::map<Monomial*,long double> polynomial_;
        long double value_;
        std::string name_;

        std::map<ComputedTree*, NLType > nonlinear_sons_;

        std::pair<ComputedTree*, NLType > nonlinear_dad_;
};

// cos and sin function can be used only for alone inputs
ComputedTree cos(ComputedTree& in);
ComputedTree sin(ComputedTree& in);

inline ComputedTree operator * (const long double b, const ComputedTree & a)
{
    return a*b;
}

#endif // COMPUTEDTREE_H
