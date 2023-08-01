#include <numeric>
#include <iostream>
#include <algorithm>
#include "match_rules.hpp"

using namespace std;

using state_t = MatchRules::state_t;

state_t MatchRules::next( const state_t state )
{
    return state + 1;
}

char MatchRules::ImplDetails::ToRule(const char ch) const {

    const auto iter = parsing_rules.find(ch);
    if( iter != std::end(parsing_rules) )
    {
        return ch;
    }
    
    // this represent non-operator charactors
    return '@';
}

uint64_t MatchRules::ImplDetails::MinimumNumberExpectedCharacters( const char ch ) const 
{
    const auto iter = parsing_rules.find( ToRule( ch ) );

    return iter == std::end(parsing_rules) ? 0 : iter->second.min_expected_matched_characters;
}
uint64_t MatchRules::ImplDetails::CountNumberOfOperators( const string & pattern ) const {

    return std::count_if( std::begin(pattern), std::end(pattern),
        [&](const char ch) 
        { 
            return parsing_rules.find(ch) != std::end(parsing_rules); 
        });
}

MatchRules::MatchRules()
{
    AddNewRule(RuleDetails{ '?', 1, 
        [](const char input, const PatternStates& states ) 
        {  
                //std::cout << "Rule: ?" << std::endl;

                return std::vector<State>{Next};
        } } );

    AddNewRule(RuleDetails{ '*', 0, 
        [](const char input, const PatternStates& states ) 
        {
            return std::vector<State>{Current};
        } } );

    AddNewRule(RuleDetails{ '@', 1, 
        [](const char input, const PatternStates& states) 
        {
                //std::cout << "Rule: @" << " states.pattern_symbol=" << states.pattern_symbol << std::endl;
                
                if( input == states.pattern_symbol )
                {
                    return std::vector<State>{Next};
                }
                
                return std::vector<State>{DiedOut};
        } } );
}

bool MatchRules::AddNewRule( MatchRules::RuleDetails && new_rule )
{
    const auto iter = details.parsing_rules.find(new_rule.symbol);

    if( iter != std::end(details.parsing_rules) )
    {
        cout << "ERROR: New rule '" << new_rule.symbol << "' cannot be added since it already exist!" << endl;
        return false;
    }

    details.parsing_rules[new_rule.symbol] = std::move(new_rule);

    cout << "INFO: New rule '" << new_rule.symbol << "' successfully was added!" << endl;

    return true;
}

void MatchRules::DeleteRule( const char symbol )
{
    details.parsing_rules.erase( symbol );
}

MatchRules::transition_t MatchRules::ImplDetails::GetTransitionFor( const char ch ) const
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
    AddNewRule(RuleDetails{ '+', 1, 
        [](const char input, const PatternStates& states ) 
        {
            return std::vector<State>{Current, Next};
        } } );
}

const MatchRulesWithPlusOperator& MatchRulesWithPlusOperator::GetInstance()
{
    static MatchRulesWithPlusOperator instance;
    return instance;
}