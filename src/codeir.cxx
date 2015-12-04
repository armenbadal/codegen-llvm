
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>

#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>

#include <llvm/IR/Type.h>
#include <llvm/IR/ValueSymbolTable.h>

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringMap.h>

#include "ast.hxx"

namespace {
  auto& context = llvm::getGlobalContext();

  llvm::IRBuilder<> builder{context};

  llvm::Module* curMod = nullptr;
  llvm::Function* curFunc = nullptr;

  llvm::StringMap<llvm::Value*> curLocs;
}

namespace {
  llvm::Type* asType(const std::string& ny)
  {
	if( "BOOLEAN" == ny )
	  return builder.getInt1Ty();

	if( "INTEGER" == ny )
	  return builder.getInt32Ty();

	return builder.getVoidTy();
  }
}


llvm::Module* ast::Module::code()
{
  auto _m = new llvm::Module{name, context};
  curMod = _m;

  for( auto& f : funcs )
	f->code();

  return _m;
}

llvm::Function* ast::Function::code()
{
  auto _t0 = asType(rtype);

  llvm::SmallVector<llvm::Type*,10> _t1;
  for( auto& p : params ) _t1.push_back( asType(p.second) );

  auto _t = llvm::FunctionType::get(_t0, _t1, false);
  auto _f = llvm::Function::Create(_t, llvm::Function::InternalLinkage, name, curMod);
  curFunc = _f;
  curLocs.clear();

  auto _b0 = llvm::BasicBlock::Create(context, "start", _f);
  builder.SetInsertPoint(_b0);

  auto& _st = _f->getValueSymbolTable();
  auto ip = _f->arg_begin();
  for( auto& p : params ) {
	ip->setName(p.first);
	auto _m = builder.CreateAlloca(ip->getType());
	builder.CreateStore(_st.lookup(p.first), _m);
	curLocs[p.first] = _m;
	++ip;
  }

  body->code();

  return _f;
}

//
void ast::Sequence::code()
{
  if( stato != nullptr )
	stato->code();
  if( stati != nullptr ) 
	stati->code();
}

//
void ast::Declare::code()
{
  auto _t = asType(type);
  auto _m = builder.CreateAlloca(_t, nullptr, name);
  curLocs[name] = _m;
}

//
void ast::Assign::code()
{
  auto _p = curLocs[variable];
  auto _e = value->code();
  builder.CreateStore(_e, _p);
}

//
void ast::Return::code()
{
  auto _r = expro->code();
  builder.CreateRet(_r);
}

//
void ast::If::code()
{
  auto _c = cond->code();

  auto _b0 = llvm::BasicBlock::Create(context, "b0", curFunc);
  auto _b2 = llvm::BasicBlock::Create(context, "b2", curFunc);
  auto _b1 = _b2;
  if( selse != nullptr )
	_b1 = llvm::BasicBlock::Create(context, "b1", curFunc, _b2);
  //auto __p = builder.saveIP();
  builder.CreateCondBr(_c, _b0, _b1);
  
  builder.SetInsertPoint(_b0);
  sthen->code();
  builder.CreateBr(_b2);

  if( selse != nullptr ) {
	builder.SetInsertPoint(_b1);
	selse->code();
	builder.CreateBr(_b2);
  }

  builder.SetInsertPoint(_b2);
  //builder.restoreIP(__p);
}

// 
llvm::Value* ast::Binary::code()
{
  auto _e0 = expro->code();
  auto _e1 = expri->code();

  if( "ADD" == oper )
	return builder.CreateNSWAdd(_e0, _e1);
  else if( "EQ" == oper )
	return builder.CreateICmpEQ(_e0, _e1);
  else if( "AND" == oper )
	return builder.CreateAnd(_e0, _e1);

  return nullptr;
}

//
llvm::Value* ast::Unary::code()
{
  auto _e = expr->code();

  if( "NEG" == oper )
	_e = builder.CreateNeg(_e);
  else if( "NOT" == oper )
	_e = builder.CreateNot(_e);

  return _e;
}

//
llvm::Value* ast::FunCall::code()
{
  auto _f = curMod->getFunction(func);

  llvm::SmallVector<llvm::Value*,10> _a;
  for( auto p : args ) 
	_a.push_back(p->code());

  return builder.CreateCall(_f, _a);
}

//
llvm::Value* ast::Constant::code()
{
  if( "INTEGER" == type )
	return builder.getInt32(std::stoi(value, 0, 10));

  if( "BOOLEAN" == type ) {
	if( "TRUE" == value )
	  return builder.getTrue();
	else if( "FALSE" == value )
	  return builder.getFalse();
  }

  return builder.getInt32(0);
}

//
llvm::Value* ast::Variable::code()
{
  return builder.CreateLoad(curLocs[name]);
}

