#include "AudioPool.h"
//#include <filesystem>
#include <experimental/filesystem>

AudioPool::AudioPool( std::string project_path)
{
	my_project_path = project_path;

	//directories = new directory_system();
	RefreshTree();
}

//Refreshes the entire pool directory info
void AudioPool::RefreshTree()
{
	//directories = new directory_system();
	std::stringstream ss;
	ss << my_project_path << "\\Sound Files";
	files_data.clear();
	folders_data.clear();
	directories = ReadDirectory(ss.str());
}

// --- File functions

//Select multiple files and add them to the pool
void AudioPool::AddFiles(std::vector<std::string> file_paths, std::string target_directory)
{
	for (size_t x = 0; x < file_paths.size(); x++)
	{
		//add file to -> directory_path
		std::size_t found = file_paths[x].find_last_of("/\\");

		std::stringstream ss;
		ss << target_directory << "\\" << file_paths[x].substr(found + 1);


		DumpFile(&file_paths[x][0], &ss.str()[0], true);
	}

	RefreshTree();
}

//Copy a source file to the output path
bool AudioPool::DumpFile(char* source_file, char *output_file, bool update_file_data)
{
	// create binary file with all input wavs
	std::ofstream ofsBinary(output_file, std::ios::binary);

	std::ifstream ifs(source_file, std::ios::binary);
	if (ifs)
	{
		const size_t initialOffset = (size_t)ofsBinary.tellp();

		// get size of file
		ifs.seekg(0, ifs.end);
		long size = long(ifs.tellg());
		ifs.seekg(0);

		// allocate memory for file content
		char* buffer = new char[size];

		// read content of infile
		ifs.read(buffer, size);

		// write to outfile
		ofsBinary.write(buffer, size);
		// TODO: padding (escribir ceros hasta que el offset sea el adecuado para la plataforma)

		ifs.close();

		if (update_file_data)
		{
			AddFileData(output_file);
		}

		return true;
	}

	return false;
}

//Move a file to the given path
bool AudioPool::MovePoolFile(std::string source_file, std::string output_file)
{
	pool_file_data* pool_file_data = GetFileData(source_file);
	int id = -1;

	if (DirectoryExists(output_file))
		return false;
	//struct stat buffer;
	//if (stat(output_file.c_str(), &buffer) == 0)
	//{
	//	//file exists
	//	return false;
	//}

	if (pool_file_data != nullptr)
	{
		id = pool_file_data->id;
	}

	bool done = DumpFile(&source_file[0], &output_file[0], false);

	if (done)
	{
		done = RemovePoolFile(source_file, false);
		//RefreshTree();
		if (done)
		{
			RenameFileData(id, output_file);
		}

		return done;
	}

	return false;
}

//Rename a file
bool AudioPool::RenamePoolFile(std::string file_name, std::string new_file_name)
{
	pool_file_data* pool_file_data = GetFileData(file_name);
	int id = -1;

	if (pool_file_data != nullptr)
	{
		id = pool_file_data->id;
	}

	bool done = rename(file_name.c_str(), new_file_name.c_str()) == 0;
	if (done)
	{
		RenameFileData(id, new_file_name);
		RefreshTree();
	}

	return done;
}

bool AudioPool::RemovePoolFile(std::string file_name, bool update_file_data)
{
	bool done = remove(file_name.c_str()) == 0;
	if (done)
	{
		if (update_file_data)
		{
			DeleteFileData(&file_name[0]);
		}

		RefreshTree();
	}

	return done;
}

// --- Directory functions

//Check if a given directory already exists
//bool AudioPool::DirectoryExists(std::string path_name)
//{
//	auto it = std::experimental::filesystem::directory_iterator(path_name);
//	auto status = it->status();
//	auto result = std::experimental::filesystem::exists(status);
//
//	return result;
//}

bool AudioPool::DirectoryExists(std::string pathname)
{
	struct stat info;

	if (stat(pathname.c_str(), &info) != 0)
		return false;
	else if (info.st_mode & S_IFDIR)
		return true;
	else
		return false; //name exists but it's not a directory
}

