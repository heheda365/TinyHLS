#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/raw_ostream.h"
#include <system_error>
#include "tinyhls/Basic/Version.h"
#include "tinyhls/Utils/HI_print.h"
int main(int argc_, const char **argv_) {
    llvm::errs() << "hello llvm !\n";
    llvm::outs() << "AAAA !\n"; 
    llvm::outs() << "bbb" << tinyhls::getTinyHLSVersion() << "\n";
    
    // llvm::raw_ostream ofstream = 
    std::error_code errorCode;
    llvm::raw_fd_ostream ofstream("test.txt", errorCode);

    ofstream << "hello world! \n";


    return 0;
}