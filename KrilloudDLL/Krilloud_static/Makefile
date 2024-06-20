
# INCLUDES = SoloudAudioSource.h AudioEngineSource.h soloud_wav.h KrilloudClasses.h
OBJS = AudioContainer.o KrilloudClass.o KrilloudLevel.o KrilloudMixer.o KrilloudSelector.o KrilloudVar.o Macro.o Output.o AudioEngineFilter.o
OBJS += KrilloudDebug.o KrilloudRT.o Main.o

SRCS = SoloudAPI.cpp SoloudAudioSource.cpp SoloudFilter.cpp
CLASSES_PATH = Classes/
GPP = g++ -std=c++17 -O2 -m64
GPP17 = g++ -std=c++17 -O2 -m64

AUDIOENGINE_FRAMEWORK_PATH = -I../includeKrilloud/AudioEngineFramework
AUDIOENGINE_PATH = -I../includeKrilloud/AudioEngines
KRILLOUD_PATH = -I../includeKrilloud
INCLUDE = -I../include
SOLOUD_PATH = -I../../soloud/include
BOOST_PATH = -I../include/boost
CEREAL_PATH = -I../include/cereal
INCLUDES = $(INCLUDE) $(SOLOUD_PATH) $(AUDIOENGINE_FRAMEWORK_PATH) $(AUDIOENGINE_PATH) $(KRILLOUD_PATH) $(BOOST_PATH) $(CEREAL_PATH)



all: libKrilloud_static.a
	echo 'fin'

AudioContainer.o:
	$(GPP) ./$(CLASSES_PATH)AudioContainer.cpp $(INCLUDES) -c -o AudioContainer.o

KrilloudClass.o:
	$(GPP) ./$(CLASSES_PATH)KrilloudClass.cpp $(INCLUDES) -c -o KrilloudClass.o

KrilloudLevel.o:
	$(GPP) ./$(CLASSES_PATH)KrilloudLevel.cpp $(INCLUDES) -c -o KrilloudLevel.o

KrilloudMixer.o:
	$(GPP) ./$(CLASSES_PATH)KrilloudMixer.cpp $(INCLUDES) -c -o KrilloudMixer.o

KrilloudSelector.o:
	$(GPP) ./$(CLASSES_PATH)KrilloudSelector.cpp $(INCLUDES) -c -o KrilloudSelector.o

KrilloudVar.o:
	$(GPP) ./$(CLASSES_PATH)KrilloudVar.cpp $(INCLUDES) -c -o KrilloudVar.o

Macro.o:
	$(GPP) ./$(CLASSES_PATH)Macro.cpp $(INCLUDES) -c -o Macro.o

Output.o:
	$(GPP) ./$(CLASSES_PATH)Output.cpp $(INCLUDES) -c -o Output.o
# Aquí acaban los de la carpeta Classes--------------------------------------------
AudioEngineFilter.o:
	$(GPP) ./AudioEngineFilter.cpp $(INCLUDES) -c -o AudioEngineFilter.o

KrilloudDebug.o:
	$(GPP) ./KrilloudDebug.cpp $(INCLUDES) -c -o KrilloudDebug.o


KrilloudRT.o:
	$(GPP) ./KrilloudRT.cpp $(INCLUDES) -c -o KrilloudRT.o

Main.o:
	$(GPP) ./Main.cpp $(INCLUDES) -c -o Main.o

libKrilloud_static.a: $(OBJS) ../EngineAPI_Soloud/libEngineAPI_Soloud.a
	ar rcs libKrilloud_static.a  ../EngineAPI_Soloud/libEngineAPI_Soloud.a $(OBJS)

clean:
	-rm *.o
	-rm *.a