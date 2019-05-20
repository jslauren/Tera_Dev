#include "stdafx.h"
#include "..\Headers\FileManager.h"


CFileManager::CFileManager()
{
}

std::wstring CFileManager::ConvertRelativePath(wstring szFullPath)
{
	TCHAR		szRelativePath[MAX_PATH] = L"";
	TCHAR		szDirPath[MAX_PATH] = L"";

	//// ���� ������ �ִ� ���丮 ��θ� ������ �Լ� ////
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
		iContinue = Find.FindNextFile(); // ���� ������ �ִ��� ���� �Ǵ� �Լ�

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
			PathCombine(szPath, szPath, (LPWSTR)wstrTextureName.c_str()); // �����̸�.png->�����̸�%d.png 
																		  //�����̸��� ������ ��� + �����̸�%d.png
			pImgPath->wstrPath = ConvertRelativePath(szPath);

			// ������Ʈ Ű ����
			PathRemoveFileSpec(szPath);
			pImgPath->wstrStateKey = PathFindFileName(szPath); // �� ��ġ�� �ִ� ������ �̸� ����

															   // obj Ű ����
			PathRemoveFileSpec(szPath);
			pImgPath->wstrObjKey = PathFindFileName(szPath); // �� ��ġ�� �ִ� ������ �̸� ����

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
