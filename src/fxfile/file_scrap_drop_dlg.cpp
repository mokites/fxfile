//
// Copyright (c) 2001-2012 Leon Lee author. All rights reserved.
//
//   homepage: http://www.flychk.com
//   e-mail:   mailto:flychk@flychk.com
//
// Use of this source code is governed by a GPLv3 license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "file_scrap_drop_dlg.h"

#include "file_scrap.h"
#include "resource.h"
#include "main_frame.h"
#include "explorer_ctrl.h"
#include "dlg_state.h"
#include "dlg_state_manager.h"

#include "command_string_table.h"
#include "cmd/file_scrap_group_dlg.h"

#include "gui/DragImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//----------------------------------------------------------------------
// Transparent
//----------------------------------------------------------------------
#define WS_EX_LAYERED   0x00080000
#define LWA_COLORKEY    0x00000001
#define LWA_ALPHA       0x00000002
#define ULW_COLORKEY    0x00000001
#define ULW_ALPHA       0x00000002
#define ULW_OPAQUE      0x00000004

typedef xpr_bool_t (WINAPI *SetLayeredWindowAttributesFunc)(HWND, COLORREF, BYTE, DWORD);
//----------------------------------------------------------------------

namespace fxfile
{
FileScrapDropDlg::FileScrapDropDlg(void)
    : super(IDD_FILE_SCRAP_DROP, XPR_NULL)
    , mTransparentPercentage(60)
    , mTransparent(XPR_FALSE)
    , mAlwaysTopMost(XPR_FALSE)
    , mDlgState(XPR_NULL)
{
}

void FileScrapDropDlg::DoDataExchange(CDataExchange* pDX)
{
    super::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(FileScrapDropDlg, super)
    ON_WM_LBUTTONDOWN()
    ON_WM_CONTEXTMENU()
    ON_WM_CLOSE()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_RBUTTONDBLCLK()
    ON_WM_INITMENUPOPUP()
    ON_WM_MENUCHAR()
    ON_WM_MEASUREITEM()
    ON_COMMAND(ID_FCD_COPY,            OnFileScrapCopy)
    ON_COMMAND(ID_FCD_MOVE,            OnFileScrapMove)
    ON_COMMAND(ID_FCD_DELETE,          OnFileScrapDelete)
    ON_COMMAND(ID_FCD_VIEW,            OnFileScrapView)
    ON_COMMAND(ID_FCD_REMOVE,          OnFileScrapRemove)
    ON_COMMAND(ID_FCD_CLOSE,           OnFileScrapClose)
    ON_COMMAND(ID_FCD_ALWAYS_TOP_MOST, OnFileScrapAlwaysTopMost)
    ON_COMMAND(ID_FCD_TRANSPARENT_0,   OnFileScrapTransparent0)
    ON_COMMAND(ID_FCD_TRANSPARENT_20,  OnFileScrapTransparent20)
    ON_COMMAND(ID_FCD_TRANSPARENT_40,  OnFileScrapTransparent40)
    ON_COMMAND(ID_FCD_TRANSPARENT_60,  OnFileScrapTransparent60)
    ON_COMMAND(ID_FCD_TRANSPARENT_80,  OnFileScrapTransparent80)
    ON_COMMAND(ID_FCD_CLIPBOARD_CUT,   OnFileScrapClipboardCut)
    ON_COMMAND(ID_FCD_CLIPBOARD_COPY,  OnFileScrapClipboardCopy)
    ON_COMMAND(ID_FCD_GROUP_DEFAULT,   OnFileScrapGroupDefault)
    ON_UPDATE_COMMAND_UI(ID_FCD_COPY,            OnUpdateFcdCopy)
    ON_UPDATE_COMMAND_UI(ID_FCD_MOVE,            OnUpdateFcdMove)
    ON_UPDATE_COMMAND_UI(ID_FCD_DELETE,          OnUpdateFcdDelete)
    ON_UPDATE_COMMAND_UI(ID_FCD_VIEW,            OnUpdateFcdView)
    ON_UPDATE_COMMAND_UI(ID_FCD_REMOVE,          OnUpdateFcdRemove)
    ON_UPDATE_COMMAND_UI(ID_FCD_ALWAYS_TOP_MOST, OnUpdateFcdAlwaysTopMost)
    ON_UPDATE_COMMAND_UI(ID_FCD_TRANSPARENT,     OnUpdateFcdTransparent)
    ON_UPDATE_COMMAND_UI(ID_FCD_CLIPBOARD_CUT,   OnUpdateFcdClipboardCut)
    ON_UPDATE_COMMAND_UI(ID_FCD_CLIPBOARD_COPY,  OnUpdateFcdClipboardCopy)
    ON_UPDATE_COMMAND_UI_RANGE(ID_FCD_TRANSPARENT_20, ID_FCD_TRANSPARENT_0, OnUpdateTransparent)
END_MESSAGE_MAP()

xpr_bool_t FileScrapDropDlg::Create(CWnd* pParentWnd) 
{
    return super::Create(IDD_FILE_SCRAP_DROP, pParentWnd);
}

xpr_bool_t FileScrapDropDlg::OnInitDialog(void) 
{
    super::OnInitDialog();

    mShellIDListClipFormat = ::RegisterClipboardFormat(CFSTR_SHELLIDLIST);

    mDropTarget.registerObserver(this);
    mDropTarget.Register(this);

    mDlgState = DlgStateManager::instance().getDlgState(XPR_STRING_LITERAL("FileScrapDrop"));
    if (XPR_IS_NOT_NULL(mDlgState))
    {
        mDlgState->load();

        xpr_sint_t x, y;
        mAlwaysTopMost         = mDlgState->getStateB(XPR_STRING_LITERAL("Always Top Most"), XPR_FALSE);
        mTransparent           = mDlgState->getStateB(XPR_STRING_LITERAL("Transparent"), XPR_FALSE);
        mTransparentPercentage = mDlgState->getStateI(XPR_STRING_LITERAL("Transparent Percentage"), 60);
        x                      = mDlgState->getStateI(XPR_STRING_LITERAL("left"), 0);
        y                      = mDlgState->getStateI(XPR_STRING_LITERAL("top"), 0);

        SetWindowPos(XPR_NULL, x, y, 26, 24, SWP_NOZORDER | SWP_NOSIZE);
    }

    if (XPR_IS_TRUE(mAlwaysTopMost)) setAlwaysTopMost(mAlwaysTopMost);
    if (XPR_IS_TRUE(mTransparent  )) setTransparent(mTransparent, mTransparentPercentage);

    return XPR_TRUE;
}

xpr_bool_t FileScrapDropDlg::DestroyWindow(void) 
{
    CRect sRect;
    GetWindowRect(&sRect);

    if (XPR_IS_NOT_NULL(mDlgState))
    {
        mDlgState->reset();
        mDlgState->setStateB(XPR_STRING_LITERAL("Always Top Most"),     mAlwaysTopMost);
        mDlgState->setStateB(XPR_STRING_LITERAL("Transparent"),         mTransparent);
        mDlgState->setStateI(XPR_STRING_LITERAL("Transparent Percent"), mTransparentPercentage);
        mDlgState->setStateI(XPR_STRING_LITERAL("left"),                sRect.left);
        mDlgState->setStateI(XPR_STRING_LITERAL("top"),                 sRect.top);
        mDlgState->save();
    }

    return super::DestroyWindow();
}

void FileScrapDropDlg::PostNcDestroy(void) 
{
    gFrame->mFileScrapDropDlg = XPR_NULL;
    delete this;
}

void FileScrapDropDlg::setAlwaysTopMost(xpr_bool_t aAlwaysTopMost)
{
    SetWindowPos(XPR_IS_TRUE(aAlwaysTopMost) ? &wndTopMost : &wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}

void FileScrapDropDlg::setTransparent(xpr_bool_t aTransparent, xpr_uint_t aPercent)
{
    if (XPR_IS_TRUE(aTransparent))
    {
        if (aPercent < 10)
            aPercent = 10;

        if (aPercent > 100)
            aPercent = 100;
    }
    else
    {
        aPercent = 100;
    }

    HINSTANCE sDll = LoadLibrary(XPR_STRING_LITERAL("USER32.DLL"));
    if (XPR_IS_NOT_NULL(sDll))
    {
        SetLayeredWindowAttributesFunc sSetLayeredWindowAttributesFunc = XPR_NULL;
        sSetLayeredWindowAttributesFunc = (SetLayeredWindowAttributesFunc)GetProcAddress(sDll, "SetLayeredWindowAttributes");
        if (XPR_IS_NOT_NULL(sSetLayeredWindowAttributesFunc))
        {
            HWND hWnd = GetSafeHwnd();
            SetWindowLongPtr(hWnd, GWL_EXSTYLE, GetWindowLongPtr(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
            sSetLayeredWindowAttributesFunc(hWnd, 0, (255 * aPercent) / 100, LWA_ALPHA);
        }

        FreeLibrary(sDll);
    }
}

void FileScrapDropDlg::OnLButtonDown(xpr_uint_t nFlags, CPoint point) 
{
    PostMessage(WM_NCLBUTTONDOWN, HTCAPTION);

    super::OnLButtonDown(nFlags, point);
}

LRESULT FileScrapDropDlg::OnMenuChar(xpr_uint_t nChar, xpr_uint_t nFlags, CMenu* pMenu) 
{
    LRESULT sResult;

    if (BCMenu::IsMenu(pMenu))
        sResult = BCMenu::FindKeyboardShortcut(nChar, nFlags, pMenu);
    else
        sResult = super::OnMenuChar(nChar, nFlags, pMenu);

    return sResult;
}

void FileScrapDropDlg::OnInitMenuPopup(CMenu* pPopupMenu, xpr_uint_t nIndex, xpr_bool_t bSysMenu) 
{
    ASSERT(pPopupMenu != XPR_NULL);

    //super::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

    // for multi-language
    BCMenu *pBCPopupMenu = dynamic_cast<BCMenu *>(pPopupMenu);
    if (pBCPopupMenu != XPR_NULL)
    {
        xpr_uint_t sId;
        xpr_sint_t sCount = pBCPopupMenu->GetMenuItemCount();

        const xpr_tchar_t *sStringId;
        const xpr_tchar_t *sString;
        xpr::string sMenuText;
        CommandStringTable &sCommandStringTable = CommandStringTable::instance();

        xpr_sint_t i;
        for (i = 0; i < sCount; ++i)
        {
            sId = pBCPopupMenu->GetMenuItemID(i);

            // apply string table
            if (sId != 0) // if sId is 0, it's separator.
            {
                if (sId == -1)
                {
                    // if sId(xpr_uint_t) is -1, it's sub-menu.
                    pBCPopupMenu->GetMenuText(i, sMenuText, MF_BYPOSITION);

                    sString = gApp.loadString(sMenuText);
                    pBCPopupMenu->SetMenuText(i, (xpr_tchar_t *)sString, MF_BYPOSITION);
                }
                else
                {
                    sStringId = sCommandStringTable.loadString(sId);
                    if (sStringId != XPR_NULL)
                    {
                        sString = gApp.loadString(sStringId);

                        pBCPopupMenu->SetMenuText(sId, (xpr_tchar_t *)sString, MF_BYCOMMAND);
                    }
                }
            }
        }
    }

    // Check the enabled state of various menu items.
    CCmdUI sState;
    sState.m_pMenu = pPopupMenu;
    ASSERT(sState.m_pOther == XPR_NULL);
    ASSERT(sState.m_pParentMenu == XPR_NULL);

    // Determine if menu is popup in top-level menu and set m_pOther to
    // it if so (m_pParentMenu == XPR_NULL indicates that it is secondary popup).
    HMENU sParentMenu;
    if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
        sState.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
    else if ((sParentMenu = ::GetMenu(m_hWnd)) != XPR_NULL)
    {
        CWnd *sParentWnd = this;
        // Child windows don't have menus--need to go to the top!
        if (sParentWnd != XPR_NULL && (sParentMenu = ::GetMenu(sParentWnd->m_hWnd)) != XPR_NULL)
        {
            xpr_sint_t sIndex;
            xpr_sint_t sIndexMax = ::GetMenuItemCount(sParentMenu);
            for (sIndex = 0; sIndex < sIndexMax; ++sIndex)
            {
                if (::GetSubMenu(sParentMenu, sIndex) == pPopupMenu->m_hMenu)
                {
                    // When popup is found, m_pParentMenu is containing menu.
                    sState.m_pParentMenu = CMenu::FromHandle(sParentMenu);
                    break;
                }
            }
        }
    }

    sState.m_nIndexMax = pPopupMenu->GetMenuItemCount();
    for (sState.m_nIndex = 0; sState.m_nIndex < sState.m_nIndexMax; ++sState.m_nIndex)
    {
        sState.m_nID = pPopupMenu->GetMenuItemID(sState.m_nIndex);
        if (sState.m_nID == 0)
            continue; // Menu separator or invalid cmd - ignore it.

        ASSERT(sState.m_pOther == XPR_NULL);
        ASSERT(sState.m_pMenu != XPR_NULL);
        if (sState.m_nID == (xpr_uint_t)-1)
        {
            // Possibly a popup menu, route to first item of that popup.
            sState.m_pSubMenu = pPopupMenu->GetSubMenu(sState.m_nIndex);
            if (sState.m_pSubMenu == XPR_NULL ||
                (sState.m_nID = sState.m_pSubMenu->GetMenuItemID(0)) == 0 ||
                sState.m_nID == (xpr_uint_t)-1)
            {
                continue;       // First item of popup can't be routed to.
            }

            sState.DoUpdate(this, XPR_TRUE);   // Popups are never auto disabled.
        }
        else
        {
            // Normal menu item.
            // Auto enable/disable if frame window has m_bAutoMenuEnable
            // set and command is _not_ a system command.
            sState.m_pSubMenu = XPR_NULL;
            sState.DoUpdate(this, XPR_FALSE);
        }

        // Adjust for menu deletions and additions.
        xpr_uint_t sCount = pPopupMenu->GetMenuItemCount();
        if (sCount < sState.m_nIndexMax)
        {
            sState.m_nIndex -= (sState.m_nIndexMax - sCount);
            while (sState.m_nIndex < sCount && pPopupMenu->GetMenuItemID(sState.m_nIndex) == sState.m_nID)
            {
                sState.m_nIndex++;
            }
        }

        sState.m_nIndexMax = sCount;
    }
}

void FileScrapDropDlg::OnMeasureItem(xpr_sint_t nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
    xpr_bool_t sSetFlag = XPR_FALSE;

    if (lpMeasureItemStruct->CtlType == ODT_MENU)
    {
        if (IsMenu((HMENU)(uintptr_t)lpMeasureItemStruct->itemID))
        {
            CMenu *sMenu = CMenu::FromHandle((HMENU)(uintptr_t)lpMeasureItemStruct->itemID);
            if (BCMenu::IsMenu(sMenu))
            {
                mMenu.MeasureItem(lpMeasureItemStruct);
                sSetFlag = XPR_TRUE;
            }
        }
    }

    if (XPR_IS_FALSE(sSetFlag))
        super::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void FileScrapDropDlg::OnContextMenu(CWnd* pWnd, CPoint pt) 
{
    BCMenu sMenu;
    if (sMenu.LoadMenu(IDR_FILE_SCRAP_DROP) == XPR_TRUE)
    {
        if (pt.x == -1 || pt.y == -1)
        {
            CRect sRect;
            GetClientRect(&sRect);
            ClientToScreen(&sRect);
            pt = sRect.TopLeft();
        }

        BCMenu *sPopupMenu = (BCMenu *)sMenu.GetSubMenu(0);
        if (XPR_IS_NOT_NULL(sPopupMenu))
        {
            sPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt, this);
        }
    }
}

DROPEFFECT FileScrapDropDlg::OnDragEnter(COleDataObject *aOleDataObject, DWORD aKeyState, CPoint aPoint)
{
    DROPEFFECT sDropEffect = DROPEFFECT_LINK;

    DragImage &sDragImage = DragImage::instance();

    if (sDragImage.isMyDragging() == XPR_TRUE)
    {
        CImageList *sMyDragImage = sDragImage.getMyDragImage();
        if (XPR_IS_NOT_NULL(sMyDragImage))
        {
            sMyDragImage->DragEnter(GetDesktopWindow(), aPoint);
            sMyDragImage->DragShowNolock(XPR_TRUE);
        }
    }
    else
    {
        if (mDropTarget.isUseDropHelper() == XPR_TRUE)
        {
            CPoint sDragPoint(aPoint);
            IDataObject *sDataObject = aOleDataObject->GetIDataObject(XPR_FALSE);
            mDropTarget.getDropHelper()->DragEnter(GetSafeHwnd(), sDataObject, &sDragPoint, sDropEffect);
        }
    }

    return sDropEffect;
}

void FileScrapDropDlg::OnDragLeave(void)
{
    DragImage &sDragImage = DragImage::instance();

    if (sDragImage.isMyDragging() == XPR_TRUE)
    {
        sDragImage.showMyDragNoLock(XPR_FALSE);
    }
    else
    {
        if (mDropTarget.isUseDropHelper() == XPR_TRUE)
        {
            mDropTarget.getDropHelper()->DragLeave();
        }
    }
}

DROPEFFECT FileScrapDropDlg::OnDragOver(COleDataObject *aOleDataObject, DWORD aKeyState, CPoint aPoint)
{
    DROPEFFECT sDropEffect = DROPEFFECT_LINK;

    DragImage &sDragImage = DragImage::instance();

    if (sDragImage.isMyDragging() == XPR_TRUE)
    {
        CImageList *sMyDragImage = sDragImage.getMyDragImage();
        if (XPR_IS_NOT_NULL(sMyDragImage))
        {
            CPoint sDragPoint(aPoint);
            ClientToScreen(&sDragPoint);
            sMyDragImage->DragMove(sDragPoint);
        }
    }
    else
    {
        if (mDropTarget.isUseDropHelper() == XPR_TRUE)
        {
            CPoint sDragPoint(aPoint);
            mDropTarget.getDropHelper()->DragOver(&sDragPoint, sDropEffect);
        }
    }

    return sDropEffect;
}

void FileScrapDropDlg::OnDragScroll(DWORD aKeyState, CPoint point)
{
}

void FileScrapDropDlg::OnDrop(COleDataObject *aOleDataObject, DROPEFFECT aDropEffect, CPoint aPoint)
{
    DragImage &sDragImage = DragImage::instance();

    if (sDragImage.isMyDragging() == XPR_TRUE)
    {
        CImageList *sMyDragImage = sDragImage.getMyDragImage();
        if (XPR_IS_NOT_NULL(sMyDragImage))
        {
            sMyDragImage->EndDrag();
            sMyDragImage->DragShowNolock(XPR_FALSE);
        }
    }
    else
    {
        if (mDropTarget.isUseDropHelper() == XPR_TRUE)
        {
            mDropTarget.getDropHelper()->DragLeave();
        }
    }

    COleDataObject *sOleDataObject = aOleDataObject;
    if (sOleDataObject->IsDataAvailable(mShellIDListClipFormat) == XPR_FALSE)
        return;

    xpr_uint_t sGroupId = FileScrap::instance().getCurGroupId();

    if (mDropTarget.isRightDrag() == XPR_TRUE)
    {
        cmd::FileScrapGroupDlg sDlg;
        if (sDlg.DoModal() != IDOK)
            return;

        xpr_uint_t sSelGroupId = sDlg.getSelGroupId();
        if (sSelGroupId != FileScrap::InvalidGroupId)
            sGroupId = sSelGroupId;
    }

    STGMEDIUM sStgMedium = {0};
    FORMATETC sFormatEtc = {(CLIPFORMAT)mShellIDListClipFormat, XPR_NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
    if (sOleDataObject->GetData(mShellIDListClipFormat, &sStgMedium, &sFormatEtc) == XPR_FALSE)
        return;

    LPIDA sIda = (LPIDA)::GlobalLock(sStgMedium.hGlobal);
    if (XPR_IS_NOT_NULL(sIda))
    {
        xpr_sint_t sCount = sIda->cidl;
        LPCITEMIDLIST sPidl1 = XPR_NULL; // folder PIDL
        LPCITEMIDLIST sPidl2 = XPR_NULL; // item PIDL

        // get folder PIDL
        sPidl1 = (LPITEMIDLIST)(((xpr_byte_t *)sIda)+(sIda)->aoffset[0]);

        LPSHELLFOLDER sShellFolder = XPR_NULL, sShellFolderDesktop = XPR_NULL;
        ::SHGetDesktopFolder(&sShellFolderDesktop);
        HRESULT sHResult = sShellFolderDesktop->BindToObject(sPidl1, XPR_NULL, IID_IShellFolder, (LPVOID *)&sShellFolder);
        if (FAILED(sHResult) || XPR_IS_NULL(sShellFolder))
        {
            sShellFolder = sShellFolderDesktop;
            sShellFolderDesktop = XPR_NULL;
        }

        LPCITEMIDLIST *sPidls = new LPCITEMIDLIST[sCount];

        xpr_sint_t i;
        xpr_sint_t sCount2 = 0;
        for (i = 0; i < sCount; ++i)
        {
            // get item PIDL and get full-qualified PIDL
            sPidl2 = (LPCITEMIDLIST)(((xpr_byte_t *)sIda)+(sIda)->aoffset[i+1]);

            if (IsFileSystem(sShellFolder, sPidl2))
            {
                sPidls[sCount2] = sPidl2;
                sCount2++;
            }
        }

        FileScrap &sFileScrap = FileScrap::instance();

        sFileScrap.addItem(sGroupId, sShellFolder, sPidls, sCount2);

        XPR_SAFE_DELETE_ARRAY(sPidls);

        COM_RELEASE(sShellFolder);
        COM_RELEASE(sShellFolderDesktop);
        ::GlobalUnlock(sStgMedium.hGlobal);
    }

    if (XPR_IS_NOT_NULL(sStgMedium.pUnkForRelease))
    {
        ::ReleaseStgMedium(&sStgMedium);
    }
}

void FileScrapDropDlg::OnFileScrapClose(void) 
{
    DestroyWindow();
}

void FileScrapDropDlg::OnClose(void) 
{
    DestroyWindow();
}

void FileScrapDropDlg::OnFileScrapCopy(void) 
{
    xpr_tchar_t sTarget[XPR_MAX_PATH + 1];
    if (browse(sTarget) == XPR_TRUE)
    {
        FileScrap &sFileScrap = FileScrap::instance();

        xpr_uint_t sGroupId = sFileScrap.getCurGroupId();
        sFileScrap.copyOperation(sGroupId, sTarget);
    }
}

void FileScrapDropDlg::OnUpdateFcdCopy(CCmdUI* pCmdUI) 
{
    FileScrap &sFileScrap = FileScrap::instance();
    pCmdUI->Enable(sFileScrap.getItemCount(sFileScrap.getCurGroupId()) > 0);
}

void FileScrapDropDlg::OnFileScrapMove(void) 
{
    xpr_tchar_t sTarget[XPR_MAX_PATH + 1];
    if (browse(sTarget) == XPR_TRUE)
    {
        FileScrap &sFileScrap = FileScrap::instance();

        xpr_uint_t sGroupId = sFileScrap.getCurGroupId();
        sFileScrap.moveOperation(sGroupId, sTarget);
    }
}

void FileScrapDropDlg::OnUpdateFcdMove(CCmdUI* pCmdUI) 
{
    FileScrap &sFileScrap = FileScrap::instance();
    pCmdUI->Enable(sFileScrap.getItemCount(sFileScrap.getCurGroupId()) > 0);
}

void FileScrapDropDlg::OnFileScrapDelete(void) 
{
    FileScrap &sFileScrap = FileScrap::instance();

    xpr_uint_t sGroupId = sFileScrap.getCurGroupId();

    const xpr_tchar_t *sMsg = gApp.loadString(XPR_STRING_LITERAL("file_scrap.msg.confirm_delete_or_trash"));
    xpr_sint_t sMsgId = AfxGetMainWnd()->MessageBox(sMsg, XPR_NULL, MB_OK | MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2);

    if (sMsgId == IDYES)
    {
        sFileScrap.deleteOperation(sGroupId);
    }
    else if (sMsgId == IDNO)
    {
        sFileScrap.trashOperation(sGroupId);
    }
}

void FileScrapDropDlg::OnUpdateFcdDelete(CCmdUI* pCmdUI) 
{
    FileScrap &sFileScrap = FileScrap::instance();
    pCmdUI->Enable(sFileScrap.getItemCount(sFileScrap.getCurGroupId()) > 0);
}

void FileScrapDropDlg::OnFileScrapClipboardCut(void) 
{
    FileScrap &sFileScrap = FileScrap::instance();
    xpr_uint_t sGroupId = sFileScrap.getCurGroupId();

    sFileScrap.setClipboardCut(sGroupId);
}

void FileScrapDropDlg::OnUpdateFcdClipboardCut(CCmdUI* pCmdUI) 
{
    FileScrap &sFileScrap = FileScrap::instance();
    pCmdUI->Enable(sFileScrap.getItemCount(sFileScrap.getCurGroupId()) > 0);
}

void FileScrapDropDlg::OnFileScrapClipboardCopy(void) 
{
    FileScrap &sFileScrap = FileScrap::instance();
    xpr_uint_t sGroupId = sFileScrap.getCurGroupId();

    sFileScrap.setClipboardCopy(sGroupId);
}

void FileScrapDropDlg::OnUpdateFcdClipboardCopy(CCmdUI* pCmdUI) 
{
    FileScrap &sFileScrap = FileScrap::instance();
    pCmdUI->Enable(sFileScrap.getItemCount(sFileScrap.getCurGroupId()) > 0);
}

void FileScrapDropDlg::OnFileScrapView(void) 
{
    gFrame->executeCommand(ID_EDIT_FILE_SCRAP_VIEW);
}

void FileScrapDropDlg::OnUpdateFcdView(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(XPR_TRUE);
}

void FileScrapDropDlg::OnFileScrapRemove(void) 
{
    FileScrap &sFileScrap = FileScrap::instance();
    xpr_uint_t sGroupId = sFileScrap.getCurGroupId();

    sFileScrap.removeAllItems(sGroupId);
}

void FileScrapDropDlg::OnUpdateFcdRemove(CCmdUI* pCmdUI) 
{
    FileScrap &sFileScrap = FileScrap::instance();
    pCmdUI->Enable(sFileScrap.getItemCount(sFileScrap.getCurGroupId()) > 0);
}

static xpr_sint_t CALLBACK BrowseCallbackProc(HWND hwnd, xpr_uint_t uMsg, LPARAM lParam, LPARAM dwData)
{
    if (uMsg == BFFM_INITIALIZED)
        ::SendMessage(hwnd, BFFM_SETSELECTION, XPR_FALSE, dwData);

    return 0;
}

xpr_bool_t FileScrapDropDlg::browse(xpr_tchar_t *aTarget)
{
    if (XPR_IS_NULL(aTarget))
        return XPR_FALSE;

    xpr_bool_t sResult = XPR_FALSE;
    static LPITEMIDLIST sFullPidl = XPR_NULL;

    BROWSEINFO sBrowseInfo = {0};
    sBrowseInfo.hwndOwner = GetSafeHwnd();
    sBrowseInfo.lpszTitle = gApp.loadString(XPR_STRING_LITERAL("popup.file_scrap_drop.folder_browse.title"));
    sBrowseInfo.ulFlags   = BIF_RETURNONLYFSDIRS;// | BIF_USENEWUI;
    sBrowseInfo.lpfn      = (BFFCALLBACK)BrowseCallbackProc;
    sBrowseInfo.lParam    = (LPARAM)sFullPidl;
    sFullPidl = SHBrowseForFolder(&sBrowseInfo);

    if (XPR_IS_NOT_NULL(sFullPidl))
    {
        SHGetPathFromIDList(sFullPidl, aTarget);
        sResult = XPR_TRUE;
    }

    return sResult;
}

void FileScrapDropDlg::OnFileScrapAlwaysTopMost(void) 
{
    mAlwaysTopMost = !mAlwaysTopMost;
    setAlwaysTopMost(mAlwaysTopMost);
}

void FileScrapDropDlg::OnUpdateFcdAlwaysTopMost(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(mAlwaysTopMost);
}

void FileScrapDropDlg::OnFileScrapTransparent0(void) 
{
    mTransparent = XPR_FALSE;
    setTransparent(mTransparent, mTransparentPercentage);
}

void FileScrapDropDlg::OnFileScrapTransparent20(void) 
{
    mTransparent = XPR_TRUE;
    mTransparentPercentage = 20;
    setTransparent(mTransparent, mTransparentPercentage);
}

void FileScrapDropDlg::OnFileScrapTransparent40(void) 
{
    mTransparent = XPR_TRUE;
    mTransparentPercentage = 40;
    setTransparent(mTransparent, mTransparentPercentage);
}

void FileScrapDropDlg::OnFileScrapTransparent60(void) 
{
    mTransparent = XPR_TRUE;
    mTransparentPercentage = 60;
    setTransparent(mTransparent, mTransparentPercentage);
}

void FileScrapDropDlg::OnFileScrapTransparent80(void) 
{
    mTransparent = XPR_TRUE;
    mTransparentPercentage = 80;
    setTransparent(mTransparent, mTransparentPercentage);
}

void FileScrapDropDlg::OnUpdateTransparent(CCmdUI *pCmdUI)
{
    pCmdUI->Enable();

    xpr_bool_t sRadio = XPR_FALSE;
    switch (pCmdUI->m_nID)
    {
    case ID_FCD_TRANSPARENT_20: sRadio = (XPR_IS_TRUE(mTransparent) && mTransparentPercentage == 20); break;
    case ID_FCD_TRANSPARENT_40: sRadio = (XPR_IS_TRUE(mTransparent) && mTransparentPercentage == 40); break;
    case ID_FCD_TRANSPARENT_60: sRadio = (XPR_IS_TRUE(mTransparent) && mTransparentPercentage == 60); break;
    case ID_FCD_TRANSPARENT_80: sRadio = (XPR_IS_TRUE(mTransparent) && mTransparentPercentage == 80); break;
    case ID_FCD_TRANSPARENT_0:  sRadio = (!mTransparent); break;
    }

    pCmdUI->SetRadio(sRadio);
}
void FileScrapDropDlg::OnUpdateFcdTransparent(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(mTransparent);
}

void FileScrapDropDlg::OnLButtonDblClk(xpr_uint_t nFlags, CPoint point) 
{
    if (nFlags & MK_LBUTTON)
        OnFileScrapView();

    super::OnLButtonDblClk(nFlags, point);
}

void FileScrapDropDlg::OnRButtonDblClk(xpr_uint_t nFlags, CPoint point) 
{
    super::OnRButtonDblClk(nFlags, point);
}

void FileScrapDropDlg::OnFileScrapGroupDefault(void) 
{
    cmd::FileScrapGroupDlg sDlg;
    sDlg.setTitle(gApp.loadString(XPR_STRING_LITERAL("popup.file_scrap_default_group_set.title")));
    if (sDlg.DoModal() != IDOK)
        return;

    xpr_uint_t sGroupId = sDlg.getSelGroupId();
    if (sGroupId == FileScrap::InvalidGroupId)
        return;

    FileScrap::instance().setCurGroupId(sGroupId);
}
} // namespace fxfile
