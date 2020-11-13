#include <iostream>

#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>

using std::unique_ptr;
using std::cout;
using std::endl;

using llvm::Module;
using llvm::SMDiagnostic;
using llvm::LLVMContext;
using llvm::parseIRFile;
using llvm::StringRef;
using llvm::ExecutionEngine;
using llvm::EngineBuilder;
using llvm::ArrayRef;
using llvm::GenericValue;
using llvm::Function;

int main(int argc, char const *argv[]) {
  LLVMContext context;
  SMDiagnostic error;
  unique_ptr<Module> mod = parseIRFile(StringRef("hosted.ll" /* .ll files also work */), error, context);
  ExecutionEngine *executionEngine = EngineBuilder(std::move(mod)).setEngineKind(llvm::EngineKind::Interpreter).create();
  Function *add = executionEngine->FindFunctionNamed(StringRef("add"));
  GenericValue param1, param2;
  param1.FloatVal = 5.5;
  param2.FloatVal = 2.7;
  GenericValue params[] = { param1, param2 };
  ArrayRef<GenericValue> args = ArrayRef<GenericValue>(params, 2);
  GenericValue result = executionEngine->runFunction(add, args);
  cout << param1.FloatVal << " + " << param2.FloatVal << " = " << result.FloatVal << endl;
}
