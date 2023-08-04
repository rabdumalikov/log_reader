#include <set>
#include <map>
#include <vector>
#include <optional>
#include <iostream>
#include <algorithm>

#include "rule.hpp"
#include "matcher.hpp"

struct StateMachine
{
    struct Transition {    
        Transition( const char pattern_symbol, const state_t current_state, 
            std::function<std::vector<Event>(const char, const PatternStates&)> && func )
        : states{pattern_symbol, current_state}
        , func(std::move(func))
        {}

        auto operator()(const char input) const
        {
            return func(input, this->states);
        }

        PatternStates states;

        private:

        std::function<std::vector<Event>(const char, const PatternStates&)> func;
    };

    std::vector< std::vector<Transition> > transitions;
    std::vector< state_t > accepting_states;
};

static std::optional<char> get_next_char( const std::string & pattern, size_t index )
{
    const size_t next_index = index + 1;

    if( next_index >= pattern.size() )
    {
        return std::nullopt;
    }

    return pattern[next_index];
}

static StateMachine create_state_machine( const std::string & pattern, const Rules & rules )
{
    const size_t max_possible_state = pattern.size();

    auto state_machine = StateMachine{ 
        std::vector<std::vector<StateMachine::Transition> >( max_possible_state ), 
        std::vector< state_t >{ max_possible_state } 
    };
        
    for( size_t i = 0; i < pattern.size(); ++i )
    {
        const state_t current_state = i;
        const char ch = pattern[i];
        
        std::vector transitions{ StateMachine::Transition(ch, current_state, get_transition(ch, rules) ) };

        if( min_characters_to_match( ch, rules ) == 0 )
        {
            if( const auto next_char = get_next_char(pattern, i); next_char )
            {
                const char next_ch = *next_char;

                /// epsilon transition
                auto next_rule = get_transition(next_ch, rules);

                transitions.push_back( StateMachine::Transition(next_ch, current_state+1, std::move(next_rule) ) );
            }
            else 
            {
                state_machine.accepting_states.push_back( current_state );
            }                
        }
        
        state_machine.transitions[ current_state ] = std::move( transitions );
    }

    return state_machine;
}

static std::set< state_t > get_next_states( const char input, 
    const std::set< state_t > & current_states, const std::vector<std::vector<StateMachine::Transition> > & transitions )
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

template< typename T1, typename T2 >
static bool is_string_accepted( const T1 & accept_states, const T2 & current_states )
{
    const auto iter = std::find_if( std::begin(accept_states), std::end(accept_states), 
        [&current_states]( const auto accept_state ) 
        {
            const auto iter = std::find( std::begin(current_states), std::end(current_states), accept_state );

            return iter != std::end(current_states);
        } );

    return iter != std::end(accept_states);
}

bool match_impl_NFA(const std::string & pattern, const std::string & line, const Rules & rules)
{
    const auto state_machine = create_state_machine( pattern, rules );

    /// std::set because states to explore should be unique.
    std::set< state_t > current_states{ 0 };

    return std::any_of( std::begin(line), std::end(line),
        [ & ]( const char input ) mutable
        {
            current_states = get_next_states(input, current_states, state_machine.transitions);

            return is_string_accepted( state_machine.accepting_states, current_states );
        } );
}

// The copy of 'pattern' here is intentional
static std::string normalize_pattern( std::string pattern ) 
{
    using namespace std;

    const static auto normalization_rules = {
        pair{"**"s, "*"s},
        pair{"+*"s, "+"s},
        pair{"*+"s, "+"s}
    };

    for( const auto & [key, value] : normalization_rules )
    {
        for( auto iter = pattern.find(key); iter != std::string::npos; iter = pattern.find(key) ) {
            pattern.replace( iter, 2, value );
        }
    }

    return pattern;
}

bool match(const std::string & pattern, const std::string & line, const Rules & rules)
{
    if( number_of_operators(pattern, rules) == 0 )
    {
        return pattern == line;
    }

    return match_impl_NFA( normalize_pattern( pattern ), line, rules );
}