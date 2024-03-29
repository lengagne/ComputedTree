#include <iostream>
#include "ComputedTree.h"

int main()
{
	std::cout<<"test of ComputedTree"<<std::endl;


	ComputedTree b = 2.0;
	ComputedTree a = 1.0;
	a.set_as_input("a");
	b.set_as_input("b");
	std::cout<<"a = "<< a <<std::endl;
	std::cout<<"b = "<< b <<std::endl;

    ComputedTree c = a*2.12332223365000005;
    std::cout<<"c = "<< c <<std::endl;
    ComputedTree d = a*(-2.3) + b;
    std::cout<<"d = "<< d <<std::endl;
    ComputedTree e = a*20.3 - 2.000*b;
    std::cout<<"e = "<< e <<std::endl;
    ComputedTree f = b*123456789.987654321;
    std::cout<<"f = "<< f <<std::endl;

    ComputedTree g = a*b;
    std::cout<<"g = "<< g <<std::endl;

    ComputedTree h = a*a - 2*a*b + b*b; //(a-2.0)*(b-2.0);
    std::cout<<"h = "<< h <<std::endl;
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
    std::cout<<"ca = "<< ca <<std::endl;
    ComputedTree cb = cos(a) * sin(b) - cos(b) * sin(a);
    std::cout<<"cb = "<< cb <<std::endl;
    ComputedTree cc = k*(cos(a) * sin(b) - cos(b) * sin(a));
    std::cout<<"cc = "<< cc <<std::endl;
    ca.set_as_output(0,4,"ca");
    cb.set_as_output(1,4,"cb");
    cc.set_as_output(2,4,"cc");

    ComputedTree toto = a * b - cos(a) * sin(a);
    toto.set_as_output(0,5,"toto");

    j.prepare_file();
    a.show_all();
	return 0;
}
