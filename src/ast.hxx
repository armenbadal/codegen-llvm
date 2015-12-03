
#ifndef AST_HXX
#define AST_HXX

#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

#include <utility>
#include <string>
#include <vector>

namespace basic {
  // այս զույգի առաջին տարրը սիմվոլի անունն է, իսկ 
  // երկրորդը ներդրված տիպի անունն է
  using Symbol = std::pair<std::string,std::string>;

  class Function; // ֆունկցիա
  
  // մոդուլը կոմպիլյացիայի միավորն է, որը պարունակում
  // է սահմանված ֆունկցիաներն ու պրոցեդուրաները
  class Module {
  private:
	std::string name;
	std::vector<Function*> funcs;
  public:
	Module(const std::string& n)
	  : name{n}
	{}
	void addFunction(Function* f)
	{ funcs.push_back(f); }
	llvm::Module* code();
  };
  
  class Statement; // հրաման
  
  // ֆունկցիան (կամ պրոցեդուրան)
  class Function {
  private:
	std::string name;
	std::vector<Symbol> params;
	std::string rtype;
	Statement* body;
  public:
	Function(const std::string& n, const std::vector<Symbol>& p, const std::string& r, Statement* b)
	  : name{n}, params{p}, rtype{r}, body{b}
	{}
	llvm::Value* code();
  };
  
  //
  class Statement {
  public:
	virtual llvm::Value* code() = 0;
	virtual ~Statement() {}
  };

  class Expression; // արտահայտություն
  
  // վերագրման գործողություն
  class Assignment : public Statement {
  private:
	std::string variable;
	Expression* value;
  public:
	Assignment(const std::string& r, Expression* l)
	  : variable{r}, value{l}
	{}
	llvm::Value* code();
  };

  // ․․․ այլ հրամաններ

  // արտահայտություն
  class Expression {
  public:
	virtual llvm::Value* code() = 0;
	virtual ~Expression() {}
  };

  // հաստատուն 
  class Constant : public Expression {
  private:
	std::string type;
	std::string value;
  public:
	Constant(const std::string& t, const std::string& v)
	  : type{t}, value{v}
	{}
	llvm::Value* code();
  };

  // փոփոխական
  class Variable : public Expression {
  private:
	std::string name;
	std::string type;
  public:
	Variable(const std::string& n, const std::string& t)
	  : name{n}, type{t}
	{}
	llvm::Value* code();
  };
}

#endif // AST_HXX

