#include <iostream>
#include "matcher.hpp"
#include "log_reader.hpp"


LogReader::~LogReader()
{
    Close();
}

bool LogReader::Open(const std::string & file_path)
{
    if( std::empty(file_path) )
    {
        std::cout << "ERROR: Empty file path" << std::endl;
        return false;
    }

    file.open(file_path);
    
    if( !file.is_open() )
    {
        std::cout << "ERROR: Error opening file: " << file_path << std::endl;
        return false;
    }

    return true;
}

void LogReader::Close() 
{
    file.close();
}


bool LogReader::SetFilterMask(const char* mask)
{
    if( mask == nullptr )
    {
        std::cout << "ERROR: Null pattern mask is not allowed" << std::endl;
        return false;
    }
    
    pattern_mask = mask;

    if( file.is_open() )
    {
        // reset file
        file.clear();
        file.seekg(0);
    }
    
    return true;
}

bool LogReader::GetNextLine(char* buffer, const int buffer_size)
{
    if( buffer_size <= 0 )
    {
        std::cout << "ERROR: Invalid buffer size" << std::endl;
        return false;
    }

    if( std::empty(pattern_mask) )
    {
        std::cout << "ERROR: Empty pattern mask" << std::endl;
        return false;
    }

    if( !file.is_open() )
    {
        std::cout << "ERROR: File is not open" << std::endl;
        return false;
    }

    if( file.eof() ) 
    {
        std::cout << "INFO: Reached the end of the file." << std::endl;
        return false;
    }

    std::string line;

    while( std::getline(file, line) )
    {
        if( std::empty(line) )
        {
            continue;
        }

        size_t pos = line.find('\r');
        if( pos != std::string::npos )
        {
            line.erase( pos, 1 );
        }

        try {
            if( match(pattern_mask, line) )
            {
                if( line.size() > buffer_size )
                {
                    std::cout << "ERROR: Buffer size is too small" << std::endl;
                    return false;
                }

                std::copy(std::begin(line), std::end(line), buffer);

                // Manually add the null-terminator at the end of the buffer
				if( line.size() < buffer_size )
				{
					buffer[line.size()] = '\0';
				}

                return true;
            }
        } 
        catch( const std::exception & e )
        {
            std::cout << "ERROR: " << e.what() << std::endl;

            return false;
        }

    }

    return false;
}