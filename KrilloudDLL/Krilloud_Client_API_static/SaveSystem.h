//#pragma once
#include "NodeInfoStructures.h"
#include "KrilloudSerialized.h"
//#include <windows.h>
//#include <tchar.h>
//#include <stdlib.h>
//#include <string.h>
//#include <strsafe.h>
//#include <commdlg.h>
//#include "stdafx.h"
#include <ctime>
#include <fstream>

void Save(const char *fileLocation, KrilloudProjectInfo info);
void Save_As(const char *fileLocation, std::string file_name, KrilloudProjectInfo info);
KrilloudProjectInfo Load(std::string fileLocation);