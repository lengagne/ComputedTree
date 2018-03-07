#include <iostream>
#include "ComputedTree.h"

int main()
{
	std::cout<<"test of ComputedTree"<<std::endl;

	ComputedTree a = 1.0;
	ComputedTree b = 2.0;
	std::cout<<"a = "<< a <<std::endl;
	a.set_as_input("a");
	b.set_as_input("b");

    ComputedTree c = a*2.12332223365000005;
    ComputedTree d = a*(-2.3) + b;
    std::cout<<"d = "<< d <<std::endl;
    ComputedTree e = a*20.3 - 2.000*b;
    ComputedTree f = b*123456789.987654321;

    ComputedTree g = a*b;
    ComputedTree h = a*a - 2*a*b + b*b; //(a-2.0)*(b-2.0);
    ComputedTree i = (a+b)*a;
    std::cout<<"i = "<< i <<std::endl;
    ComputedTree j =  (a+b)*(a-b);
    std::cout<<"j = "<< j <<std::endl;
    ComputedTree k =  j*i;

    c.set_as_output(0,0,"c");
    f.set_as_output(1,0,"f");
    d.set_as_output(0,1,"d");
    e.set_as_output(1,1,"e");
    g.set_as_output(0,2,"g");
    h.set_as_output(1,2,"h");
    i.set_as_output(0,3,"i");
    j.set_as_output(1,3,"j");
    k.set_as_output(2,3,"k");

    ComputedTree ca = cos(a);
    ComputedTree cb = cos(a) * sin(b) - cos(b) * sin(a);
    ComputedTree cc = k*(cos(a) * sin(b) - cos(b) * sin(a));
    std::cout<<"CA = "<< ca <<std::endl;
    ca.set_as_output(0,4,"ca");
    cb.set_as_output(1,4,"cb");
    cc.set_as_output(2,4,"cc");

    ComputedTree toto = a * b - cos(a) * sin(a);
    toto.set_as_output(0,5,"toto");

    j.prepare_file();

    AbstractGeneratedCode* recompiled_code = j.get_recompile_code();
    std::cout<<"recompiled_code = "<< recompiled_code <<std::endl;

    std::vector<double> in(2);
    in[0] = 1;
    in[1] = 1.5;

    std::cout<<"nb_in = "<< recompiled_code->get_nb_in();

    recompiled_code->set_input(in);
    std::cout<<"c : reel = "<< in[0]*2.12332223365000005<<" vs "<< recompiled_code->function(0,0)<<std::endl;
    std::cout<<"toto : reel = "<< in[0]*in[1] - cos(in[0])*sin(in[0])<<" vs "<< recompiled_code->function(0,5)<<std::endl;

    std::cout<<"end of the program"<<std::endl;
	return 0;
}
