#pragma once

#include <string>

#include "rules.hpp"

// forward declaration
struct StateMachine;

// The goal of this structure is to force users of function 'match'
// to state class name explicitly to know that it is pattern.
// Otherwise, it is quite easy to make a mistake and misplace 
// first argument with second one in function 'mathc'.
struct Pattern
{  
    std::string pattern;
};

Pattern operator ""_pattern( const char*, std::size_t );

bool match( const std::string & line, const Pattern & pattern,
    const Rules & rules = RulesWithPlus::GetInstance() );

bool match( const std::string & line, const StateMachine & state_machine,
    const Rules & rules = RulesWithPlus::GetInstance() );
