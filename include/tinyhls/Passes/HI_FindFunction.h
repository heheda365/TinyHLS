#ifndef _HI_FINDFUNCTION
#define _HI_FINDFUNCTION

#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <string>
#include <sstream>
#include <set>
#include <map>

class HI_FindFunctions : public llvm::ModulePass {

public:
    HI_FindFunctions() : ModulePass(ID) {
        Function_Demangle = new llvm::raw_fd_ostream("Function_Demangle", ErrInfo);
        Function_Demangle_Map.clear();
    } // define a pass, which can be inherited from ModulePass, LoopPass, FunctionPass and etc.
    ~HI_FindFunctions() {
        Function_Demangle->flush();
        delete Function_Demangle;
    }
    virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;
    virtual bool runOnModule(llvm::Module &M) override;
    virtual bool doInitialization(llvm::Module &M) override {
        Function_Checked.clear();
        return false;
    }
    static char ID;
    std::string DemangleFunctionName(std::string mangled_name)
    {
        std::string demangled_name;

        // demangle the function
        if (mangled_name.find("_Z") == std::string::npos)
            demangled_name = mangled_name;
        else {
            std::stringstream iss(mangled_name);
            iss.ignore(1, '_');
            iss.ignore(1, 'Z');
            int len;
            iss >> len;
            while (len--)
            {
                char tc;
                iss >> tc;
                demangled_name += tc;
            }
        }
        return demangled_name;
    }

    std::set<llvm::Function *> Function_Checked;
    std::error_code ErrInfo;
    llvm::raw_ostream *Function_Demangle;
    std::map<std::string, std::string> Function_Demangle_Map;

    /// Timer

    struct timeval tv_begin;
    struct timeval tv_end;
};

#endif
