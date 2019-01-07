#include <iostream>
#include <tao/pegtl/contrib/parse_tree.hpp>
#include "language/grammar.hpp"
#include "writers/writers.hpp"
#include "AST/AST.hpp"
#include "CodeGen/CodeGen.hpp"
#include <nlohmann/json.hpp>
#include <memory>

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/SymbolTableListTraits.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/Support/raw_ostream.h"

int main(int argc, char* argv[]) {
  using namespace tao::TAO_PEGTL_NAMESPACE;
  using namespace llvm;

  // Link Codegen polymorphic extensions
  // to AST definition
  //iolang::link();

  LLVMContext Context;
  llvm::Module *M = new llvm::Module("test", Context);

  FunctionType *FT =
    FunctionType::get(Type::getInt32Ty(Context), /*not vararg*/false);

  Function *F = Function::Create(FT, Function::ExternalLinkage, "main", M);

  // Add a basic block to the function... again, it automatically inserts
  // because of the last argument.
  BasicBlock *BB = BasicBlock::Create(Context, "EntryBlock", F);

  // Get pointers to the constant integers...
  Value *Two = ConstantInt::get(Type::getInt32Ty(Context), 2);
  Value *Three = ConstantInt::get(Type::getInt32Ty(Context), 3);

  // Create the add instruction... does not insert...
  Instruction *Add = BinaryOperator::Create(Instruction::Add, Two, Three,
                                            "addresult");

  Instruction *Deduce = BinaryOperator::Create(Instruction::Sub, Two, Three,
                                            "subresult");

  auto y = std::vector<Instruction*>{Add, Deduce};

  BB->getInstList().push_back(y.at(0));
  BB->getInstList().push_back(y.at(1));

  // Create the return instruction and add it to the basic block
  BB->getInstList().push_back(ReturnInst::Create(Context, Add));

  llvm::WriteBitcodeToFile(*M, outs());

  delete M;

  if ( argc > 1 ) {
    argv_input in( argv, 1 );

    try {
      if(const auto root = parse_tree::parse<iolang::language::grammar, iolang::tree::selector>( in ) ) {
        iolang::writers::print(*root);
        auto ast = iolang::AST::loadFromParseTree(*root);

        iolang::codegen::declare();
        std::cout << "FINALLY? " << iolang::codegen::to_string(*ast) << std::endl;
      } else {
        std::cout << "PARSE FAILED" << std::endl;
      }
    } catch( const std::exception& e ) {
        std::cout << "PARSE FAILED WITH EXCEPTION: " << e.what() << std::endl;
    }

    std::cout << "Done" << std::endl;
  }

  return 0;
}
