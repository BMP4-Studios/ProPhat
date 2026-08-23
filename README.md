# Pro**Phat**

[![](https://github.com/vberthiaume/ProPhat/actions/workflows/build_and_test.yml/badge.svg)](https://github.com/vberthiaume/ProPhat/actions)

A **phat** virtual synthesizer inspired by the Prophet REV2!

![image](https://github.com/vberthiaume/ProPhat/assets/3721265/09299357-186f-4edf-92af-c5df1645bcc9)

<!-- 
## Build:
- Configure Visual Studio project on Windows: `cmake -B Builds`, then open the resulting project in Visual Studio.
- Configure Xcode project on Mac: `cmake -B Builds -G Xcode`, then open the resulting `Builds/ProPhat.xcodeproj` project in Xcode.
- With Visual Studio Code:
    - open the folder in Visual Studio Code
    - make sure your CMake extension is configured to use the `Builds` folder (`cmd/ctrl + ,`, then set `Cmake: Build Directory` to `${workspaceFolder}/Builds`)
    - cmd/ctrl + shift + P, then `CMake: Configure`
    - F7 to build
    - F5 to run
- With Visual Studio Code on Ubuntu 24.04:
    - same as for mac right above, but make sure to install all dependencies like this: `sudo apt-get update && sudo apt install libasound2-dev libx11-dev libxinerama-dev libxext-dev libfreetype6-dev libwebkit2gtk-4.1-dev libglu1-mesa-dev xvfb ninja-build ladspa-sdk libcurl4-openssl-dev libxcomposite-dev libxcursor-dev libxrandr-dev mesa-common-dev libjack-dev sccache`
-->

## Install dependencies
### macOS
```bash
brew install cmake ninja clang-format          # Homebrew: https://brew.sh
```

## clang-tidy
Install LLVM and clang-tidy:
- Open your Terminal and install the full LLVM package using Homebrew: `brew install llvm`
- Restart your terminal or reload your shell profile to apply changes
- Configure it with cmake: `cmake -B ./Builds -S . -DCMAKE_EXPORT_COMPILE_COMMANDS=ON`
- Run it: `/opt/homebrew/Cellar/llvm/21.1.4/bin/run-clang-tidy -p ./Builds`

### Linux (Ubuntu / Debian)
```bash
sudo apt update
sudo apt install -y \
  cmake ninja-build clang clang-format lld \
  libasound2-dev libx11-dev libxinerama-dev libxext-dev \
  libfreetype6-dev libwebkit2gtk-4.1-dev libglu1-mesa-dev
```

### Windows
- **[CMake](https://cmake.org/download/)** (add to PATH during install).
- **[Ninja](https://github.com/ninja-build/ninja/releases)** on PATH (or `choco install ninja`).

## Install the pre-commit hook
One-time, per clone. Refuses commits whose staged C/C++ files aren't clang-format clean (see `.githooks/pre-commit`):

```bash
git config core.hooksPath .githooks
```

## Build (and run tests)
```bash
cmake -B Builds -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build Builds
ctest --test-dir Builds --output-on-failure
```

For a universal macOS binary, add `-DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"` to the configure step.

## Run RTSan locally (macOS)
CI runs RealtimeSanitizer on Linux. To check locally on macOS, install Homebrew LLVM — Apple Clang doesn't ship the RTSan runtime:
```bash
brew install llvm
```

Configure a separate build dir using brew's clang and the realtime flags:
```bash
CC=/opt/homebrew/opt/llvm/bin/clang \
CXX=/opt/homebrew/opt/llvm/bin/clang++ \
cmake -B Builds-rtsan -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_FLAGS="-fsanitize=realtime" \
  -DCMAKE_CXX_FLAGS="-fsanitize=realtime" \
  -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=realtime"
```

Build and run:
```bash
cmake --build Builds-rtsan --target Tests
ctest --test-dir Builds-rtsan --output-on-failure --verbose -E NOT_BUILT
```

## CI
Every push and PR triggers:
- `build_and_test` — Linux/macOS/Windows, `pluginval` validation, artifact upload
- `sanitizers` — ASan / UBSan / TSan / RTSan (clang-20 for the latter)
- `clang-tidy` — posts review comments on PRs

`nightly.yml` runs the same fan-out daily at 10:00 UTC, to catch external drift (JUCE submodule on `develop`, apt packages, GitHub runner images) between commits. Disable by commenting out the `schedule:` block in that file if you don't want the daily runs.

## Releasing
Releases are tag-driven. The `release` job in `build_and_test.yml` is gated on `contains(github.ref, 'tags/v')` and uses `softprops/action-gh-release` to publish build artifacts (`.exe`, `.zip`, `.pkg`) as a GitHub prerelease.

If your repo blocks direct pushes to `main`, do the version bump through a PR; otherwise you can push directly.

To cut a release:
1. On a release branch, update the `VERSION` file in the repo root (e.g. `0.1.0`).
2. Open a PR against `main` and get it merged (or push directly if your repo allows it).
3. Pull the merged commit locally, then tag it with a `v`-prefixed tag matching the version and push the tag:
   ```bash
   git checkout main
   git pull
   git tag v0.1.0
   git push origin v0.1.0
   ```
4. CI runs the full matrix on the tag, then the `release` job picks up the artifacts and publishes a draft prerelease on GitHub. Open the Releases page, fill in the description, flip the prerelease flag off if it's a real release, and publish.

The `v` prefix is required; a bare `0.1.0` tag won't trigger the release job.

## License
Starty is released under the [GNU Affero General Public License, version 3](LICENSE) (AGPLv3). Copyright (C) 2026 Vincent Berthiaume.

This project links against [JUCE](https://juce.com/), used under the AGPLv3 free-use option of JUCE Ltd's dual-license terms.

### Third-party attribution
Portions of the build system and project scaffolding derive from the [Pamplejuce](https://github.com/sudara/pamplejuce) template, which is distributed under the MIT License:

> MIT License
>
> Copyright (c) 2022 Sudara Williams
>
> Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
