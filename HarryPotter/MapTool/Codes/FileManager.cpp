#include "stdafx.h"
#include "..\Headers\FileManager.h"


CFileManager::CFileManager()
{
}

std::wstring CFileManager::ConvertRelativePath(wstring szFullPath)
{
	TCHAR		szRelativePath[MAX_PATH] = L"";
	TCHAR		szDirPath[MAX_PATH] = L"";

	//// 현재 파일이 있는 디렉토리 경로를 얻어오는 함수 ////
	GetCurrentDirectory(sizeof(szDirPath), szDirPath);

	PathRelativePathTo((LPWSTR)szRelativePath,
		szDirPath, FILE_ATTRIBUTE_DIRECTORY,
		szFullPath.c_str(), FILE_ATTRIBUTE_NORMAL);

	return szRelativePath;
}

void CFileManager::DirInfoExraction(const wstring & wstrPath, list<IMGPATH*>& rPathInfoList)
{
	CFileFind		Find;
	Find.FindFile(wstrPath.c_str());

	int		iContinue = 1;

	while (iContinue)
	{
		iContinue = Find.FindNextFile(); // 다음 파일이 있는지 유무 판단 함수

		if (Find.IsDots())
			continue;

		if (Find.IsDirectory())
			DirInfoExraction(wstring(Find.GetFilePath() + L"\\*.*"), rPathInfoList);
		else
		{
			if (Find.IsSystem())
				continue;

			IMGPATH*		pImgPath = new IMGPATH;
			TCHAR			szPath[MAX_PATH] = L"";

			lstrcpy(szPath, Find.GetFilePath().operator LPCWSTR());
			PathRemoveFileSpec(szPath);

			pImgPath->iCount = DirFileCount(szPath);

			wstring	wstrTextureName = Find.GetFileTitle().operator LPCWSTR();
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";

			lstrcpy(szPath, (LPWSTR)Find.GetFilePath().operator LPCWSTR());

			PathRemoveFileSpec(szPath);
			PathCombine(szPath, szPath, (LPWSTR)wstrTextureName.c_str()); // 파일이름.png->파일이름%d.png 
																		  //파일이름을 제거한 경로 + 파일이름%d.png
			pImgPath->wstrPath = ConvertRelativePath(szPath);

			// 스테이트 키 추출
			PathRemoveFileSpec(szPath);
			pImgPath->wstrStateKey = PathFindFileName(szPath); // 현 위치에 있는 파일의 이름 추출

															   // obj 키 추출
			PathRemoveFileSpec(szPath);
			pImgPath->wstrObjKey = PathFindFileName(szPath); // 현 위치에 있는 파일의 이름 추출

			rPathInfoList.push_back(pImgPath);

			Find.FindFile(Find.GetFilePath());
			Find.FindNextFile();
		}
	}
}

int CFileManager::DirFileCount(const wstring & wstrPath)
{
	wstring		wstrTemp = wstrPath + L"\\*.png";

	CFileFind	Find;
	Find.FindFile(wstrTemp.c_str());

	int		iContinue = 1;
	int		iFileCount = 0;

	while (iContinue)
	{
		iContinue = Find.FindNextFile();

		if (!Find.IsDirectory())
			++iFileCount;
	}

	return iFileCount;
}
