
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


llvm::Module* basic::Module::code()
{
  auto _m = new llvm::Module{name, context};
  curMod = _m;

  for( auto& f : funcs )
	f->code();

  return _m;
}

llvm::Value* basic::Function::code()
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
llvm::Value* basic::Sequence::code()
{
  if( stato != nullptr ) stato->code();
  if( stati != nullptr ) stati->code();

  return nullptr;
}

//
llvm::Value* basic::Declare::code()
{
  auto _t = asType(type);
  auto _m = builder.CreateAlloca(_t, nullptr, name);
  curLocs[name] = _m;

  return nullptr;
}

//
llvm::Value* basic::Assign::code()
{
  auto _p = curLocs[variable];
  auto _e = value->code();
  builder.CreateStore(_e, _p);

  return nullptr;
}

//
llvm::Value* basic::Return::code()
{
  auto _r = expro->code();
  return builder.CreateRet(_r);
}

//
llvm::Value* basic::Constant::code()
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
llvm::Value* basic::Variable::code()
{
  return builder.CreateLoad(curLocs[name]);
}

