#include <algorithm>

#include "state_machine.hpp"


Transition::Transition( const char pattern_symbol, const state_t current_state, 
    std::function<std::vector<Event>(const char, const PatternStates&)> && func )
: states{pattern_symbol, current_state}
, func(std::move(func))
{}

std::vector<Event> Transition::operator()(const char input) const
{
    return func(input, this->states);
}

StateMachine::StateMachine( const std::string & pattern )
: StateMachine( normalize_pattern(pattern), create_state_machine( normalize_pattern(pattern), RulesWithPlus::GetInstance() ) )
{
}

StateMachine::StateMachine( const std::string & pattern, const Rules & rules )
: StateMachine( normalize_pattern(pattern), create_state_machine( normalize_pattern(pattern), rules ) )
{
}

StateMachine::StateMachine( std::string && pattern, std::pair< StateMachine::transitions_t, std::vector< state_t > > && transitions_and_accepting_states )
: pattern( std::move(pattern) )
, transitions( std::move(transitions_and_accepting_states.first) )
, accepting_states( std::move(transitions_and_accepting_states.second ) )
{    
}

bool StateMachine::IsStateAccepted( const std::set< state_t > & current_states ) const
{
    const auto iter = std::find_if( std::begin(accepting_states), std::end(accepting_states), 
        [&current_states]( const auto accept_state ) 
        {
            const auto iter = std::find( std::begin(current_states), std::end(current_states), accept_state );

            return iter != std::end(current_states);
        } );

    return iter != std::end(accepting_states);
}

const StateMachine::transitions_t & StateMachine::GetTransitions() const
{
    return transitions;
}

const std::string & StateMachine::GetPattern() const
{
    return pattern;
}

std::pair< StateMachine::transitions_t, std::vector< state_t > > 
    StateMachine::create_state_machine( const std::string & pattern, const Rules & rules )
{
    const size_t max_possible_state = pattern.size();

    auto state_machine = std::make_pair( 
        transitions_t( max_possible_state ), 
        std::vector< state_t >{ max_possible_state } 
    );
        
    for( size_t i = 0; i < pattern.size(); ++i )
    {
        const state_t current_state = i;
        const char ch = pattern[i];
        
        std::vector transitions{ Transition(ch, current_state, get_transition(ch, rules) ) };

        if( min_characters_to_match( ch, rules ) == 0 )
        {
            if( const auto next_char = get_next_char(pattern, i); next_char )
            {
                const char next_ch = *next_char;

                auto next_rule = get_transition(next_ch, rules);

                transitions.push_back( Transition(next_ch, current_state+1, std::move(next_rule) ) );
            }
            else 
            {
                state_machine.second.push_back( current_state );
            }                
        }
        
        state_machine.first[ current_state ] = std::move( transitions );
    }

    return state_machine;
}

// The copy of 'pattern' here is intentional
std::string StateMachine::normalize_pattern( std::string pattern ) 
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

std::optional<char> StateMachine::get_next_char( const std::string & pattern, size_t index )
{
    const size_t next_index = index + 1;

    if( next_index >= pattern.size() )
    {
        return std::nullopt;
    }

    return pattern[next_index];
}