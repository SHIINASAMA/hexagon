import os
from pathlib import Path
import urllib.request

cwd = Path(__file__).parent.parent

os.makedirs(f"{cwd}/AppDir", exist_ok=True)

if 0 != os.system(f"cp -r {cwd}/build/Hexagon {cwd}/AppDir/AppRun"):
    raise Exception("Failed to copy AppRun")

if 0 != os.system(f"cp -r {cwd}/logo.svg {cwd}/AppDir/icon.svg"):
    raise Exception("Failed to copy icon.svg")

if 0 != os.system(f"cp -r {cwd}/tls {cwd}/AppDir/"):
    raise Exception("Failed to copy tls")

if 0 != os.system(f"cp {cwd}/config.yml {cwd}/AppDir/"):
    raise Exception("Failed to copy config.yml")

if 0 != os.system(f"cp -r {cwd}/out/ {cwd}/AppDir/"):
    raise Exception("Failed to copy out")

if not os.path.exists(f"{cwd}/appimagetool"):
    url = "https://github.com/AppImage/appimagetool/releases/download/continuous/appimagetool-x86_64.AppImage"
    output = cwd / "appimagetool"
    urllib.request.urlretrieve(url, output)
    os.chmod(output, 0o755)

name = cwd.name
with os.fdopen(os.open(f"{cwd}/AppDir/.desktop", os.O_WRONLY | os.O_CREAT | os.O_TRUNC), "w") as file:
    if file is None:
        raise Exception("Failed to open .desktop file")
    file.write("[Desktop Entry]\n")
    file.write(f"Name={name}\n")
    file.write("Exec=AppRun\n")
    file.write("Icon=icon\n")
    file.write("Type=Application\n")
    file.write("Categories=Network;\n")

os.system(f"{cwd}/appimagetool {cwd}/AppDir")
