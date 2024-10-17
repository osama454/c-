#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <sstream>

// For simplicity, we'll use a JSON library for serialization/deserialization.
// In a real-world scenario, you might use a dedicated library like nlohmann/json.
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Item {
public:
    int id;
    std::string name;
    float value;
    bool is_active;

    Item(int id, const std::string& name, float value, bool is_active)
        : id(id), name(name), value(value), is_active(is_active) {}

    // Overload the < operator for sorting
    bool operator<(const Item& other) const {
        if (value == other.value) {
            return id < other.id;
        }
        return value < other.value;
    }

    // Serialize Item to JSON
    friend void to_json(json& j, const Item& item) {
        j = json{ {"id", item.id}, {"name", item.name}, {"value", item.value}, {"is_active", item.is_active} };
    }

    // Deserialize Item from JSON
    friend void from_json(const json& j, Item& item) {
        j.at("id").get_to(item.id);
        j.at("name").get_to(item.name);
        j.at("value").get_to(item.value);
        j.at("is_active").get_to(item.is_active);
    }
};

class ItemManager {
private:
    std::vector<Item> items;
    // Change tracking (simplified example)
    std::vector<std::string> changeLog;

    // Helper function to find the index of an item by ID
    int findItemIndex(int id) const {
        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i].id == id) {
                return static_cast<int>(i);
            }
        }
        return -1; // Not found
    }

public:
    // Add or update an item
    void addItem(const Item& item) {
        int index = findItemIndex(item.id);
        if (index != -1) {
            // Update existing item
            items[index] = item;
            changeLog.push_back("Updated item with ID: " + std::to_string(item.id));
        }
        else {
            // Add new item
            items.push_back(item);
            changeLog.push_back("Added item with ID: " + std::to_string(item.id));
        }
    }

    // Remove an item by ID
    void removeItem(int id) {
        int index = findItemIndex(id);
        if (index != -1) {
            items.erase(items.begin() + index);
            changeLog.push_back("Removed item with ID: " + std::to_string(id));
        }
        else {
            throw std::runtime_error("Item not found.");
        }
    }

    // Find an item by ID
    Item& findItem(int id) {
        int index = findItemIndex(id);
        if (index != -1) {
            return items[index];
        }
        else {
            throw std::runtime_error("Item not found.");
        }
    }

    // Get all active items
    std::vector<Item> getActiveItems() const {
        std::vector<Item> activeItems;
        for (const auto& item : items) {
            if (item.is_active) {
                activeItems.push_back(item);
            }
        }
        return activeItems;
    }

    // Get the total value of active items
    float getTotalValue() const {
        float total = 0.0f;
        for (const auto& item : items) {
            if (item.is_active) {
                total += item.value;
            }
        }
        return total;
    }

    // Sort items
    void sortItems() {
        std::sort(items.begin(), items.end());
        changeLog.push_back("Items sorted.");
    }

    // Serialize to JSON
    void serialize(const std::string& filename) const {
        json j = items;
        std::ofstream file(filename);
        if (file.is_open()) {
            file << j.dump(4); // Use 4 spaces for indentation
            file.close();
        }
        else {
            throw std::runtime_error("Unable to open file for writing.");
        }
    }

    // Deserialize from JSON
    void deserialize(const std::string& filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            json j;
            file >> j;
            items = j.get<std::vector<Item>>();
            file.close();
        }
        else {
            throw std::runtime_error("Unable to open file for reading.");
        }
    }

    // Access the change log
    const std::vector<std::string>& getChangeLog() const {
        return changeLog;
    }
};

int main() {
    ItemManager manager;

    // Add sample items
    manager.addItem({ 1, "Item1", 10.0f, true });
    manager.addItem({ 2, "Item2", 20.0f, false });
    manager.addItem({ 3, "Item3", 15.0f, true });
    manager.addItem({ 4, "Item4", 25.0f, true });
    manager.addItem({ 5, "Item5", 30.0f, false });

    // Demonstrate functionalities
    try {
        std::cout << "Total Value: " << manager.getTotalValue() << std::endl;

        manager.removeItem(2); // Remove Item2
        std::cout << "Total Value after removing Item2: " << manager.getTotalValue() << std::endl;

        manager.findItem(3).value = 18.0f; // Update value of Item3
        std::cout << "Total Value after updating Item3: " << manager.getTotalValue() << std::endl;

        manager.sortItems();

        manager.serialize("items.json");

        ItemManager loadedManager;
        loadedManager.deserialize("items.json");

        // Print change log
        std::cout << "\nChange Log:\n";
        for (const auto& change : manager.getChangeLog()) {
            std::cout << change << std::endl;
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
