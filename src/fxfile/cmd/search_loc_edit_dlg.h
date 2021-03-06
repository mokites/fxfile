//
// Copyright (c) 2001-2012 Leon Lee author. All rights reserved.
//
//   homepage: http://www.flychk.com
//   e-mail:   mailto:flychk@flychk.com
//
// Use of this source code is governed by a GPLv3 license that can be
// found in the LICENSE file.

#ifndef __FXFILE_SEARCH_LOC_EDIT_DLG_H__
#define __FXFILE_SEARCH_LOC_EDIT_DLG_H__ 1
#pragma once

#include "search.h"
#include "resource.h"

namespace fxfile
{
namespace cmd
{
class SearchLocEditDlg : public CDialog
{
    typedef CDialog super;

public:
    SearchLocEditDlg(void);
    virtual ~SearchLocEditDlg(void);

public:
    const xpr_tchar_t *getPath(void) const;
    xpr_bool_t isInclude(void) const;
    xpr_bool_t isSubFolder(void) const;

    void setPath(const xpr_tchar_t *aPath);
    void setPath(LPITEMIDLIST aFullPidl);
    void setInclude(xpr_bool_t aInclude);
    void setSubFolder(xpr_bool_t aSubFolder);
    void setPathSet(SearchUserLocPathSet *aSearchUserLocPathSet);

protected:
    xpr::string           mPath;
    xpr_bool_t            mInclude;
    xpr_bool_t            mSubFolder;
    SearchUserLocPathSet *mSearchUserLocPathSet;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
    DECLARE_MESSAGE_MAP()
    virtual xpr_bool_t OnInitDialog(void);
    virtual void OnOK(void);
    afx_msg void OnIncludeExclude(void);
    afx_msg void OnPathBrowse(void);
};
} // namespace cmd
} // namespace fxfile

#endif // __FXFILE_SEARCH_LOC_EDIT_DLG_H__
