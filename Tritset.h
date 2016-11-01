#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include "Trit.h"

class Tritset {
private:
    typedef unsigned cell_t;
    enum class trit_pack {
        False = 0,
        Unknown = 1,
        True = 3,
        mask = 0b11
    };

    cell_t _fill_value(trit val);

    std::vector<cell_t> array;
    int _last;

    void update();


public:
    Tritset();

    Tritset(const size_t size, const Trit fill_value = Trit(trit::Unknown));

    Tritset(std::vector<bool> binary_logic);

    Tritset(std::vector<int> prist_ternary_logic);

    Tritset(const Tritset &other);

    static const unsigned int trits_per_element;

    class reference : public Trit, public std::iterator<std::input_iterator_tag, Trit> {
    private:
        Tritset *set;
        size_t index;
    public:
        reference(Tritset *nset, size_t nindex);

        virtual reference &operator&=(const Trit &other);

        virtual reference &operator|=(const Trit &other);

        virtual reference &operator^=(const Trit &other);

        virtual reference &operator=(const Trit &other);

        reference &operator=(const reference &other);

        virtual reference &operator++();

        virtual bool operator==(const reference &other);

        virtual bool operator!=(const reference &other);

        Trit operator*();

        Trit get_value() const;

        void set_value(const Trit &value);

        operator Trit() const;
    };

    typedef reference iterator;

    iterator begin();

    iterator end();

    Tritset &operator&=(const Tritset &other);

    Tritset &operator|=(const Tritset &other);

    Tritset &operator^=(const Tritset &other);

    Tritset operator~() const;

    Tritset operator&(const Tritset &other);

    Tritset operator|(const Tritset &other);

    Tritset operator^(const Tritset &other);

    size_t count(const Trit state) const;

    size_t capacity() const;

    size_t length() const;

    size_t size() const;

    size_t shrink();

    size_t cut(const size_t last_index);

    void extend(const size_t new_size);

    reference operator[](const int index);

    Trit operator[](const int index) const;

    std::string to_string() const;

    friend Trit reference::get_value() const;

    friend void reference::set_value(const Trit &value);
};