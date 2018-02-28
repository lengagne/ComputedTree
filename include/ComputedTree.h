#ifndef COMPUTEDTREE_H
#define COMPUTEDTREE_H

#include "ComputedTreeList.h"


ComputedTreeList chief_;

class ComputedTree
{
    public:
        ComputedTree();
        virtual ~ComputedTree();
        ComputedTree(const ComputedTree& other);

        ComputedTree(const long double& input);

        void set_as_input();

        friend std::ostream& operator<<(std::ostream& os, const ComputedTree& obj);

    protected:
    private:
        //std::map<ComputedTreeList*,long double> polynomial_;
        long double* value_;
};

#endif // COMPUTEDTREE_H
