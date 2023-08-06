#pragma once

#include <set>
#include <string>
#include <optional>

#include "rules.hpp"

struct Transition 
{    
    Transition( const char pattern_symbol, const state_t current_state, 
        std::function<std::vector<Event>(const char, const PatternStates&)> && func );

    std::vector<Event> operator()(const char input) const;

    PatternStates states;

private:
    std::function<std::vector<Event>(const char, const PatternStates&)> func;
};


struct StateMachine
{
    // types
    using transitions_t = std::vector< std::vector<Transition> >;

    // ctors
    explicit StateMachine( const std::string & pattern );
    StateMachine( const std::string & pattern, const Rules & rules );

    // methods
    bool IsStateAccepted( const std::set< state_t > & current_states ) const;
    const transitions_t & GetTransitions() const;
    const std::string & GetPattern() const;

private:        
    StateMachine( std::string && pattern, std::pair< transitions_t, std::vector< state_t > > && transitions_and_accepting_states );
    
    static std::pair< transitions_t, std::vector< state_t > > 
        create_state_machine( const std::string & pattern, const Rules & rules );

    static std::string normalize_pattern( std::string pattern );
    static std::optional<char> get_next_char( const std::string & pattern, size_t index );

    std::string pattern;
    std::vector< std::vector<Transition> > transitions;
    std::vector< state_t > accepting_states;
};
