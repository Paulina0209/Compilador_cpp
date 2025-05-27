#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <memory>
#include "ast.h"
#include "object.h"
#include "environment.h"

// Eval para nodos como unique_ptr<Node>
std::shared_ptr<Object> eval(std::unique_ptr<Node>& node, std::shared_ptr<Environment> env);

// Eval para punteros crudos Node*
std::shared_ptr<Object> eval(Node* node, std::shared_ptr<Environment> env);

#endif // EVALUATOR_H