#include "src/matcher.cpp"

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch2/catch.hpp"

using namespace std;

// TEST_CASE( "ToRule", "" ) 
// {
//     MatchRulesWithPlusOperator rules;
//     REQUIRE( ToRule('*', rules) == '*' );
//     REQUIRE( ToRule('+', rules) == '+' );
//     REQUIRE( ToRule('?', rules) == '?' );
//     REQUIRE( ToRule('a', rules) == '@' );
//     REQUIRE( ToRule(' ', rules) == '@' );
// }

// TEST_CASE( "CountMinimumNumberExpectedCharacters", "" ) 
// {
//     MatchRulesWithPlusOperator rules;
//     REQUIRE( CountMinimumNumberExpectedCharacters("*t", rules) == 1 );
//     REQUIRE( CountMinimumNumberExpectedCharacters("+t", rules) == 2 );
//     REQUIRE( CountMinimumNumberExpectedCharacters("?t", rules) == 2 );
//     REQUIRE( CountMinimumNumberExpectedCharacters("*?test", rules) == 5 );
//     REQUIRE( CountMinimumNumberExpectedCharacters("+?test", rules) == 6 );
//     REQUIRE( CountMinimumNumberExpectedCharacters("??test", rules) == 6 );
//     REQUIRE( CountMinimumNumberExpectedCharacters("*?+est", rules) == 5 );
// }

// TEST_CASE( "CountNumberOfOperators", "" ) 
// {
//     MatchRulesWithPlusOperator rules;
//     REQUIRE( CountNumberOfOperators("*t", rules) == 1 );
//     REQUIRE( CountNumberOfOperators("+t", rules) == 1 );
//     REQUIRE( CountNumberOfOperators("?t", rules) == 1 );
//     REQUIRE( CountNumberOfOperators("*?test", rules) == 2 );
//     REQUIRE( CountNumberOfOperators("+?test", rules) == 2 );
//     REQUIRE( CountNumberOfOperators("??test", rules) == 2 );
//     REQUIRE( CountNumberOfOperators("*?+est", rules) == 3 );
// }

TEST_CASE( "pattern_normalization", "" ) 
{
    //REQUIRE( normalize_pattern("*?test") == "?*test" );
    //REQUIRE( normalize_pattern("*?*?test") == "??*test" );
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