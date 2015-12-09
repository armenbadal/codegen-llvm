
CXXFLAGS=`llvm-config --cxxflags`
# LIBS=`llvm-config --libs core --system-libs --ldflags`
LIBS=-L/home/armenba/llvm-3.7/lib -lLLVMCore -lLLVMSupport -lrt -ldl -lpthread -lm

all:
	clang++ $(CXXFLAGS) *.cxx $(LIBS)


