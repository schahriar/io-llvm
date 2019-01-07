#ifndef IOLANG_UTILS_DISPATCHER_HPP
#define IOLANG_UTILS_DISPATCHER_HPP

/**
 * Addressing the Expression Problem
 * ---------------------------------
 * This is a rather naive implementation
 * of a runtime dispatch table but introduces
 * the concepts needed towards having a working
 * solution. While yomm2 (https://github.com/jll63/yomm2)
 * was integrated prior to this implementation it Segfaulted
 * within a docker container and rather than spending time on
 * debugging yomm2 (it is a great implementation) this solution
 * was put together.
 * 
 * As mentioned elsewhere, this is rather an experiment on
 * implementing an AST without the visitor pattern. Having used
 * visitor pattern extensively in the past it is a great design
 * pattern for rather complex and large ASTs much of which may
 * be bloated with unnecessary features. Revisiting the Visitor
 * pattern can be an option for the future should this way of
 * extending functionality on top of the AST fail miserably.
 */

#include <functional>
#include <map>
#include <exception>

namespace iolang {
  namespace utils {
    struct DispatchFunctionNotFound : public std::exception {
      const char * what () const throw () {
        return "Dispatching function was not found during the runtime";
      }
    };

    template <typename T>
    struct DispatchReference {
      DispatchReference(T func, T next) : func(func), next(next) {};

      T func;
      T next;
    };

    template <class T>
    class Dispatcher {
      private:
        std::map<std::string, std::map<std::string, T>> routes;
        std::map<std::string, std::string> fallbacks;

      public:
        Dispatcher(
          std::map<std::string, std::map<std::string, T>> routes = {{}},
          std::map<std::string, std::string> fallbacks = {}
        ): routes(routes), fallbacks(fallbacks) {};

        void define(std::vector<std::string> scopes) {
          for (auto& scope : scopes) {
            routes.insert(
              std::pair<
                std::string,
                std::map<std::string, T>
              >(scope, std::map<std::string, T>{})
            );
          }
        }

        void declare(std::string scope, std::string reference, T func) {
          auto& table = routes.find(scope)->second;

          table.insert(
            std::pair<std::string, T>(reference, func)
          );
        }

        void similar(std::string reference, std::string next) {
          fallbacks.insert(
            std::pair<std::string, std::string>(reference, next)
          );
        }

        T invoke(std::string scope, std::string reference) {
          const auto& table = routes.find(scope)->second;
          const auto& ref = table.find(reference);
          const auto& next = fallbacks.find(reference);

          // Attempt to automatically switch
          // to the next best option
          if (ref == table.end()) {
            if (next != fallbacks.end()) {
              return invoke(scope, next->second);
            }

            throw DispatchFunctionNotFound();
          }

          return ref->second;
        }
    };
  }
}

#endif
