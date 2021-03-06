//
// Copyright (c) 2001-2013 Leon Lee author. All rights reserved.
//
//   homepage: http://www.flychk.com
//   e-mail:   mailto:flychk@flychk.com
//
// Use of this source code is governed by a GPLv3 license that can be
// found in the LICENSE file.

#ifndef __FXFILE_EXPLORER_CTRL_H__
#define __FXFILE_EXPLORER_CTRL_H__ 1
#pragma once

#include "shell_change_notify.h"
#include "file_change_watcher.h"
#include "adv_file_change_watcher.h"
#include "history.h"

#include "gui/DropTarget.h"
#include "gui/ListCtrlEx.h"

#include "fxfile_def.h"
#include "folder_layout.h"

namespace fxfile
{
class RenameHelper;
class ShellIcon;
class ContextMenu;

class ExplorerCtrlObserver;

struct FileSysItem
{
    xpr::string mPath;
    xpr_ulong_t mFileAttributes;
};

typedef std::deque<FileSysItem *> FileSysItemDeque;

class ExplorerCtrl : public ListCtrlEx, public DropTargetObserver
{
    typedef ListCtrlEx super;

public:
    struct Option
    {
        xpr_sint_t  mMouseClick;
        xpr_bool_t  mShowHiddenAttribute;
        xpr_bool_t  mShowSystemAttribute;
        xpr_bool_t  mTooltip;
        xpr_bool_t  mTooltipWithFileName;
        xpr_sint_t  mFileExtType;
        xpr_sint_t  mRenameExtType;
        xpr_bool_t  mRenameByMouse;

        xpr_bool_t  mCustomFont;
        xpr_tchar_t mCustomFontText[MAX_FONT_TEXT + 1];

        xpr_bool_t  mBkgndImage;
        xpr_tchar_t mBkgndImagePath[XPR_MAX_PATH + 1];
        xpr_sint_t  mBkgndColorType;
        xpr_uint_t  mBkgndColor;
        xpr_sint_t  mTextColorType;
        xpr_uint_t  mTextColor;
        xpr_sint_t  mSizeUnit;
        xpr_sint_t  mListType;
        xpr_bool_t  mParentFolder;
        xpr_bool_t  mGoUpSelSubFolder;
        xpr_bool_t  mCustomIcon;
        xpr_tchar_t mCustomIconFile16[XPR_MAX_PATH + 1];
        xpr_tchar_t mCustomIconFile32[XPR_MAX_PATH + 1];
        xpr_tchar_t mCustomIconFile48[XPR_MAX_PATH + 1];
        xpr_tchar_t mCustomIconFile256[XPR_MAX_PATH + 1];
        xpr_bool_t  m24HourTime;
        xpr_bool_t  m2YearDate;
        xpr_bool_t  mShowDrive;
        xpr_bool_t  mShowDriveItem;
        xpr_bool_t  mShowDriveSize;
        xpr_sint_t  mNameCaseType;
        xpr_bool_t  mCreateAndEditText;
        xpr_bool_t  mAutoColumnWidth;
        xpr_sint_t  mSaveFolderLayout;
        xpr_bool_t  mSaveViewStyle;
        xpr_sint_t  mDefaultViewStyle;
        xpr_sint_t  mDefaultSort;
        xpr_sint_t  mDefaultSortOrder;
        xpr_bool_t  mNoSort;
        xpr_bool_t  mClassicThemeStyle;
        xpr_bool_t  mGridLines;
        xpr_bool_t  mFullRowSelect;

        xpr_sint_t  mThumbnailWidth;
        xpr_sint_t  mThumbnailHeight;
        xpr_bool_t  mThumbnailSaveCache;
        xpr_sint_t  mThumbnailPriority;
        xpr_bool_t  mThumbnailLoadByExt;

        xpr_bool_t  mFileScrapContextMenu;

        xpr_sint_t  mDragType;
        xpr_sint_t  mDragDist;
        xpr_sint_t  mDragScrollTime;
        xpr_sint_t  mDragDefaultFileOp;
        xpr_bool_t  mDragNoContents;

        xpr_bool_t  mExternalCopyFileOp;
        xpr_bool_t  mExternalMoveFileOp;

        xpr_bool_t  mNoRefresh;
        xpr_bool_t  mRefreshSort;

        xpr_sint_t  mHistoryCount;
        xpr_sint_t  mBackwardCount;
    };

public:
    ExplorerCtrl(void);
    virtual ~ExplorerCtrl(void);

public:
    void setObserver(ExplorerCtrlObserver *aObserver);

