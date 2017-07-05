
CXXFLAGS=`llvm-config --cxxflags`
LIBS=`llvm-config --libs core --system-libs --ldflags`
# LIBS=-L/home/armen/llvm-4.0.0/lib -lLLVMCore -lLLVMSupport -lrt -ldl -lpthread -lm

all:
	clang++ $(CXXFLAGS) *.cxx $(LIBS)

lib:
	clang -S -emit-llvm internals.c


