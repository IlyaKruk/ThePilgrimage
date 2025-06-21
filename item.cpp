#include "item.h"

Item::Item(const std::string& name, Type type, int value, const std::string& description) :
    name_(name), type_(type), value_(value), description_(description) {
}

const std::string& Item::GetName() const { return name_; }
Item::Type Item::GetType() const { return type_; }
int Item::GetValue() const { return value_; }
const std::string& Item::GetDescription() const { return description_; }