    xpr_sint_t getViewIndex(void) const;
    void       setViewIndex(xpr_sint_t aViewIndex);

    virtual xpr_bool_t Create(CWnd *aParentWnd, xpr_uint_t aId, const RECT &aRect);

    void setOption(Option &aOption);

public:
    xpr_bool_t explore(LPITEMIDLIST aFullPidl, xpr_bool_t aUpdateBuddy = XPR_TRUE);
    void       refresh(void);

    LPTVITEMDATA       getFolderData(void) const;
    const xpr_tchar_t *getCurPath(void) const;
    const xpr_tchar_t *getCurFullPath(void) const;
    void               getCurPath(xpr_tchar_t *aCurPath) const;
    void               getCurFullPath(xpr_tchar_t *aCurFullPath) const;
    void               getCurPath(xpr::string &aCurPath) const;
    void               getCurFullPath(xpr::string &aCurFullPath) const;
    LPITEMIDLIST       getFolderGroup(void) const;

    static LPITEMIDLIST getDefInitFolder(void);

    FolderType getFolderType(void) const;
    xpr_bool_t isFolderType(FolderType aFolderType) const;

    void loadHistory(HistoryDeque *aBackwardDeque,
                     HistoryDeque *aForwardDeque,
                     HistoryDeque *aHistoryDeque);

    // file processing
    void createFolder(const xpr::string &aNewFolder, xpr_bool_t aEditDirectly);
    void createTextFile(const xpr::string &aNewTextFile, xpr_bool_t aEditDirectly);
    void createGeneralFile(const xpr::string &aNewGeneralFile, xpr_bool_t aEditDirectly);

    LPITEMIDLIST getCopyToPidl(void) const;
    LPITEMIDLIST getMoveToPidl(void) const;
    void         setCopyToPidl(LPITEMIDLIST aPidl);
    void         setMoveToPidl(LPITEMIDLIST aPidl);

    void renameDirectly(xpr_sint_t aIndex);

    xpr_bool_t isSelected(void) const;
    xpr_bool_t isFileSystem(void) const;
    xpr_bool_t isFileSystem(xpr_sint_t aIndex) const;
    xpr_bool_t isFileSystemFolder(void) const;

    // item execution
    xpr_sint_t execute(xpr_sint_t aIndex, xpr_bool_t aParamExec = XPR_FALSE);
    void       executeAllSelItems(void);

    // find item
    xpr_sint_t findItemName(const xpr_tchar_t *aFind, xpr_sint_t aStart = -1) const;
    xpr_sint_t findItemFileName(const xpr_tchar_t *aFileName) const;
    xpr_sint_t findItemFileName(const xpr_tchar_t *aName, const xpr_tchar_t *aFileName) const;
    xpr_sint_t findItemFolder(const xpr_tchar_t *aFolderName) const;
    xpr_sint_t findItemPath(const xpr_tchar_t *aPath) const;
    xpr_sint_t findItemPath(const xpr_tchar_t *aName, const xpr_tchar_t *aPath) const;
    xpr_sint_t findItemPath(LPITEMIDLIST aFullPidl) const;
    xpr_sint_t findItemPath(LPSHELLFOLDER aShellFolder, LPITEMIDLIST aPidl) const;
    xpr_sint_t findItemSignature(xpr_uint_t aSignature) const;

    // file information
    xpr_bool_t getItemName(xpr_sint_t aIndex, xpr_tchar_t *aName, xpr_size_t aMaxLen, xpr_sint_t aExtensionType) const;
    xpr_bool_t getFileSize(xpr_sint_t aIndex, xpr_uint64_t &aFileSize) const;
    void       getFileTime(xpr_sint_t aIndex, xpr_tchar_t *aModifiedDateTime, xpr_size_t aMaxLen) const;
    void       getFileType(xpr_sint_t aIndex, xpr_tchar_t *aTypeName, xpr_size_t aMaxLen) const;
    void       getFileAttr(xpr_sint_t aIndex, xpr_tchar_t *aFileAttributesText, xpr_size_t aMaxLen) const;

    // clipboard
    xpr_sint_t getClipFormatCount(void) const;
    void       getClipFormat(CLIPFORMAT *aClipFormat) const;
    CLIPFORMAT getClipFormatDropEffect(void) const;
    CLIPFORMAT getClipFormatShellIDList(void) const;
    xpr_bool_t getSelDataObject(LPDATAOBJECT *aDataObject, xpr_uint_t aShellAttributes = 0, DROPEFFECT *aOkDropEffect = XPR_NULL);
    xpr_bool_t hasSelShellAttributes(xpr_uint_t aShellAttributes) const;

