#pragma once
//#include "stdafx.h"
//#include "Shobjidl.h"
#include <sstream>
//#include <windows.h>

//#include <direct.h>

//#include <shellapi.h>
#include <vector>
#include <fstream>
#include <locale>
#include <sys/stat.h>
//#include <codecvt>
//#include <tchar.h>
#include "NodeInfoStructures.h"
#include "KrilloudAPIStructures.h"
class AudioPool 
{
public:
	// --- File data variables
	//struct pool_file_data
	//{
	//	int id;
	//	//size_t id;
	//	std::string file_name;
	//};
	std::vector<pool_file_data> files_data;
	std::vector<pool_file_data> folders_data;
	int last_given_id = -1;

	/*struct directory_system
	{
		std::string folder_name;
		std::vector<std::string> files;
		std::vector<directory_system*> directories;
	};*/
	directory_system* directories;

	// --- selection variables
	int tree_node_selected = -1;
	std::string selected_item_name;
	bool selected_item_is_file = false;
	pool_file_data* selected_item_file_data;
	directory_system* selected_folder_data;

	bool show_tree_popup = false;
	int open_modal_popup = -1;

	bool window_accepted = false;
	bool window_closed = false;

	AudioPool(std::string project_path/*, std::vector<std::string???> * pool_list*/);

	
	void RefreshTree();

	// --- File data functions
	void AddFileData(std::string file_name);
	int FindInFileData(std::string file_name);
	int FindInFileData(int id);
	int GetID(std::string file_name);
	pool_file_data* GetFileData(std::string file_name);
	pool_file_data* GetFileData(int id);
	pool_file_data* GetFileDataByIndex(int index);
	pool_file_data* GetFolderDataByIndex(int index);
	size_t GetFileDataCount();
	size_t GetFolderDataCount();
	void DeleteFileData(std::string file_name);
	void DeleteFileData(int id);
	void RenameFileData(int id, std::string new_file_name);

	directory_system* GetDirectorySystem();

	// --- File functions
	void AddFiles(std::vector<std::string> file_paths, std::string target_directory);
	bool DumpFile(char* source_file, char *output_file, bool update_file_data);
	bool MovePoolFile(std::string source_file, std::string directory_name);
	bool RenamePoolFile(std::string file_name, std::string new_file_name);
	bool RemovePoolFile(std::string file_name, bool update_file_data);

	// --- Directory functions
	bool DirectoryExists(std::string path_name);
	bool CreateNewSubdirectory(std::string path_name);
	bool MovePoolDirectory(std::string source_path, std::string output_path);
	bool DeleteDirectory(std::string path_name);

private:
	std::string my_project_path;

	bool window_draw_started = false;
	
	// --- Helper functions
	bool IsAllowedFileExtension(std::string file_extension);
	directory_system* ReadDirectory(std::string name);
	/*std::wstring ConvertStringToWString(const std::string& str);
	std::string ConvertWStringToString(const std::wstring& wstr);*/
};