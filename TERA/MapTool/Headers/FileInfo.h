#pragma once
#include "Tool_Defines.h"

class CFileInfo
{
public:
	explicit CFileInfo();
	virtual ~CFileInfo() = default;

public:
	wstring	ConvertRelativePath(wstring szFullPath);
	void DirInfoExraction(const wstring& wstrPath, list<IMGPATH*>& rPathInfoList); // ���� ������ �����ϴ� �뵵�� �Լ�
	int	 DirFileCount(const wstring& wstrPath); // ���� �ȿ� �̹����� �� �� �ִ��� ����ϴ� �Լ�

};
