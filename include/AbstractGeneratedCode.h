#include <vector>
#include <math.h>

class AbstractGeneratedCode
{
public:
	virtual unsigned int get_nb_in()const  = 0;

	virtual void set_input(std::vector<double> & in) = 0;

	virtual double function(unsigned int index, unsigned int out=0) = 0;
};
// the types of the class factories
typedef AbstractGeneratedCode* create_code();
typedef void destroy_code(AbstractGeneratedCode*);
