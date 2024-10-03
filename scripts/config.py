import os
import subprocess
import sys
import tarfile
import urllib.request
import json
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

generate_preset = {
    "name": "python_generate_preset",
    "binaryDir": "build",
    "toolchainFile": str(vcpkg_root / "scripts/buildsystems/vcpkg.cmake"),
    "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release",
    }
}

if os.path.exists(cwd / "CMakeUserPresets.json"):
    print("Using existing CMakeUserPresets.json...")
    with open(cwd / "CMakeUserPresets.json", "r") as file:
        preset = json.load(file)
        preset["configurePresets"].append(generate_preset)
    with open(cwd / "CMakeUserPresets.json", "w") as file:
        json.dump(preset, file)
else:
    print("Generating CMakeUserPresets.json...")
    with open(cwd / "CMakeUserPresets.json", "w") as file:
        json.dump({"version":6, "configurePresets": {"python_generate_preset": generate_preset}}, file)
