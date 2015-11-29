#ifndef FILEINFO_H
#define FILEINFO_H

#ifndef LFN_LENGTH
#define LFN_LENGTH 250
#endif

//used to store information about files when they are first listed
//http://forum.arduino.cc/index.php?topic=173562.0
struct FileInfo {
	uint32_t index;
	//pointer to an array to be created at runtime
	char fileName[LFN_LENGTH] = {};
};

bool operator==(const FileInfo &left, const FileInfo &right)
{
	for (uint i = 0; i < sizeof(left.fileName)/sizeof(*(left.fileName)); ++i)
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
	for (uint i = 0; i < sizeof(left.fileName)/sizeof(*(left.fileName)); ++i)
	{
		if(tolower(left.fileName[i]) > tolower(right.fileName[i]))
			return false;
		else if(tolower(left.fileName[i]) < tolower(right.fileName[i]))
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