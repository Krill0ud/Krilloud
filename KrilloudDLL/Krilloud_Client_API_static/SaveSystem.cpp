#include "SaveSystem.h"

void Save(const char *fileLocation, KrilloudProjectInfo info) {

	char path[250];
	strcpy_s(path, fileLocation);
	//strcat_s(path, "/info.kpj");
	strcat_s(path, "/");
	strcat_s(path, info.project_name.c_str());
	strcat_s(path, ".kpj");

	try
	{
		std::ofstream outf(path, std::ios_base::out | std::ios::binary);
		cereal::BinaryOutputArchive oarchive(outf);
		oarchive(info);
		outf.close();
	}
	catch (...)
	{

	}
}

void Save_As(const char *fileLocation, std::string file_name, KrilloudProjectInfo info) {

	char path[250];
	strcpy_s(path, fileLocation);
	//strcat_s(path, "/info.kpj");
	strcat_s(path, "/");
	strcat_s(path, file_name.c_str());
	strcat_s(path, ".kpj");

	try
	{
		std::ofstream outf(path, std::ios_base::out | std::ios::binary);
		cereal::BinaryOutputArchive oarchive(outf);
		oarchive(info);
		outf.close();
	}
	catch (...)
	{

	}
}

KrilloudProjectInfo Load(std::string fileLocation) {

	//char path[250];
	//strcpy_s(path, fileLocation);
	//strcat_s(path, "/info.kpj");
	KrilloudProjectInfo info;

	try
	{
		std::ifstream inf(fileLocation, std::ios_base::in | std::ios::binary);
		cereal::BinaryInputArchive iarchive(inf);
		iarchive(info);
		inf.close();
	}
	catch (...)
	{

	}

	return (info);
}