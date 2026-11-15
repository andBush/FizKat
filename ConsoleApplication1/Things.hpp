#pragma once

#include <vector>
#include <memory>
#include <string>
#include <string_view>
#include "Inventory.hpp"

class Helmet : public InventoryItem {
public:
	Helmet(const std::string& id, const std::string& name)
		: InventoryItem(id, name) {}

	std::string_view kind() const noexcept override {
		return "helmet";
	}

	std::string getId() const override {
		return InventoryItem::getId();
	}

	std::string getName() const override {
		return InventoryItem::getName();
	}

	std::string describe() const override {
		return getId() + " " + getName();
	}
};

class Dock : public InventoryItem, public EnergyConsumer {
public:
	Dock(const std::string& id, const std::string& name, double energy)
		: InventoryItem(id, name), EnergyConsumer(energy) {}

	std::string_view kind() const noexcept override {
		return "dock";
	}

	std::string getId() const override {
		return InventoryItem::getId();
	}

	std::string getName() const override {
		return InventoryItem::getName();
	}

	std::string describe() const override {
		return getId() + " " + getName();
	}

	double dailyEnergy() const override {
		return EnergyConsumer::dailyEnergy();
	}
};
