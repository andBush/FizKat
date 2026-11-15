#include "Inventory.hpp"
#include "Errors.hpp"

#include <cmath>
#include <ostream>
#include <string>

InventoryItem::InventoryItem(const std::string& inventoryId,
    const std::string& itemName)
    : id(inventoryId), name(itemName) {
    if (id.empty()) {
        throw CommandError("empty id");
    }
    if (name.empty()) {
        throw CommandError("empty name");
    }
}

std::string InventoryItem::getId() const {
    return id;
}

std::string InventoryItem::getName() const {
    return name;
}

bool operator<(const InventoryItem& a, const InventoryItem& b) {
    return a.getId() < b.getId();
}

std::ostream& operator<<(std::ostream& out, const InventoryItem& item) {
    return out << item.describe();
}

EnergyConsumer::EnergyConsumer(double energy) : energy_(energy) {
    if (!std::isfinite(energy_) || energy_ < 0.0) {
        throw CommandError("energy must be finite and nonnegative");
    }
}

double EnergyConsumer::dailyEnergy() const {
    return energy_;
}
