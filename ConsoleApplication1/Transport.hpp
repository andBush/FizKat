#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "Inventory.hpp"

struct ModelSpec {
	std::string_view kind;
	int minNoviceScore;
};

class Transport : public InventoryItem {
	int score_;
	std::shared_ptr<const ModelSpec> model_;
protected:
	Transport(const std::string& id, const std::string& name, int score,
		std::shared_ptr<const ModelSpec> model);
public:
	std::string getId() const override {
		return InventoryItem::getId();
	}

	std::string getName() const override {
		return InventoryItem::getName();
	}

	const ModelSpec& modelSpec() const noexcept {
		return *model_;
	}

	std::string_view kind() const noexcept override {
		return modelSpec().kind;
	}

	virtual bool suitableForNovice() const {
		return score_ >= modelSpec().minNoviceScore;
	}
};

class Bike : public Transport {
public:
	Bike(const std::string& id, const std::string& name, int score);

	std::string describe() const override{
		return getId() + " " + getName();
	}
};

class Scooter : public Transport, public EnergyConsumer {
public:
	Scooter(const std::string& id, const std::string& name, double energy, int score);

	std::string describe() const override {
		return getId() + " " + getName();
	}

	double dailyEnergy() const override {
		return EnergyConsumer::dailyEnergy();
	}
};

class EBike : public Transport, public EnergyConsumer {
public:
	EBike(const std::string& id, const std::string& name, double energy, int score);

	std::string describe() const override {
		return getId() + " " + getName();
	}

	double dailyEnergy() const override {
		return EnergyConsumer::dailyEnergy();
	}
};
