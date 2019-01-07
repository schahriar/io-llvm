#ifndef IOLANG_GRAMMAR_HPP
#define IOLANG_GRAMMAR_HPP

#include <string>
#include <type_traits>
#include <tao/pegtl/contrib/parse_tree.hpp>
#include <tao/pegtl.hpp>

namespace iolang {
  namespace language {
    using namespace tao::TAO_PEGTL_NAMESPACE;

    struct char_eos : plus< one< ';' > > {};

    struct primative_number : TAO_PEGTL_STRING("number") {};
    struct primative_string : TAO_PEGTL_STRING("string") {};
    struct primative_struct : TAO_PEGTL_STRING("struct") {};

    struct add : pad< one< '+' >, space > {};
    struct equal : pad< one< '=' >, space > {};

    struct s0 : star< one< ' ' > > {};
    struct s1 : plus< one< ' ' > > {};
    struct type : sor< primative_number, primative_string, primative_struct > {};
    struct name : plus< identifier > {};
    struct value : plus< digit > {};
    struct variable : must< type, s1, name > {};
    struct expression : must< variable, opt< s1 >, sor < equal, add >, opt< s1 >, value > {};
    struct statement : seq< expression, opt< char_eos > > {};
    struct grammar : seq< statement, eof > {};
  }

  namespace tree {
    using namespace tao::TAO_PEGTL_NAMESPACE;
    using namespace language;

    struct rearrange : std::true_type
    {
        template< typename... States >
        static void transform(std::unique_ptr<parse_tree::node>& n, States&&... st)
        {
          if ( n-> children.size() == 0) {
            return;
          }
          
          if( n->children.size() == 1 ) {
              n = std::move( n->children.back() );
          } else {
              n->remove_content();
              auto& c = n->children;
              auto r = std::move( c.back() );
              c.pop_back();
              auto o = std::move( c.back() );
              c.pop_back();
              o->children.emplace_back( std::move( n ) );
              o->children.emplace_back( std::move( r ) );
              n = std::move( o );
              transform( n->children.front(), st... );
          }
        }
    };

    template< typename Rule >
    using selector = parse_tree::selector<
                     Rule,
                     parse_tree::apply_store_content::to<type, name, value>,
                     parse_tree::apply_remove_content::to<
                      equal,
                      add
                      >,
                     parse_tree::apply<rearrange>::to<
                      expression
                      >,
                     parse_tree::apply_discard_empty::to<expression>
                    >;
  }
}

#endif
