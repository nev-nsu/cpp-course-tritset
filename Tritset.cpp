//
// Created by nev on 9/21/16.
//

#include <iostream>
#include "Tritset.h"

const unsigned int Tritset::trits_per_element (sizeof(cell_t)*8/2);


Tritset::Tritset() {
    array.resize(0);
    _last = -1;
}

Tritset::Tritset(const size_t size, const Trit fill_value) {
    _last = (int) size - 1;
    size_t needed = size / trits_per_element;
    needed += ((size % trits_per_element)? 1 : 0);
    bool flag = (size % trits_per_element) > 0;
    switch (fill_value.value()){
        case trit::False:
            array.resize(needed, _fill_value(trit::False));
            break;
        case trit::True:
            array.resize(needed, _fill_value(trit::True));
            break;
        case trit::Unknown:
            array.resize(needed, _fill_value(trit::Unknown));
            _last = -1;
            break;
    }
    if (flag && fill_value.value() != trit::Unknown)
        for (size_t i = size; i < needed * trits_per_element; i++)
            (*this)[i] = trit::Unknown;
}

Tritset::reference Tritset::operator[](const int index) {
    return Tritset::reference (this, size_t(index));
}

Tritset &Tritset::operator&=(const Tritset &other) {
    size_t len = other.length();
    extend (len);
    for (int i = 0; i < len; i++)
        ((*this)[i]) &= Trit(other[i]);
    for (size_t i = len; i <= _last; i++)
        (*this)[i] &= Trit(trit::Unknown);
    update();
    return (*this);
}

Tritset &Tritset::operator|=(const Tritset &other) {
    size_t len = other.length();
    extend (len);
    for (size_t i = 0; i < len; i++)
        (*this)[i] |= other[i];
    for (size_t i = len; i <= _last; i++)
        (*this)[i] |= Trit(trit::Unknown);
    update();
    return (*this);
}

Tritset &Tritset::operator^=(const Tritset &other) {
    size_t len = other.length();
    extend (len);
    for (int i = 0; i < len; i++)
        (*this)[i] ^= other[i];
    for (size_t i = len; i <= _last; i++)
        (*this)[i] = Trit(trit::Unknown);
    update();
    return (*this);
}

Tritset Tritset::operator~() const {
    Tritset cp(*this);
    for (int i = 0; i <= _last; i++)
        (cp[i]) = !(cp[i]);
    cp.update();
    return cp;
}

void Tritset::update() {
    _last = -1;
    size_t s = array.size() * trits_per_element;
    for (int i = 0; i < s; i++){
        Trit t = (*this)[i];
        if (trit(t) != trit::Unknown)
            _last = i;
    }
}

size_t Tritset::count(const Trit state) const {
    size_t count = 0;
    for (int i = 0; i < array.size() * trits_per_element; i++)
        if ((*this)[i] == state)
            count++;
    return count;
}

size_t Tritset::capacity() const {
    return array.size() * trits_per_element;
}

size_t Tritset::length() const {
    return size_t(_last + 1);
}

size_t Tritset::shrink(){
    if (_last < 0){
        array.resize(0);
        _last = -1;
        return 0;
    }
    return cut (size_t(_last));
}

size_t Tritset::cut(const size_t last_index) {
    size_t needed = last_index / trits_per_element;
    needed += ((last_index % trits_per_element)? 1 : 0);
    array.resize(needed);
    for (size_t i = last_index + 1; i < array.size() * trits_per_element; i++)
        (*this)[i] = trit::Unknown;
    update();
    return needed;
}

void Tritset::extend(const size_t new_size) {
    size_t needed = new_size / trits_per_element;
    needed += ((new_size % trits_per_element)? 1 : 0);
    if (needed > array.size())
        array.resize(needed, _fill_value(trit::Unknown));
}

std::string Tritset::to_string() const {
    std::string str;
    size_t len = array.size() * trits_per_element;
    str.resize (len);
    for (int i = 0; i < len; i++){
        Trit t = (*this)[i];
        if (trit(t) == trit::Unknown)
            str[i] = '?';
        else if (trit(t) == trit::True)
            str[i] = '1';
        else str[i] = '0';
    }
    return str;
}

Tritset::reference::operator Trit() const{
    return get_value();
}

