#include "../src/IR_dump.hpp"

IR_dump_t::IR_dump_t(){
    TheModule=new llvm::Module("top", TheContext);
    Builder=new llvm::IRBuilder<>(TheContext);
}

IR_dump_t::~IR_dump_t(){
    delete TheModule;
    delete Builder;
}

void IR_dump_t::run(node_t::one_t* ast){
    {
        switch (ast->node_type)
        {
            case node_t::node_type_t::def_func_t:
            {
                auto temp=(node_t::def_func_t*)ast;
                //後で戻り値と型を定義
                llvm::Function* Func = llvm::Function::Create(
                    llvm::FunctionType::get(llvm::Type::getInt32Ty(TheContext), false),
                    llvm::Function::ExternalLinkage,
                    temp->name->name ,
                    TheModule
                );

                Builder->SetInsertPoint(llvm::BasicBlock::Create(TheContext, "", Func));

                //関数定義処理
                run(temp->body);
                break;
            }
        case node_t::node_type_t::some_t:
            {
                auto temp=(node_t::some_t*)ast;
                for(int i=0;i<temp->i_size;i++){
                    run(temp->data[i]);
                }
                break;
            }
        case node_t::node_type_t::block_t:
            {
                //スコープ
                auto temp=(node_t::block_t*)ast;
                for(int i=0;i<temp->i_size;i++){
                    run(temp->data[i]);
                }
                break;
            }
            case node_t::node_type_t::op_t:
            {
            }
        }
    }

}