//Create a directory
bool AudioPool::CreateNewSubdirectory(std::string path_name) {

	if (DirectoryExists(path_name)) {
		return false;
	}
	else
	{
		//int result = _mkdir(path_name.c_str());
		bool result = std::experimental::filesystem::create_directory(path_name.c_str());

		RefreshTree();
		return result; // == 0;
	}
}

//Move a directory and its content to the output destination
bool AudioPool::MovePoolDirectory(std::string source_path, std::string output_path)
{
	std::stringstream ss;

	if (output_path.find(source_path) != std::string::npos)
	{
		//source_path is substring of output_path
		// Files/A cannot be moved to Files/A/B
		return false;
	}



	if (source_path.find(output_path) == 0)
	{
		//output_path is substring of source_path
		//Check if output_path is not the folder where source_path folder already is
		std::size_t directory_name = source_path.find_last_of("/\\");
		if (output_path == source_path.substr(0, directory_name)) {
			return false;
		}
	}


	ss.clear();
	ss << source_path << "\\";
	size_t result = output_path.find(ss.str());
	bool everything_ok = true;

	if (result != 0)
	{
		std::size_t directory_name_found = source_path.find_last_of("/\\");

		ss.clear();
		ss.str(std::string());
		ss << output_path << "\\" << source_path.substr(directory_name_found + 1);

		CreateNewSubdirectory(ss.str().c_str());

		std::string current_copied_path = ss.str();


		std::string pattern(source_path);
		pattern.append("\\*");

		//------------------
		for (auto& p : std::experimental::filesystem::directory_iterator(source_path))
		{
			if (std::experimental::filesystem::is_directory(p)) 
			{
				ss.clear();
				ss.str(std::string());
				ss << p; // source_path << "\\" << ConvertWStringToString(data.cFileName);

				everything_ok |= MovePoolDirectory(ss.str(), current_copied_path);
			}
			else
			{
				ss.clear();
				ss.str(std::string());
				ss << p;// source_path << "\\" << ConvertWStringToString(data.cFileName);

				int last_slash = ss.str().find_last_of("/\\");
				if (last_slash != 0) 
				{
					std::stringstream ss2;
					ss2 << current_copied_path << "\\" << ss.str().substr(last_slash + 1);

					everything_ok |= MovePoolFile(&ss.str()[0], &ss2.str()[0]);
				}
			}
		}

		//------------------


		/*WIN32_FIND_DATA data;
		HANDLE hFind;

		if ((hFind = FindFirstFile(ConvertStringToWString(pattern.c_str()).c_str(), &data)) != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (!(data.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT))
				{
					if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						if (wcscmp(data.cFileName, L".") != 0 && wcscmp(data.cFileName, L"..") != 0)
						{
							ss.clear();
							ss.str(std::string());
							ss << source_path << "\\" << ConvertWStringToString(data.cFileName);

							everything_ok |= MovePoolDirectory(ss.str(), current_copied_path);
						}
					}
					else
					{
						ss.clear();
						ss.str(std::string());
						ss << source_path << "\\" << ConvertWStringToString(data.cFileName);

						std::stringstream ss2;
						ss2 << current_copied_path << "\\" << ConvertWStringToString(data.cFileName);

						everything_ok |= MovePoolFile(&ss.str()[0], &ss2.str()[0]);
					}
				}
			} while (FindNextFile(hFind, &data) != 0);

			FindClose(hFind);
		}*/
		
		//-------------------------

		if (everything_ok)
		{
			DeleteDirectory(&source_path[0]);
		}
	}

	return everything_ok;
}

