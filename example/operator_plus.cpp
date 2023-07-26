#include <iostream>

#include "src/match_rules.hpp"
#include "src/matcher.hpp"

// '+' rule to match any one or more characters
struct PlusOperator
{
    MatchRules::state_t operator()( const char input, const MatchRules::PatternStates& states )
    {
        /*
        We can break down the logic into two parts:
        1. Match just one character: in this we just move to next state without even checking. Since any character will do.
        2. Match more than one character: Since our new operator '+' is absorbing, which means that if later match would fail then transition would be done to latest absorbing state. If it is the case then moving to the next state would be done if current input would match with next symbol/rule in pattern.
        */
        if( !first_match )
        {
            first_match = true;
            return  MatchRules::next( states.current_state );
        } 
        else {
            const bool is_next_char_available = !std::empty( states.remaining_pattern );
            
            if( !is_next_char_available ) return states.current_state;

            const char next_ch = states.remaining_pattern[0];
            
            if( next_ch == input )
            {
                return  MatchRules::next( MatchRules::next(states.current_state));
            }
            
            return states.current_state;
        }

        return MatchRules:: MatchRules::next( states.current_state );
    }

    private:
        bool first_match = false;
};

int main()
{    
    using namespace std;

    MatchRules rules;

    // Adding new rule for '+' to default ones, such as '*', and '?'
    rules.AddNewRule(MatchRules::RuleDetails{ '+', true, 1, PlusOperator() } );

    // Now we could use new set of rules inside match function.
    const bool match_result_beg = match("+test"s, "bbbtestbbb"s, rules );
    const bool match_result_mid = match(  "t+t"s, "bbbtestbbb"s, rules );
    const bool match_result_end = match("test+"s, "bbbtestbbb"s, rules );

    cout << "Match('+test', 'bbbtestbbb') => " << std::boolalpha << match_result_beg << endl;
    cout << "Match(  't+t', 'bbbtestbbb') => "   << std::boolalpha << match_result_mid << endl;
    cout << "Match('test+', 'bbbtestbbb') => " << std::boolalpha << match_result_end << endl;
}




