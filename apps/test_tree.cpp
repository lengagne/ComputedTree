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
    ComputedTree d = a*(-2.3);
    ComputedTree e = a*20.3;
    ComputedTree f = b*123456789.987654321;

    c.set_as_output(17,0,"c");
    d.set_as_output(2,10,"d");
    e.set_as_output(0,1,"e");
    f.set_as_output(1,1,"f");


    c.prepare_file();
    std::cout<<"end of the program"<<std::endl;
	return 0;
}
