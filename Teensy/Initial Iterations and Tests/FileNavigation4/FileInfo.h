#ifndef FILEINFO_H
#define FILEINFO_H

//used to store information about files when they are first listed
//http://forum.arduino.cc/index.php?topic=173562.0
struct FileInfo {
	//pointer to an array to be created at runtime
	char* fileName;
	char** relatedFiles;
};

bool operator==(const FileInfo &left, const FileInfo &right)
{
	if(strlen(left.fileName) != strlen(right.fileName))
	{
		return false;
	}
	for (uint i = 0; i < strlen(left.fileName) && i < strlen(right.fileName); ++i)
	{
		if(tolower(left.fileName[i]) != tolower(right.fileName[i]))
			return false;
	}
	return true;
}

bool operator!=(const FileInfo &left, const FileInfo &right)
{
	return !(left==right);
}

bool operator<(const FileInfo &left, const FileInfo &right)
{
	for (uint i = 0; i < strlen(left.fileName) && i < strlen(right.fileName); ++i)
	{
		if(tolower(left.fileName[i]) > tolower(right.fileName[i]))
			return false;
		else if(tolower(left.fileName[i]) < tolower(right.fileName[i]))
			return true;
	}
	if(strlen(left.fileName) < strlen(right.fileName))
	{
		return true;
	}
	return false;
}

bool operator>(const FileInfo &left, const FileInfo &right)
{
	return right<left;
}

bool operator<=(const FileInfo &left, const FileInfo &right)
{
	return !(right<left);
}

bool operator>=(const FileInfo &left, const FileInfo &right)
{
	return !(left<right);
}

#endif