    // item selection
    void         selectAll(void);
    void         unselectAll(void);
    void         selectNextItemWithFocus(void);
    void         selectItem(xpr_sint_t aIndex);
    xpr_bool_t   getSelItemPath(xpr_tchar_t **aPaths, xpr_sint_t &aCount, xpr_ulong_t aShellAttributes = 0) const;
    void         getSelItemData(LPSHELLFOLDER *aShellFolder, LPCITEMIDLIST **aPidls, xpr_sint_t &aCount, xpr_ulong_t aShellAttributes = 0) const;
    xpr_size_t   getRealSelCount(xpr_bool_t aCalculate = XPR_FALSE, xpr_size_t *aRealSelFolderCount = XPR_NULL, xpr_size_t *aRealSelFileCount = XPR_NULL);
    xpr_uint64_t getSelFileSize(void) const;
    xpr_uint64_t getTotalFileSize(xpr_sint_t *aTotalCount = XPR_NULL, xpr_sint_t *aFileCount = XPR_NULL, xpr_sint_t *aFolderCount = XPR_NULL) const;
    void         setInsSelPath(const xpr_tchar_t *aInsSelPath, xpr_bool_t aUnselectAll = XPR_TRUE, xpr_bool_t aEdit = XPR_FALSE);

    void       getAllFileSysItems(FileSysItemDeque &aItemDeque, xpr_bool_t aOnlyFileName, xpr_ulong_t aMask = SFGAO_FILESYSTEM, xpr_ulong_t aUnmask = 0) const;
    xpr_bool_t getSelFileSysItems(FileSysItemDeque &aItemDeque, xpr_bool_t aOnlyFileName, xpr_ulong_t aMask = SFGAO_FILESYSTEM, xpr_ulong_t aUnmask = 0);

    void updateStatus(void);
    void resetStatus(void);

    // column
    xpr_sint_t addColumn(ColumnId *aColumnId, xpr_sint_t aInsert = -1);
    xpr_bool_t deleteColumn(ColumnId *aColumnId, xpr_sint_t *aIndex = XPR_NULL);
    void       useColumn(ColumnId *aColumnId, xpr_bool_t aNotifyToObserver = XPR_TRUE);
    xpr_bool_t isUseColumn(ColumnId *aColumnId) const;
    ColumnId  *getColumnId(xpr_sint_t aColumnIndex) const;
    xpr_sint_t getColumnFromId(ColumnId *aColumnId) const;

    void getColumnDataList(LPSHELLFOLDER2 &aShellFolder2, ColumnDataList &aColumnList) const;
    void setColumnDataList(ColumnDataList &aUseColumnList);

    // sort
    void resortItems(void);
    void sortItems(ColumnId *aColumnId);
    void sortItems(ColumnId *aColumnId, xpr_bool_t aAscending, xpr_bool_t aNotifyToObserver = XPR_TRUE);
    const ColumnId &getSortColumnId(void) const;
    xpr_bool_t      getSortAscending(void) const;

    // parent
    xpr_bool_t goUp(void);
    xpr_bool_t goUp(xpr_uint_t aUp);
    xpr_uint_t getUpCount(void);

    // history
    void       goBackward(void);
    void       goBackward(xpr_size_t aBack);
    xpr_size_t getBackwardCount(void);
    void       clearBackward(void);
    void       goForward(void);
    void       goForward(xpr_size_t aForward);
    xpr_size_t getForwardCount(void);
    void       clearForward(void);
    void       goHistory(void);
    void       goHistory(xpr_size_t aHistory);
    xpr_size_t getHistoryCount(void);
    void       clearHistory(void);
    void       clearAllHistory(void);
    void       setMaxBackward(xpr_size_t aBackwardCount);
    void       setMaxHistory(xpr_size_t aHistoryCount);
    const HistoryDeque *getBackwardDeque(void) const;
    const HistoryDeque *getForwardDeque(void) const;
    const HistoryDeque *getHistoryDeque(void) const;

