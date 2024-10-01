#include <sese/Init.h>
#include <sese/Log.h>
#include <sese/util/Exception.h>
#include <sese/system/Paths.h>

#include "HexagonApplication.hpp"
#include "HexagonConfig.hpp"

std::unique_ptr<HexagonApplication> application;

void CtrlC() {
    SESE_WARN("Ctrl-C");
    if (application) {

    }
    exit(0);
}

#ifdef _WIN32
#include <Windows.h>

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType) {
    switch (fdwCtrlType) {
        case CTRL_C_EVENT:
        // case CTRL_BREAK_EVENT:
        // case CTRL_CLOSE_EVENT:
        // case CTRL_LOGOFF_EVENT:
        // case CTRL_SHUTDOWN_EVENT:
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

[[noreturn]] int main(int argc, char **argv) {
    sese::initCore(argc, argv);
#ifdef _WIN32
    SetConsoleCtrlHandler(CtrlHandler, TRUE);
#else
    signal(SIGINT, CtrlHandler);
#endif

#ifdef _DEBUG
    std::string base_path = PROJECT_SOURCE_PATH;
#else
    std::string base_path = sese::system::Paths::getExecutablePath().getNativePath();
#endif
    try {
        application = std::make_unique<HexagonApplication>(base_path);
    } catch (sese::Exception &exception) {
        exception.printStacktrace(sese::record::getLogger());
        return -1;
    } catch (std::exception &exception) {
        SESE_ERROR("{}", exception.what());
        return -1;
    }
    while (1) {
        sese::sleep(1s);
    }
}
