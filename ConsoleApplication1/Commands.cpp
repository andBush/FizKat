#include "Commands.hpp"
#include "Errors.hpp"
#include "Fleet.hpp"
#include "Things.hpp"
#include "Transport.hpp"

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>

namespace {
std::string readToken(std::istringstream& input, const char* argument) {
	std::string token;
	if (!(input >> token)) {
		throw CommandError(std::string("missing ") + argument);
	}
	return token;
}

template<typename T>
T readNumber(std::istringstream& input, const char* argument) {
	std::istringstream number(readToken(input, argument));
	T value{};
	if (!(number >> value) || !number.eof()) {
		throw CommandError(std::string("invalid ") + argument);
	}
	return value;
}

void requireEnd(std::istringstream& input) {
	input >> std::ws;
	if (!input.eof()) {
		throw CommandError("unexpected arguments");
	}
}

bool isItemType(std::string_view type) {
	return type == "helmet" || type == "bike" || type == "scooter" || type == "ebike" || type == "dock";
}

void addItem(std::istringstream& input, Fleet& fleet) {
	const auto type = readToken(input, "item type");
	if (!isItemType(type)) {
		throw CommandError("unknown item type: " + type);
	}

	const auto id = readToken(input, "id");
	const auto name = readToken(input, "name");
	if (type == "helmet") {
		requireEnd(input);
		fleet.add(std::make_unique<Helmet>(id, name));
		return;
	}
	if (type == "bike") {
		const auto score = readNumber<int>(input, "score");
		requireEnd(input);
		fleet.add(std::make_unique<Bike>(id, name, score));
		return;
	}

	const auto energy = readNumber<double>(input, "energy");
	if (type == "dock") {
		requireEnd(input);
		fleet.add(std::make_unique<Dock>(id, name, energy));
		return;
	}

	const auto score = readNumber<int>(input, "score");
	requireEnd(input);
	if (type == "scooter") {
		fleet.add(std::make_unique<Scooter>(id, name, energy, score));
	} else {
		fleet.add(std::make_unique<EBike>(id, name, energy, score));
	}
}
}

bool executeCommand(const std::string& line, Fleet& fleet) {
	std::istringstream input(line);
	std::string command;
	if (!(input >> command)) {
		return true;
	}
	if (command == "quit") {
		return false;
	}
	if (command == "add") {
		addItem(input, fleet);
		return true;
	}
	if (command == "focus" || command == "remove") {
		const auto id = readToken(input, "id");
		requireEnd(input);
		const bool found = command == "focus" ? fleet.focus(id) : fleet.remove(id);
		if (!found) {
			throw CommandError("unknown id: " + id);
		}
		return true;
	}

	if (command == "show_focus") {
		std::string type;
		input >> type;
		requireEnd(input);
		if (!type.empty() && type != "all" && !isItemType(type)) {
			throw CommandError("unknown item type: " + type);
		}
		fleet.showFocus(std::cout, type);
		return true;
	}

	if (command != "list" && command != "novice" && command != "energy" && command != "count") {
		throw CommandError("unknown command: " + command);
	}
	requireEnd(input);
	if (command == "list") {
		fleet.list(std::cout);
	} else if (command == "novice") {
		fleet.listNovice(std::cout);
	} else if (command == "energy") {
		fleet.TotalEnergy(std::cout);
	} else {
		std::cout << fleet.count() << '\n';
	}
	return true;
}
