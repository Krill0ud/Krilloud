OBJECTS = Krill_Delay.o Krill_FFT_Pitch.o SoloudAPI.o SoloudAudioSource.o SoloudFilter.o

KRILL_FILTERS = ./KrillFilters/
SOLOUD_PATH = -I../../soloud/include
KRILL_FILTERS_PATH = -I./KrillFilters
AUDIOENGINE_FRAMEWORK_PATH = -I../includeKrilloud/AudioEngineFramework
AUDIOENGINE_PATH = -I../includeKrilloud/AudioEngines
KRILLOUD_PATH = -I../includeKrilloud
BOOST_PATH = -I../include/boost
CEREAL_PATH = -I../include/cereal
INCLUDE = -I../include
INCLUDES = $(INCLUDE) $(SOLOUD_PATH) $(KRILL_FILTERS_PATH) $(AUDIOENGINE_FRAMEWORK_PATH) $(AUDIOENGINE_PATH) $(KRILLOUD_PATH) $(BOOST_PATH) $(CEREAL_PATH)
GPP = g++ -std=c++17 -O2 -m64

all: libEngineAPI_Soloud.a
	echo "Fin de la compilación de EngineAPI_Soloud.a"

Krill_Delay.o:  $(KRILL_FILTERS)Krill_Delay.cpp
	$(GPP) $(INCLUDES) -c $(KRILL_FILTERS)Krill_Delay.cpp

Krill_FFT_Pitch.o:  $(KRILL_FILTERS)Krill_FFT_Pitch.cpp
	$(GPP) $(INCLUDES) -c $(KRILL_FILTERS)Krill_FFT_Pitch.cpp

SoloudAPI.o: SoloudAPI.cpp
	$(GPP) $(INCLUDES) -c SoloudAPI.cpp

SoloudAudioSource.o: SoloudAudioSource.cpp
	$(GPP) $(INCLUDES) -c SoloudAudioSource.cpp

SoloudFilter.o: SoloudFilter.cpp
	$(GPP) $(INCLUDES) -c SoloudFilter.cpp

libEngineAPI_Soloud.a: ../../soloud/contrib/libsoloud.a $(OBJECTS)
	ar rcs libEngineAPI_Soloud.a $(OBJECTS)

clean:
	-rm *.o
	-rm *.a
