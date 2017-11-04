#include <stdexcept>
#include <iostream>
using namespace std;


int f (int x) throw (runtime_error)
{
    if (x == 0)
	throw runtime_error("f() passed 0");
    else
	return x;
}


int g (int x)
{
    return f(x);
}


int
main ()
try
{
    cout << "Calling f(1): returns " << g(1) << endl;
    cout << "Calling f(0): returns " << g(0) << endl;
}
catch (exception& e) {
    cerr << "Exception: " << e.what() << endl;
    exit(1);
}
