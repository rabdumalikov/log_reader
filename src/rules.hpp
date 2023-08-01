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

        struct ImplDetails 
        {            
            char ToRule( const char ch ) const;
            uint64_t MinimumNumberExpectedCharacters( const char ch ) const;
            uint64_t CountNumberOfOperators( const std::string & pattern ) const;        
            Rule::transition_t GetTransitionFor( const char ch ) const;
        
            std::map< char, Rule > parsing_rules;
        };

    private:
        friend bool match(const std::string & pattern, const std::string & line, const Rules & rules);
        friend bool match_impl_NFA(const std::string & pattern, const std::string & line, const Rules & rules);
        
        ImplDetails details;
};


struct RulesWithPlus : public Rules
{
    RulesWithPlus();

    static const RulesWithPlus& GetInstance();
};

