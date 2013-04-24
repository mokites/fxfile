//
// Copyright (c) 2013 Leon Lee author. All rights reserved.
//
//   homepage: http://www.flychk.com
//   e-mail:   mailto:flychk@flychk.com
//
// Use of this source code is governed by a GPLv3 license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "shell_item.h"
#include "pidl_win.h"

namespace fxfile
{
namespace base
{
static const xpr_wchar_t kSpecialFolderMyDocumentsGuidString[] = XPR_WIDE_STRING_LITERAL("::{450d8fba-ad25-11d0-98a8-0800361b1103}");
static const xpr_wchar_t kSpecialFolderLibariesGuidString   [] = XPR_WIDE_STRING_LITERAL("::{031E4825-7B94-4DC3-B131-E946B44C8DD5}");

ShellItem::ShellItem(void)
    : mShellFolder(XPR_NULL)
    , mPidl(XPR_NULL)
{
}

ShellItem::ShellItem(ShellItem &aShellItem)
    : mShellFolder(XPR_NULL)
    , mPidl(XPR_NULL)
{
    setShellItem(aShellItem);
}

ShellItem::ShellItem(xpr_sint_t aSpecialFolder)
    : mShellFolder(XPR_NULL)
    , mPidl(XPR_NULL)
{
    setSpecialFolder(aSpecialFolder);
}

ShellItem::ShellItem(const xpr_tchar_t *aFilePath)
    : mShellFolder(XPR_NULL)
    , mPidl(XPR_NULL)
{
    setFilePath(aFilePath);
}

ShellItem::~ShellItem(void)
{
    reset();
}

void ShellItem::reset(void)
{
    COM_RELEASE(mShellFolder);
    COM_FREE(mPidl);
}

xpr_bool_t ShellItem::setShellItem(const ShellItem &aShellItem)
{
    LPSHELLFOLDER sShellFolder = XPR_NULL;
    LPITEMIDLIST  sPidl        = XPR_NULL;

    if (XPR_IS_NOT_NULL(aShellItem.mShellFolder))
    {
        sShellFolder = aShellItem.mShellFolder;
    }

    if (XPR_IS_NOT_NULL(aShellItem.mPidl))
    {
        sPidl = Pidl::clone(aShellItem.mPidl);
        if (XPR_IS_NULL(sPidl))
        {
            return XPR_FALSE;
        }
    }

    if (XPR_IS_NOT_NULL(aShellItem.mShellFolder))
    {
        sShellFolder->AddRef();
    }

    reset();

    mShellFolder = sShellFolder;
    mPidl        = sPidl;

    return XPR_TRUE;
}

xpr_bool_t ShellItem::setFilePath(const xpr_tchar_t *aPath)
{
    LPITEMIDLIST sFullPidl = Pidl::create(aPath);
    if (XPR_IS_NULL(sFullPidl))
    {
        return XPR_FALSE;
    }

    reset();

    mShellFolder = XPR_NULL;
    mPidl        = sFullPidl;

    return XPR_TRUE;
}

xpr_bool_t ShellItem::setSpecialFolder(xpr_sint_t aSpecialFolder)
{
    LPITEMIDLIST sFullPidl = XPR_NULL;

    if (aSpecialFolder > 0xffff)
    {
        aSpecialFolder -= 0xffff;

        HRESULT sHResult = ::SHGetSpecialFolderLocation(XPR_NULL, aSpecialFolder, &sFullPidl);
        if (FAILED(sHResult) || XPR_IS_NULL(sFullPidl))
        {
            COM_FREE(sFullPidl);
        }
    }
    else
    {
        switch (aSpecialFolder)
        {
        case SpecialFolderMyDocuments:
        case SpecialFolderLibraries:
            {
                const xpr_wchar_t *sSpecialFolderGuidString = XPR_NULL;

                switch (aSpecialFolder)
                {
                case SpecialFolderMyDocuments: sSpecialFolderGuidString = kSpecialFolderMyDocumentsGuidString; break;
                case SpecialFolderLibraries:   sSpecialFolderGuidString = kSpecialFolderLibariesGuidString;    break;

                default:
                    XPR_ASSERT(0);
                    break;
                }

                HRESULT sHResult;
                LPSHELLFOLDER sShellFolder = XPR_NULL;

                sHResult = ::SHGetDesktopFolder(&sShellFolder);
                if (SUCCEEDED(sHResult) && XPR_IS_NOT_NULL(sShellFolder))
                {
                    sHResult = sShellFolder->ParseDisplayName(
                        XPR_NULL,
                        XPR_NULL,
                        (xpr_wchar_t *)sSpecialFolderGuidString,
                        XPR_NULL,
                        &sFullPidl,
                        XPR_NULL);
                }
                else
                {
                    COM_RELEASE(sShellFolder);
                }

                if (FAILED(sHResult))
                {
                    COM_FREE(sFullPidl);
                }

                COM_RELEASE(sShellFolder);
                break;
            }
        }
    }

    if (XPR_IS_NULL(sFullPidl))
    {
        return XPR_FALSE;
    }

    reset();

    mShellFolder = XPR_NULL;
    mPidl        = sFullPidl;

    return XPR_TRUE;
}
} // namespace base
} // namespace fxfile