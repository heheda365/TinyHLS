#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/raw_ostream.h"
#include "tinyhls/Basic/Version.h"
#include "tinyhls/Utils/HI_print.h"
int main(int argc_, const char **argv_) {
    llvm::errs() << "hello llvm !\n";
    llvm::outs() << "AAAA !\n"; 
    llvm::outs() << "bbb" << tinyhls::getTinyHLSVersion() << "\n";


    return 0;
}