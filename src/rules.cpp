#include <numeric>
#include <iostream>
#include <algorithm>
#include "rules.hpp"

Rules::Rules()
: default_rule( Rule{ '@', 1, 
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
    const auto iter = parsing_rules.find(new_rule.symbol);

    if( iter != std::end(parsing_rules) )
    {
        std::cout << "ERROR: New rule '" << new_rule.symbol << "' cannot be added since it already exist!" << std::endl;
        return false;
    }

    parsing_rules[new_rule.symbol] = std::move(new_rule);

    //std::cout << "INFO: New rule '" << new_rule.symbol << "' successfully was added!" << std::endl;

    return true;
}

void Rules::DeleteRule( const char symbol )
{
    parsing_rules.erase( symbol );
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


Rule::transition_t get_transition( const char ch, const Rules & rules )
{    
    auto iter = rules.parsing_rules.find( ch );
    
    if( iter != std::end(rules.parsing_rules) )
    {
        return iter->second.func;
    }

    // default rule
    return rules.default_rule.func;
}

uint64_t min_characters_to_match( const char ch, const Rules & rules ) 
{
    const auto iter = rules.parsing_rules.find( ch );

    return iter == std::end(rules.parsing_rules) ? 1 : iter->second.min_expected_characters_to_match;
}

uint64_t number_of_operators( const std::string & pattern, const Rules & rules ) 
{
    return std::count_if( std::begin(pattern), std::end(pattern),
        [&](const char ch) 
        { 
            return rules.parsing_rules.find(ch) != std::end(rules.parsing_rules); 
        });
}