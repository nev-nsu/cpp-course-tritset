//
// Created by nev on 9/22/16.
//

#include "Trit.h"

Trit::Trit() : _value(trit::Unknown) {}

Trit::Trit(const trit value) : _value(value) {}

Trit &Trit::operator&=(const Trit &other) {
    if (_value == trit::False || other._value == trit::False)
        _value = trit::False;
    else if (_value == trit::Unknown || other._value == trit::Unknown)
        _value = trit::Unknown;
    return *this;
}

Trit &Trit::operator|=(const Trit &other) {
    if (_value == trit::True || other._value == trit::True)
        _value = trit::True;
    else if (_value == trit::Unknown || other._value == trit::Unknown)
        _value = trit::Unknown;
    return *this;
}

Trit &Trit::operator^=(const Trit &other) {
    if (_value == trit::Unknown || other._value == trit::Unknown)
        _value = trit::Unknown;
    else if (_value != other._value)
        _value = trit::True;
    else
        _value = trit::False;
    return *this;
}

Trit Trit::operator!() const {
    if (_value == trit::False)
        return Trit(trit::True);
    if (_value == trit::True)
        return Trit(trit::False);
    return Trit(trit::Unknown);
}

Trit Trit::operator&(const Trit &other) const {
    return Trit(*this) &= other;
}

Trit Trit::operator|(const Trit &other) const {
    return Trit(*this) |= other;
}

Trit Trit::operator^(const Trit &other) const {
    return Trit(*this) ^= other;
}

bool Trit::operator==(const Trit &other) const {
    return _value == other._value;
}

bool Trit::operator!=(const Trit &other) const {
    return _value != other._value;
}

bool Trit::operator==(const trit &other) const {
    return _value == other;
}

bool Trit::operator!=(const trit &other) const {
    return _value != other;
}

bool Trit::operator==(const bool &other) const {
    return _value == Trit(other)._value;
}

bool Trit::operator!=(const bool &other) const {
    return _value != Trit(other)._value;
}

Trit::Trit(const Trit &obj) : _value(obj._value) {}

Trit &Trit::operator=(const Trit &other) {
    _value = other._value;
    return (*this);
}

trit Trit::value() const {
    return _value;
}

Trit::operator bool() {
    return _value == trit::True;
}

Trit::operator trit() {
    return _value;
}

Trit::Trit(const bool value) {
    if (value) _value = trit::True;
    else _value = trit::False;
}

Trit::Trit(const int value) {
    if (value > 0) _value = trit::True;
    else if (value == 0) _value = trit::Unknown;
    else _value = trit::False;
}

Trit::operator int() {
    switch (_value) {
        case trit::False:
            return -1;
        case trit::Unknown:
            return 0;
        case trit::True:
            return 1;
    }
}


