//
// Created by nev on 9/22/16.
//

#ifndef TRITSET_TRIT_H
#define TRITSET_TRIT_H

enum class trit {
    True,
    Unknown,
    False
};

class Trit {
protected:
    trit _value;
public:
    Trit();
    Trit (const trit value);
    Trit (const Trit & obj);
    Trit (const bool value);
    Trit (const int value);

    virtual Trit& operator&= (const Trit& other);
    virtual Trit& operator|= (const Trit& other);
    virtual Trit& operator^= (const Trit& other);
    virtual Trit& operator=(const Trit& other);

    Trit operator& (const Trit& other) const;
    Trit operator| (const Trit& other) const;
    Trit operator^ (const Trit& other) const;
    Trit operator!() const;

    bool operator==(const Trit& other) const;
    bool operator!=(const Trit& other) const;
    bool operator==(const trit& other) const;
    bool operator!=(const trit& other) const;
    bool operator==(const bool& other) const;
    bool operator!=(const bool& other) const;

    operator bool();
    operator trit();
    operator int();

    trit value() const;
};

#endif //TRITSET_TRIT_H