    // view style
    xpr_sint_t getViewStyle(void) const;
    void setViewStyle(xpr_sint_t aViewStyle, xpr_bool_t aRefresh = XPR_FALSE, xpr_bool_t aNotifyToObserver = XPR_TRUE);
    void setViewStyleChange(const FolderLayoutChange &aFolderLayoutChange);
    void syncFolderLayout(void);
    void saveToFolderLayout(FolderLayout &aFolderLayout) const;
    FolderLayout *getFolderLayout(void) const;
    FolderLayout *getFolderLayout(FolderType aFolderType) const;
    void saveFolderLayout(void);
    void saveFolderLayout(xpr_sint_t aSaveFolderLayout);

    // image list
    CImageList *getImageList(xpr_bool_t aNormal) const;
    void        setImageList(CImageList *aExtraLargeImgList,
                             CImageList *aLargeImgList,
                             CImageList *aMediumImgList,
                             CImageList *aSmallImgList);

    // custom font
    void setCustomFont(xpr_bool_t aCustomFont, xpr_tchar_t *aFontText);
    void setCustomFont(xpr_bool_t aCustomFont, CFont *aFont);

    // item data
    LPARAM      GetItemData(xpr_sint_t aItem) const;
    DWORD       getItemDataType(xpr_sint_t aItem) const;
    xpr_bool_t  isShellItem(xpr_sint_t aItem) const;
    xpr_tchar_t getDriveFromItemType(DWORD aItemDataType) const;
    static void verifyItemData(LPLVITEMDATA *aLvItemData);
    xpr_sint_t  getLastInsertIndex(void) const;

    void setDragContents(xpr_bool_t aDragContents = XPR_TRUE);

    void setHiddenSystem(xpr_bool_t aModifiedHidden, xpr_bool_t aModifiedSystem);
    void doShChangeNotifyUpdateDir(xpr_bool_t aPidl = XPR_TRUE, xpr_bool_t aFlush = XPR_TRUE);
    void setChangeNotify(xpr::string aPath, xpr_bool_t aAllSubTree, xpr_bool_t aEnable = XPR_TRUE);
    void enableChangeNotify(xpr_bool_t aNotify);

    xpr_bool_t doPasteSelect(HWND aHwnd, COleDataObject *aOleDataObject, xpr_bool_t aDragDrop, CLIPFORMAT aShellIDListClipForamt, DROPEFFECT aDropEffect, xpr_tchar_t *aTarget);
    xpr_bool_t doPasteSelect(HWND aHwnd, CLIPFORMAT aShellIDListClipFormat, CLIPFORMAT aDropEffect, xpr_tchar_t *aTarget, xpr_bool_t aCopy = XPR_FALSE);
    xpr_uint_t getPasteSelItemMessage(void) const;

protected:
    void applyOption(Option &aNewOption);

    xpr_bool_t exploreItem(LPITEMIDLIST aFullPidl, xpr_bool_t aUpdateBuddy = XPR_TRUE);

    void       preEnumeration(LPTVITEMDATA aNewTvItemData);
    xpr_bool_t insertPidlItem(LPSHELLFOLDER aShellFolder, LPITEMIDLIST aPidl, xpr_sint_t aIndex);
    void       postEnumeration(xpr_bool_t aUpdateBuddy);

    void watchFileChange(void);

    void addParentItem(void);
    void addDriveItem(void);
    void updateDriveItem(void);
    void getDriveItemTotalSize(xpr_tchar_t *aDrive, xpr_tchar_t *aText, const xpr_size_t aMaxLen) const;
    void getDriveItemFreeSize(xpr_tchar_t *aDrive, xpr_tchar_t *aText, const xpr_size_t aMaxLen) const;

    xpr_bool_t OnGetdispinfoParentItem(LVITEM &aLvItem, LPLVITEMDATA aLvItemData);
    xpr_bool_t OnGetdispinfoDriveItem (LVITEM &aLvItem, LPLVITEMDATA aLvItemData);
    xpr_bool_t OnGetdispinfoShellItem (LVITEM &aLvItem, LPLVITEMDATA aLvItemData);
    xpr_bool_t OnGetdispinfoShellItemDefault (const ColumnId &aColumnId, LVITEM &aLvItem, LPLVITEMDATA aLvItemData);
    xpr_bool_t OnGetdispinfoShellItemComputer(const ColumnId &aColumnId, LVITEM &aLvItem, LPLVITEMDATA aLvItemData);
    xpr_bool_t OnGetdispinfoShellItemVirtual (const ColumnId &aColumnId, LVITEM &aLvItem, LPLVITEMDATA aLvItemData);

