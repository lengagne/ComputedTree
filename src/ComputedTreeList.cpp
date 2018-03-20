#include "ComputedTreeList.h"
#include <iostream>
#include <fstream>
#include <dlfcn.h>

ComputedTreeList::~ComputedTreeList()
{

}

ComputedTree* ComputedTreeList::add_input(const ComputedTree& in)
{
    ComputedTree *t = new ComputedTree(in);
    inputs_.push_back(t);
    return t;
}

ComputedTree* ComputedTreeList::add_intermediate(const ComputedTree& in)
{
    ComputedTree *t = new ComputedTree(in);
    tmp_var_.push_back(t);
    t->me_ = t;
    return t;
}

void ComputedTreeList::add_output(  ComputedTree* in,
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

AbstractGeneratedCode* ComputedTreeList::get_recompile_code(const std::string & libname)
{
    std::string lib;
    if (libname =="")
        lib = "./lib"+class_name_ +".so";
    else
        lib = libname;

    void* library =dlopen(lib.c_str(), RTLD_LAZY);
    if (!library) {
        std::cerr <<"Error in "<<__FILE__<<" at line "<<__LINE__<< " : Cannot load library ("<< lib <<"), with the error : " << dlerror() << '\n';
        exit(0);
    }
    // load the symbols
    creator_ = (create_code*) dlsym(library, "create");
    destructor_ = (destroy_code*) dlsym(library, "destroy");
    if (!creator_ || !destructor_)
    {
        std::cerr <<"Error in "<<__FILE__<<" at line "<<__LINE__<< " : Cannot load symbols of ("<< lib <<"), with the error : " << dlerror() << '\n';
        exit(0);
    }

    return creator_();
}


void ComputedTreeList::prepare_file( const std::string & filename)
{
    for( std::vector<ComputedTree*>::iterator it = tmp_var_.begin(); it!= tmp_var_.end();++it)
    {
        (*it)->reset();
    }

    for(std::vector<ComputedTree*>::iterator it = outputs_.begin(); it!=outputs_.end();++it)
    {
        (*it)->set_and_sons();
    }

    unsigned int nb_v=0;
    for( std::vector<ComputedTree*>::iterator it = inputs_.begin(); it!= inputs_.end();++it)
        (*it)->set_id(nb_v++);
    for( std::vector<ComputedTree*>::iterator it = tmp_var_.begin(); it!= tmp_var_.end();++it)
    {
        if((*it)->is_set())
        {
            (*it)->set_id(nb_v++);
        }
    }
    for (std::vector<ComputedTree*>::const_iterator it=outputs_.begin(); it!=outputs_.end(); it++)
        (*it)->set_id(nb_v++);

    unsigned int nb_in = inputs_.size();

    std::string command = "rm -fv "+ filename;
    std::cout<<"Compilation command is : "<< command<<std::endl;
    int dummy = system ( command.c_str() );

    // create the file
    std::ofstream f (filename );

    class_name_  = filename;
    class_name_.erase(class_name_.find_last_of("."), std::string::npos);
    f<<"#include <vector>\n#include <math.h>\n#include <iostream>\n#include \"AbstractGeneratedCode.h\" \n\n\n";
    f<<"class "<<class_name_<<": public AbstractGeneratedCode\n{\npublic:\n";

    f<<"\tunsigned int get_nb_in()const \n\t{\n\t\treturn "<< nb_in <<";\n\t}\n\n";
    f<<"\t // intermediate variables\n";
    f<<"\tdouble tmp["<<nb_v<<"];\n";
    unsigned int cpt = 0;
    f<<"\n\n\tvoid set_input(std::vector<double> & in)\n\t{\n";
    for (std::vector<ComputedTree*>::const_iterator it=inputs_.begin(); it!=inputs_.end(); it++)
    {
        const ComputedTree& tree = *(*it);
        f<< "\t\ttmp["<< tree.get_tmp_index()<<"] =  in["<<cpt++<<"];"<<std::endl;
    }
    f<<"\t}\n\n";
//
    f<<"\tdouble function(unsigned int index, unsigned int out=0)\n\t{\n\t\tswitch(out)\n\t\t{\n";
    // find max output
    unsigned int nb_output = 0; // std::max_element(output_num_.begin(), output_num_.end()) + 1;
    for (std::vector<unsigned int>::const_iterator it=output_num_.begin(); it!=output_num_.end(); it++)
        if(*it > nb_output)
            nb_output=*it;
    nb_output ++;
    for (unsigned int i=0;i<nb_output;i++)
    {
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
            f<<"\t\t\tcase("<<i<<"): // out number "<< i<<" \n";
            // reset all the monomial computation
            for( std::vector<ComputedTree*>::iterator it = tmp_var_.begin(); it!= tmp_var_.end();++it)
            {
                (*it)->reset();
            }
            f<<"\t\t\t\tswitch(index)\n\t\t\t\t{\n";
            for (unsigned int j=0;j<=max_index;j++)
            {
                // try to find if we have to do this index.
                std::vector<unsigned int>::const_iterator itout=output_num_.begin();
                std::vector<unsigned int>::const_iterator itindex=output_index_.begin();
                std::vector<ComputedTree* >::const_iterator itctree = outputs_.begin();
                for( ; itout!=output_num_.end(); itout++,itindex++,itctree++ )
                {
                    if(*itout == i && *itindex == j)
                    {
                        f<<"\t\t\t\t\tcase("<<j<<"):\n";
                        update_var_file( f , (*itctree),"\t\t\t\t\t");
                        f<<"\t\t\t\t\treturn tmp["<< (*itctree)->get_tmp_index()<<"];\n";
                        break;
                    }
                }
            }
            f<<"\t\t\t\t\tdefault: return 0.;\n\t\t\t\t};\n";
            f<<"\t\t\t\tbreak;\n";
        }
    }
    f<<"\t\t\tdefault: return 0.;\n";
    f<<"\n\t\t}\n\t}\n};\n\n";
    f<<"extern \"C\" " + class_name_ +"* create()\n{\n\treturn new " + class_name_ + "();\n}\n\n";
    f<<"extern \"C\" void destroy(" + class_name_ +"* p)\n{\n\tdelete p;\n}\n\n";
    f.close();
    // Create the library
    command = "g++ -O3 -ggdb -shared " + filename + " -I" + std::string(INCLUDE_DIR) + " -o lib"+class_name_+".so -fPIC";
    std::cout<<"Compilation command is : "<< command<<std::endl;
    system ( command.c_str() );
}

