// including 'cpp' instead of 'hpp' intentional, since I want to get
// access to functions that only accessable inside 'matcher' translation unit (internal linkage).
// Hiding those function was done to avoid contamination of external reference of object file.
#include "src/matcher.cpp"

#include <iostream>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch2/catch.hpp"

using namespace std;

TEST_CASE( "get_next_char", "" ) 
{
    SECTION("normal_string")
    {
        string str{"Hello"};
        
        for( size_t i = 0; i < str.size() - 1; ++i )
        {
            const auto res = get_next_char( str, i );

            REQUIRE( res != std::nullopt );
            REQUIRE( *res == str[i+1] );
        }

        REQUIRE_FALSE( get_next_char( str, str.size() ) != std::nullopt );
    }

    SECTION("empty_string")
    {
        string str{""};
        
        REQUIRE_FALSE( get_next_char( str, str.size() ) != std::nullopt );
    }
}

TEST_CASE( "is_string_accepted", "" )
{
    REQUIRE( is_string_accepted( std::vector{5}, std::vector{1,2,3,4,5}) );
    REQUIRE( is_string_accepted( std::vector{5}, std::vector{1,2,3,4,5,6,7}) );
    REQUIRE( is_string_accepted( std::vector{5,6}, std::vector{1,2,3,4,5,6,7}) );

    REQUIRE_FALSE( is_string_accepted( std::vector<state_t>{}, std::vector{1,2,3,4,5,6,7}) );
    REQUIRE_FALSE( is_string_accepted( std::vector{8}, std::vector{1,2,3,4,5,6,7}) );
    REQUIRE_FALSE( is_string_accepted( std::vector{1,2}, std::vector{3,4,5,6,7}) );
}

TEST_CASE( "get_next_states", "" )
{
    const auto next_states = get_next_states( 'i', std::set<state_t>{}, std::vector<std::vector<StateMachine::Transition> >{} );
    
    REQUIRE( next_states.size() == 1 );
    REQUIRE( *std::begin(next_states) == 0 );
}

TEST_CASE( "number_of_operators", "" ) 
{
    const auto rules = RulesWithPlus();
    REQUIRE( number_of_operators("*t", rules) == 1 );
    REQUIRE( number_of_operators("+t", rules) == 1 );
    REQUIRE( number_of_operators("?t", rules) == 1 );
    REQUIRE( number_of_operators("*?test", rules) == 2 );
    REQUIRE( number_of_operators("+?test", rules) == 2 );
    REQUIRE( number_of_operators("??test", rules) == 2 );
    REQUIRE( number_of_operators("*?+est", rules) == 3 );
}

TEST_CASE( "min_characters_to_match", "" ) 
{
    const auto rules = RulesWithPlus();
    REQUIRE( min_characters_to_match('*', rules) == 0 );
    REQUIRE( min_characters_to_match('+', rules) == 1 );
    REQUIRE( min_characters_to_match('?', rules) == 1 );
    
    for( int i = 'a'; i <= 'z'; ++i )
        REQUIRE( min_characters_to_match(static_cast<char>(i), rules) == 1 );
}

TEST_CASE( "pattern_normalization", "" ) 
{
    REQUIRE( normalize_pattern("******test") == "*test" );
    REQUIRE( normalize_pattern("test******") == "test*" );
    REQUIRE( normalize_pattern("test*+") == "test+" );
    REQUIRE( normalize_pattern("test****+") == "test+" );
    REQUIRE( normalize_pattern("*+test") == "+test" );
    REQUIRE( normalize_pattern("****+test") == "+test" );
    REQUIRE( normalize_pattern("******test******") == "*test*" );
    REQUIRE( normalize_pattern("?????test?????") == "?????test?????" );
    REQUIRE( normalize_pattern("?????test") == "?????test" );
    REQUIRE( normalize_pattern("test?????") == "test?????" );
    REQUIRE( normalize_pattern("test") == "test" );
}

