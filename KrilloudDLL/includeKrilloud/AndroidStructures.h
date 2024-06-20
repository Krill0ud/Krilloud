#ifndef ANDROID_STRUCTURES_H
#define ANDROID_STRUCTURES_H
#include <string>
#include <sstream>

#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

class AndroidFile
{
public:
	AndroidFile(AAssetManager* AssetManager, const char* FileName)
	{
		Asset_ = AAssetManager_open(AssetManager, FileName, AASSET_MODE_UNKNOWN);
		ok = (Asset_ != nullptr);
		Position_ = 0;
	}

	virtual ~AndroidFile()
	{
		if (Asset_)
		{
			AAsset_close(Asset_);
		}
	}

	int eof()
	{
		return Position_ >= Length();
	}

	unsigned int Read(void *aDst, unsigned int aBytes)
	{
		AAsset_seek(Asset_, Position_, SEEK_SET);
		AAsset_read(Asset_, aDst, aBytes);
		Position_ += aBytes;
		return aBytes;
	}

	std::string ReadCompletely()
	{
		std::stringstream ss;

		long size = Length();
		char * charResult = (char*)malloc(sizeof(char)*size);

		AAsset_seek(Asset_, 0, SEEK_SET);
		AAsset_read(Asset_, charResult, size);
		ss << charResult;
		return ss.str();
	}

	unsigned int Length()
	{
		return static_cast<unsigned int>(AAsset_getLength(Asset_));
	}

	void Seek(int aOffset)
	{
		Position_ = aOffset;
	}

	unsigned int GetCurrentPos()
	{
		return Position_;
	}

	bool EverythingOK()
	{
		return ok;
	}

	virtual FILE * getFilePtr() { return 0; }
	virtual unsigned char * getMemPtr() { return 0; }

private:
	AAsset* Asset_;
	unsigned int Position_;
	bool ok;
};

#endif 