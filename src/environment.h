#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <unordered_map>
#include <memory>
#include "object.h"

class Environment {
public:
    Environment() = default;
    explicit Environment(std::shared_ptr<Environment> outer_env)
        : outer(outer_env) {}

    std::shared_ptr<Object> get(const std::string& name) {
        if (store.find(name) != store.end()) {
            return store[name];
        } else if (outer) {
            return outer->get(name);
        } else {
            return nullptr;
        }
    }

    void set(const std::string& name, std::shared_ptr<Object> value) {
        store[name] = value;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Object>> store;
    std::shared_ptr<Environment> outer;
};

#endif // ENVIRONMENT_H
