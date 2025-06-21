#ifndef ITEM_H
#define ITEM_H

#include <string>

class Item {
public:
    enum class Type { CONSUMABLE, WEAPON, ARMOR, OTHER };

    Item(const std::string& name, Type type, int value, const std::string& description);

    const std::string& GetName() const;
    Type GetType() const;
    int GetValue() const;
    const std::string& GetDescription() const;

private:
    std::string name_;
    Type type_;
    int value_;
    std::string description_;
};

#endif