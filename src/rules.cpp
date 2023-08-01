#include <numeric>
#include <iostream>
#include <algorithm>
#include "rules.hpp"

using namespace std;

Rules::ImplDetails::ImplDetails( Rule && default_rule )
: default_rule( std::move(default_rule) )
{
}

uint64_t Rules::ImplDetails::MinMatchRequiredCharacters( const char ch ) const 
{
    const auto iter = parsing_rules.find( ch );

    return iter == std::end(parsing_rules) ? 1 : iter->second.min_expected_characters_to_match;
}

uint64_t Rules::ImplDetails::CountNumberOfOperators( const string & pattern ) const {

    return std::count_if( std::begin(pattern), std::end(pattern),
        [&](const char ch) 
        { 
            return parsing_rules.find(ch) != std::end(parsing_rules); 
        });
}

Rules::Rules()
: details( Rule{ '@', 1, 
    [](const char input, const PatternStates& states) 
    {
        return input == states.pattern_symbol  ?
            std::vector{Event::Move} :
            std::vector{Event::Halt};
    } } )
{
    AddNewRule( Rule{ '?', 1, 
        [](const char input, const PatternStates& states ) 
        {  
            return std::vector{Event::Move};
        } } );

    AddNewRule( Rule{ '*', 0, 
        [](const char input, const PatternStates& states ) 
        {
            return std::vector{Event::Stay};
        } } );
}

bool Rules::AddNewRule( Rule && new_rule )
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

void Rules::DeleteRule( const char symbol )
{
    details.parsing_rules.erase( symbol );
}

Rule::transition_t Rules::ImplDetails::GetTransitionFor( const char ch ) const
{    
    auto iter = parsing_rules.find( ch );
    
    if( iter != std::end(parsing_rules) )
    {
        return iter->second.func;
    }

    // default rule
    return default_rule.func;
}

const Rules& Rules::GetInstance()
{
    static Rules instance;
    return instance;
}

RulesWithPlus::RulesWithPlus()
{
    AddNewRule( Rule{ '+', 1, 
        [](const char input, const PatternStates& states ) 
        {
            return std::vector{Event::Stay, Event::Move};
        } } );
}

const RulesWithPlus& RulesWithPlus::GetInstance()
{
    static RulesWithPlus instance;
    return instance;
}