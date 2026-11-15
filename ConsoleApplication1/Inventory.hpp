#pragma once

#include <string>
#include <iosfwd>
#include <string_view>

class InventoryItem {
	std::string id, name;
protected:
    InventoryItem(const std::string& inventoryId, const std::string& itemName);
public:
    virtual ~InventoryItem() = default;
    virtual std::string getId() const = 0;
    virtual std::string getName() const = 0;
    virtual std::string_view kind() const noexcept = 0;
    virtual std::string describe() const = 0;
};

bool operator<(const InventoryItem& a, const InventoryItem& b);
std::ostream& operator<<(std::ostream& out, const InventoryItem& item);

class EnergyConsumer {
    double energy_;
protected:
    explicit EnergyConsumer(double energy);
public:
    virtual ~EnergyConsumer() = default;
    virtual double dailyEnergy() const = 0;
};
