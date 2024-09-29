#include <sese/Init.h>
#include <sese/Log.h>
#include <sese/io/File.h>
#include <sese/util/Exception.h>

#include "HexagonConfig.hpp"
#include "HexagonApplication.hpp"

int main(int argc, char **argv) {
    sese::initCore(argc, argv);

    auto file = sese::io::File::create(PROJECT_SOURCE_PATH "/config.yml", sese::io::File::T_READ);
    try {
        auto config = std::make_unique<HexagonConfig>(file.get());
    } catch (sese::Exception &exception) {
        exception.printStacktrace(sese::record::getLogger());
        return -1;
    } catch (std::exception &exception) {
        SESE_ERROR("{}", exception.what());
        return -1;
    }
    return 0;
}
