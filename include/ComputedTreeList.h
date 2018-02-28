#ifndef COMPUTEDTREELIST_H
#define COMPUTEDTREELIST_H
#include <list>
#include <map>
#include <algorithm>


class ComputedTree;

class ComputedTreeList
{
    public:
        ComputedTreeList(){}
        ~ComputedTreeList();

        void add_input(ComputedTree* in);
    private:

    std::list<ComputedTree* > inputs_;
    long double updated_value_;
};

#endif // COMPUTEDTREELIST_H
