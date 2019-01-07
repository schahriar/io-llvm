#ifndef IOLANG_AST_HPP
#define IOLANG_AST_HPP

#include <string>
#include <memory>
#include <tao/pegtl/contrib/parse_tree.hpp>
#include <exception>
#include <iostream>
#include <variant>

namespace iolang {
  static std::map<std::string, std::string> parse_tree_aliases = {
    {"iolang::language::type", "type"},
    {"iolang::language::name", "variable"},
    {"iolang::language::value", "value"},
    {"iolang::language::operation", "operation"},
    {"iolang::language::expression", "expression"}
  };

  // Forward declaration
  struct ASTNode;
  struct DebugNode;

  typedef std::string ast_ref_t;
  typedef std::shared_ptr<ASTNode> ast_shared_node_t;
  typedef std::multimap<ast_ref_t, ast_shared_node_t> ast_children_t;

  struct DebugNode {
    DebugNode(
      ast_ref_t ref,
      long start,
      long end)
      : ref(ref), start(start), end(end) {}

    ast_ref_t ref;
    long start;
    long end;
  };

  struct ASTNode {
    ASTNode() {};

    ASTNode(
      ast_ref_t ref,
      std::string content,
      ast_children_t nodes
    ): ref(ref), nodes(nodes), content(content) {};

    ASTNode(
      ast_ref_t ref,
      std::string content,
      ast_children_t nodes,
      std::shared_ptr<const DebugNode> debug
    ): ref(ref), nodes(nodes), content(content), debug(debug) {};

    ast_ref_t ref;
    std::string content;
    ast_children_t nodes;
    std::shared_ptr<const DebugNode> debug;
  };

  using namespace tao::TAO_PEGTL_NAMESPACE;

  class AST {
    public:
      static ast_shared_node_t loadFromParseTree(const parse_tree::node& parseNode) {
        auto name = (parseNode.is_root()) ? "ROOT" : parseNode.name();
        auto debug = std::make_shared<DebugNode>(name, parseNode.begin().line, parseNode.end().line);
        auto children = ast_children_t{};
        // DFS-ish recursion (if you have a fancy term, add it here)
        if(!parseNode.children.empty()) {
          for(auto& node : parseNode.children) {
            auto child = iolang::AST::loadFromParseTree(*node);
            children.insert(std::make_pair(child->ref, child));
          }
        }

        /* to be implemented
        const auto it = allowed.find(name);

        if (it == node_mapping.end()) {
          throw std::runtime_error("ASTNode of type" + name + " was not found");
        }
        */

        // @todo: Only attach during debug mode
        const auto content = (parseNode.has_content()) ? parseNode.content() : "";
        auto node = std::make_unique<ASTNode>(name, content, children, debug);

        return node;
      }
  };
}

#endif
