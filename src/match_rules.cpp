#include <iostream>
#include <algorithm>
#include "match_rules.hpp"

using namespace std;

using state_t = MatchRules::state_t;

state_t MatchRules::next( const state_t state )
{
    return state + 1;
}

char MatchRules::ToRule(const char ch) const {

    const auto iter = parsing_rules.find(ch);
    if( iter != std::end(parsing_rules) )
    {
        return ch;
    }
    
    // this represent non-operator charactors
    return '@';
}

bool MatchRules::IsOperatorAbsorbing(const char ch) const {
    
    const auto iter = parsing_rules.find(ch);

    if( iter != std::end(parsing_rules) )
    {
        return iter->second.is_absorbing;
    }

    return false;
}

uint64_t MatchRules::CountMinimumNumberExpectedCharacters( const string & pattern) const {
    uint64_t amount = 0;

    for( const char ch : pattern )
    {
        const auto iter = parsing_rules.find( ToRule( ch ) );

        if( iter != std::end(parsing_rules) )
        {
            amount += iter->second.min_expected_matched_characters;
        }     
    }

    return amount;
}

uint64_t MatchRules::CountNumberOfOperators( const string & pattern ) const {

    return std::count_if( std::begin(pattern), std::end(pattern),
        [&](const char ch) 
        { 
            return parsing_rules.find(ch) != std::end(parsing_rules); 
        });

}

MatchRules::MatchRules()
{
    AddNewRule(RuleDetails{ '?', false, 1, [](const char /*input*/, const PatternStates& states ) {  
            return next(states.current_state);
        } } );

    AddNewRule(RuleDetails{ '*', true, 0, [](const char input, const PatternStates& states ) {  
        const bool is_next_char_available = !std::empty( states.remaining_pattern );

        if( !is_next_char_available ) return states.current_state;

        const char next_ch = states.remaining_pattern[0];
                    
        return next_ch == input ? ( states.remaining_pattern.size() == 1 ? next(states.current_state) :
                    next(next(states.current_state)) ) : states.current_state;
    } } );

    AddNewRule(RuleDetails{ '@', false, 1, [](const char input, const PatternStates& states) {
            return input == states.pattern_symbol ? next(states.current_state) : states.returning_state;
        } } );
}

bool MatchRules::AddNewRule( MatchRules::RuleDetails && new_rule )
{
    const auto iter = parsing_rules.find(new_rule.symbol);

    if( iter != std::end(parsing_rules) )
    {
        cout << "ERROR: New rule '" << new_rule.symbol << "' cannot be added since it already exist!" << endl;
        return false;
    }

    parsing_rules[new_rule.symbol] = std::move(new_rule);

    cout << "INFO: New rule '" << new_rule.symbol << "' successfully was added!" << endl;

    return true;
}

void MatchRules::DeleteRule( const char symbol )
{
    parsing_rules.erase( symbol );
}

MatchRules::transition_t MatchRules::GetTransitionFor( const char ch ) const
{
    const auto rule_key = ToRule(ch);
    
    // Entry always must exists otherwise something is wrong
    auto iter = parsing_rules.find(rule_key);
    
    if( iter == std::end(parsing_rules) )
    {
        throw std::runtime_error("Rule for '" + std::string(1, ch) + "' not found!");
    }

    return iter->second.func;
}

const MatchRules& MatchRules::GetInstance()
{
    static MatchRules instance;
    return instance;
}

MatchRulesWithPlusOperator::MatchRulesWithPlusOperator()
{
    AddNewRule(RuleDetails{ '+', true, 1, [first_match=false](const char input, const PatternStates& states ) mutable {
        if( !first_match )
        {
            first_match = true;
            return next( states.current_state );
        } 
        else {
            const bool is_next_char_available = !std::empty( states.remaining_pattern );
            
            if( !is_next_char_available ) return states.current_state;

            const char next_ch = states.remaining_pattern[0];
            
            if( next_ch == input )
            {
                return next(next(states.current_state));
            }
            
            return states.current_state;
        }

        return next( states.current_state );
    } } );
}

const MatchRulesWithPlusOperator& MatchRulesWithPlusOperator::GetInstance()
{
    static MatchRulesWithPlusOperator instance;
    return instance;
}