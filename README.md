# Krilloud
## _Make it sound_
[![Krilloud](https://krilloud.com/_nuxt/img/Krilloud_logo_RGB.d4f36bb.png)](https://krilloud.com/)

[Download](https://github.com/Krill0ud/Krilloud/releases/download/1.0.0/Krilloud_Release_v_1.0.0.zip)
## Requirements
Windows:
- [MSYS2](https://www.msys2.org/)
- [python 3.10](https://www.python.org/downloads/release/python-3100) (Windows installer 64-bit) and add it to PATH
- [nvm](https://github.com/coreybutler/nvm-windows/releases)
    - Download .exe, run it and then run 'nvm install 14' and 'nvm use 14' on windows cmd to install node. Write 'node -v' on cmd to check that installation has gone well.
    - To install Visual Studio tools run `npm install -g windows-build-tools@4.0.0` on **cmd executed as administrator**.
    
## Quick Start
DLL compilation will be done in the **mingw64 console** and client compilation will be done in the **Windows cmd**.
1. Download MSYS2 package at the link provided above. We advise to use mingw64 console inside this package to compile DLL.
    You should install git and make in the **mingw64 environment**. You can use pacman installer for git and make installation:
    `pacman -S git make`
2. Run MSYS2's mingw64 enviroment and clone this repository:
    `git clone <this_url>`
3. Open repository path and run `make deps`.  This will install all Krilloud dependencies before the build so you dont have to install anything else.
After this step, you should check if installation goes right you can type `make check-deps`
4. Execute make. This will run `make` in chain to compile Soloud at first and KrilloudDLL then. At the end, Krilloud_API_x64.dll will been created at KrilloudDLL/src/Krilloud_API_dynamic and Make will copy and paste it into krilloud-client/public/lib to use it at client.
5. Open krilloud-client directory. Copy `.env.example` into `.env` file.
6. Run `npm install` on **Windows cmd**.
7. Run `npm run electron` on Windows cmd to work on development.
    After doing this, electron will open an app window with Krilloud Client.
8. Run `npm run build` to get Krilloud executable. **Make it sound!**

> Note: If you use a code editor like Visual Studio Code with an own console (and with dependencies shared with system cmd) you can use it for client compilation (steps 6 and 7)


## Tipical Errors

## Useful information
- **Audio backends**: Krilloud currently is build for WINMM, WASAPI and XAUDIO2. If you want to build for other backend, Soloud provides many options which you can configure at 'soloud/contrib/Configure.cmake'.
- **Client enviroment**: If you want to restart client enviroment, you only should remove node-modules and .quasar directories and run 'npm install' again.

Contact with us <https://krilloud.com/>.

