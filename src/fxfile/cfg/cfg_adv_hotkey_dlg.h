//
// Copyright (c) 2001-2013 Leon Lee author. All rights reserved.
//
//   homepage: http://www.flychk.com
//   e-mail:   mailto:flychk@flychk.com
//
// Use of this source code is governed by a GPLv3 license that can be
// found in the LICENSE file.

#ifndef __FXFILE_CFG_ADV_HOTKEY_DLG_H__
#define __FXFILE_CFG_ADV_HOTKEY_DLG_H__ 1
#pragma once

#include "cfg_dlg.h"

namespace fxfile
{
namespace cfg
{
class CfgAdvHotKeyDlg : public CfgDlg
{
    typedef CfgDlg super;

public:
    CfgAdvHotKeyDlg(void);

protected:
    virtual void onInit(const Option::Config &aConfig);
    virtual void onApply(Option::Config &aConfig);

protected:
    CHotKeyCtrl mHotKeyCtrl;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual xpr_bool_t OnCommand(WPARAM wParam, LPARAM lParam);

protected:
    DECLARE_MESSAGE_MAP()
    virtual xpr_bool_t OnInitDialog(void);
};
} // namespace cfg
} // namespace fxfile

#endif // __FXFILE_CFG_ADV_HOTKEY_DLG_H__