bool AudioPool::DeleteDirectory(std::string path_name)
{
	std::string pattern(path_name);
	pattern.append("\\*");

	//------------------
	for (auto& p : std::experimental::filesystem::directory_iterator(path_name))
	{
		if (std::experimental::filesystem::is_directory(p))
		{
			std::stringstream ss;
			ss << p;

			DeleteDirectory(ss.str().c_str());
		}
		else
		{
			std::stringstream ss;
			ss << p;// source_path << "\\" << ConvertWStringToString(data.cFileName);

			int last_slash = ss.str().find_last_of("/\\");
			if (last_slash != 0)
			{
				std::string file_name = ss.str().substr(last_slash + 1);
				std::size_t found = file_name.find_last_of(".");
				//if (file_name.substr(found + 1) == "ogg" || file_name.substr(found + 1) == "wav")
				if (IsAllowedFileExtension(file_name.substr(found + 1)))
				{
					std::stringstream ss;
					ss << p;

					RemovePoolFile(&ss.str()[0], true);
				}
			}			
		}
	}

	//------------------
	//WIN32_FIND_DATA data;
	//HANDLE hFind;

	//if ((hFind = FindFirstFile(ConvertStringToWString(pattern.c_str()).c_str(), &data)) != INVALID_HANDLE_VALUE)
	//{
	//	do
	//	{
	//		if (!(data.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT))
	//		{
	//			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	//			{
	//				if (wcscmp(data.cFileName, L".") != 0 && wcscmp(data.cFileName, L"..") != 0)
	//				{
	//					std::stringstream ss;
	//					ss << path_name << "\\" << ConvertWStringToString(data.cFileName);

	//					DeleteDirectory(ss.str().c_str());
	//				}
	//			}
	//			else
	//			{
	//				std::string file_name = ConvertWStringToString(data.cFileName);
	//				std::size_t found = file_name.find_last_of(".");
	//				//if (file_name.substr(found + 1) == "ogg" || file_name.substr(found + 1) == "wav")
	//				if (IsAllowedFileExtension(file_name.substr(found + 1)))
	//				{
	//					std::stringstream ss;
	//					ss << path_name << "\\" << ConvertWStringToString(data.cFileName);

	//					RemovePoolFile(&ss.str()[0], true);
	//				}
	//			}
	//		}
	//	} while (FindNextFile(hFind, &data) != 0);

	//	FindClose(hFind);
	//}
	//---------------------

	//bool done = _rmdir(path_name.c_str()) == 0;
	bool done = std::experimental::filesystem::remove(path_name.c_str());


	RefreshTree();
	return done;
}

// --- Helper functions

bool AudioPool::IsAllowedFileExtension(std::string file_extension)
{
	std::string valid_extensions[] = { "ogg", "wav", "mp3", "flac" };
	bool isValid = false;

	size_t mysize = valid_extensions->length();

	for (size_t x = 0; x <= valid_extensions->length(); x++)
	{
		if (file_extension == valid_extensions[x])
		{
			isValid = true;
			break;
		}
	}

	return isValid;
}

// read files and directories from a specified path
directory_system* AudioPool::ReadDirectory(std::string name)
{
	std::string pattern(name);
	pattern.append("\\*");

	//------------------
	directory_system* folders_tree = new directory_system();
	folders_tree->folder_name = name.c_str();

	for (auto& p : std::experimental::filesystem::directory_iterator(name))
	{
		if (std::experimental::filesystem::is_directory(p))
		{
			std::stringstream ss;
			ss << p.path().generic_string();

			directory_system* new_directory = ReadDirectory(ss.str());

			folders_tree->directories.push_back(new_directory);

			pool_file_data pool_folder_data;

			pool_folder_data.file_name = ss.str();
			pool_folder_data.id = -1;
			folders_data.push_back(pool_folder_data);

		}
		else
		{
			std::stringstream ss;
			ss << p.path().generic_string();// source_path << "\\" << ConvertWStringToString(data.cFileName);

			int last_slash = ss.str().find_last_of("/\\");
			if (last_slash != 0)
			{
				std::string file_name = ss.str().substr(last_slash + 1);
				std::size_t found = file_name.find_last_of(".");
				//if (file_name.substr(found + 1) == "ogg" || file_name.substr(found + 1) == "wav")
				if (IsAllowedFileExtension(file_name.substr(found + 1)))
				{
					std::stringstream ss;
					ss << p.path().generic_string();

					folders_tree->files.push_back(ss.str().c_str());
					//files.push_back(ss.str());

					AddFileData(&ss.str()[0]);
				}
			}
		}
	}

	return folders_tree;
}


