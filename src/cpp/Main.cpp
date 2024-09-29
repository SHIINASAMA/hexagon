#include <sese/Init.h>
#include <sese/Log.h>
#include <sese/io/File.h>
#include <sese/util/Exception.h>

#include "HexagonApplication.hpp"
#include "HexagonConfig.hpp"

void CtrlC() {
    SESE_WARN("Ctrl-C");
    exit(0);
}

#ifdef _WIN32
#include <Windows.h>

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType) {
    switch (fdwCtrlType) {
        case CTRL_C_EVENT:
            CtrlC();
            return TRUE;
        default:
            return FALSE;
    }
}
#else
void CtrlHandler(int sig) {
    CtrlC();
}
#endif

int main(int argc, char **argv) {
    sese::initCore(argc, argv);
#ifdef _WIN32
    SetConsoleCtrlHandler((PHANDLER_ROUTINE) CtrlHandler, TRUE);
#else
    signal(SIGINT, CtrlHandler);
#endif
    sese::sleep(20s);

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
