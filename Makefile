default:
	make soloud
	make krilloud
	make install

deps:
	pacman -S mingw-w64-x86_64-gcc --noconfirm
	pacman -S cmake --noconfirm

check-deps:
	cmake --version && gcc --version

.PHONY: soloud
soloud:
	-# --- BUILD: Soloud ---
	(cd soloud/contrib/ && cmake . -G 'Unix Makefiles')
	make -C soloud/contrib/

.PHONY: krilloud
krilloud:
	-# --- BUILD: Krilloud ---
	make -C KrilloudDLL/

install:
	 cp KrilloudDLL/Krilloud_Clien_API_DLL/Krilloud_API.dll KrilloudDLL/Krilloud_Clien_API_DLL/Krilloud_API_x64.dll
	 mkdir -p krilloud-client/public/lib
	 cp KrilloudDLL/Krilloud_Clien_API_DLL/Krilloud_API.dll krilloud-client/public/lib/Krilloud_API_x64.dll

clean:
	-# --- CLEAN Everything ---
	-(cd soloud/contrib/ && rm CMakeCache.txt Makefile cmake_install.cmake libsoloud.a )
	-(cd soloud/contrib/ && rm -R CMakeFiles/ )
	-make -C soloud/contrib/ clean
	-make -C KrilloudDLL/ clean
