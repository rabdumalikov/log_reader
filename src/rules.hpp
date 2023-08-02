#pragma once

#include <map>
#include <vector>
#include <string>
#include <functional>

#include "rule.hpp"

class Rules {
    public:
        Rules();

        bool AddNewRule( Rule && new_rule );
        void DeleteRule( const char symbol );

        // Create a singleton to avoid creation of many instances
        static const Rules& GetInstance();

    private:
        friend uint64_t min_characters_to_match( const char ch, const Rules & rules );
        friend uint64_t number_of_operators( const std::string & pattern, const Rules & rules );        
        friend Rule::transition_t get_transition( const char ch, const Rules & rules );
        
        const Rule default_rule; /// i.e. rule for all non-operators
        std::map< char, Rule > parsing_rules;
};

/// Rules that include additional operator '+'
struct RulesWithPlus : public Rules
{
    RulesWithPlus();

    static const RulesWithPlus& GetInstance();
};

/// Helper functions
Rule::transition_t get_transition( const char ch, const Rules & rules );
uint64_t min_characters_to_match( const char ch, const Rules & rules );
uint64_t number_of_operators( const std::string & pattern, const Rules & rules );        
