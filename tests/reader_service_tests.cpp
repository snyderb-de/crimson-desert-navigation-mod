#include "reader_service.hpp"
#include <iostream>
int main() {
    crimson_nav::ReaderService service;
    service.start();
    for(int i=0;i<200;++i){if(service.snapshot().status.find("Map layout unavailable")!=std::string::npos)break;Sleep(10);}
    auto rejected=service.snapshot();
    if(rejected.valid||rejected.status.find("Map layout unavailable")==std::string::npos)return 1;
    for(int i=0;i<10;++i){service.start();service.stop();if(service.snapshot().valid)return 2;}
    std::cout<<"PASS: unsupported host rejected, 10 worker start/stop cycles\n";
}
