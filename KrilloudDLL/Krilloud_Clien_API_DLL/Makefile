
KRILL_FILTERS = ./KrillFilters/
SOLOUD_PATH = -I../../soloud/include
# KRILL_FILTERS_PATH = -I./KrillFilters
AUDIOENGINE_FRAMEWORK_PATH = -I../includeKrilloud/AudioEngineFramework
AUDIOENGINE_PATH = -I../includeKrilloud/AudioEngines
KRILLOUD_PATH = -I../includeKrilloud
BOOST_PATH = -I../include/boost
CEREAL_PATH = -I../include/cereal
INCLUDE = -I../include
INCLUDES = $(INCLUDE) $(SOLOUD_PATH) $(AUDIOENGINE_FRAMEWORK_PATH) $(AUDIOENGINE_PATH) $(KRILLOUD_PATH) $(BOOST_PATH) $(CEREAL_PATH)

LIB_SOLOUD = -L../../soloud/contrib -l soloud
LIB_ENGINE = -L../EngineAPI_Soloud -l EngineAPI_Soloud
LIB_STATIC = -L../Krilloud_static -l Krilloud_static
LIB_API_STATIC = -L../Krilloud_Client_API_static -l Krilloud_API_static

WIN_SOCK = -L"ws2_32" -lws2_32 -lwsock32
WIN_MM = -lwinmm -lksuser -luuid
# LIB_FILESYSTEM = -static-libstdc++ -lstdc++fs -static-libgcc
LIB_FILESYSTEM = -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -lstdc++fs

GPP = g++ -fPIC -std=c++17 -O2 -m64

LIB = $(LIB_API_STATIC) $(LIB_STATIC) $(LIB_ENGINE) $(LIB_SOLOUD) $(WIN_SOCK) $(WIN_MM) $(LIB_FILESYSTEM)

all: Krilloud_API.dll
	echo "Fin de la compilación de Krilloud_API.dll"

Main.o: Main.cpp
	$(GPP) Main.cpp $(INCLUDES) -c -o KrilloudAPI_DLL.o

Krilloud_API.dll: Main.o ../Krilloud_Client_API_static/libKrilloud_API_static.a
	$(GPP) -shared -O2 $(INCLUDES) KrilloudAPI_DLL.o $(LIB) -o Krilloud_API.dll -lole32

clean:
	-rm *.o
	-rm *.dll
