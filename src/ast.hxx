
#ifndef AST_HXX
#define AST_HXX

#include <utility>
#include <string>
#include <vector>

namespace llvm {
  class Module;
  class Function;
  class Value;
}

namespace ast {
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
	Statement* body = nullptr;
  public:
	Function(const std::string& n, const std::vector<Symbol>& p, const std::string& r, Statement* b)
	  : name{n}, params{p}, rtype{r}, body{b}
	{}
	llvm::Function* code();
  };
  
  // հրամաններ ինտերֆեյս
  class Statement {
  public:
	virtual void code() = 0;
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
	void code();
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
	void code();
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
	void code();
  };

  // ֆունկցիայից արժեքի վերադարձ
  class Return : public Statement {
  private:
	Expression* expro = nullptr;
  public:
	Return(Expression* eo)
	  : expro{eo}
	{}
	void code();
  };
  
  // ճյուղավորման հրաման
  class If : public Statement {
  private:
	Expression* cond = nullptr;
	Statement* sthen = nullptr;
	Statement* selse = nullptr;
  public:
	If(Expression* c, Statement* t, Statement* e)
	  : cond{c}, sthen{t}, selse{e}
	{}
	void code();
  };

  // նախապայմանով կրկնման հրաման
  class While : public Statement {
  private:
	Expression* cond = nullptr;
	Statement* body = nullptr;
  public:
	While(Expression* c, Statement* b)
	  : cond{c}, body{b}
	{}
	void code();
  };

  // պարամետրով կրկնման ցիկլ
  class For : public Statement {
  private:
	std::string par; // պարամետր
	Expression* start = nullptr; // սկզբնական արժեք
	Expression* end = nullptr; // վերջնական արժեք
	Expression* step = nullptr; // արժեքի քայլ
	Statement* body = nullptr; // ցիկլի մարմին
  public:
	For(const std::string& p, Expression* i, Expression* e, Expression* s, Statement* b)
	  : par{p}, start{i}, end{e}, step{s}, body{b}
	{}
	void code();
  };

  // արտածման հրաման
  class Print : public Statement {
  private:
	std::vector<Expression*> exprs;
  public:
	Print(const std::vector<Expression*>& es)
	  : exprs{es}
	{}
	void code();
  };

  // ․․․ այլ հրամաններ

  // արտահայտություն
  class Expression {
  public:
	virtual llvm::Value* code() = 0;
	virtual ~Expression() {}
  };

  // բինար գործողություն
  class Binary : public Expression {
  private:
	std::string oper;
	Expression* expro;
	Expression* expri;
  public:
	Binary(const std::string& o, Expression* eo, Expression* ei)
	  : oper{o}, expro{eo}, expri{ei}
	{}
	llvm::Value* code();
  };

  // ունար գործողություն
  class Unary : public Expression {
  private:
	std::string oper;
	Expression* expr;
  public:
	Unary(const std::string& o, Expression* e)
	  : oper{o}, expr{e}
	{}
	llvm::Value* code();
  };

  // ֆունկցիայի կանչ
  class FunCall : public Expression {
  private:
	std::string func;
	std::vector<Expression*> args;
  public:
	FunCall(const std::string& f, const std::vector<Expression*>& a)
	  : func{f}, args{a}
	{}
	llvm::Value* code();
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

