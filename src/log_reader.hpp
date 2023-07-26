#pragma once 

#include <string>
#include <fstream>

class LogReader final
{
public:
    ~LogReader();

    // should open the file, true in case of success, false in case of an error
    bool Open(const std::string & file_path); // parameters are up to implementation

    // should close the file
    void Close();

    // sets the mask for filtering the log lines, returns false in case of an error
    bool SetFilterMask(const char* mask);

    // Retrieves the next line from file which matching the filter mask
    // the buffer - where the line will be written, buffer_size - is max buffer size
    // Function will return false in case of an error or end of file is reached
    bool GetNextLine(char* buffer, const int buffer_size); 

private:
    std::ifstream file; 
    std::string pattern_mask;
};