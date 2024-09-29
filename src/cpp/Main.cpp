#include <sese/Init.h>
#include <sese/Log.h>
#include <sese/io/File.h>
#include <sese/util/Exception.h>

#include "HexagonApplication.hpp"
#include "HexagonConfig.hpp"

#ifdef _WIN32
#include <Windows.h>

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType) {
    switch (fdwCtrlType) {
        case CTRL_C_EVENT:
            SESE_WARN("Ctrl-C");
            return TRUE;
        default:
            return FALSE;
    }
}
#endif

int main(int argc, char **argv) {
#ifdef _WIN32
    SetConsoleCtrlHandler((PHANDLER_ROUTINE) CtrlHandler, TRUE);
#endif
    sese::initCore(argc, argv);
    auto file = sese::io::File::create(PROJECT_SOURCE_PATH "/config.yml",
                                       sese::io::File::T_READ);
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
