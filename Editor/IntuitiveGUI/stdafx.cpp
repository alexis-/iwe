// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// stdafx.cpp : source file that includes just the standard includes
// IntuitiveEditor.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include <string>
#include <map>
#include "stdafx.h"

#include <algorithm>
#include <sstream>
#include <string>
#include <cctype>
#include <locale>

const char*
getUniqueName(const std::string& name)
{
	static std::map<std::string, unsigned long>		nameMap;
	static char																		pNameRet[2048];
	unsigned long																	id = 0;

	if (nameMap.find(name) != nameMap.end())
	{
		id = ++nameMap[name];
		nameMap[name] = id;
	}
	else
		nameMap[name] = id;

  std::stringstream stream;

  stream << name;
	stream << ".";
  stream << id;

	strncpy(pNameRet, stream.str().c_str(), 2048);

	return pNameRet;
}

std::wstring s2ws(const std::string& s)
{
 int len;
 int slength = (int)s.length() + 1;
 len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
 wchar_t* buf = NEW wchar_t[len];
 MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
 std::wstring r(buf);
 delete[] buf;
 return r;
}

bool
listContains(const std::list<std::string>& list, std::string name)
{
	for (std::list<std::string>::const_iterator it = list.begin();
			 it != list.end();
			 it++)
		if (*it == name)
			return true;

	return false;
}

std::string upper_string(const std::string& str)
{
  std::string upper;

  std::transform(str.begin(), str.end(), std::back_inserter(upper), std::toupper);
  return str;
}

bool hasCIEnding(std::string const &fullString, std::string const &ending)
{
	std::string newFullString = upper_string(fullString);
	std::string newEnding = upper_string(ending);
  unsigned int lastMatchPos = newFullString.rfind(newEnding); // Find the last occurrence of ending
  bool isEnding = lastMatchPos != std::string::npos; // Make sure it's found at least once

  // If the string was found, make sure that any characters that follow it are the ones we're trying to ignore
  for(int i = lastMatchPos + newEnding.length(); (i < newFullString.length()) && isEnding; i++)
  {
    if((newFullString[i] != '\n') &&
        (newFullString[i] != '\r') )
    {
      isEnding = false;
    }
  }

  return isEnding;
}

bool hasEnding(std::string const &fullString, std::string const &ending)
{
  unsigned int lastMatchPos = fullString.rfind(ending); // Find the last occurrence of ending
  bool isEnding = lastMatchPos != std::string::npos; // Make sure it's found at least once

  // If the string was found, make sure that any characters that follow it are the ones we're trying to ignore
  for(int i = lastMatchPos + ending.length(); (i < fullString.length()) && isEnding; i++)
  {
    if((fullString[i] != '\n') &&
        (fullString[i] != '\r') )
    {
      isEnding = false;
    }
  }

  return isEnding;
}

