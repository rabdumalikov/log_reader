#include "src/log_reader.hpp"

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch2/catch.hpp"

using namespace std;

TEST_CASE( "log_reader", "" )
{
    SECTION("Open") {
        LogReader reader;
    
        REQUIRE_FALSE( reader.Open("not_existing_file.txt") );
        REQUIRE_FALSE( reader.Open("") );

        REQUIRE( reader.Open("../../tests/test.txt") );
    }

    SECTION("Close_With_Open") {
        LogReader reader;
        REQUIRE( reader.Open("../../tests/test.txt") );
        REQUIRE_NOTHROW( reader.Close() );
    }

    SECTION("Close_Without_Open") {
        LogReader reader;
        REQUIRE_NOTHROW( reader.Close() );
    }

    SECTION("SetFilterMask_With_Open") {
        LogReader reader;
        REQUIRE( reader.Open("../../tests/test.txt") );
        REQUIRE( reader.SetFilterMask("test") );
        REQUIRE( reader.SetFilterMask("") );

        REQUIRE_FALSE( reader.SetFilterMask(nullptr) );
    }

    SECTION("SetFilterMask_Without_Open") {
        LogReader reader;
        REQUIRE( reader.SetFilterMask("test") );
        REQUIRE( reader.SetFilterMask("") );

        REQUIRE_FALSE( reader.SetFilterMask(nullptr) );
    }

    SECTION("GetNextLine_With_Open") {
        LogReader reader;
        REQUIRE( reader.Open("../../tests/test.txt") );
        REQUIRE( reader.SetFilterMask("test") );
        
        {
            std::array< char, 100 > buffer{};
            
            REQUIRE( reader.GetNextLine(buffer.data(), buffer.size()) );
            REQUIRE( std::string(buffer.data()) == "test" );
        }
        {
            std::array< char, 100 > buffer{};

            REQUIRE_FALSE( reader.GetNextLine(buffer.data(), buffer.size()) );
            REQUIRE( std::string(buffer.data()) == "" );
        }
    }    

    SECTION("GetNextLine_With_Wrong_Buffer_Size") {
        LogReader reader;
        REQUIRE( reader.Open("../../tests/test.txt") );
        REQUIRE( reader.SetFilterMask("test") );
        
        {
            std::array< char, 100 > buffer{};
            
            REQUIRE_FALSE( reader.GetNextLine(buffer.data(), -1 ) );
            REQUIRE( std::string(buffer.data()) == "" );
        }
        {
            std::array< char, 100 > buffer{};
            
            REQUIRE_FALSE( reader.GetNextLine(buffer.data(), 0 ) );
            REQUIRE( std::string(buffer.data()) == "" );
        }
        {
            std::array< char, 100 > buffer{};
            
            REQUIRE_FALSE( reader.GetNextLine(buffer.data(), 2 ) );
            REQUIRE( std::string(buffer.data()) == "" );
        }
    }

    SECTION("GetNextLine_Without_Mask") {
        LogReader reader;
        REQUIRE( reader.Open("../../tests/test.txt") );        
        {
            std::array< char, 100 > buffer{};
            
            REQUIRE_FALSE( reader.GetNextLine(buffer.data(), buffer.size()) );
            REQUIRE( std::string(buffer.data()) == "" );
        }
    }

    SECTION("GetNextLine_Without_Open") {
        LogReader reader; 
        REQUIRE( reader.SetFilterMask("test") );       
        {
            std::array< char, 100 > buffer{};
            
            REQUIRE_FALSE( reader.GetNextLine(buffer.data(), buffer.size()) );
            REQUIRE( std::string(buffer.data()) == "" );
        }
    }

    SECTION("GetNextLine_Change_Mask_On_the_Fly") {
        LogReader reader; 
        REQUIRE( reader.Open("../../tests/test.txt") );
        REQUIRE( reader.SetFilterMask("test?") );       

        for( const auto & val : { "test123"s, "123test123"s } )
        {
            std::array< char, 100 > buffer{};
            
            REQUIRE( reader.GetNextLine(buffer.data(), buffer.size()) );
            REQUIRE( std::string(buffer.data()) == val );
        }

        {
            std::array< char, 100 > buffer{};

            REQUIRE_FALSE( reader.GetNextLine(buffer.data(), buffer.size()) );
            REQUIRE( std::string(buffer.data()) == "" );
        }

        REQUIRE( reader.SetFilterMask("test*") ); 

        for( const auto & val : { "test123"s, "123test123"s, "123test"s, "test"s } )
        {
            std::array< char, 100 > buffer{};
            
            REQUIRE( reader.GetNextLine(buffer.data(), buffer.size()) );
            REQUIRE( std::string(buffer.data()) == val );
        }

        {
            std::array< char, 100 > buffer{};

            REQUIRE_FALSE( reader.GetNextLine(buffer.data(), buffer.size()) );
            REQUIRE( std::string(buffer.data()) == "" );
        }

        REQUIRE( reader.SetFilterMask("*test*") ); 

        for( const auto & val : { "test123"s, "123test123"s, "123test"s, "test"s } )
        {
            std::array< char, 100 > buffer{};
            
            REQUIRE( reader.GetNextLine(buffer.data(), buffer.size()) );
            REQUIRE( std::string(buffer.data()) == val );
        }

        {
            std::array< char, 100 > buffer{};

            REQUIRE_FALSE( reader.GetNextLine(buffer.data(), buffer.size()) );
            REQUIRE( std::string(buffer.data()) == "" );
        }
    }

    SECTION("GetNextLine") 
    {
        LogReader reader1; 
        REQUIRE( reader1.Open("../../tests/test2.txt") );
        REQUIRE( reader1.SetFilterMask("whose*") );       

        LogReader reader2; 
        REQUIRE( reader2.Open("../../tests/test2.txt") );
        REQUIRE( reader2.SetFilterMask("*whose") );

        std::array< char, 100 > buffer1{};
        REQUIRE( reader1.GetNextLine(buffer1.data(), buffer1.size()) );
        
        std::array< char, 100 > buffer2{};
        REQUIRE( reader2.GetNextLine(buffer2.data(), buffer2.size()) );

        REQUIRE( buffer1 == buffer2 );

        REQUIRE_FALSE( reader1.GetNextLine(buffer1.data(), buffer1.size()) );
        REQUIRE_FALSE( reader2.GetNextLine(buffer2.data(), buffer2.size()) );
    }

    SECTION("GetNextLine") 
    {
        LogReader reader1; 
        REQUIRE( reader1.Open("../../tests/test2.txt") );
        REQUIRE( reader1.SetFilterMask("whose+") );       

        LogReader reader2; 
        REQUIRE( reader2.Open("../../tests/test2.txt") );
        REQUIRE( reader2.SetFilterMask("+whose") );

        std::array< char, 100 > buffer1{};
        REQUIRE( reader1.GetNextLine(buffer1.data(), buffer1.size()) );
        
        std::array< char, 100 > buffer2{};
        REQUIRE( reader2.GetNextLine(buffer2.data(), buffer2.size()) );

        REQUIRE( buffer1 == buffer2 );

        REQUIRE_FALSE( reader1.GetNextLine(buffer1.data(), buffer1.size()) );
        REQUIRE_FALSE( reader2.GetNextLine(buffer2.data(), buffer2.size()) );
    }

    SECTION("GetNextLine") 
    {
        LogReader reader1; 
        REQUIRE( reader1.Open("../../tests/test2.txt") );
        REQUIRE( reader1.SetFilterMask("Misshapen*") );       

        LogReader reader2; 
        REQUIRE( reader2.Open("../../tests/test2.txt") );
        REQUIRE( reader2.SetFilterMask("*Misshapen") );

        LogReader reader3; 
        REQUIRE( reader3.Open("../../tests/test2.txt") );
        REQUIRE( reader3.SetFilterMask("*Misshapen*") );

        std::array< char, 100 > buffer1{};
        REQUIRE( reader1.GetNextLine(buffer1.data(), buffer1.size()) );
        
        std::array< char, 100 > buffer2{};
        REQUIRE( reader2.GetNextLine(buffer2.data(), buffer2.size()) );

        std::array< char, 100 > buffer3{};
        REQUIRE( reader3.GetNextLine(buffer3.data(), buffer3.size()) );

        REQUIRE( buffer1 == buffer2 );
        REQUIRE( buffer2 == buffer3 );

        REQUIRE_FALSE( reader1.GetNextLine(buffer1.data(), buffer1.size()) );
        REQUIRE_FALSE( reader2.GetNextLine(buffer2.data(), buffer2.size()) );
        REQUIRE_FALSE( reader3.GetNextLine(buffer3.data(), buffer3.size()) );
    }

    SECTION("GetNextLine") 
    {
        LogReader reader1; 
        REQUIRE( reader1.Open("../../tests/test2.txt") );
        REQUIRE( reader1.SetFilterMask("Misshapen+") );       

        LogReader reader2; 
        REQUIRE( reader2.Open("../../tests/test2.txt") );
        REQUIRE( reader2.SetFilterMask("+Misshapen") );

        std::array< char, 100 > buffer1{};
        REQUIRE( reader1.GetNextLine(buffer1.data(), buffer1.size()) );
        
        std::array< char, 100 > buffer2{};
        REQUIRE_FALSE( reader2.GetNextLine(buffer2.data(), buffer2.size()) );

        REQUIRE_FALSE( buffer1 == buffer2 );

        REQUIRE_FALSE( reader1.GetNextLine(buffer1.data(), buffer1.size()) );
        REQUIRE_FALSE( reader2.GetNextLine(buffer2.data(), buffer2.size()) );
    }

    SECTION("GetNextLine") 
    {
        LogReader reader1; 
        REQUIRE( reader1.Open("../../tests/test2.txt") );
        REQUIRE( reader1.SetFilterMask("Mi??hapen") );   

        std::array< char, 100 > buffer1{};
        REQUIRE( reader1.GetNextLine(buffer1.data(), buffer1.size()) );
    
        REQUIRE( std::string(buffer1.data()) == "Misshapen chaos of well-seeming forms!" );
        REQUIRE_FALSE( reader1.GetNextLine(buffer1.data(), buffer1.size()) );
    }

    SECTION("GetNextLine") 
    {
        LogReader reader1; 
        REQUIRE( reader1.Open("../../tests/test2.txt") );
        REQUIRE( reader1.SetFilterMask("Mi?????en") );   

        std::array< char, 100 > buffer1{};
        REQUIRE( reader1.GetNextLine(buffer1.data(), buffer1.size()) );
    
        REQUIRE( std::string(buffer1.data()) == "Misshapen chaos of well-seeming forms!" );
        REQUIRE_FALSE( reader1.GetNextLine(buffer1.data(), buffer1.size()) );
    }
}