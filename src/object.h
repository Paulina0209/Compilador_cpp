#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

// Tipo de objeto que representa un valor evaluado
enum class ObjectType {
    INTEGER_OBJ,
    BOOLEAN_OBJ,
    RETURN_VALUE_OBJ,
    FUNCTION_OBJ,
    NULL_OBJ
};

// Forward declaration
class Environment;

// Clase base
class Object {
public:
    virtual ~Object() = default;
    virtual ObjectType type() const = 0;
    virtual std::string inspect() const = 0;
};

// Integer
class Integer : public Object {
public:
    int value;
    Integer(int v) : value(v) {}
    ObjectType type() const override { return ObjectType::INTEGER_OBJ; }
    std::string inspect() const override { return std::to_string(value); }
};

// Boolean
class Boolean : public Object {
public:
    bool value;
    Boolean(bool v) : value(v) {}
    ObjectType type() const override { return ObjectType::BOOLEAN_OBJ; }
    std::string inspect() const override { return value ? "true" : "false"; }
};

// Null
class Null : public Object {
public:
    ObjectType type() const override { return ObjectType::NULL_OBJ; }
    std::string inspect() const override { return "null"; }
};

// Return value (para return dentro de funciones)
class ReturnValue : public Object {
public:
    std::shared_ptr<Object> value;
    ReturnValue(std::shared_ptr<Object> v) : value(v) {}
    ObjectType type() const override { return ObjectType::RETURN_VALUE_OBJ; }
    std::string inspect() const override { return value->inspect(); }
};

// Funciones definidas por el usuario
class Function : public Object {
public:
    std::vector<std::string> parameters;
    std::shared_ptr<class BlockStatement> body;
    std::shared_ptr<Environment> env;

    Function(std::vector<std::string> params,
             std::shared_ptr<BlockStatement> bod,
             std::shared_ptr<Environment> environment)
        : parameters(std::move(params)), body(std::move(bod)), env(std::move(environment)) {}

    ObjectType type() const override { return ObjectType::FUNCTION_OBJ; }

    std::string inspect() const override {
        std::string result = "fn(";
        for (size_t i = 0; i < parameters.size(); ++i) {
            result += parameters[i];
            if (i != parameters.size() - 1) result += ", ";
        }
        result += ") { ... }";
        return result;
    }
};

#endif // OBJECT_H
