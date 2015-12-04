
#include "ast.hxx"

int main()
{
  auto mod = new basic::Module{"Test0"};

  auto s0 = new basic::Declare{"z", "INTEGER"};
  auto e0 = new basic::Constant{"INTEGER", "77"};
  auto s1 = new basic::Assign{"z", e0};
  auto s2 = new basic::Sequence{s0, s1};
  auto s3 = new basic::Return{new basic::Variable{"z"}};
  auto s4 = new basic::Sequence{s2, s3};
  auto f0 = new basic::Function{"f0", {{"x", "INTEGER"}, {"y", "INTEGER"}}, "INTEGER", s4};
  mod->addFunction(f0);

  mod->code()->dump();

  return 0;
}