    // File Information
    xpr_bool_t getItemName(LPLVITEMDATA aLvItemData, xpr_tchar_t *aName, const xpr_size_t aMaxLen) const;
    xpr_bool_t getItemName(LPLVITEMDATA aLvItemData, xpr_tchar_t *aName, const xpr_size_t aMaxLen, xpr_sint_t aExtensionType) const;
    xpr_bool_t getItemName(LPCITEMIDLIST aFullPidl, xpr_tchar_t *aName, const xpr_size_t aMaxLen) const;
    xpr_bool_t getItemName(LPCITEMIDLIST aFullPidl, xpr_tchar_t *aName, const xpr_size_t aMaxLen, xpr_sint_t aExtensionType) const;
    xpr_bool_t getItemName(LPSHELLFOLDER aShellFolder, LPCITEMIDLIST aPidl, xpr_tchar_t *aName, const xpr_size_t aMaxLen) const;
    xpr_bool_t getItemName(LPSHELLFOLDER aShellFolder, LPCITEMIDLIST aPidl, xpr_tchar_t *aName, const xpr_size_t aMaxLen, xpr_sint_t aExtensionType) const;
    xpr_bool_t getItemName(LPSHELLFOLDER aShellFolder, LPCITEMIDLIST aPidl, xpr_ulong_t aShellAttributes, xpr_tchar_t *aName, const xpr_size_t aMaxLen, xpr_sint_t aExtensionType) const;
    xpr_bool_t getItemName(const xpr_tchar_t *aPath, xpr_tchar_t *aName, const xpr_size_t aMaxLen, xpr_sint_t aExtensionType) const;
    xpr_bool_t getItemName(const xpr_tchar_t *aPath, xpr_tchar_t *aName, const xpr_size_t aMaxLen) const;
    xpr_sint_t getFileIconIndex(LPLVITEMDATA aLvItemData, const xpr_tchar_t *aPath = XPR_NULL) const;
    void       getFileSize(LPLVITEMDATA aLvItemData, xpr_tchar_t *aFileSize, const xpr_size_t aMaxLen) const;
    xpr_bool_t getFileSize(LPLVITEMDATA aLvItemData, xpr_uint64_t &aFileSize) const;
    void       getFileTime(LPLVITEMDATA aLvItemData, xpr_tchar_t *aModifiedDateTime, const xpr_size_t aMaxLen) const;
    void       getFileType(LPLVITEMDATA aLvItemData, xpr_tchar_t *aTypeName, const xpr_size_t aMaxLen) const;
    void       getFileAttr(LPLVITEMDATA aLvItemData, xpr_tchar_t *aFileAttr, const xpr_size_t aMaxLen) const;
    void       getFileExtension(LPLVITEMDATA aLvItemData, xpr_tchar_t *aExtension, const xpr_size_t aMaxLen) const;
    xpr_bool_t getDriveTotalSize(LPSHELLFOLDER aShellFolder, LPITEMIDLIST aPidl, xpr_tchar_t *aTotalSize, const xpr_size_t aMaxLen) const;
    xpr_bool_t getDriveFreeSize(LPSHELLFOLDER aShellFolder, LPITEMIDLIST aPidl, xpr_tchar_t *aFreeSize, const xpr_size_t aMaxLen) const;
    xpr_bool_t getDescription(LPLVITEMDATA aLvItemData, xpr_tchar_t *aDesc, const xpr_sint_t aMaxLen, xpr_bool_t aOriginal = XPR_FALSE) const;
    xpr_bool_t getNetDescription(LPLVITEMDATA aLvItemData, xpr_tchar_t *aDesc, const xpr_size_t aMaxLen) const;
    void       getRootDriveType(LPSHELLFOLDER aShellFolder, LPITEMIDLIST aPidl, xpr_tchar_t *aDriveType, const xpr_size_t aMaxLen) const; // Drive Type
    void       getItemAttributes(LPSHELLFOLDER aShellFolder, LPITEMIDLIST aPidl, xpr_ulong_t &aShellAttributes, DWORD &aFileAttributes);

    // Callback Sorting Function
    static xpr_sint_t CALLBACK DefaultItemCompareProc(LPARAM, LPARAM, LPARAM); // Sorting(Name, Size, Type, Date)

    // drag and drop
    CImageList *createDragImageEx(LPPOINT aPoint); // Create Custom Drag Image
    void        beginDragDrop(NM_LISTVIEW *aNmListView);