TEST_CASE( "testing_all_operators", "" )
{
    SECTION("?") {
        REQUIRE( match("????"s, "test"s ) );
        REQUIRE( match("????"s, "testbestwest"s ) );

        REQUIRE_FALSE( match("?test"s, "test123"s ) );
        REQUIRE( match("?test"s, "1test1"s ) );
        REQUIRE( match("?test"s, "1test"s ) );
        REQUIRE( match("?test"s, "1testbest"s ) );
        REQUIRE( match("?test"s, "123test"s ) );
        
        REQUIRE( match("test?"s, "test1"s ) );
        REQUIRE( match("test?"s, "testt"s ) );
        REQUIRE( match("test?"s, "testwow"s ) );
        REQUIRE_FALSE( match("test?"s, "wowtest"s ) );
        REQUIRE( match("test?"s, "wowtestwow"s ) );

        REQUIRE_FALSE( match("?test?"s, "testt"s ) );
        REQUIRE_FALSE( match("?test?"s, "ttest"s ) );
        REQUIRE( match("?test?"s, "ttestt"s ) );
        REQUIRE( match("????test?"s, "testtesttest"s ) );
        REQUIRE( match("????test????"s, "testtesttest"s ) );
        REQUIRE_FALSE( match("????test?????"s, "testtesttest"s ) );
    }

    SECTION("*") {
        REQUIRE( match("*test"s, "test"s ) );
        REQUIRE( match("*test"s, "testtest"s ) );

        REQUIRE( match("test*"s, "test"s ) );
        REQUIRE( match("test*"s, "testtest"s ) );

        REQUIRE( match("*test*"s, "test123"s ) );
        REQUIRE( match("test*"s, "test123"s ) );

        REQUIRE( match("*test"s, "123test"s ) );

        REQUIRE( match("t*t"s, "tt"s ) );

        REQUIRE( match("t*t"s, "test"s ) );
        REQUIRE( match("t*t"s, "testesttest"s ) );

        REQUIRE( match("*test*"s, "test"s ) );
        REQUIRE( match("*test*"s, "test123"s ) );
        REQUIRE( match("*test*"s, "123test"s ) );
        REQUIRE( match("*test*"s, "123test123"s ) );

        REQUIRE( match("*test"s, "test"s ) );
        REQUIRE( match("*test"s, "btest"s ) );
        REQUIRE( match("*test"s, "qwertyuioptest"s ) );
        REQUIRE( match("*test"s, "testest"s ) );

        REQUIRE_FALSE( match("cunq*"s, "that cunning peculiar wo"s ) );

        REQUIRE( match("eding*", "above once a quarter--I hope you keep accunt of Roger's purseeding in") );
        REQUIRE( match("*eding", "above once a quarter--I hope you keep accunt of Roger's purseeding in") );
    }

    SECTION("+") {
        REQUIRE_FALSE( match("+fart"s, "face or front did it have; no conceivable token of either sensation or"s ) );        

        REQUIRE( match("+t+t+"s, "besttestbest"s ) );
        REQUIRE( match("+t+t+"s, "besttestwes"s ) );

        REQUIRE_FALSE( match("+tt+"s, "bestestwes"s ) );

        REQUIRE( match("+tt+"s, "besttestwes"s  ) );
        REQUIRE_FALSE( match("t+t"s, "tt"s ) );
        REQUIRE( match("t+t"s, "test"s ) );
        REQUIRE( match("t+t"s, "testesttest"s ) );

        REQUIRE_FALSE( match("+test"s, "test"s ) );
        REQUIRE( match("+test"s, "testtest"s ) );
        REQUIRE( match("+test"s, "bbbtestbbb"s ) );

        REQUIRE( match("test+"s, "bbbtestbbb"s ) );
        REQUIRE( match("test+"s, "testbbb"s ) );
        REQUIRE_FALSE( match("test+"s, "test"s ) );
        REQUIRE_FALSE( match("test+"s, "bbbtest"s ) );

        REQUIRE( match("+test+"s, "bbbtestbbb"s ) );
        REQUIRE_FALSE( match("+test+"s, "testbbb"s ) );
        REQUIRE_FALSE( match("+test+"s, "bbbtest"s ) );
        
        REQUIRE( match("t+st"s, "bbbtestbbb"s) );
        REQUIRE_FALSE( match("t+st"s, "bbbtstbbb"s) );
        REQUIRE( match("t+st"s, "testtest"s) );    
    }

    SECTION("+_and_*_and_?" )
    {
        REQUIRE( match("*test+"s, "testbest"s) );
        REQUIRE_FALSE( match("+test*"s, "testbest"s) );
        REQUIRE( match("+test*"s, "besttest"s) );
        REQUIRE( match("+t+t*"s, "besttestbest"s) );
        
        REQUIRE( match("+t*t+"s, "besttestwes"s) );

        // beginning
        REQUIRE( match("*?test"s, "btest"s) );
        REQUIRE( match("*?test"s, "btestwaste"s) );
        REQUIRE_FALSE( match("*?test"s, "testHASTE"s) );

        // // middle
        REQUIRE( match("te*?t"s, "test"s) );
        REQUIRE( match("test*?e"s, "btestwaste"s) );
        REQUIRE_FALSE( match("te*?st"s, "testHASTE"s) );

        // // end
        REQUIRE( match("test*?"s, "testb"s) );
        REQUIRE( match("test*?"s, "btestwaste"s) );
        REQUIRE( match("test*?"s, "testHASTE"s) );

    }

    SECTION( "without_operators" )
    { 
        REQUIRE( match("test"s, "test"s) );
        REQUIRE_FALSE( match("test"s, "testtest"s ) );
    }
}