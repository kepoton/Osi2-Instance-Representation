#include "Constraint.hpp"

namespace Osi2 {

uint32_t Constraint::next_id = 1;

Constraint::Constraint() : id(next_id++), name("UNNAMED"+std::to_string(id)) {}

Constraint::Constraint(const std::string& name) : id (next_id++), name(name) {}

Constraint::Constraint(const Constraint& other) : id(other.id), name(other.name) {}

Constraint& Constraint::operator=(const Constraint& other){
    id = other.id;
    name = other.name;

    return *this;
}

Constraint::Constraint(Constraint&& other) : id(other.id) {
    std::swap(name, other.name);
}

}