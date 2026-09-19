#include "worldmap_roads.hpp"
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

namespace {
void write_archive(const std::filesystem::path &root,std::uint32_t layers,const char *first="worldmaproad") {
    std::vector<unsigned char> bytes(3207193);
    std::size_t p=0;
    const auto put_u32=[&](std::uint32_t value) {
        std::memcpy(bytes.data()+p,&value,sizeof(value));p+=sizeof(value);
    };
    const auto put_text=[&](const char *value) {
        const auto length=std::uint32_t(std::strlen(value));put_u32(length);
        std::memcpy(bytes.data()+p,value,length);p+=length;
    };
    put_u32(46);p+=16;put_u32(layers);put_text(first);
    put_u32(0); // worldmaproad link count
    put_u32(0); // worldmaproad edge count
    put_u32(0); // worldmaproad node count
    put_text("wagon");
    const auto directory=root/"0008";std::filesystem::create_directories(directory);
    std::ofstream output(directory/"1.paz",std::ios::binary);
    output.write(reinterpret_cast<const char *>(bytes.data()),bytes.size());
}
bool loads(const std::filesystem::path &root) {
    try {crimson_nav::load_worldmap_roads(root);return true;}catch(...) {return false;}
}
}

int main() {
    int checks=0,failures=0;
    const auto check=[&](bool ok,const char *name) {
        ++checks;if(!ok){++failures;std::cerr<<"FAIL: "<<name<<'\n';}
    };
    const auto base=std::filesystem::path("work/build/worldmap-roads-fixtures");
    write_archive(base/"three",3);check(loads(base/"three"),"legacy three-section archive accepted");
    write_archive(base/"four",4);check(loads(base/"four"),"patch 2.03 four-section archive accepted");
    write_archive(base/"two",2);check(!loads(base/"two"),"unexpected two-section archive rejected");
    write_archive(base/"five",5);check(loads(base/"five"),"additional archive sections do not block the required road layer");
    write_archive(base/"too-many",65);check(!loads(base/"too-many"),"implausible section count rejected");
    write_archive(base/"wrong-name",4,"notworldroad");check(!loads(base/"wrong-name"),"wrong first road layer rejected");
    std::cout<<checks<<" road archive header checks, "<<failures<<" failures\n";
    return failures?1:0;
}
