#pragma once

#include <string>
#include <functional>
#include "rules.hpp"

bool match(const std::string & pattern, const std::string & line, 
    const Rules & rules = RulesWithPlus::GetInstance() );
