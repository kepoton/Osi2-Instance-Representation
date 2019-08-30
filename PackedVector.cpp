#include "PackedVector.hpp"

#include <iostream>

#include <algorithm>

namespace Osi2 {

PackedVector::PackedVector(){}

PackedVector::PackedVector(const PackedVector& other) : values(other.values) {}

PackedVector::PackedVector(PackedVector&& other){
    std::swap(values, other.values);
}

PackedVector& PackedVector::operator=(const PackedVector& other){
    values = other.values;

    return (*this);
}

bool PackedVector::insert(uint32_t index, double value){
    const double threshold = 0.01; //TODO Change that
    bool ret_val = false;
    if ( std::abs(value) > threshold){
        auto response = values.insert(std::make_pair(index, value));
        ret_val = response.second;
    }

    return ret_val;
}

bool PackedVector::remove(uint32_t index){
    auto response = values.erase(index);

    return response == 1 ? true : false;
}

void PackedVector::set(uint32_t index, double value){
    values[index] = value;
}

double PackedVector::get(uint32_t index) const {
    return values.find(index) != std::end(values) ? values.at(index) : 0;
}

double PackedVector::at(uint32_t index) const {
    return get(index);
}

double PackedVector::operator[](uint32_t index) const {
    return get(index);
}

bool PackedVector::has(uint32_t index) const {
    return values.find(index) != std::end(values);
}

void PackedVector::clear(){
    values.clear();
}

size_t PackedVector::size() const {
    return values.size();
}

std::vector<uint32_t> PackedVector::indices() const {
    std::vector<uint32_t> ret_val;
    for( auto pair : values){
        ret_val.push_back(pair.first);
    }
    return ret_val;
}

uint32_t PackedVector::maxIndex() const {
    if (begin() == end())
        throw std::out_of_range("Empty PackedVector, so no maximum index\n");
    return (--std::end(values))->first;
}

uint32_t PackedVector::dimension() const {
    uint32_t ret_val = 0;
    if ( begin() != end() ){
        ret_val = ( (--end())->first ) + 1;
    }

    return ret_val;
}

const std::vector<Element> PackedVector::elements() const {
    std::vector<Element> ret_val;

    for (auto e : values){
        ret_val.emplace_back(e.first, e.second);
    }

    return ret_val;
}

const std::map<uint32_t, double> PackedVector::data() const {
    return values;
}

std::string PackedVector::toString() const {
    std::string ret_val;

    for (auto e : values){
        ret_val += std::to_string(e.first) + " : " + std::to_string(e.second) + "\n";
    }

    return ret_val;
}

std::map<uint32_t, double>::const_iterator PackedVector::begin() const {
    return std::begin(values);
}

std::map<uint32_t, double>::const_iterator PackedVector::end() const {
    return std::end(values);
}


////////////////////////////////////////////////////////////////////////////////////

PackedVector binaryOP(const PackedVector& v1, const PackedVector& v2, std::function<double(double, double)> op){
    PackedVector ret_val;

    auto v1_ind = v1.indices();
    auto v2_ind = v2.indices();
    std::vector<uint32_t> indices;

    std::merge(begin(v1_ind), end(v1_ind), begin(v2_ind), end(v2_ind), std::back_inserter(indices));
    indices.erase(std::unique(begin(indices), end(indices)), end(indices));

    for ( auto e : indices ){
        ret_val.insert(e, op(v1[e], v2[e]) );
    }

    return ret_val;
}


PackedVector operator+(const PackedVector& v1, const PackedVector& v2){

    return binaryOP(v1, v2, [](double v1, double v2) -> double {
        return v1 + v2;
    });
}

PackedVector operator-(const PackedVector& v1, const PackedVector& v2){
    return binaryOP(v1, v2, [](double v1, double v2) -> double {
        return v1 - v2;
    });
}

double dotProduct(const PackedVector& v1, const PackedVector& v2){
    double ret_val = 0;

    auto it_v1 = v1.begin();
    auto it_v2 = v2.begin();

    auto it_v1_end = v1.end();
    auto it_v2_end = v2.end();

    while (it_v1 != it_v1_end && it_v2 != it_v2_end){
        if ( it_v1->first < it_v2->first ) ++it_v1;
        else if ( it_v1->first > it_v2->first ) ++it_v2;
        else{
            ret_val += it_v1->second * it_v2->second;
            ++it_v1;
            ++it_v2;
        }
    }
    
    return ret_val;
}

}