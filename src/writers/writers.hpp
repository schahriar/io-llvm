#ifndef IOLANG_WRITERS_PRINT_HPP
#define IOLANG_WRITERS_PRINT_HPP

#include <nlohmann/json.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

namespace iolang {
  namespace writers {
    using namespace tao::pegtl;

    static std::map<std::string, std::string> aliases = {
      {"iolang::language::type", "type"},
      {"iolang::language::name", "variable"},
      {"iolang::language::value", "value"},
      {"iolang::language::operation", "operation"},
      {"iolang::language::expression", "expression"}
    };

    void print(const parse_tree::node&, const int = 0);
    nlohmann::json json(const parse_tree::node&);
  }
}

#endif
