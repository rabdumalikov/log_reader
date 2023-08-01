#include <iostream>

#include "src/match_rules.hpp"
#include "src/matcher.hpp"

using state_t = MatchRules::state_t;
using State = MatchRules::State;
using PatternStates = MatchRules::PatternStates;
using RuleDetails = MatchRules::RuleDetails;


// '+' rule to match any one or more characters
struct PlusOperator
{
    std::vector<State> operator()( const char input, const PatternStates& states )
    {
        /***
        With the introduction of the operator '+', we are presented with two distinct branches to explore:

        1. 'Current' Branch: This path involves staying in the same state and branching out for all subsequent symbols. By taking this route, we can effectively cover cases where the '+' operator absorbs multiple characters in one go.

        2. 'Next' Branch: On the other hand, this path leads to moving to the next state immediately after the first match. In theory, this allows us to complete the matching process with '+'. However, this branch continues to evaluate the remaining characters, and it can either succeed or come to a halt.
        ****/
        return std::vector<State>{MatchRules::Current, MatchRules::Next};
    }
};

int main()
{    
    using namespace std;

    MatchRules rules;

    // Adding new rule for '+' to default ones, such as '*', and '?'
    rules.AddNewRule( RuleDetails{ '+', 1, PlusOperator() } );

    // Now we could use new set of rules inside match function.
    const bool match_result_beg = match("+test"s, "bbbtestbbb"s, rules );
    const bool match_result_mid = match(  "t+t"s, "bbbtestbbb"s, rules );
    const bool match_result_end = match("test+"s, "bbbtestbbb"s, rules );

    cout << "Match('+test', 'bbbtestbbb') => " << std::boolalpha << match_result_beg << endl;
    cout << "Match(  't+t', 'bbbtestbbb') => " << std::boolalpha << match_result_mid << endl;
    cout << "Match('test+', 'bbbtestbbb') => " << std::boolalpha << match_result_end << endl;
}




