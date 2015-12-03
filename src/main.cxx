
#include "ast.hxx"

int main()
{
  auto mod = new basic::Module{"Test0"};

  auto e0 = new basic::Constant{"INTEGER", "77"};
  auto s0 = new basic::Assignment{"z", e0};
  auto f0 = new basic::Function{"f0", {{"x", "INTEGER"}, {"y", "INTEGER"}}, "BOOLEAN", s0};
  mod->addFunction(f0);

  mod->code()->dump();

  return 0;
}