CImage*
getOgreBitmap(const std::string& sName, const std::string& sGroupName)
{
  uint width, height;
  uchar bpp;

  void* pData = GetResourceMgr()->getBitmapMaterial(sName, sGroupName, width, height, bpp);

  //if (CEngineUtils::getBitmapMaterial(*it, "EditorAlphaBrushes", *img, 50, 50))
  if (pData)
  {
    CImage* img = NEW CImage();

    img->Create(width, height, bpp);

    void* pImgData = img->GetBits();
    memcpy(pImgData, pData, width * height * bpp / 8);

    delete pData;

    return img;
  }

  return NULL;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

BOOL
checkPathExists(LPCTSTR szPath)
{
	return (GetFileAttributes(szPath) != INVALID_FILE_ATTRIBUTES);
}


BOOL
isDirectory(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
	        (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

BOOL CSynchronizer::Wait(DWORD dwTimeOut)
{
    if(CSingleLock::IsLocked())
    {
        CSingleLock::Unlock();
    }
    return CSingleLock::Lock(dwTimeOut);
}

BOOL CSynchronizer::Notify()
{
    if(CSingleLock::IsLocked())
    {
        return CSingleLock::Unlock();
    }
    return TRUE;
}

void SetThreadName(DWORD dwThreadID, const char* threadName)
{
   THREADNAME_INFO info;
   info.dwType = 0x1000;
   info.szName = threadName;
   info.dwThreadID = dwThreadID;
   info.dwFlags = 0;

   __try
   {
      RaiseException(MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
   }
   __except(EXCEPTION_EXECUTE_HANDLER)
   {
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void ResizeCBitmap(CBitmap& bmpIn, CBitmap& bmpOut, int newWidth, int newHeight)
{
  // Original BMP
  BITMAP bOri;
  bmpIn.GetBitmap(&bOri);

  // Create DCs
  CDC ScrnDC, dc1, dc2;

  ScrnDC.CreateIC(_T("DISPLAY"), NULL, NULL, NULL);
  dc1.CreateCompatibleDC(&ScrnDC);
  dc2.CreateCompatibleDC(&ScrnDC);

  bmpOut.CreateCompatibleBitmap(&ScrnDC, newWidth, newHeight);

  ScrnDC.DeleteDC();

  // Do the work
  HGDIOBJ pOldBmp1 = dc1.SelectObject(bmpIn),
          pOldBmp2 = dc2.SelectObject(bmpOut);

  dc2.SetStretchBltMode(HALFTONE);
  dc2.StretchBlt(0, 0, newWidth, newHeight, &dc1, 0, 0, bOri.bmWidth, bOri.bmHeight, SRCCOPY);
 
  // Unbind
  dc1.SelectObject(pOldBmp1);
  dc2.SelectObject(pOldBmp2);
}

HICON HICONFromCBitmap(CBitmap& bitmap)
{
   BITMAP bmp;
   bitmap.GetBitmap(&bmp);
   
   HBITMAP hbmMask = ::CreateCompatibleBitmap(::GetDC(NULL), 
                                              bmp.bmWidth, bmp.bmHeight);

   ICONINFO ii = {0};
   ii.fIcon    = TRUE;
   ii.hbmColor = bitmap;
   ii.hbmMask  = hbmMask;

   HICON hIcon = ::CreateIconIndirect(&ii);
   ::DeleteObject(hbmMask);

   return hIcon;
}

HICON HICONFromData(const s_IWEImageData& imgData)
{
  CBitmap bmp;

  bmp.CreateBitmap(imgData.width, imgData.height, 1, imgData.bpp, imgData.pData);

  return HICONFromCBitmap(bmp);
}

HICON HICONFromData(const s_IWEImageData& imgData, int newWidth, int newHeight)
{
  CBitmap bmpOri;
  CBitmap bmpOut;

  bmpOri.CreateBitmap(imgData.width, imgData.height, 1, imgData.bpp, imgData.pData);
  ResizeCBitmap(bmpOri, bmpOut, newWidth, newHeight);

  return HICONFromCBitmap(bmpOut);
}

void CBitmapFromData(CBitmap& bmpOut, const s_IWEImageData& imgData)
{
  bmpOut.CreateBitmap(imgData.width, imgData.height, 1, imgData.bpp, imgData.pData);
}

void CBitmapFromData(CBitmap& bmpOut, const s_IWEImageData& imgData, int newWidth, int newHeight)
{
  CBitmap bmpOri;

  bmpOri.CreateBitmap(imgData.width, imgData.height, 1, imgData.bpp, imgData.pData);
  ResizeCBitmap(bmpOri, bmpOut, newWidth, newHeight);
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int SearchDirectory(std::vector<std::string> &refvecFiles,
                    const std::string        &refcstrRootDirectory,
                    const std::string        &refcstrExtension,
                    bool                     bSearchSubdirectories)
{
  std::string     strFilePath;             // Filepath
  std::string     strPattern;              // Pattern
  std::string     strExtension;            // Extension
  HANDLE          hFile;                   // Handle to file
  WIN32_FIND_DATA FileInformation;         // File information


  strPattern = refcstrRootDirectory + "\\*.*";

  hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
  if(hFile != INVALID_HANDLE_VALUE)
  {
    do
    {
      if(FileInformation.cFileName[0] != '.')
      {
        strFilePath.erase();
        strFilePath = refcstrRootDirectory + "\\" + FileInformation.cFileName;

        if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
          if(bSearchSubdirectories)
          {
            // Search subdirectory
            int iRC = SearchDirectory(refvecFiles,
                                      strFilePath,
                                      refcstrExtension,
                                      bSearchSubdirectories);
            if(iRC)
              return iRC;
          }
        }
        else
        {
          // Check extension
          strExtension = FileInformation.cFileName;
          strExtension = strExtension.substr(strExtension.rfind(".") + 1);

          if(strExtension == refcstrExtension)
          {
            // Save filename
            refvecFiles.push_back(strFilePath);
          }
        }
      }
    } while(::FindNextFile(hFile, &FileInformation) == TRUE);

    // Close handle
    ::FindClose(hFile);

    DWORD dwError = ::GetLastError();
    if(dwError != ERROR_NO_MORE_FILES)
      return dwError;
  }

  return 0;
}
