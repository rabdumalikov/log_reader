
// !!! HACK below is INTENTIONAL !!!
// It might look suspicios but I did that intentionally
// to test private static functions. I could declare those 
// functions as free ones. However, I didn't want to do that
// since they are not general purpose functions, and making 
// them free might create confusion since usage context would 
// be lost partially. Also, since it is separate binary file, thus 
// there is not harm for main application or production code.
#define private public
#include "src/state_machine.cpp"


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
            const auto res = StateMachine::get_next_char( str, i );

            REQUIRE( res != std::nullopt );
            REQUIRE( *res == str[i+1] );
        }

        REQUIRE_FALSE( StateMachine::get_next_char( str, str.size() ) != std::nullopt );
    }

    SECTION("empty_string")
    {
        string str{""};
        
        REQUIRE_FALSE( StateMachine::get_next_char( str, str.size() ) != std::nullopt );
    }
}

// TEST_CASE( "is_string_accepted", "" )
// {
//     REQUIRE( is_string_accepted( std::vector{5}, std::vector{1,2,3,4,5}) );
//     REQUIRE( is_string_accepted( std::vector{5}, std::vector{1,2,3,4,5,6,7}) );
//     REQUIRE( is_string_accepted( std::vector{5,6}, std::vector{1,2,3,4,5,6,7}) );

//     REQUIRE_FALSE( is_string_accepted( std::vector<state_t>{}, std::vector{1,2,3,4,5,6,7}) );
//     REQUIRE_FALSE( is_string_accepted( std::vector{8}, std::vector{1,2,3,4,5,6,7}) );
//     REQUIRE_FALSE( is_string_accepted( std::vector{1,2}, std::vector{3,4,5,6,7}) );
// }

TEST_CASE( "get_next_states", "" )
{
    const auto next_states = get_next_states( 'i', std::set<state_t>{}, std::vector<std::vector<Transition> >{} );
    
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
    REQUIRE( StateMachine::normalize_pattern("******test") == "*test" );
    REQUIRE( StateMachine::normalize_pattern("test******") == "test*" );
    REQUIRE( StateMachine::normalize_pattern("test*+") == "test+" );
    REQUIRE( StateMachine::normalize_pattern("test****+") == "test+" );
    REQUIRE( StateMachine::normalize_pattern("*+test") == "+test" );
    REQUIRE( StateMachine::normalize_pattern("****+test") == "+test" );
    REQUIRE( StateMachine::normalize_pattern("******test******") == "*test*" );
    REQUIRE( StateMachine::normalize_pattern("?????test?????") == "?????test?????" );
    REQUIRE( StateMachine::normalize_pattern("?????test") == "?????test" );
    REQUIRE( StateMachine::normalize_pattern("test?????") == "test?????" );
    REQUIRE( StateMachine::normalize_pattern("test") == "test" );
}

