MODULES = ./Modules/
FILTERS = ./Modules/Filters/

OBJECTS = AudioSourceNode.o ContainerNode.o FilterNode.o GraphNode.o
OBJECTS += IngameVariable.o InputNode.o MixerNode.o OutputNode.o
OBJECTS += SelectorNode.o VariableSelectorNode.o
OBJECTS += Filter_BandPass.o Filter_Bassboost.o Filter_BQR.o Filter_Dc_Remover.o Filter_Delay.o Filter_Echo.o Filter_Flanger.o Filter_FreeVerb.o
OBJECTS += Filter_HighPass.o Filter_LoFi.o Filter_LowPass.o Filter_Pitch.o Filter_Robotizer.o Filter_Waveshaper.o
OBJECTS += KrilloudAPI.o AudioPool.o KrilloudProjectHandler.o NodeGraphSystemLevel.o SaveSystem.o

SOLOUD_PATH = -I../../soloud/include
THIS_PATH = -I./
MODULES_PATH = -I./Modules
FILTERS_PATH = -I./Modules/Filters
# KRILL_FILTERS_PATH = -I./KrillFilters
# AUDIOENGINE_FRAMEWORK_PATH = -I../../include/Krilloud/AudioEngineFramework
# AUDIOENGINE_PATH = -I../../include/Krilloud/AudioEngines

KRILLOUD_PATH = -I../includeKrilloud
BOOST_PATH = -I../include/boost
CEREAL_PATH = -I../include/cereal
INCLUDE = -I../include
INCLUDES = $(INCLUDE) $(THIS_PATH) $(MODULES_PATH) $(SOLOUD_PATH) $(KRILLOUD_PATH) $(BOOST_PATH) $(CEREAL_PATH) $(FILTERS_PATH)
GPP = g++ -std=c++17 -O2 -m64

all: libKrilloud_API_static.a
	echo "Fin de la compilación de Krilloud_API_static.a"
#Ficheros de la carpeta Modules--------------------------------------------------------------------
AudioSourceNode.o: $(MODULES)AudioSourceNode.cpp
	$(GPP) $(INCLUDES) -c $(MODULES)AudioSourceNode.cpp

ContainerNode.o: $(MODULES)ContainerNode.cpp
	$(GPP) $(INCLUDES) -c $(MODULES)ContainerNode.cpp

FilterNode.o: $(MODULES)FilterNode.cpp
	$(GPP) $(INCLUDES) -c $(MODULES)FilterNode.cpp

GraphNode.o: $(MODULES)GraphNode.cpp
	$(GPP) $(INCLUDES) -c $(MODULES)GraphNode.cpp

IngameVariable.o: $(MODULES)IngameVariable.cpp
	$(GPP) $(INCLUDES) -c $(MODULES)IngameVariable.cpp

InputNode.o: $(MODULES)InputNode.cpp
	$(GPP) $(INCLUDES) -c $(MODULES)InputNode.cpp

MixerNode.o: $(MODULES)MixerNode.cpp
	$(GPP) $(INCLUDES) -c $(MODULES)MixerNode.cpp

OutputNode.o: $(MODULES)OutputNode.cpp
	$(GPP) $(INCLUDES) -c $(MODULES)OutputNode.cpp

SelectorNode.o: $(MODULES)SelectorNode.cpp
	$(GPP) $(INCLUDES) -c $(MODULES)SelectorNode.cpp

VariableSelectorNode.o: $(MODULES)VariableSelectorNode.cpp
	$(GPP) $(INCLUDES) -c $(MODULES)VariableSelectorNode.cpp

#Ficheros de la carpeta filtros-------------------------------------------
Filter_BandPass.o: $(FILTERS)Filter_BandPass.cpp
	$(GPP) $(INCLUDES) -c $(FILTERS)Filter_BandPass.cpp

Filter_Bassboost.o: $(FILTERS)Filter_Bassboost.cpp
	$(GPP) $(INCLUDES) -c $(FILTERS)Filter_Bassboost.cpp

Filter_BQR.o: $(FILTERS)Filter_BQR.cpp
	$(GPP) $(INCLUDES) -c $(FILTERS)Filter_BQR.cpp

Filter_Dc_Remover.o: $(FILTERS)Filter_Dc_Remover.cpp
	$(GPP) $(INCLUDES) -c $(FILTERS)Filter_Dc_Remover.cpp

Filter_Delay.o: $(FILTERS)Filter_Delay.cpp
	$(GPP) $(INCLUDES) -c $(FILTERS)Filter_Delay.cpp

Filter_Echo.o: $(FILTERS)Filter_Echo.cpp
	$(GPP) $(INCLUDES) -c $(FILTERS)Filter_Echo.cpp

Filter_Flanger.o: $(FILTERS)Filter_Flanger.cpp
	$(GPP) $(INCLUDES) -c $(FILTERS)Filter_Flanger.cpp

Filter_FreeVerb.o: $(FILTERS)Filter_FreeVerb.cpp
	$(GPP) $(INCLUDES) -c $(FILTERS)Filter_FreeVerb.cpp

Filter_HighPass.o: $(FILTERS)Filter_HighPass.cpp
	$(GPP) $(INCLUDES) -c $(FILTERS)Filter_HighPass.cpp

Filter_LoFi.o: $(FILTERS)Filter_LoFi.cpp
	$(GPP) $(INCLUDES) -c $(FILTERS)Filter_LoFi.cpp

Filter_LowPass.o: $(FILTERS)Filter_LowPass.cpp
	$(GPP) $(INCLUDES) -c $(FILTERS)Filter_LowPass.cpp

Filter_Pitch.o: $(FILTERS)Filter_Pitch.cpp
	$(GPP) $(INCLUDES) -c $(FILTERS)Filter_Pitch.cpp

Filter_Robotizer.o: $(FILTERS)Filter_Robotizer.cpp
	$(GPP) $(INCLUDES) -c $(FILTERS)Filter_Robotizer.cpp

Filter_Waveshaper.o: $(FILTERS)Filter_Waveshaper.cpp
	$(GPP) $(INCLUDES) -c $(FILTERS)Filter_Waveshaper.cpp
#Ficheros de la carpeta filtros-------------------------------------------
#Ficheros de la carpeta Modules--------------------------------------------------------------------
AudioPool.o:  AudioPool.cpp
	$(GPP) $(INCLUDES) -c AudioPool.cpp

KrilloudAPI.o: KrilloudAPI.cpp
	$(GPP) $(INCLUDES) -c KrilloudAPI.cpp

KrilloudProjectHandler.o: KrilloudProjectHandler.cpp
	$(GPP) $(INCLUDES) -c KrilloudProjectHandler.cpp

NodeGraphSystemLevel.o: NodeGraphSystemLevel.cpp
	$(GPP) $(INCLUDES) -c NodeGraphSystemLevel.cpp

SaveSystem.o: SaveSystem.cpp
	$(GPP) $(INCLUDES) -c SaveSystem.cpp

libKrilloud_API_static.a: $(OBJECTS) ../Krilloud_static/libKrilloud_static.a
	ar rcs libKrilloud_API_static.a  $(OBJECTS) ../Krilloud_static/libKrilloud_static.a
clean:
	-rm *.o
	-rm *.a