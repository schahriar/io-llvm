#ifndef IOLANG_CODEGEN_HPP
#define IOLANG_CODEGEN_HPP

#include <memory>
#include <vector>
#include <functional>
#include <variant>
#include "AST/AST.hpp"
#include "utils/Dispatcher.hpp"
#include <iostream>

/**
 * Concious implementation against the visitor pattern,
 * the following was considered and eventually replaced:
 * https://arne-mertz.de/2018/05/overload-build-a-variant-visitor-on-the-fly/
 * with a std::variant based AST
 * also see https://bitbashing.io/std-visit.html
 */

namespace iolang {
  namespace codegen {
    using iolang::ASTNode;
    using iolang::utils::Dispatcher;

    // Dispatcher definitions
    typedef std::function<std::string(const ASTNode&)> lambda_r_string_t;
    auto dispatcher = Dispatcher<lambda_r_string_t>();

    // Method definitions
    std::string to_string(const ASTNode& node) {
      std::cout << "NOW INVOKING " << node.ref << std::endl;
      auto x = dispatcher.invoke("to_string", node.ref);
      return x(node);
    };

    const ast_shared_node_t find(const ASTNode& node, std::string ref) {
      const auto& it = node.nodes.find(ref);

      if (it == node.nodes.end()) {
        throw std::runtime_error("Unable to find node " + ref);
      }

      return it->second;
    }

    void declare() {
      // Weird wrapper but should be inlined
      const auto declare = [&](auto a, auto b, auto c) { dispatcher.declare(a, b, c); };

      dispatcher.define(std::vector<std::string>{
        "to_string"
      });

      dispatcher.similar("iolang::language::name", "iolang::language");

      // Method implementation
      declare("to_string", "ROOT", [](const ASTNode& node) {
        return "ROOT" + to_string(*node.nodes.find("iolang::language::equal")->second);
      });

      declare("to_string", "iolang::language", [](const ASTNode& node) {
        return node.ref;
      });

      declare("to_string", "iolang::language::type", [](const ASTNode& node) {
        auto& next = find(node, "iolang::language::name");
        return "TYPE" + to_string(*next);
      });

      declare("to_string", "iolang::language::equal", [](const ASTNode& node) {
        auto& next = find(node, "iolang::language::type");
        return "EQUAL" + node.ref + " " + to_string(*next);
      });
    }
  }
}

#endif
