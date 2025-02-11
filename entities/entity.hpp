#include "../components/component.hpp"
#include <vector>

class Entity {
    public:
    std::vector<IComponent> components;
    void add(IComponent component);
};