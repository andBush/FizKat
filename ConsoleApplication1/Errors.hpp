#pragma once

#include <stdexcept>

class FleetError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class InspectionError : public FleetError {
public:
    using FleetError::FleetError;
};

class CommandError : public FleetError {
public:
    using FleetError::FleetError;
};
