#include <array>
#include <iostream>

#include "log_reader.hpp"

int main( int argc, char* argv[] )
{
    if( argc < 3 || argc > 3 )
    {
        std::cout << "ERROR: Wrong number [" << (argc - 1) << "] of argument(s) were provided. You must provide exactly two arguments: file_path and search_pattern.\n";

        return -1;
    }
    
    const std::string file_path( argv[1] );
    const std::string pattern( argv[2] );

    LogReader reader;
    
    if( !reader.Open(file_path.c_str()) )
    {
        return -1;
    }

    if( !reader.SetFilterMask(pattern.c_str()) )
    {
        return -1;
    }
    
    std::array< char, 100 > buffer{};
    
    while( reader.GetNextLine(buffer.data(), buffer.size()) )
    {
        std::cout << buffer.data() << std::endl;
    }
    
    reader.Close();

    return 0;
}