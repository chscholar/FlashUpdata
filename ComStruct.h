#ifndef COMSTRUCT_H
#define COMSTRUCT_H

struct TransInterFace
{
	char Header[4];
	char Length[2];
	char Commandp[4];
	char BinFileId[4];
	char BinFileSize[4];
	char TransId[4];
	char TransSeqNum[4];
	char DataLength[4];
	char DataCRC[4];
	char Data;
	char Padding[1];
};

#endif
