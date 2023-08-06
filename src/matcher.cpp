#include <set>
#include <iostream>
#include <algorithm>

#include "matcher.hpp"
#include "state_machine.hpp"

static std::set< state_t > get_next_states( const char input, 
    const std::set< state_t > & current_states, const StateMachine::transitions_t & transitions )
{
    // We must keep start state for false positive cases.
    // For example: let say we want to use pattent="ning*" and input="cunning".
    // Matching first 'n' in the input is false positive. Because
    // after that match we move to the next state which has only transition 'i', and the whole
    // match would fail. Thus we always need to have start state, and this
    // branch carry on again for the second 'n' in the input, and matching
    // would be successful.
    std::set< state_t > next_states{ 0 };

    for( const state_t current_state : current_states )
    {
        for( const auto & transition : transitions[current_state] )
        {                
            for( const Event new_event : transition(input) )
            {
                switch(new_event)
                {
                    case Event::Stay: 
                        next_states.insert(transition.states.current_state); 
                        break;
                    case Event::Move: 
                        next_states.insert(transition.states.current_state + 1); 
                        break;                            
                    case Event::Halt:
                    default: break;
                }
            }
        }
    }

    return next_states;
}

bool match( const std::string & line, const Pattern & pattern, const Rules & rules )
{
    return match( line, StateMachine( pattern.pattern, rules ), rules );
}

bool match( const std::string & line, const StateMachine & state_machine, const Rules & rules )
{
    const auto & pattern( state_machine.GetPattern() );

    if( number_of_operators(pattern, rules) == 0 )
    {
        return pattern == line;
    }

    /// std::set because states to explore should be unique.
    std::set< state_t > current_states{ 0 };

    return std::any_of( std::begin(line), std::end(line),
        [ & ]( const char input ) mutable
        {
            current_states = get_next_states( input, current_states, state_machine.GetTransitions() );

            return state_machine.IsStateAccepted( current_states );
        } );
}

Pattern operator ""_pattern(const char* text, std::size_t size)
{
    return Pattern{std::string(text, size)};
}