    virtual DROPEFFECT OnDragEnter(COleDataObject *aOleDataObject, DWORD aKeyState, CPoint aPoint);
    virtual void       OnDragLeave(void);
    virtual DROPEFFECT OnDragOver(COleDataObject *aOleDataObject, DWORD aKeyState, CPoint aPoint);
    virtual void       OnDragScroll(DWORD aKeyState, CPoint aPoint);
    virtual void       OnDrop(COleDataObject *aOleDataObject, DROPEFFECT aDropEffect, CPoint aPoint);

    void showHiddenSystem(xpr_bool_t aModifiedHidden, xpr_bool_t aModifiedSystem);
    void hideHiddenSystem(xpr_bool_t aModifiedHidden, xpr_bool_t aModifiedSystem);

    void       enumerateShcn(ShNotifyInfo *aShNotifyInfo, xpr_bool_t aHiddenAdd); // Shell Change Notify
    void       preShcnEnumeration(void);
    xpr_bool_t updateShcnPidlItem(LPSHELLFOLDER aShellFolder, LPITEMIDLIST aPidl, ShNotifyInfo *aShNotifyInfo);
    void       postShcnEnumeration(void);

    xpr_bool_t OnShcnCreateItem(Shcn *aShcn);
    xpr_bool_t OnShcnRenameItem(Shcn *aShcn);
    xpr_bool_t OnShcnRenameItem(const xpr_tchar_t *aOldPath, LPITEMIDLIST aFullPidl);
    xpr_bool_t OnShcnUpdateDir(Shcn *aShcn);
    xpr_bool_t updateShcnLvItemData(LPLVITEMDATA aLvItemData);
    xpr_bool_t OnShcnDeleteItem(Shcn *aShcn = XPR_NULL, const xpr_tchar_t *aParsing = XPR_NULL);
    xpr_bool_t OnShcnUpdateItem(Shcn *aShcn);
    xpr_bool_t OnShcnUpdateName(void);
    xpr_bool_t OnShcnNetShare(Shcn *aShcn);
    xpr_bool_t OnShcnEnumNetShare(LPLVITEMDATA aLvItemData, ShNotifyInfo *aShNotifyInfo);
    xpr_bool_t OnShcnDriveAdd(Shcn *aShcn);
    xpr_bool_t OnShcnDriveRemove(Shcn *aShcn);
    xpr_bool_t OnShcnMediaInserted(Shcn *aShcn);
    xpr_bool_t OnShcnMediaRemoved(Shcn *aShcn);
    xpr_bool_t OnShcnFreeSize(Shcn *aShcn);
    xpr_bool_t OnShcnBitBucket(Shcn *aShcn);

    inline xpr_bool_t beginShcn(DWORD aEventId);
    inline void endShcn(DWORD aEventId, xpr_bool_t aResult);

    xpr_uint_t getThumbImageId(const xpr_tchar_t *aPath);

    // shell context menu
    xpr_bool_t canInvokeCommandSelf(const xpr_tchar_t *aVerb);
    void       invokeCommandSelf(ContextMenu &aContextMenu, const xpr_tchar_t *aVerb);
    xpr_bool_t OnContextMenuShell(CWnd *aWnd, CPoint aPoint, CRect aWindowRect);
    xpr_bool_t OnContextMenuShellBkgnd(CWnd *aWnd, CPoint aPoint, CRect aWindowRect);
    xpr_bool_t OnContextMenuParent(CWnd *aWnd, CPoint aPoint, CRect aWindowRect);
    xpr_bool_t OnContextMenuDrive(CWnd *aWnd, CPoint aPoint, CRect aWindowRect, DWORD aDataType);
    xpr_bool_t OnContextMenuBkgnd(CWnd *aWnd, CPoint aPoint, CRect aWindowRect);
    xpr_bool_t OnContextMenuBkgndNone(CWnd *aWnd, CPoint aPoint, CRect aWindowRect);
    xpr_bool_t OnContextMenuHeader(CWnd *aWnd, CPoint aPoint, CRect aWindowRect);

    // item execution
    void       executeSelFolder(LPSHELLFOLDER aShellFolder, LPITEMIDLIST aPidl);
    xpr_sint_t executeLink(LPSHELLFOLDER aShellFolder, LPITEMIDLIST aPidl);
    xpr_sint_t executeLinkFolder(LPITEMIDLIST aPidl);
    void       executeLinkFile(LPSHELLFOLDER aShellFolder, LPITEMIDLIST aPidl);
    void       executeUrl(LPSHELLFOLDER aShellFolder, LPITEMIDLIST aPidl);
    void       executeFile(LPSHELLFOLDER aShellFolder, LPITEMIDLIST aPidl, xpr_ulong_t aShellAttributes, xpr_tchar_t *aStartLocation = XPR_NULL, xpr_bool_t aParameter = XPR_FALSE);
    void       doExecuteError(const xpr_tchar_t *aPath);
    void       doExecuteError(LPSHELLFOLDER aShellFolder, LPCITEMIDLIST aPidl);

