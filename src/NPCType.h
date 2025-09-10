#pragma once
#include <string>

enum class NPCType {
    STUDENT,
    PROFESSOR,
    CANTEEN_STAFF,
    ROOMMATE,
    DORM_MANAGER,
    EMPLOYER,
    LIBRARIAN,
    COACH
};

// std::string to_string(NPCType type);
// NPCType npc_type_from_string(const std::string& typeStr);