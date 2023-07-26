#pragma once

#include <map>
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

            // Return state could be used in case of match failure of later letters.
            // But if it is absorbing operator, then we should stay in the same state.
            const state_t returning_state;

            // Part of the pattern that is left to match           
            const std::string remaining_pattern; 
        };

        using transition_t = std::function<state_t(const char, const PatternStates&)>;

        struct RuleDetails {
            // Any symbol you want to give special meaning to. 
            // Like the ones that already exists: '*' - match many or non characters, '?' - match only one any character etc.
            // Thus we could introduce new operator, for example '.' which would match any two characters.
            char symbol;

            // For example, '*' is absorbing operator, because it can match any number of characters,
            // if we introduce new operator, for example '.' which will match any two characters,
            // in this case it is not absorbing operator, because characters can be anything, which means
            // we couldn't fail matching them. The primarily reason for knowing whether pattern symbol
            // can absorb or not is that it give us understanding to which state we should return in case
            // of match failure of later letters.
            bool is_absorbing;

            // For example, '*' match at least zero characters, '?' match at least one character,
            // '.' would match at least two characters, and regular characters would match at least one character.
            uint32_t min_expected_matched_characters;

            // This function where based you specific logic we either move 
            // to the next state or stay/return to the some other state. 
            // For example, if we would introduce '.' operator, which would match any two consecutive characters,
            // i.e. "te." would match "test" or "te12", etc. We could implement it like this:

            /***
            [ first_match = false ](const char input, const PatternStates& states ) mutable {                    
                    if( !first_match ) // i.e. first match
                    {
                        first_match = true;
                        // we should stay in the same state until we match second character
                        return states.current_state;
                    }
                    else
                        // here we match second character, so we can move to the next state,
                        // since we done matching two characters.
                        return next(states.current_state);
            }
            ***/
            transition_t func;
        };

        bool AddNewRule( RuleDetails && new_rule );
        void DeleteRule( const char symbol );

        // Move to the next state
        static state_t next( const state_t state );
        static const MatchRules& GetInstance();

    private:
        friend bool match_impl(const std::string &, const std::string &, const MatchRules &);
        
        // For testing purposes
        friend char ToRule(const char, const MatchRules & );
        friend bool IsOperatorAbsorbing(const char, const MatchRules &);
        friend uint64_t CountMinimumNumberExpectedCharacters( const std::string &, const MatchRules & );
        friend uint64_t CountNumberOfOperators( const std::string &, const MatchRules & );
        
        // Hide this functions from the public interface to prevent interface contamination
        char ToRule(const char ch ) const;
        bool IsOperatorAbsorbing(const char ch) const;
        uint64_t CountMinimumNumberExpectedCharacters( const std::string & pattern ) const;
        uint64_t CountNumberOfOperators( const std::string & pattern ) const;        
        transition_t GetTransitionFor( const char ch ) const;
        
        using rules_t = std::map< char, RuleDetails >;        

        rules_t parsing_rules;
};


struct MatchRulesWithPlusOperator : public MatchRules
{
    MatchRulesWithPlusOperator();

    static const MatchRulesWithPlusOperator& GetInstance();
};