    struct DefColumnInfo;

    void          loadFolderLayout(void);
    void          deleteAllColumns(void);
    void          loadFolderLayout(xpr_sint_t &aDefColumnCount, DefColumnInfo **aDefColumnInfo);
    FolderLayout *newFolderLayout(void);
    FolderLayout *newFolderLayout(FolderType aFolderType);
    void          getDefColumnInfo(xpr_sint_t &aDefColumnCount, DefColumnInfo **aDefColumnInfo) const;

    void insertNameHash(LPLVITEMDATA aLvItemData);
    void eraseNameHash(LPLVITEMDATA aLvItemData);
    void clearNameHash(void);

protected:
    static xpr_sint_t mRefCount;
    static xpr_uint_t mCodeMgr;
    xpr_uint_t mCode;
    xpr_uint_t mSignature;

    xpr_sint_t mViewIndex;

    ExplorerCtrlObserver *mObserver;

    // option
    Option       mOption;
    Option      *mNewOption;

    // folder information
    LPTVITEMDATA mTvItemData;    // current folder data
    xpr::string  mCurFullPath;   // virtual full path
    xpr::string  mCurPath;       // file system path
    FolderType   mFolderType;
    xpr::string  mSubFolder;
    xpr_bool_t   mSorted;
    xpr_bool_t   mUpdated;
    xpr_bool_t   mVisible;
    xpr_bool_t   mFirstExplore;

    // history
    xpr_bool_t  mGo;
    History    *mHistory;

    // shell change notify
    xpr_uint_t                       mShcnId;
    FileChangeWatcher::WatchId       mWatchId;
    AdvFileChangeWatcher::AdvWatchId mAdvWatchId;
    xpr_bool_t                       mNotify;

    typedef std::tr1::unordered_multimap<xpr::string, LPLVITEMDATA> NameMap;
    typedef std::pair<NameMap::iterator, NameMap::iterator> NameMapPairIterator;
    typedef std::pair<NameMap::const_iterator, NameMap::const_iterator> NameMapPairConstIterator;
    NameMap mNameMap;

    // edit and insert
    xpr::string   mInsSel;           // Create Folder/Text File and Edit
    xpr_bool_t    mInsUnsellAll;
    xpr_bool_t    mInsSelEdit;
    RenameHelper *mRenameHelper;
    xpr_bool_t    mMouseEdit;
    xpr_bool_t    mRenameResorting;

    // drag and drop
    xpr_bool_t mIntraDrag;
    xpr_sint_t mDropIndex;
    xpr_bool_t mScrollUpTimer;
    xpr_bool_t mScrollDownTimer;
    xpr_bool_t mScrollLeftTimer;
    xpr_bool_t mScrollRightTimer;
    DropTarget mDropTarget;

    ShellIcon  *mShellIcon;
    xpr_size_t  mRealSelCount;
    xpr_size_t  mRealSelFolderCount;
    xpr_size_t  mRealSelFileCount;

    LPITEMIDLIST mCopyFullPidl;
    LPITEMIDLIST mMoveFullPidl;

    // folder layout
    xpr_sint_t    mViewStyle;
    FolderLayout *mFolderLayout;
    FolderLayout *mDefaultFolderLayout;
    FolderLayout *mComputerFolderLayout;
    FolderLayout *mVirtualFolderLayout;

    CImageList *mExtraLargeImgList;
    CImageList *mLargeImgList;
    CImageList *mMediumImgList;
    CImageList *mNormalImgList;
    CImageList *mSmallImgList;
    CFont      *mCustomFont;

    struct SortData;
    ColumnId   mSortColumnId;
    xpr_bool_t mSortAscending;
    xpr_sint_t mSortColumn;

    static DefColumnInfo mDefColumnDefault[];  // Column Default
    static DefColumnInfo mDefColumnComputer[]; // Column Computer
    static DefColumnInfo mDefColumnVirtual[];  // Column Virtual

