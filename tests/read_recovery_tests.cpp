#include "read_recovery.hpp"
#include <cassert>
#include <iostream>
int main(){
 crimson_nav::ReadRecovery recovery;
 assert(!recovery.needs_discovery(100));
 recovery.failed(100);
 assert(!recovery.needs_discovery(101));
 assert(recovery.needs_discovery(2100));
 recovery.discovery_attempted(2100);
 assert(!recovery.needs_discovery(2200));
 assert(recovery.needs_discovery(32100));
 recovery.succeeded();
 assert(!recovery.needs_discovery(40000));
 recovery.failed(41000);
 assert(recovery.needs_discovery(43000));
 std::cout<<"PASS: transient recovery, persistent failure, scan backoff, recovery reset\n";
}
