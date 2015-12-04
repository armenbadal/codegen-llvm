
#include <llvm/IR/Module.h>

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

  
  auto e0 = new ast::Binary{"AND", new ast::Variable{"a"}, new ast::Variable{"b"}};
  auto s0 = new ast::Declare{"c", "INTEGER"};
  auto s1 = new ast::Assign{"c", new ast::Constant{"INTEGER", "777"}};
  auto s2 = new ast::Assign{"c", new ast::Constant{"INTEGER", "444"}};
  auto s3 = new ast::If{e0, s1, s2};
  auto s4 = new ast::Sequence{s0, s3};
  auto f0 = new ast::Function{"f", {{"a", "BOOLEAN"}, {"b", "BOOLEAN"}}, "VOID", s4};
  mod->addFunction(f0);

  mod->code()->dump();

  return 0;
}

