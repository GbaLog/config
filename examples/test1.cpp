/////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "../config.hpp"
/////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    tools::config cfg{};
    
    if( !cfg.load_file("examples/test1.cfg") )
    {
        std::cerr << "Can't load file!\n";
    }
    
    std::string str = "string";
    cfg.get( "string", str ); // in file she's "some string"
    
    int value = 0;
    cfg.get( "int", value ); // in file she's "2147483647"
    
    double dbl = 3.141592653589;
    cfg.get( "double", dbl ); // in file she's "2.7182818284590"
    
    bool b = false;
    cfg.get( "bool", b ); // in file she's "true"
    
    std::cout << std::boolalpha;
    std::cout
        << "string: " << str << std::endl
        << "int: " << value << std::endl
        << "double: " << dbl << std::endl
        << "bool: " << b << std::endl;
}