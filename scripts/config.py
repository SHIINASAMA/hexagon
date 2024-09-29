import os
import subprocess
import sys
from pathlib import Path

vcpkg_root: Path
cwd = Path(__file__).parent.parent
if "VCPKG_ROOT" not in os.environ:
    print("Downloading and initializing vcpkg...")
    subprocess.run(
        ["git", "submodule", "update", "--init", "--recursive", "--depth=1"],
        cwd=cwd,
        check=True,
    )
    vcpkg_root = cwd / "vcpkg"
    if sys.platform == "win32":
        if not os.path.exists(".\\vcpkg\\vcpkg.exe"):
            subprocess.run(
                [".\\vcpkg\\bootstrap-vcpkg.bat", "-disableMetrics"],
                cwd=cwd,
                check=True,
            )
    else:
        if not os.path.exists("./vcpkg/vcpkg"):
            subprocess.run(
                ["./vcpkg/bootstrap-vcpkg.sh", "-disableMetrics"], cwd=cwd, check=True
            )
else:
    print("Using existing vcpkg...")
    vcpkg_root = Path(os.environ["VCPKG_ROOT"])

subprocess.run(
    [
        "cmake",
        "-S",
        ".",
        "-B",
        "build",
        "-G",
        "Ninja",
        "-DCMAKE_BUILD_TYPE=Debug",
        "-DCMAKE_TOOLCHAIN_FILE={}/scripts/buildsystems/vcpkg.cmake".format(vcpkg_root)
    ],
    cwd=cwd,
    check=True,
)
