import os
import subprocess
import sys
import tarfile
import urllib.request
from pathlib import Path

vcpkg_root: Path
cwd = Path(__file__).parent.parent
if "VCPKG_ROOT" not in os.environ:
    if not os.path.exists(cwd / "vcpkg"):
        print("Downloading and initializing vcpkg...")
        version = "2024.09.23"
        url = "https://github.com/microsoft/vcpkg/archive/refs/tags/{}.tar.gz".format(version)
        output = cwd / "vcpkg.tar.gz"
        dest = cwd
        urllib.request.urlretrieve(url, output)
        with tarfile.open(output, "r") as tar:
            tar.extractall(dest)
        os.rename(cwd / "vcpkg-{}".format(version), cwd / "vcpkg")
        os.remove(output)

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
