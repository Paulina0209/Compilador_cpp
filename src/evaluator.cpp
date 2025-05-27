#include "evaluator.h"
#include <iostream>

std::shared_ptr<Object> eval(Node* node, std::shared_ptr<Environment> env);

std::shared_ptr<Object> eval(std::unique_ptr<Node>& node, std::shared_ptr<Environment> env) {
    return eval(node.get(), env);
}

std::shared_ptr<Object> eval(Node* node, std::shared_ptr<Environment> env) {
    if (auto program = dynamic_cast<Program*>(node)) {
        std::shared_ptr<Object> result;
        for (auto& stmt : program->statements) {
            result = eval(stmt.get(), env);
            if (result && result->type() == ObjectType::RETURN_VALUE_OBJ) {
                return std::dynamic_pointer_cast<ReturnValue>(result)->value;
            }
        }
        return result;
    }

    if (auto block = dynamic_cast<BlockStatement*>(node)) {
        std::shared_ptr<Object> result;
        for (auto& stmt : block->statements) {
            result = eval(stmt.get(), env);
            if (result && result->type() == ObjectType::RETURN_VALUE_OBJ) {
                return result;
            }
        }
        return result;
    }

    if (auto stmt = dynamic_cast<ExpressionStatement*>(node)) {
        return eval(stmt->expression.get(), env);
    }

    if (auto int_lit = dynamic_cast<IntegerLiteral*>(node)) {
        return std::make_shared<Integer>(int_lit->value);
    }

    if (auto bool_lit = dynamic_cast<BooleanLiteral*>(node)) {
        return std::make_shared<Boolean>(bool_lit->value);
    }

    if (auto ident = dynamic_cast<Identifier*>(node)) {
        auto val = env->get(ident->value);
        if (val) return val;
        std::cerr << "Identificador no definido: " << ident->value << "\n";
        return nullptr;
    }

    if (auto let_stmt = dynamic_cast<LetStatement*>(node)) {
        auto val = eval(let_stmt->value.get(), env);

        if (val && val->type() == ObjectType::FUNCTION_OBJ) {
            auto func = std::dynamic_pointer_cast<Function>(val);
            auto func_env = std::make_shared<Environment>(func->env);
            std::shared_ptr<Function> self_ref = std::make_shared<Function>(func->parameters, func->body, func_env);
            func_env->set(let_stmt->name, self_ref);
            val = self_ref;
        }

        if (val) {
            env->set(let_stmt->name, val);
        }
        return val;
    }

    if (auto prefix = dynamic_cast<PrefixExpression*>(node)) {
        auto right = eval(prefix->right.get(), env);
        if (!right) return nullptr;

        if (prefix->op == "!") {
            if (right->type() == ObjectType::BOOLEAN_OBJ) {
                return std::make_shared<Boolean>(!std::dynamic_pointer_cast<Boolean>(right)->value);
            } else if (right->type() == ObjectType::NULL_OBJ) {
                return std::make_shared<Boolean>(true);
            } else {
                return std::make_shared<Boolean>(false);
            }
        }
        if (prefix->op == "-") {
            if (right->type() == ObjectType::INTEGER_OBJ) {
                return std::make_shared<Integer>(-std::dynamic_pointer_cast<Integer>(right)->value);
            }
        }
        return nullptr;
    }

    if (auto infix = dynamic_cast<InfixExpression*>(node)) {
        auto left = eval(infix->left.get(), env);
        auto right = eval(infix->right.get(), env);
        if (!left || !right) return nullptr;

        if (left->type() == ObjectType::INTEGER_OBJ && right->type() == ObjectType::INTEGER_OBJ) {
            int lval = std::dynamic_pointer_cast<Integer>(left)->value;
            int rval = std::dynamic_pointer_cast<Integer>(right)->value;
            if (infix->op == "+") return std::make_shared<Integer>(lval + rval);
            if (infix->op == "-") return std::make_shared<Integer>(lval - rval);
            if (infix->op == "*") return std::make_shared<Integer>(lval * rval);
            if (infix->op == "/") return std::make_shared<Integer>(lval / rval);
            if (infix->op == "==") return std::make_shared<Boolean>(lval == rval);
            if (infix->op == "!=") return std::make_shared<Boolean>(lval != rval);
            if (infix->op == "<") return std::make_shared<Boolean>(lval < rval);
            if (infix->op == ">") return std::make_shared<Boolean>(lval > rval);
        }

        if (left->type() == ObjectType::BOOLEAN_OBJ && right->type() == ObjectType::BOOLEAN_OBJ) {
            bool lval = std::dynamic_pointer_cast<Boolean>(left)->value;
            bool rval = std::dynamic_pointer_cast<Boolean>(right)->value;
            if (infix->op == "==") return std::make_shared<Boolean>(lval == rval);
            if (infix->op == "!=") return std::make_shared<Boolean>(lval != rval);
        }

        return nullptr;
    }

    if (auto if_expr = dynamic_cast<IfExpression*>(node)) {
        auto condition = eval(if_expr->condition.get(), env);
        if (!condition) return nullptr;

        bool cond = false;
        if (condition->type() == ObjectType::BOOLEAN_OBJ) {
            cond = std::dynamic_pointer_cast<Boolean>(condition)->value;
        } else if (condition->type() == ObjectType::NULL_OBJ) {
            cond = false;
        } else {
            cond = true;
        }

        if (cond) {
            return eval(if_expr->consequence.get(), env);
        } else if (if_expr->alternative) {
            return eval(if_expr->alternative.get(), env);
        } else {
            return std::make_shared<Null>();
        }
    }

    if (auto while_stmt = dynamic_cast<WhileStatement*>(node)) {
        std::shared_ptr<Object> result;
        while (true) {
            auto cond = eval(while_stmt->condition.get(), env);
            if (!cond || (cond->type() == ObjectType::BOOLEAN_OBJ && !std::dynamic_pointer_cast<Boolean>(cond)->value)) {
                break;
            }
            result = eval(while_stmt->body.get(), env);
        }
        return result;
    }

    if (auto func = dynamic_cast<FunctionLiteral*>(node)) {
        return std::make_shared<Function>(func->parameters, func->body, env);
    }

    if (auto call = dynamic_cast<CallExpression*>(node)) {
        auto func_obj = eval(call->function.get(), env);
        if (!func_obj || func_obj->type() != ObjectType::FUNCTION_OBJ) {
            std::cerr << "Llamando a algo que no es funcion\n";
            return nullptr;
        }

        auto func = std::dynamic_pointer_cast<Function>(func_obj);
        if (func->parameters.size() != call->arguments.size()) {
            std::cerr << "Cantidad de argumentos incorrecta\n";
            return nullptr;
        }

        auto extended_env = std::make_shared<Environment>(func->env);
        for (size_t i = 0; i < func->parameters.size(); ++i) {
            auto arg_val = eval(call->arguments[i].get(), env);
            if (!arg_val) return nullptr;
            extended_env->set(func->parameters[i], arg_val);
        }

        auto result = eval(func->body.get(), extended_env);
        if (result && result->type() == ObjectType::RETURN_VALUE_OBJ) {
            return std::dynamic_pointer_cast<ReturnValue>(result)->value;
        }
        return result;
    }

    return nullptr;
}
