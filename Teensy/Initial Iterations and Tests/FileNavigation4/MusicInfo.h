#include "FileInfo.h"

struct MusicInfo
{
	FileInfo info;

	//associated files for searching the other collections of files
	char associatedFiles[][];
};