#pragma once

#include <cstddef>
#include <functional>
#include <map>
#include <memory>
#include <iosfwd>
#include <string>
#include <string_view>
#include "Inventory.hpp"
#include "Registry.hpp"

class Fleet {
private:
    struct FocusEntry {
        const InventoryItem* item;
    };

    struct Entry {
        std::unique_ptr<InventoryItem> item;
        std::shared_ptr<FocusEntry> handle;
    };

    Registry<Entry> items_;
    std::weak_ptr<FocusEntry> focus_;
    std::map<std::string, std::weak_ptr<FocusEntry>, std::less<>> focusByKind_;

    Registry<Entry>::iterator findById(const std::string& id);
    void showAllFocus(std::ostream& out) const;

public:
    void add(std::unique_ptr<InventoryItem> item);
    void list(std::ostream& out) const;
    bool remove(const std::string& id);
    bool focus(const std::string& id);
    void showFocus(std::ostream& out, std::string_view kind = {}) const;
    void listNovice(std::ostream& out) const;
    std::size_t count() const noexcept;
    void TotalEnergy(std::ostream& out) const;
};
