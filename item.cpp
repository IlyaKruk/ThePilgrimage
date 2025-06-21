#include "item.h"
#include "config.h"
#include <iostream>

Item::Item(const std::string& type) {
    name_ = GameConfig::GetInstance().GetString("item_" + type + "_name", type);
    type_ = GameConfig::GetInstance().GetString("item_" + type + "_type", type);
    itemConst_ = GameConfig::GetInstance().GetInt("item_" + type + "_const", 0);
    description_ = GameConfig::GetInstance().GetString("item_" + type + "_description", type);
}

std::string Item::GetName() const {
    return name_;
}

std::string Item::GetType() const {
    return type_;
}

int Item::GetItemConst() const {
    return itemConst_;
}

std::string Item::GetDescription() const {
    return description_;
}