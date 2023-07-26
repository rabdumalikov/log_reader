#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include "matcher.hpp"

using state_t = MatchRules::state_t;

struct Rule {

    Rule( const char filter, const state_t current_state, const state_t failure_state, std::string && remaining_filter, 
        std::function<state_t(const char, const MatchRules::PatternStates&)> && func )
    : states{filter, current_state, failure_state, std::move(remaining_filter)}
    , func(std::move(func))
    {}

    state_t operator()(const char input) const
    {
        return func(input, this->states);
    }

    private:
        MatchRules::PatternStates states;
        std::function<state_t(const char, const MatchRules::PatternStates&)> func;
};

bool match_impl(const std::string & pattern, const std::string & line, const MatchRules & rules)
{
    const auto min_exp_chars = rules.CountMinimumNumberExpectedCharacters(pattern);

    if( line.size() < min_exp_chars )
        return false;

    if( rules.CountNumberOfOperators(pattern) == 0 )
        return pattern == line;

    using transition_info_t = std::pair< char /*pattern symbol*/, Rule /*for transtition*/ >;

    auto create_transition_map = []( const std::string & pattern, const MatchRules & rules )
    {
        std::map<state_t, transition_info_t> transition_map;

        state_t failure_state = 0;
        state_t current_state = 0;
            
        for( int i = 0; i < pattern.size(); ++i, ++current_state )
        {
            const char ch = pattern[i];
            
            auto rule = rules.GetTransitionFor(ch);
            
            transition_map.emplace( current_state, 
                std::make_pair( ch, Rule(ch, current_state, failure_state, pattern.substr(i+1), std::move(rule) ) ) );

            if( rules.IsOperatorAbsorbing(ch) )
            {
                failure_state = current_state;
            }
        }

        const state_t final_state = current_state;

        return transition_map;
    };
    
    auto transition_map = create_transition_map( pattern, rules );

    state_t current_state = 0;

    uint64_t matched_chars = 0;
    uint64_t matched_chars_retractor = 0;

    for( int i = 0; i < line.size(); ++i )
    {
        const char input = line[i];

        const auto iter = transition_map.find(current_state);

        // Indicate that we reached final state
        if( iter == std::end(transition_map) )
        {
            //std::cout << "INFO: Reached final state" << std::endl;
            break;
        }
        
        const auto & [pattern_symbol, transition] = iter->second;
    
        const auto new_state = transition(input);
        
        if( new_state < current_state )
        {
            i -= 1;

            matched_chars -= matched_chars_retractor;
            matched_chars_retractor = 0;            
        }
        else if( new_state > current_state )
        {
            uint32_t num_matches = (new_state-current_state);

            if( pattern_symbol == '*' && num_matches > 1 )
            {
                num_matches -= 1;
            }

            matched_chars += num_matches;

            const auto new_state_iter = transition_map.find(new_state);
            
            // Indicate that we reached final state
            if( new_state_iter == std::end(transition_map) )
            {
                //std::cout << "INFO: Reached final state from new_state" << std::endl;
                break;
            }

            if( !rules.IsOperatorAbsorbing(new_state_iter->second.first) )
            {
                matched_chars_retractor += num_matches;
            }
            else
            {
                matched_chars_retractor = 0;
            }
        }

        current_state = new_state;

        if( matched_chars == min_exp_chars ) 
        {
            break;
        }
    }

    return matched_chars >= min_exp_chars;
}
// The copy of 'pattern' here is intentional
static std::string normalize_pattern( std::string pattern ) 
{
    const static std::vector<std::pair<std::string, std::string>> normalization_rules = {
        {"*?", "?*"},
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
    return match_impl(normalize_pattern( pattern ), line, rules );
}