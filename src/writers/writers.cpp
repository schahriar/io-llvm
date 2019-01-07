#include <iostream>
#include <string>
#include "writers.hpp"
#include <tao/pegtl/contrib/parse_tree.hpp>
#include <nlohmann/json.hpp>

void iolang::writers::print(const parse_tree::node& n, const int depth) {
  const auto prefix = std::string(depth * 2, ' ');
  // Detect the root node
  if(n.is_root()) {
    std::cout << "ROOT" << std::endl;
  } else {
    if(n.has_content()) {
      std::cout << prefix << n.name() << " \"" << n.content() << "\" at " << n.begin() << " to " << n.end() << std::endl;
    } else {
      std::cout << prefix << n.name() << " at " << n.begin() << std::endl;
    }
  }
  // print all child nodes
  if( !n.children.empty() ) {
    for( auto& up : n.children ) {
      print( *up, depth + 1 );
    }
  }
}

nlohmann::json iolang::writers::json(const parse_tree::node& n) {
  using json = nlohmann::json;
  using json_pointer = nlohmann::json::json_pointer;
  using string = std::string;

  const json base = {};
  auto children = json::array({});

  if (!n.children.empty()) {
    for( auto& up : n.children ) {
      auto child = iolang::writers::json( *up );
      children.push_back(child.get_ref<const json::object_t&>());
    }
  }

  if (n.is_root()) {
    return json({
      {"root", {
        "body", children
      }}
    });
  } else {
    auto iterator = aliases.find(n.name());
    const string type_name = (iterator != aliases.end()) ? iterator->second : n.name();
    const auto token = json({
      {"start", n.begin().line},
      {"end", n.end().line},
      {"source", n.begin().source},
    }).get_ref<const json::object_t&>();

    if(n.has_content()) {
      return json({
        {"type", type_name},
        {"value", n.content()},
        {"token", token},
        {"body", children }
      });
    } else {
      return json({
        {"type", type_name},
        {"token", token},
        {"body", children}
      });
    }
  }

  return json({});
}
