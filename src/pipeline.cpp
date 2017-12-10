#include <vector>
#include <ios>
#include <fstream>

#include "pipeline.h"

namespace {
    std::vector<char> loadShader(const std::string& filename) {
        std::ifstream ifs(filename, std::ios::binary);
        std::vector<char> buffer{
                std::istreambuf_iterator<char>(ifs),
                std::istreambuf_iterator<char>()
        };

        return buffer;
    }
}

void pipeline::create() {
    auto vertShader = loadShader("../spv/vert.spv");
    auto fragShader = loadShader("../spv/frag.spv");
}

void pipeline::destroy() {

}
