#include "stdafx.h"
#include "FileInfo.h"

CFileInfo::CFileInfo()
{
}

wstring CFileInfo::ConvertRelativePath(wstring szFullPath)
{
	TCHAR		szRelativePath[MAX_PATH] = L"";
	TCHAR		szDirPath[MAX_PATH] = L"";
	GetCurrentDirectory(sizeof(szDirPath), szDirPath);
	PathRelativePathTo((LPWSTR)szRelativePath,
		szDirPath, FILE_ATTRIBUTE_DIRECTORY,
		szFullPath.c_str(), FILE_ATTRIBUTE_NORMAL);

	return szRelativePath;
}

void CFileInfo::DirInfoExraction(const wstring & wstrPath, list<IMGPATH*>& rPathInfoList)
{
	CFileFind finder;
	BOOL bWorking = finder.FindFile(wstrPath.c_str());
	if (!bWorking)
	{
		AfxMessageBox(L"No Direc");
		return;
	}
	int		iContinue = 1;

	while (iContinue)
	{
		iContinue = finder.FindNextFile(); // ���� ������ �ִ��� ���� �Ǵ� �Լ�

		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
		{
			DirInfoExraction(wstring(finder.GetFilePath() + L"\\*.*"), rPathInfoList);
		}
		else
		{
			if (finder.IsSystem())
				continue;

			IMGPATH*		pImgPath = new IMGPATH;
			TCHAR			szPath[MAX_PATH] = L"";

			lstrcpy(szPath, finder.GetFilePath().operator LPCWSTR());
			PathRemoveFileSpec(szPath);

			pImgPath->iCount = DirFileCount(szPath);

			wstring	wstrTextureName = finder.GetFileTitle().operator LPCWSTR();
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.dds";

			lstrcpy(szPath, (LPWSTR)finder.GetFilePath().operator LPCWSTR());

			PathRemoveFileSpec(szPath);
			PathCombine(szPath, szPath, (LPWSTR)wstrTextureName.c_str());

			pImgPath->wstrPath = ConvertRelativePath(szPath);

			// ������Ʈ Ű ����
			PathRemoveFileSpec(szPath);
			pImgPath->wstrStateKey = PathFindFileName(szPath); // �� ��ġ�� �ִ� ������ �̸� ����

													   // obj Ű ����
			PathRemoveFileSpec(szPath);
			pImgPath->wstrObjKey = PathFindFileName(szPath); // �� ��ġ�� �ִ� ������ �̸� ����

			rPathInfoList.push_back(pImgPath);

			finder.FindFile(finder.GetFilePath());
			finder.FindNextFile();
		}
	}
}

int CFileInfo::DirFileCount(const wstring & wstrPath)
{
	wstring		wstrTemp = wstrPath + L"\\*.dds";

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

