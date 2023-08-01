#pragma once

#include <vector>
#include <functional>

using state_t = uint64_t;

struct PatternStates {
    // Current pattern symbol
    const char pattern_symbol; 
    
    // Current state
    const state_t current_state;
};

enum class Event {
    Stay, // stay in the current state
    Move, // move to the subsequent state
    Halt  // no where to move
};


struct Rule 
{
    using transition_t = std::function<std::vector<Event>(const char, const PatternStates&)>;

    /***
    Any symbol you want to give special meaning to. 
    Like the ones that already exists: '*' - match many or non characters, '?' - match only one any character etc.
    Thus we could introduce new operator, for example '.' which would match any two characters.
    ***/
    char symbol;

    /***
    For example, '*' match at least zero characters, '?' match at least one character,
    '.' would match at least two characters, and regular characters would match at least one character.
    ***/
    uint32_t min_expected_characters_to_match;

    /***
    This function where based you specific logic we either move 
    to the next state or stay/return to the some other state. 
    For example, if we would introduce '.' operator, which would match any two consecutive characters,
    i.e. "te." would match "test" or "te12", etc. We could implement it like this:

    [first_match=false](const char input, const PatternStates& states ) mutable {
        if( !first_match ) // i.e. first match
        {
            first_match = true;
            // we should stay in the same state until we match second character
            return std::vector<Event>{Event::Stay};
        }
        else
            // here we match second character, so we can move to the next state,
            // since we done matching two characters.
            return std::vector<Event>{Event::Move};
    }
    ***/
    transition_t func;
};