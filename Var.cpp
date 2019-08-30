#include "Var.hpp"
#include "LinearExpr.hpp"

namespace Osi2 {

uint32_t Var::next_id = 1;

Var::Var(Domaine d) : name("UNNAMED"+std::to_string(Var::next_id)), domaine(d), id(Var::next_id++) {}

Var::Var(std::string name, Domaine d) : name(name), domaine(d), id(Var::next_id++) {}

Var::Var(std::string name, const std::vector<Range>& ranges, Domaine d) : name(name), ranges(ranges), domaine(d), id(Var::next_id++) {}

Var::Var(std::string name, const Range& range, Domaine d) : name(name), domaine(d), id(Var::next_id++) {
    ranges.push_back(range);
}

Var::Var(const std::vector<Range>& ranges, Domaine d) : name("UNNAMED"+std::to_string(Var::next_id)), ranges(ranges), domaine(d), id(Var::next_id++) {}

Var::Var(const Range& range, Domaine d) : name("UNNAMED"+std::to_string(Var::next_id)), domaine(d), id(Var::next_id++) {
    ranges.push_back(range);
}

Var::Var(const Var& other) : name(other.name), ranges(other.ranges), domaine(other.domaine), id(other.id) {}

Var::Var(Var&& other) :  ranges(other.ranges), domaine(other.domaine), id(other.id) {
    std::swap(name, other.name);
}

Var& Var::operator=(const Var& other){
    name = other.name;
    domaine = other.domaine;
    ranges = other.ranges;
    id = other.id;

    return (*this);
}

void Var::addRange(double low, double up){
    Range r(low,up);
    bool merged = false;
    for (auto& range : ranges){
        if ( range.merge(r) ){
            merged = true;
            break; //TODO leave this line ??
        }
    }

    if (!merged){
        ranges.push_back(r);
    }
}     
        
void Var::addRange(const Range& r){
    bool merged = false;
    for (auto& range : ranges){
        if ( range.merge(r) ){
            merged = true;
            break; //TODO leave this line ??
        }
    }

    if (!merged){
        ranges.push_back(r);
    }
}

bool Var::lesser(const Var& other) const {
    return getID() < other.getID();
}

bool Var::equals(const Var& other) const {
    return getID() == other.getID();
}

bool lesser(const Var& a, const Var& b){
    return a.lesser(b);
}

bool equals(const Var& a, const Var& b){
    return a.equals(b);
}

bool operator<(const Var& a, const Var& b){
    return lesser(a,b);
}

bool operator==(const Var& a, const Var& b){
    return equals(a,b);
}

std::ostream& operator<<(std::ostream& flux, const Var& var){
    if (var.getName() == ""){
        flux << "Unnamed" << var.getID();
    }
    else{
        flux << var.getName();
    }

    return flux;
}

}