// --- File data functions
//Get the position of the item with the given file_name
int AudioPool::FindInFileData(std::string file_name)
{
	for (size_t x = 0; x < files_data.size(); x++)
	{
		if (files_data[x].file_name == file_name)
		{
			return (int)x;
		}
	}

	return -1;
}

//Get the position of the item with the given id
int AudioPool::FindInFileData(int id)
{
	for (size_t x = 0; x < files_data.size(); x++)
	{
		if (files_data[x].id == id)
		{
			return (int)x;
		}
	}

	return -1;
}

//Get the ID of the data with the given file name
int AudioPool::GetID(std::string file_name)
{
	pool_file_data * file_data = GetFileData(file_name);

	if (file_data != nullptr)
	{
		return file_data->id;
	}

	return -1;
}

//Returns the pointer of the object containing file_name
pool_file_data* AudioPool::GetFileData(std::string file_name)
{
	int index = FindInFileData(file_name);

	if (index > -1)
	{
		return &files_data[index];
	}
	else {
		return nullptr;
	}
}

//Returns the pointer of the object containing id
pool_file_data* AudioPool::GetFileData(int id)
{
	int index = FindInFileData(id);

	if (index > -1)
	{
		return &files_data[index];
	}
	else {
		return nullptr;
	}
}

//Returns the pointer of the object containing id
pool_file_data* AudioPool::GetFileDataByIndex(int index)
{
	if (index > -1 && index < files_data.size())
	{
		return &files_data[index];
	}
	else {
		return nullptr;
	}
}

pool_file_data* AudioPool::GetFolderDataByIndex(int index)
{
	if (index > -1 && index < folders_data.size())
	{
		return &folders_data[index];
	}
	else {
		return nullptr;
	}
}


size_t AudioPool::GetFileDataCount() 
{
	return files_data.size();
}

size_t AudioPool::GetFolderDataCount()
{
	return folders_data.size();
}


//Adds a new item
void AudioPool::AddFileData(std::string file_name)
{
	int index = FindInFileData(file_name);

	if (index == -1)
	{
		pool_file_data file_data;

		size_t project_name_size = my_project_path.size();
		std::string filename_string(file_name);
		std::string filename = filename_string.substr(project_name_size + 13);

		int file_id = 0;

		for (size_t x = 0; x < filename.size(); x++)
		{
			file_id += (x + 1) * filename[x];
		}

		file_data.id = file_id;
		file_data.file_name = file_name.c_str();

		files_data.push_back(file_data);
	}
	else
	{
		size_t project_name_size = my_project_path.size();
		std::string filename_string(file_name);
		std::string filename = filename_string.substr(project_name_size + 13);

		int file_id = 0;

		for (size_t x = 0; x < filename.size(); x++)
		{
			file_id += (x + 1) * filename[x];
		}

		files_data[index].id = file_id;
	}
}

//Deletes the item containing file_name
void AudioPool::DeleteFileData(std::string file_name)
{
	int index = FindInFileData(file_name);

	if (index != -1)
	{
		files_data.erase(files_data.begin() + index);
	}
}

//Deletes the item containing id
void AudioPool::DeleteFileData(int id)
{
	int index = FindInFileData(id);

	if (index != -1)
	{
		files_data.erase(files_data.begin() + index);
	}
}

//Rename the item stored data
void AudioPool::RenameFileData(int id, std::string new_file_name)
{
	int index = FindInFileData(id);

	if (index != -1)
	{
		files_data[index].file_name = new_file_name.c_str();
	}
}

directory_system* AudioPool::GetDirectorySystem()
{
	return directories;
}