TEST_CASE( "testing_all_operators", "" )
{
    SECTION("?") {
        REQUIRE( match("test"s, "????"_pattern ) );
        REQUIRE( match("testbestwest"s, "????"_pattern ) );

        REQUIRE_FALSE( match("test123"s, "?test"_pattern ) );
        REQUIRE( match("1test1"s, "?test"_pattern ) );
        REQUIRE( match("1test"s, "?test"_pattern) );
        REQUIRE( match("1testbest"s, "?test"_pattern ) );
        REQUIRE( match("123test"s, "?test"_pattern ) );
        
        REQUIRE( match( "test1"s  , "test?"_pattern ) );
        REQUIRE( match( "testt"s  , "test?"_pattern ) );
        REQUIRE( match( "testwow"s, "test?"_pattern ) );
        REQUIRE_FALSE( match("wowtest"s, "test?"_pattern ) );
        REQUIRE( match( "wowtestwow"s, "test?"_pattern ) );

        REQUIRE_FALSE( match("testt"s, "?test?"_pattern ) );
        REQUIRE_FALSE( match("ttest"s, "?test?"_pattern ) );
        REQUIRE( match( "ttestt"s, "?test?"_pattern ) );
        REQUIRE( match("testtesttest"s, "????test?"_pattern ) );
        REQUIRE( match("testtesttest"s, "????test????"_pattern ) );
        REQUIRE_FALSE( match("testtesttest"s, "????test?????"_pattern ) );
    }

    SECTION("*") {
        REQUIRE( match("test"s    , "*test"_pattern ) );
        REQUIRE( match("testtest"s, "*test"_pattern ) );

        REQUIRE( match("test"s    , "*test"_pattern ) );
        REQUIRE( match("testtest"s, "*test"_pattern ) );

        REQUIRE( match( "test123"s, "*test*"_pattern ) );
        REQUIRE( match( "test123"s, "test*"_pattern ) );

        REQUIRE( match( "123test"s, "*test"_pattern ) );

        REQUIRE( match( "tt"s, "t*t"_pattern ) );

        REQUIRE( match( "test"s, "t*t"_pattern ) );
        REQUIRE( match( "testesttest"s, "t*t"_pattern ) );

        REQUIRE( match("test"s      , "*test*"_pattern ) );
        REQUIRE( match("test123"s   , "*test*"_pattern ) );
        REQUIRE( match("123test"s   , "*test*"_pattern ) );
        REQUIRE( match("123test123"s, "*test*"_pattern ) );

        REQUIRE( match("test"s          , "*test"_pattern ) );
        REQUIRE( match("btest"s         , "*test"_pattern ) );
        REQUIRE( match("qwertyuioptest"s, "*test"_pattern ) );
        REQUIRE( match("testest"s       , "*test"_pattern ) );

        REQUIRE_FALSE( match("that cunning peculiar wo"s, "cunq*"_pattern ) );

        REQUIRE( match("above once a quarter--I hope you keep accunt of Roger's purseeding in"s, "eding*"_pattern) );
        REQUIRE( match("above once a quarter--I hope you keep accunt of Roger's purseeding in"s, "*eding"_pattern) );
    }

    SECTION("+") {
        REQUIRE_FALSE( match( "face or front did it have; no conceivable token of either sensation or"s, "+fart"_pattern ) );        

        REQUIRE( match( "besttestbest"s, "+t+t+"_pattern ) );
        REQUIRE( match( "besttestwes"s , "+t+t+"_pattern ) );

        REQUIRE_FALSE( match( "bestestwes"s, "+tt+"_pattern ) );

        REQUIRE( match( "besttestwes"s, "+tt+"_pattern  ) );
        REQUIRE_FALSE( match( "tt"s, "t+t"_pattern ) );
        REQUIRE( match( "test"s       , "t+t"_pattern ) );
        REQUIRE( match( "testesttest"s, "t+t"_pattern ) );

        REQUIRE_FALSE( match( "test"s, "+test"_pattern ) );
        REQUIRE( match( "testtest"s  , "+test"_pattern ) );
        REQUIRE( match( "bbbtestbbb"s, "+test"_pattern ) );

        REQUIRE( match( "bbbtestbbb"s, "test+"_pattern ) );
        REQUIRE( match( "testbbb"s   , "test+"_pattern ) );
        REQUIRE_FALSE( match( "test"s   , "test+"_pattern ) );
        REQUIRE_FALSE( match( "bbbtest"s, "test+"_pattern ) );

        REQUIRE( match( "bbbtestbbb"s, "+test+"_pattern ) );
        REQUIRE_FALSE( match( "testbbb"s, "+test+"_pattern ) );
        REQUIRE_FALSE( match( "bbbtest"s, "+test+"_pattern ) );
        
        REQUIRE( match( "bbbtestbbb"s, "t+st"_pattern) );
        REQUIRE_FALSE( match( "bbbtstbbb"s, "t+st"_pattern) );
        REQUIRE( match( "testtest"s, "t+st"_pattern) );    
    }

    SECTION("+_and_*_and_?" )
    {
        REQUIRE( match( "testbest"s, "*test+"_pattern) );
        REQUIRE_FALSE( match( "testbest"s, "+test*"_pattern ) );
        REQUIRE( match( "besttest"s   , "+test*"_pattern) );
        REQUIRE( match("besttestbest"s, "+t+t*"_pattern) );
        
        REQUIRE( match("besttestwes"s, "+t*t+"_pattern) );

        // beginning
        REQUIRE( match("btest"s     , "*?test"_pattern) );
        REQUIRE( match("btestwaste"s, "*?test"_pattern) );
        REQUIRE_FALSE( match( "testHASTE"s, "*?test"_pattern) );

        // // middle
        REQUIRE( match( "test"s      , "te*?t"_pattern ) );
        REQUIRE( match( "btestwaste"s, "test*?e"_pattern) );
        REQUIRE_FALSE( match( "testHASTE"s, "te*?st"_pattern) );

        // // end
        REQUIRE( match( "testb"s     , "test*?"_pattern) );
        REQUIRE( match( "btestwaste"s, "test*?"_pattern) );
        REQUIRE( match( "testHASTE"s , "test*?"_pattern) );

    }

    SECTION( "without_operators" )
    { 
        REQUIRE( match("test"s, "test"_pattern) );
        REQUIRE_FALSE( match("testtest"s, "test"_pattern) );
    }
}