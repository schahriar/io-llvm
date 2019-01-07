Stream-based language designed mainly as a testbed for new concepts in Compiler design.

## Parser, Lexer and ParseTree
The entire pipeline from Parser, Lexer to ParseTree is handled by PEG. We consume the ParseTree into an AST in the simplest form possible and run optimizations only on the IR which goes hand in hand with the ways of LLVM. The main use of the AST in this case would be for validation and CodeGen.

The entire grammar for the core language is defined in a single header file, this is currently a terrible experiment to keep the complexity of the developer written part to a minimum with the idea behind it being that it is preferable that the grammar and part of the vocabulary of a language (spoken, written or otherwise) should fit within a pamphlet or two, albeit source lines of code for a grammar doesn't represent its complexity, nor an easily written language equates to one that is less complex (Brainfuck being a good example here).

## CodeGen
We break tradition here by moving away from the well-known and accepted visitor-pattern and into a multi-methods structure. This is partly for experimentation and partly for allowing clean and quick expansion of the AST to solve the Expression problem. Since C++ does not have native support for multimethods, we have opted to use [YOMM2](https://github.com/jll63/yomm2) with a slight performance overhead towards calls to the CodeGen.
