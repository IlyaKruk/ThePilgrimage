#ifndef ITEM_H
#define ITEM_H

#include <string>

class Item {
public:
    Item(const std::string& type);

    std::string GetName() const;
    std::string GetType() const;
    int GetItemConst() const;
    std::string GetDescription() const;

private:
    std::string name_;
    std::string type_;
    int itemConst_;
    std::string description_;
};

#endif