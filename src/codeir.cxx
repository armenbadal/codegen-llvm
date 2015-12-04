
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>

#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>

#include <llvm/IR/Type.h>
#include <llvm/IR/ValueSymbolTable.h>

#include <map>
#include <vector>

#include "ast.hxx"

namespace {
  auto& context = llvm::getGlobalContext();

  llvm::IRBuilder<> builder{context};

  llvm::Module* curMod = nullptr;
  llvm::Function* curFunc = nullptr;

  std::map<std::string,llvm::Value*> curLocs;
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

  std::vector<llvm::Type*> _t1;
  for( auto& p : params ) _t1.push_back( asType(p.second) );

  auto _t = llvm::FunctionType::get(_t0, llvm::ArrayRef<llvm::Type*>{_t1}, false);
  auto _f = llvm::Function::Create(_t, llvm::Function::InternalLinkage, name, curMod);
  curFunc = _f;
  curLoc.clear();

  auto ip = _f->arg_begin();
  for( auto& p : params ) {
	ip->setName(p.first);
	++ip;
  }

  auto _b0 = llvm::BasicBlock::Create(context, "start", _f);
  builder.SetInsertPoint(_b0);

  body->code();

  return _f;
}

llvm::Value* basic::Assignment::code()
{
  auto& _s = curFunc->getValueSymbolTable();
  auto _v = _s.lookup(variable);
  if( _v == nullptr )
	_v = builder.CreateAlloca(asType("INTEGER"), nullptr, variable);

  auto _e = value->code();
  builder.CreateStore(_v, _e);

  return nullptr;
}

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


