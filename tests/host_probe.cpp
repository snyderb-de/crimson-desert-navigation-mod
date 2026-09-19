// Tests native load and ReShade registration lifecycle; does not invoke GUI callbacks.
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdint>
#include <iostream>
static int registrations=0,events=0,panels=0;
static bool accept=true;
static int table_placeholder=0;
extern "C" __declspec(dllexport) bool ReShadeRegisterAddon(void *,uint32_t version) {
    if(!accept || version!=20) return false;
    ++registrations;return true;
}
extern "C" __declspec(dllexport) void ReShadeUnregisterAddon(void *) {registrations=0;}
extern "C" __declspec(dllexport) const void *ReShadeGetImGuiFunctionTable(uint32_t version) {return version==19250?&table_placeholder:nullptr;}
extern "C" __declspec(dllexport) void ReShadeRegisterEvent(uint32_t,void *) {++events;}
extern "C" __declspec(dllexport) void ReShadeUnregisterEvent(uint32_t,void *) {--events;}
extern "C" __declspec(dllexport) void ReShadeRegisterOverlay(const char *,void *) {++panels;}
extern "C" __declspec(dllexport) void ReShadeUnregisterOverlay(const char *,void *) {--panels;}
extern "C" __declspec(dllexport) void ReShadeLogMessage(void *,int,const char *) {}
int main() {
    auto module=LoadLibraryW(L"outputs/local-test-1.0.7/build/CrimsonNavigationLab.addon64");
    if(!module){std::cerr<<"LoadLibrary error "<<GetLastError()<<'\n';return 1;}
    auto init=reinterpret_cast<bool(*)(HMODULE,HMODULE)>(GetProcAddress(module,"AddonInit"));
    auto uninit=reinterpret_cast<void(*)(HMODULE,HMODULE)>(GetProcAddress(module,"AddonUninit"));
    if(!init||!uninit) return 2;
    auto host=GetModuleHandleW(nullptr);
    accept=false;
    if(init(module,host)||registrations||events||panels) return 3;
    accept=true;
    for(int i=0;i<3;++i) {
        if(!init(module,host)||registrations!=1||events!=1||panels!=1) return 4;
        uninit(module,host);
        if(registrations||events||panels) return 5;
    }
    if(!FreeLibrary(module)) return 6;
    std::cout<<"PASS: rejected host, 3 init/uninit cycles, native unload. GUI and game compatibility untested.\n";
}
