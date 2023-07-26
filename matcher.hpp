#pragma once

#include <string>
#include <functional>
#include "match_rules.hpp"

bool match(const std::string & pattern, const std::string & line, const MatchRules & rules = MatchRulesWithPlusOperator::GetInstance() );
