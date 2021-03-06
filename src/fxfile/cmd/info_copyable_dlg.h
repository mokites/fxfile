//
// Copyright (c) 2014 Leon Lee author. All rights reserved.
//
//   homepage: http://www.flychk.com
//   e-mail:   mailto:flychk@flychk.com
//
// Use of this source code is governed by a GPLv3 license that can be
// found in the LICENSE file.

#ifndef __FXFILE_INFO_COPYABLE_DLG_H__
#define __FXFILE_INFO_COPYABLE_DLG_H__ 1
#pragma once

#include "gui/ResizingDialog.h"

namespace fxfile
{
namespace cmd
{
class InfoCopyableDlg : public CResizingDialog
{
    typedef CResizingDialog super;

public:
    InfoCopyableDlg(void);
    virtual ~InfoCopyableDlg(void);

public:
    void setTitle(const xpr_tchar_t *aTitle);
    void setInfo(const xpr_tchar_t *aInfo);
    void setInfo(const xpr::string &aInfo);
    void setInfoFont(const xpr_tchar_t *aFaceName);

protected:
    xpr::string mTitle;
    xpr::string mInfo;
    xpr::string mInfoFontFaceName;

protected:
    HICON mIcon;
    CFont mFont;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
    DECLARE_MESSAGE_MAP()
    virtual xpr_bool_t OnInitDialog(void);
    afx_msg void OnDestroy(void);
    afx_msg void OnCopy(void);
};
} // namespace cmd
} // namespace fxfile

#endif // __FXFILE_INFO_COPYABLE_DLG_H__
