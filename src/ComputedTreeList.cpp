        #include "ComputedTreeList.h"
#include <iostream>
#include <fstream>

ComputedTreeList::~ComputedTreeList()
{
    inputs_.clear();
    monomials_.clear();
}

Monomial* ComputedTreeList::add_input(ComputedTree* in)
{
//    std::cout<<"ComputedTreeListadd_input " << *in <<std::endl;
    // if does not already exist
    std::list<ComputedTree* >::iterator it = std::find(inputs_.begin(), inputs_.end(), in);
    if(it == inputs_.end())
    {
        inputs_.push_back(in);
        Monomial tmp ;
        tmp.mono.push_back(in);
        tmp.update = true;
        update_name(&tmp);
        monomials_.push_back(tmp);
        Monomial* out = &( *std::find(monomials_.begin(), monomials_.end(), tmp));
        return out;
    }
    else
    {
        std::cerr<<"We do not add this input since it already exists"<<std::endl;
        exit(2);
    }
}

Monomial* ComputedTreeList::add_intermediate(Monomial& in)
{
    std::list<Monomial >::iterator it = std::find(monomials_.begin(), monomials_.end(), in);
    if(it == monomials_.end())
    {
        monomials_.push_back(in);
        return & (*std::find(monomials_.begin(), monomials_.end(), in));
    }
    else
    {
        return &(*it);
    }
}

void ComputedTreeList::add_output(  const ComputedTree& in,
                                    unsigned int index,
                                    unsigned int out)
{
    output_index tmp;
    tmp.index1 = out;
    tmp.index2 = index;

    unsigned int nb = outputs_.size();
    for (unsigned int i=0;i<nb;i++)
    {
        if(output_num_[i] == out && output_index_[i] == index)
        {
            std::cerr<<"Error when add_output index and out are already defined"<<std::endl;
            exit(1);
        }
    }
    outputs_.push_back(in);
    output_num_.push_back(out);
    output_index_.push_back(index);
}

void ComputedTreeList::prepare_file( const std::string & filename)
{
    std::cout<<"ComputedTreeList::prepare_file "<< filename<<std::endl;
    unsigned int nb_in = inputs_.size();

    // create the file
    std::ofstream f (filename );

    std::string class_name  = filename;
    class_name.erase(class_name.find_last_of("."), std::string::npos);

    f<<"class "<<class_name<<"\n{\n";

    f<<"\tunsigned int nb_in = "<< nb_in <<";\n";
    f<<"\t // temporary variables\n";

    for (std::list<Monomial>::iterator it=monomials_.begin(); it!=monomials_.end(); it++)
    {
        f<<"\tlong double "<<it->name<<";\n";
    }
    unsigned int cpt = 0;

    f<<"\tlong double set_input(std::vector<long double> & in)\n\t{\n";
    for (std::list<ComputedTree*>::const_iterator it=inputs_.begin(); it!=inputs_.end(); it++)
        f<< "\t\t"<< (*it)->get_name() <<" = "<<" in["<<cpt++<<"];"<<std::endl;

    f<<"\t}\n\n";

    f<<"\tlong double function(unsigned int out, unsigned int index)\n\t{\n\t\tswitch(out)\n\t\t{\n";

    // find max output
    unsigned int nb_output = 0; // std::max_element(output_num_.begin(), output_num_.end()) + 1;
    for (std::vector<unsigned int>::const_iterator it=output_num_.begin(); it!=output_num_.end(); it++)
        if(*it > nb_output)
            nb_output=*it;

    nb_output ++;
    for (unsigned int i=0;i<nb_output;i++)
    {
//        std::cout<<"preparing outputs number "<< i<<std::endl;
        // find the maximal index for this output
        unsigned int max_index = 0;
        bool one_find = false;  // allows to know if one index is found for the output
        std::vector<unsigned int>::const_iterator itout=output_num_.begin();
        std::vector<unsigned int>::const_iterator itindex=output_index_.begin();
        for( ; itout!=output_num_.end(); itout++,itindex++ )
        {
            if(*itout == i )
            {
                one_find = true;
                if(*itindex>max_index)
                    max_index=*itindex;
            }
        }

        if(one_find)
        {
            f<<"\t\t\tcase("<<i<<"):\n";
            std::cout<<"maximal index: "<<max_index<<" of output "<<i<<std::endl;
            // reset all the monomial computation
            for (std::list<Monomial>::iterator it=monomials_.begin(); it!=monomials_.end(); it++)
                it->update = false;

            f<<"\t\t\t\tswitch(index)\n\t\t\t\t{\n";


            for (unsigned int j=0;j<=max_index;j++)
            {
                // try to find if we have to do this index.
                std::vector<unsigned int>::const_iterator itout=output_num_.begin();
                std::vector<unsigned int>::const_iterator itindex=output_index_.begin();
                std::vector<ComputedTree >::const_iterator itctree = outputs_.begin();
                for( ; itout!=output_num_.end(); itout++,itindex++,itctree++ )
                {
//                    std::cout<<*itout<<" "<< i<< "       "<<*itindex<<" "<< j <<std::endl;
                    if(*itout == i && *itindex == j)
                    {
                        f<<"\t\t\t\t\t//prepare the basic elements of output "<< i<<" index "<< j<< std::endl;
                        f<<"\t\t\t\t\tcase("<<j<<"):\n";

                        // update the intermediate value
                        std::vector<Monomial*> mono_list = get_monomial_update_list( & (*itctree) );    // get the list of monomial to update
                        for(std::vector<Monomial*>::iterator itmono= mono_list.begin();itmono!= mono_list.end();itmono++)
                        {
                                (*itmono)->update = true;
                                f<<"\t\t\t\t\t\t// update of "<< (*itmono)->name<<":\n";
                                f<<"\t\t\t\t\t\t"<< (*itmono)->name<<" = 1.";
                                for(std::list<ComputedTree*>::const_iterator itct= (*itmono)->mono.begin();itct!= (*itmono)->mono.end();itct++)
                                {
                                    f<<"*"<< (*itct)->get_name();
                                }
                                f<<"\n";

                        }
                        std::cout<<"update done"<<std::endl;



                        std::string formula = "return ";
                        // update the monomial

                        std::map<Monomial*,long double>::const_iterator itpol = itctree->polynomial_.begin();
                        for(itpol ; itpol!=itctree->polynomial_.end(); itpol++)
                        {
                            //(*itpol)
                            if(itpol->second  > 0)formula += " + ";
                            formula += to_string_with_precision(itpol->second) + "*" + (itpol->first)->name;
                        }

                        // compute the value from all the monomial
                        f<<"\t\t\t\t\t\t"<<formula<<";\n";
                        break;
                    }
                }
            }
            f<<"\t\t\t\t\tdefault: return 0.;\n\t\t\t\t};\n";
    //        f<<"\t\t\t\tbreak;\n";
        }
    }
    f<<"\t\t\tdefault: return 0.;\n";
    f<<"\n\t\t}\n\t}\n};";


      f.close();
}

std::vector<Monomial*> ComputedTreeList::get_monomial_update_list( const ComputedTree* in) const
{
    std::vector<Monomial*> out;
    for (std::map<Monomial*,long double>::const_iterator it= in->polynomial_.begin(); it!= in->polynomial_.end(); it++)
    {
        std::cout<<"looking at "<< it->first->name <<"  depending on "<< it->first->mono.size()<<" inputs update = "<< it->first->update <<std::endl;
        if (!(it->first)->update &&  (it->first)->mono.size()!= 1)
        {
            out.push_back((it->first));
//            std::cout<<"it->first = "<< it->first<<std::endl;
        }

    }
    return out;
}
