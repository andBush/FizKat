#include "Fleet.hpp"
#include "Errors.hpp"
#include "Transport.hpp"

#include <utility>
#include <ostream>
#include <algorithm>
#include <cctype>
#include <iomanip>

namespace {
struct EnergyTotal {
	double value = 0.0;

	EnergyTotal& operator+=(const EnergyConsumer& consumer) {
		value += consumer.dailyEnergy();
		return *this;
	}
};

void printById(Registry<const InventoryItem*>& items, std::ostream& out) {
	std::sort(items.begin(), items.end(), [](const InventoryItem* a, const InventoryItem* b) {
		return *a < *b;
	});
	for (const auto* item : items) {
		out << *item << '\n';
	}
}
}

void Fleet::add(std::unique_ptr<InventoryItem> item) {
	if (!item) {
		throw CommandError("missing inventory item");
	}

	if (const auto* transport = dynamic_cast<const Transport*>(item.get())) {
		auto name = transport->getName();
		std::transform(name.begin(), name.end(), name.begin(), [](unsigned char ch) {
			return static_cast<char>(std::tolower(ch));
		});
		if (name.find("broken") != std::string::npos) {
			throw InspectionError("inspection rejected");
		}
	}

	const auto id = item->getId();
	if (findById(id) != items_.end()) {
		throw CommandError("duplicate id: " + id);
	}

	auto handle = std::make_shared<FocusEntry>(FocusEntry{item.get()});
	items_.add({std::move(item), std::move(handle)});
}

void Fleet::list(std::ostream& out) const {
	Registry<const InventoryItem*> sorted;
	for (const auto& entry : items_) {
		sorted.add(entry.item.get());
	}
	printById(sorted, out);
}

Registry<Fleet::Entry>::iterator Fleet::findById(const std::string& id) {
	return items_.findIf([&id](const Entry& entry) {
		return entry.item->getId() == id;
	});
}

bool Fleet::remove(const std::string& id) {
	auto found = findById(id);
	if (found == items_.end()) {
		return false;
	}
	items_.erase(found);
	return true;
}

bool Fleet::focus(const std::string& id) {
	auto found = findById(id);
	if (found == items_.end()) {
		return false;
	}
	focusByKind_.insert_or_assign(std::string(found->item->kind()), found->handle);
	focus_ = found->handle;
	return true;
}

void Fleet::showFocus(std::ostream& out, std::string_view kind) const {
	if (kind == "all") {
		showAllFocus(out);
		return;
	}

	std::weak_ptr<FocusEntry> selected;
	if (kind.empty()) {
		selected = focus_;
	} else if (const auto found = focusByKind_.find(kind); found != focusByKind_.end()) {
		selected = found->second;
	}

	if (const auto handle = selected.lock()) {
		out << *(handle->item) << '\n';
	} else {
		out << "expired\n";
	}
}

void Fleet::showAllFocus(std::ostream& out) const {
	Registry<const InventoryItem*> focused;
	for (const auto& entry : focusByKind_) {
		if (const auto handle = entry.second.lock()) {
			focused.add(handle->item);
		}
	}
	if (focused.size() == 0) {
		out << "expired\n";
		return;
	}
	printById(focused, out);
}

void Fleet::listNovice(std::ostream& out) const {
	for (const auto& entry : items_) {
		const auto* transport = dynamic_cast<const Transport*>(entry.item.get());

		if (transport && transport->suitableForNovice()) {
			out << *transport << std::endl;
		}
	}
}

std::size_t Fleet::count() const noexcept {
	return items_.size();
}

void Fleet::TotalEnergy(std::ostream& out) const {
	EnergyTotal total;
	for (const auto& entry : items_) {
		const auto* consumer = dynamic_cast<const EnergyConsumer*>(entry.item.get());
		if (consumer) {
			total += *consumer;
		}
	}
	out << std::fixed << std::setprecision(1) << total.value << '\n';
}
