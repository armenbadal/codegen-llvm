
#include <llvm/IR/Module.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/IRPrintingPasses.h>

//#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/raw_os_ostream.h>

#include <fstream>

#include "ast.hxx"

int main()
{
  auto mod = new ast::Module{"Test0"};

  // auto s0 = new ast::Declare{"z", "INTEGER"};
  // auto e0 = new ast::Binary{"ADD", new ast::Variable{"x"}, new ast::Variable{"y"}};
  // auto s1 = new ast::Assign{"z", e0};
  // auto s2 = new ast::Sequence{s0, s1};
  // auto s3 = new ast::Return{new ast::Variable{"z"}};
  // auto s4 = new ast::Sequence{s2, s3};
  // auto f0 = new ast::Function{"f0", {{"x", "INTEGER"}, {"y", "INTEGER"}}, "INTEGER", s4};
  // mod->addFunction(f0);

  // auto e1 = new ast::Constant{"INTEGER", "88"};
  // auto e2 = new ast::Constant{"INTEGER", "44"};
  // auto e3 = new ast::FunCall{"f0", {e1, e2}};
  // auto s5 = new ast::Return{e3};
  // auto f1 = new ast::Function{"f1", {}, "BOOLEAN", s5};
  // mod->addFunction(f1);

  // auto e0 = new ast::Binary{"AND", new ast::Variable{"a"}, new ast::Variable{"b"}};
  // auto s0 = new ast::Declare{"c", "INTEGER"};
  // auto s1 = new ast::Assign{"c", new ast::Constant{"INTEGER", "777"}};
  // auto s2 = new ast::Assign{"c", new ast::Constant{"INTEGER", "444"}};
  // auto s3 = new ast::If{e0, s1, s2};
  // auto s5 = new ast::If{e0, s3, nullptr};
  // auto s4 = new ast::Sequence{s0, s5};
  // auto f0 = new ast::Function{"f", {{"a", "BOOLEAN"}, {"b", "BOOLEAN"}}, "VOID", s4};
  // mod->addFunction(f0);

  // auto a0 = new ast::Unary{"NOT", new ast::Variable{"a"}};
  // auto s0 = new ast::Declare{"c", "INTEGER"};
  // auto s1 = new ast::Assign{"c", new ast::Constant{"INTEGER", "88"}};
  // auto s2 = new ast::While{a0, s1};
  // auto a1 = new ast::Unary{"NOT", new ast::Variable{"b"}};
  // auto s3 = new ast::While{a1, s2};
  // auto s4 = new ast::Sequence{s0, s3};
  // auto f0 = new ast::Function{"f", {{"a", "BOOLEAN"}, {"b", "BOOLEAN"}}, "VOID", s4};
  // mod->addFunction(f0);

  auto s0 = new ast::Declare{"c", "INTEGER"};
  auto s1 = new ast::Input{"c"};
  auto s2 = new ast::Print{{new ast::Variable{"c"}}};
  auto s3 = new ast::Sequence{s1, s2};
  auto s4 = new ast::Sequence{s0, s3};
  auto f0 = new ast::Function{"f", {}, "VOID", s4};
  mod->addFunction(f0);

  mod->code()->dump();


  std::ofstream sout{"a.ll"};
  llvm::raw_os_ostream roo{sout};
  auto pmp = llvm::createPrintModulePass(roo, "");
  llvm::legacy::PassManager pm;
  pm.add(pmp);
  pm.run(*(mod->code()));
  sout.close();

  return 0;
}

