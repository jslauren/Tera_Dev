#pragma once
class CFileManager
{
public:
	explicit CFileManager();
	virtual ~CFileManager() = default;

public:
	std::wstring	ConvertRelativePath(wstring szFullPath);
	void			DirInfoExraction(const wstring& wstrPath, list<IMGPATH*>& rPathInfoList);
	int				DirFileCount(const wstring& wstrPath);

};

