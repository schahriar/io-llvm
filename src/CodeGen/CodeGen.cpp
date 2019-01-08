#include "CodeGen.hpp"
#include <iostream>

using iolang::ASTNode;

std::string iolang::codegen::to_string(const ASTNode& node) {
  using iolang::codegen::invoke;

  auto func = invoke("to_string", node.ref);
  return func(node);
};

void iolang::codegen::declare_standard_dispatchers() {
  using iolang::codegen::find;
  using iolang::codegen::declare;
  using iolang::codegen::define;
  using iolang::codegen::similar;
  using iolang::codegen::to_string;

  // Absolute default, may throw during runtime
  define("to_string", [](const ASTNode& node) {
    return node.ref;
  });

  similar("iolang::language::value", "iolang::language:type");

  // Method implementation
  declare("to_string", "ROOT", [](const ASTNode& node) {
    return "ROOT" + to_string(*node.nodes.find("iolang::language::equal")->second);
  });

  declare("to_string", "iolang::language::type", [](const ASTNode& node) {
    auto& next = find(node, "iolang::language::name");
    return "TYPE" + to_string(*next);
  });

  declare("to_string", "iolang::language::equal", [](const ASTNode& node) {
    auto& next = find(node, "iolang::language::type");
    return "EQUAL" + node.ref + " " + to_string(*next);
  });
};
