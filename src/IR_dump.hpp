#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include "../src/AST.hpp"

class IR_dump_t{
    llvm::LLVMContext TheContext;
    llvm::IRBuilder<>* Builder;
    llvm::Module* TheModule;
    public:
    IR_dump_t();
    ~IR_dump_t();
    void run(node_t::one_t* ast);
};
