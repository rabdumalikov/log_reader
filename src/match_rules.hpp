#pragma once

#include <map>
#include <vector>
#include <string>
#include <functional>

class MatchRules {
    public:
        MatchRules();

        using state_t = uint64_t;

        struct PatternStates {
            // Current pattern symbol
            const char pattern_symbol; 
            
            // Current state
            const state_t current_state;
        };

        enum State {
            Current,
            Next,
            DiedOut
        };

        using transition_t = std::function<std::vector<State>(const char, const PatternStates&)>;

        struct RuleDetails {
            // Any symbol you want to give special meaning to. 
            // Like the ones that already exists: '*' - match many or non characters, '?' - match only one any character etc.
            // Thus we could introduce new operator, for example '.' which would match any two characters.
            char symbol;

            // For example, '*' match at least zero characters, '?' match at least one character,
            // '.' would match at least two characters, and regular characters would match at least one character.
            uint32_t min_expected_matched_characters;

            // This function where based you specific logic we either move 
            // to the next state or stay/return to the some other state. 
            // For example, if we would introduce '.' operator, which would match any two consecutive characters,
            // i.e. "te." would match "test" or "te12", etc. We could implement it like this:

            /***
            [first_match=false](const char input, const PatternStates& states ) mutable {
                if( !first_match ) // i.e. first match
                {
                    first_match = true;
                    // we should stay in the same state until we match second character
                    return std::vector<State>{Current};
                }
                else
                    // here we match second character, so we can move to the next state,
                    // since we done matching two characters.
                    return std::vector<State>{Next};
            }
            ***/
            transition_t func;
        };

        bool AddNewRule( RuleDetails && new_rule );
        void DeleteRule( const char symbol );

        // Move to the next state
        static state_t next( const state_t state );

        // Create a singleton to avoid creation of many instances
        static const MatchRules& GetInstance();

        struct ImplDetails 
        {            
            char ToRule( const char ch ) const;
            uint64_t MinimumNumberExpectedCharacters( const char ch ) const;
            uint64_t CountNumberOfOperators( const std::string & pattern ) const;        
            transition_t GetTransitionFor( const char ch ) const;
        
            std::map< char, RuleDetails > parsing_rules;
        };

    private:
        friend bool match(const std::string & pattern, const std::string & line, const MatchRules & rules);
        friend bool match_impl_NFA(const std::string & pattern, const std::string & line, const MatchRules & rules);
        
        ImplDetails details;
};


struct MatchRulesWithPlusOperator : public MatchRules
{
    MatchRulesWithPlusOperator();

    static const MatchRulesWithPlusOperator& GetInstance();
};

