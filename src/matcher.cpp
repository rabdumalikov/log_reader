#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include "matcher.hpp"

using state_t = MatchRules::state_t;
using State = MatchRules::State;
using PatternStates = MatchRules::PatternStates;
using ImplDetails = MatchRules::ImplDetails;

struct Rule {
    
    Rule( const char filter, const state_t current_state, 
        std::function<std::vector<State>(const char, const PatternStates&)> && func )
    : states{filter, current_state}
    , func(std::move(func))
    {}

    auto operator()(const char input) const
    {
        return func(input, this->states);
    }

    PatternStates states;

    private:

    std::function<std::vector<State>(const char, const PatternStates&)> func;
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

template< typename T >
static bool is_string_accepted( const T & accept_states, const T & current_states )
{
    const auto iter = std::find_if( std::begin(accept_states), std::end(accept_states), [&current_states]( const auto accept_state ) 
    {
        const auto iter = std::find( std::begin(current_states), std::end(current_states), accept_state );

        return iter != std::end(current_states);
    } );

    return iter != std::end(accept_states);
}

struct StateMachine
{
    std::vector< std::vector<Rule> > transitions;
    std::vector< state_t > accepting_states;
};

static StateMachine create_state_machine( const std::string & pattern, const ImplDetails & details )
{
    const size_t max_possible_state = pattern.size();

    auto state_machine = StateMachine{ 
        std::vector<std::vector<Rule> >( max_possible_state ), 
        std::vector< state_t >{ max_possible_state } 
    };
        
    for( size_t i = 0; i < pattern.size(); ++i )
    {
        const state_t current_state = i;
        const char ch = pattern[i];
        
        std::vector<Rule> transitions{ Rule(ch, current_state, details.GetTransitionFor(ch) ) };

        if( details.MinimumNumberExpectedCharacters( ch ) == 0 )
        {
            if( const auto next_char = get_next_char(pattern, i); next_char )
            {
                const char next_ch = *next_char;

                /// epsilon transition
                auto next_rule = details.GetTransitionFor(next_ch);

                transitions.push_back( Rule(next_ch, current_state+1, std::move(next_rule) ) );
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

static std::vector< state_t > get_next_states( const char input, 
    const std::vector< state_t > & current_states, const std::vector<std::vector<Rule> > & transitions )
{
    // We must keep start state for false positive cases.
    // For example: let say we want to use pattent=ning* and input=cunning.
    // Matchiing first 'n' in the input is false positive. Because
    // after that match we move to the next state which is 'i', and the whole
    // match would fail. Thus we always need to have start state, and this
    // branch carry on again for the second 'n' in the input, and matching
    // would be successful.
    std::vector< state_t > next_states{ 0 };

    for( const state_t current_state : current_states )
    {
        for( const auto & transition : transitions[current_state] )
        {                
            for( const State new_state : transition(input) )
            {
                switch(new_state)
                {
                    case MatchRules::Current: 
                        next_states.push_back(transition.states.current_state); 
                        break;
                    case MatchRules::Next: 
                        next_states.push_back(transition.states.current_state + 1); 
                        break;                            
                    case MatchRules::DiedOut:
                    default: break;
                }
            }
        }
    }

    return next_states;
}

bool match_impl_NFA(const std::string & pattern, const std::string & line, const MatchRules & rules)
{
    const auto state_machine = create_state_machine( pattern, rules.details );

    std::vector< state_t > current_states{ 0 };

    for( const char input : line )
    {
        current_states = get_next_states(input, current_states, state_machine.transitions);

        std::sort( std::begin(current_states), std::end(current_states) );
        current_states.erase( std::unique(  std::begin(current_states), std::end(current_states) ), std::end(current_states) );

        if( is_string_accepted( state_machine.accepting_states, current_states ))
        {
            return true;
        }
    }

    return is_string_accepted( state_machine.accepting_states, current_states );
}

// The copy of 'pattern' here is intentional
static std::string normalize_pattern( std::string pattern ) 
{
    const static std::vector<std::pair<std::string, std::string>> normalization_rules = {
        {"**", "*"},
        {"+*", "+"},
        {"*+", "+"}
    };

    for( const auto & [key, value] : normalization_rules )
    {
        for( auto iter = pattern.find(key); iter != std::string::npos; iter = pattern.find(key) ) {
            pattern.replace( iter, 2, value );
        }
    }

    return std::move( pattern );
}

bool match(const std::string & pattern, const std::string & line, const MatchRules & rules)
{
    if( rules.details.CountNumberOfOperators(pattern) == 0 )
    {
        return pattern == line;
    }

    return match_impl_NFA(normalize_pattern( pattern ), line, rules );
}