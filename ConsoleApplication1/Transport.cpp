#include "Transport.hpp"
#include "Errors.hpp"

#include <memory>
#include <utility>

namespace {
const auto bikeSpec = std::make_shared<const ModelSpec>(ModelSpec{"bike", 6});
const auto scooterSpec = std::make_shared<const ModelSpec>(ModelSpec{"scooter", 6});
const auto ebikeSpec = std::make_shared<const ModelSpec>(ModelSpec{"ebike", 6});
}

Transport::Transport(const std::string& id, const std::string& name, int score,
    std::shared_ptr<const ModelSpec> model)
    : InventoryItem(id, name), score_(score), model_(std::move(model)) {
    if (score_ < 1 || score_ > 10) {
        throw CommandError("score must be between 1 and 10");
    }
}

Bike::Bike(const std::string& id, const std::string& name, int score)
    : Transport(id, name, score, bikeSpec) {}

Scooter::Scooter(const std::string& id, const std::string& name, double energy, int score)
    : Transport(id, name, score, scooterSpec), EnergyConsumer(energy) {}

EBike::EBike(const std::string& id, const std::string& name, double energy, int score)
    : Transport(id, name, score, ebikeSpec), EnergyConsumer(energy) {}
