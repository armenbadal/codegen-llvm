
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

  // հրամանների հաջորդականություն
  class Sequence : public Statement {
  private:
	Statement* stato = nullptr;
	Statement* stati = nullptr;
  public:
	Sequence(Statement* so, Statement* si)
	  : stato{so}, stati{si}
	{}
	llvm::Value* code();
  };

  // փոփոխականի հայտարարություն
  class Declare : public Statement {
  private:
	std::string name;
	std::string type;
  public:
	Declare(const std::string& n, const std::string& t)
	  : name{n}, type{t}
	{}
	llvm::Value* code();
  };

  class Expression; // արտահայտություն
  
  // վերագրման գործողություն
  class Assign : public Statement {
  private:
	std::string variable;
	Expression* value;
  public:
	Assign(const std::string& r, Expression* l)
	  : variable{r}, value{l}
	{}
	llvm::Value* code();
  };

  // ֆունկցիայից արժեքի վերադարձ
  class Return : public Statement {
  private:
	Expression* expro = nullptr;
  public:
	Return(Expression* eo)
	  : expro{eo}
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
  public:
	Variable(const std::string& n)
	  : name{n}
	{}
	llvm::Value* code();
  };
}

#endif // AST_HXX