    DefColumnInfo *mDefColumnInfo;
    xpr_sint_t     mDefColumnCount;

protected:
    virtual xpr_bool_t PreCreateWindow(CREATESTRUCT &aCreateStruct);
    virtual xpr_bool_t PreTranslateMessage(MSG *aMsg);
    virtual xpr_bool_t OnNotify(WPARAM wParam, LPARAM lParam, LRESULT *aResult);

    void OnCustomdrawThumbnail(LPNMLVCUSTOMDRAW pLVCD);

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg xpr_sint_t OnCreate(LPCREATESTRUCT aCreateStruct);
    afx_msg void OnLButtonDblClk(xpr_uint_t aFlags, CPoint aPoint);
    afx_msg void OnKeyDown(xpr_uint_t aChar, xpr_uint_t aRepCnt, xpr_uint_t aFlags);
    afx_msg void OnContextMenu(CWnd *aWnd, CPoint aPoint);
    afx_msg void OnSetFocus(CWnd *aOldWnd);
    afx_msg void OnKeyUp(xpr_uint_t aChar, xpr_uint_t aRepCnt, xpr_uint_t aFlags);
    afx_msg void OnKillFocus(CWnd *aNewWnd);
    afx_msg void OnSysColorChange(void);
    afx_msg void OnDestroy(void);
    afx_msg void OnTimer(UINT_PTR aIdEvent);
    afx_msg xpr_bool_t OnEraseBkgnd(CDC *aDC);
    afx_msg xpr_sint_t  OnMouseActivate(CWnd *aDesktopWnd, xpr_uint_t aHitTest, xpr_uint_t aMessage);
    afx_msg void OnLButtonDown(xpr_uint_t aFlags, CPoint aPoint);
    afx_msg void OnMButtonDown(xpr_uint_t aFlags, CPoint aPoint);
    afx_msg void OnRButtonDown(xpr_uint_t aFlags, CPoint aPoint);
    afx_msg void OnRButtonUp(xpr_uint_t aFlags, CPoint aPoint);
    afx_msg void OnLButtonUp(xpr_uint_t aFlags, CPoint aPoint);
    afx_msg void OnMouseMove(xpr_uint_t aFlags, CPoint aPoint);
    afx_msg void OnVScroll(xpr_uint_t aSBCode, xpr_uint_t aPos, CScrollBar *aScrollBar);
    afx_msg void OnGetdispinfo(NMHDR *aNmHdr, LRESULT *aResult);
    afx_msg void OnColumnclick(NMHDR *aNmHdr, LRESULT *aResult);
    afx_msg void OnDblclk(NMHDR *aNmHdr, LRESULT *aResult);
    afx_msg void OnBegindrag(NMHDR *aNmHdr, LRESULT *aResult);
    afx_msg void OnBeginlabeledit(NMHDR *aNmHdr, LRESULT *aResult);
    afx_msg void OnEndlabeledit(NMHDR *aNmHdr, LRESULT *aResult);
    afx_msg void OnDeleteitem(NMHDR *aNmHdr, LRESULT *aResult);
    afx_msg void OnClick(NMHDR *aNmHdr, LRESULT *aResult);
    afx_msg void OnItemchanged(NMHDR *aNmHdr, LRESULT *aResult);
    afx_msg void OnDeleteallitems(NMHDR *aNmHdr, LRESULT *aResult);
    afx_msg void OnBeginrdrag(NMHDR *aNmHdr, LRESULT *aResult);
    afx_msg void OnLvnKeyDown(NMHDR *aNmHdr, LRESULT *aResult);
    afx_msg void OnSetdispinfo(NMHDR *aNmHdr, LRESULT *aResult);
    afx_msg void OnInsertitem(NMHDR *aNmHdr, LRESULT *aResult);
    afx_msg void OnMarqueebegin(NMHDR *aNmHdr, LRESULT *aResult);
    afx_msg void OnCustomdraw(NMHDR *aNmHdr, LRESULT *aResult);
    afx_msg void OnGetinfotip(NMHDR *aNmHdr, LRESULT *aResult);
    afx_msg void OnItemActivate(NMHDR *aNmHdr, LRESULT *aResult);
    afx_msg void OnHdnItemChanged(NMHDR *aNmHdr);
    afx_msg LRESULT OnDriveShellChangeNotify(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnShellChangeNotify(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnFileChangeNotify(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnAdvFileChangeNotify(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnThumbnailProc(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnShellAsyncIcon(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnShellColumnProc(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnPasteSelItem(WPARAM wParam, LPARAM lParam);
};
} // namespace fxfile

#endif // __FXFILE_EXPLORER_CTRL_H__
