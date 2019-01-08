#ifndef IOLANG_CODEGEN_HPP
#define IOLANG_CODEGEN_HPP

#include <memory>
#include <vector>
#include <functional>
#include <variant>
#include <utility>
#include "AST/AST.hpp"
#include "utils/Dispatcher.hpp"

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
    static Dispatcher<lambda_r_string_t> dt = Dispatcher<lambda_r_string_t>();

    // Method definitions
    std::string to_string(const ASTNode& node);

    inline const ast_shared_node_t find(const ASTNode& node, std::string ref) {
      const auto& it = node.nodes.find(ref);

      if (it == node.nodes.end()) {
        throw std::runtime_error("Unable to find node " + ref);
      }

      return it->second;
    };

    static const auto invoke = [](auto a, auto b) { return dt.invoke(a, b); };
    static const auto define = [](auto a, auto b) { dt.define(a, b); };
    static const auto similar = [](auto a, auto b) { dt.similar(a, b); };
    static const auto declare = [](auto a, auto b, auto c) { dt.declare(a, b, c); };

    void declare_standard_dispatchers();
    static void register_decl() {
      declare_standard_dispatchers();
    };
  }
}

#endif