Tritset::reference &Tritset::reference::operator=(const Trit &other) {
    set_value(other);
    return (*this);
}

Tritset::reference &Tritset::reference::operator^=(const Trit &other) {
    set_value (get_value() ^ other);
    return (*this);
}

Tritset::reference &Tritset::reference::operator&=(const Trit &other) {
    set_value (get_value() & other);
    return (*this);
}

Tritset::reference &Tritset::reference::operator|=(const Trit &other) {
    set_value (get_value() | other);
    return (*this);
}

Tritset::reference::reference(Tritset *nset, size_t nindex)
        : set(nset), index(nindex){
    if (nset->capacity() * Tritset::trits_per_element > nindex)
        _value = get_value();
}

Trit Tritset::reference::get_value() const {
    if (index >= set->capacity())
        return Trit(trit::Unknown);
    int value = set->array[index/Tritset::trits_per_element];
    value >>= 2*(index%Tritset::trits_per_element);
    value &= 0b11;
    if (value == 0)
        return Trit(false);
    if (value == 0b11)
        return Trit(true);
    return Trit(trit::Unknown);
}

void Tritset::reference::set_value(const Trit &value) {
    if (index >= set->capacity()){
        if (value == trit::Unknown)
            return;
        set->extend(index + 1);
    }
    size_t id = index/Tritset::trits_per_element;
    size_t shift = 2*(index%Tritset::trits_per_element);
    if (value == trit::True)
        set->array[id] |= 0b11 << shift;
    else {
        set->array[id] &= ~(0b11 << shift);
        if (value == trit::Unknown)
            set->array[id] |= 0b01 << shift;
    }
    if ((int) index > set->_last && value != trit::Unknown)
        set->_last = (int) index;
}

Tritset::reference &Tritset::reference::operator=(const Tritset::reference &other) {
    return (*this) = other.get_value();
}

Tritset::reference &Tritset::reference::operator++() {
    index++;
    return *this;
}

bool Tritset::reference::operator==(const Tritset::reference &other) {
    return other.index==index;
}

bool Tritset::reference::operator!=(const Tritset::reference &other) {
    return other.index != index;
}

Trit Tritset::reference::operator*() {
    return get_value();
}

Trit Tritset::operator[](const int index) const {
    int value = array[index/trits_per_element];
    value >>= 2*(index%trits_per_element);
    value &= 0b11;
    if (value == 0)
        return Trit(false);
    if (value == 0b11)
        return Trit(true);
    return Trit();
}

Tritset::Tritset(std::vector<bool> binary_logic) {
    array.resize(0);
    _last = -1;
    extend (binary_logic.size());
    for (int i = 0; i < binary_logic.size(); i++)
        (*this)[i] = Trit(binary_logic[i]);
}

Tritset::Tritset(std::vector<int> prist_ternary_logic) {
    array.resize(0);
    _last = -1;
    extend (prist_ternary_logic.size());
    for (int i = 0; i < prist_ternary_logic.size(); i++)
        (*this)[i] = Trit(prist_ternary_logic[i]);
}

Tritset::Tritset(const Tritset &other)
        : array(other.array), _last(other._last) {
    shrink();
}

size_t Tritset::size() const {
    return array.size();
}

Tritset Tritset::operator&(const Tritset &other) {
    Tritset a (*this);
    a &= other;
    return a;
}

Tritset Tritset::operator|(const Tritset &other) {
    return Tritset (*this) |= other;
}

Tritset Tritset::operator^(const Tritset &other) {
    return Tritset (*this) ^= other;
}

Tritset::cell_t Tritset::_fill_value(trit val) {
    int elem;
    switch (val){
        case trit::False:
            elem = int(trit_pack::False);
            break;
        case trit::True:
            elem = int(trit_pack::True);
            break;
        case trit::Unknown:
            elem = int(trit_pack::Unknown);
            break;
    }
    cell_t res = cell_t(0);
    for (int i = 0; i < trits_per_element; i++)
        res |= elem << (i * 2);
    return res;
}

Tritset::iterator Tritset::begin() {
    return Tritset::iterator(this, 0);
}

Tritset::iterator Tritset::end() {
    return Tritset::iterator(this, _last + 1);
}