void ComputedTreeList::show_all()const
{
    for (int i=0;i<inputs_.size();i++)
        std::cout<<"inputs["<<i<<"]:"<<inputs_[i]->get_name()<<" = "<< *inputs_[i]<<std::endl;

    for (int i=0;i<tmp_var_.size();i++)
        std::cout<<"tmp_var["<<i<<"]:"<<tmp_var_[i]->get_name()<<" = "<< *tmp_var_[i]<<std::endl;
}

void ComputedTreeList::update_var_file(std::ofstream& f , ComputedTree* v, const std::string& val)
{
    v->set();
    if(v->in1_ && v->in1_->type_ != NLDOUBLE && !v->in1_->is_set())
    {
        update_var_file(f,v->in1_,val);
    }
    if(v->in2_ && v->in2_->type_ != NLDOUBLE && !v->in2_->is_set())
    {
        update_var_file(f,v->in2_,val);
    }

    f<<val<<"tmp["<< v->get_tmp_index()<<"] = ";
    switch(v->type_)
    {
        case(NLCOS):    f<<"cos("<<v->in1_->get_tmp_name()<<");"; break;
        case(NLSIN):    f<<"sin("<<v->in1_->get_tmp_name()<<");"; break;
        case(NLOPP):    f<<"- "<<v->in1_->get_tmp_name()<<";"; break;
        case(NLADD):    f<<v->in1_->get_tmp_name()<<"+"<< v->in2_->get_tmp_name()<<";"; break;
        case(NLMUL):    f<<v->in1_->get_tmp_name()<<"*"<< v->in2_->get_tmp_name()<<";"; break;
        case(NLSUB):    f<<v->in1_->get_tmp_name()<<"-"<< v->in2_->get_tmp_name()<<";"; break;
        default:    break;
    }
    f<<"\n";

}
