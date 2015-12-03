
#ifndef AST_HXX
#define AST_CXX

#include <utility>
#include <string>
#include <vector>

namespace basic {
using V = std::pair<std::string,std::string>;

class F; // ֆունկցիա

// մոդուլ
class M {
private:
  std::string name;
  std::vector<F*> funcs;
public:
  M(const std::string& n)
	: name{n}
  {}
  llvm::Value* code();
};

class S; // հրաման

// ֆունկցիա
class F {
private:
  std::string name;
  std::vector<V> params;
  std::string rtype;
  S* body;
public:
  F(const std::string& n, const std::vector<V>& p, const std::string& r, S* b)
	: name{n}, params{p}, rtype{r}, body{b}
  {}
  llvm::Value* code();
};

//
class S {
public:
  virtual llvm::Value* code() = 0;
  virtual ~S() {}
};

class E; // արտահայտություն

//
class A : public S {
private:
  std::string var;
  E* val;
public:
  A(const std::string& r, E* l)
	: var{r}, val{l}
  {}
  llvm::Value* code();
};

}

#endif // AST_HXX

