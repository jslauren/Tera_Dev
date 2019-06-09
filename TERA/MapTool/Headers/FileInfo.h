#pragma once
#include "Tool_Defines.h"

class CFileInfo
{
public:
	explicit CFileInfo();
	virtual ~CFileInfo() = default;

public:
	wstring	ConvertRelativePath(wstring szFullPath);
	void DirInfoExraction(const wstring& wstrPath, list<IMGPATH*>& rPathInfoList); // 폴더 정보를 추출하는 용도의 함수
	int	 DirFileCount(const wstring& wstrPath); // 폴더 안에 이미지가 몇 장 있는지 계산하는 함수

};
