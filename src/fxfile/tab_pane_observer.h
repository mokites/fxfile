//
// Copyright (c) 2013 Leon Lee author. All rights reserved.
//
//   homepage: http://www.flychk.com
//   e-mail:   mailto:flychk@flychk.com
//
// Use of this source code is governed by a GPLv3 license that can be
// found in the LICENSE file.

#ifndef __FXFILE_TAB_PANE_OBSERVER_H__
#define __FXFILE_TAB_PANE_OBSERVER_H__ 1
#pragma once

namespace fxfile
{
class TabPane;

class TabPaneObserver
{
public:
    virtual void onCreated(TabPane &aTabPane) = 0;
    virtual void onDestroyed(TabPane &aTabPane) = 0;
    virtual void onExplored(TabPane &aTabPane, xpr_uint_t aId, LPITEMIDLIST aFullPidl, xpr_bool_t aUpdateBuddy) = 0;
    virtual xpr_bool_t onOpenFolder(TabPane &aTabPane, const xpr_tchar_t *aDir, const xpr_tchar_t *aSelPath) = 0;
    virtual xpr_bool_t onOpenFolder(TabPane &aTabPane, LPCITEMIDLIST aFullPidl) = 0;
    virtual void onSetFocus(TabPane &aTabPane) = 0;
    virtual void onMoveFocus(TabPane &aTabPane, xpr_sint_t aCurWnd) = 0;
};
} // namespace fxfile

#endif // __FXFILE_TAB_PANE_OBSERVER_H__
