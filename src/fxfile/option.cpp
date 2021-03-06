//
// Copyright (c) 2001-2014 Leon Lee author. All rights reserved.
//
//   homepage: http://www.flychk.com
//   e-mail:   mailto:flychk@flychk.com
//
// Use of this source code is governed by a GPLv3 license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "option.h"

#include "conf_file_ex.h"
#include "bookmark.h"
#include "size_format.h"
#include "thumbnail.h"
#include "history.h"
#include "folder_layout.h"
#include "folder_layout_manager.h"

#include "app_ver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace fxfile;
using namespace fxfile::base;

namespace fxfile
{
namespace
{
const xpr_tchar_t kMainSection               [] = XPR_STRING_LITERAL("main");
const xpr_tchar_t kConfigSection             [] = XPR_STRING_LITERAL("config");

const xpr_tchar_t kVersionKey                [] = XPR_STRING_LITERAL("version");
const xpr_tchar_t kCurTabKey                 [] = XPR_STRING_LITERAL("main.view%d.current_tab");
const xpr_tchar_t kTabFolderLayoutDefaultKey [] = XPR_STRING_LITERAL("main.view%d.tab%d.folder_layout.default");
const xpr_tchar_t kTabFolderLayoutComputerKey[] = XPR_STRING_LITERAL("main.view%d.tab%d.folder_layout.computer");
const xpr_tchar_t kTabFolderLayoutVirtualKey [] = XPR_STRING_LITERAL("main.view%d.tab%d.folder_layout.virtual");
const xpr_tchar_t kTabPathKey                [] = XPR_STRING_LITERAL("main.view%d.tab%d.path");
const xpr_tchar_t kBackwardKey               [] = XPR_STRING_LITERAL("main.view%d.tab%d.backward.path%d");
const xpr_tchar_t kForwardKey                [] = XPR_STRING_LITERAL("main.view%d.tab%d.forward.path%d");
const xpr_tchar_t kHistoryKey                [] = XPR_STRING_LITERAL("main.view%d.tab%d.history.path%d");
} // namespace anonymous

struct OptionKey
{
    enum Type
    {
        TypeBoolean,
        TypeInteger,
        TypeDouble,
        TypeString,
        TypeRect,
        TypeColor,
    };

    const xpr_tchar_t *mKey;
    Type               mType;
    void              *mValue;
    void              *mDefValue;
};

static void loadOptionKeys(fxfile::base::ConfFileEx &aConfFile, ConfFile::Section *aSection, const OptionKey *aOptionKeys, xpr_sint_t aKeyCount);
static void saveOptionKeys(fxfile::base::ConfFileEx &aConfFile, ConfFile::Section *aSection, const OptionKey *aOptionKeys, xpr_sint_t aKeyCount);

static const OptionKey gMainOptionKeys[] =
{
    { XPR_STRING_LITERAL("main.window.position"),                              OptionKey::TypeRect,    &Option::mMain.mWindowRect,                     (void *)new CRect(-1,-1,-1,-1)         },
    { XPR_STRING_LITERAL("main.window.status"),                                OptionKey::TypeInteger, &Option::mMain.mWindowStatus,                   (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("main.folder_tree.mode"),                             OptionKey::TypeBoolean, &Option::mMain.mSingleFolderPaneMode,           (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("main.single_folder_tree.show"),                      OptionKey::TypeBoolean, &Option::mMain.mShowSingleFolderPane,           (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("main.single_folder_tree.size"),                      OptionKey::TypeInteger, &Option::mMain.mSingleFolderPaneSize,           (void *)DEF_FOLDER_PANE_SIZE           },
    { XPR_STRING_LITERAL("main.single_folder_tree.left_side"),                 OptionKey::TypeBoolean, &Option::mMain.mLeftSingleFolderPane,           (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("main.view1.folder_tree.show"),                       OptionKey::TypeBoolean, &Option::mMain.mShowEachFolderPane[0],          (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("main.view2.folder_tree.show"),                       OptionKey::TypeBoolean, &Option::mMain.mShowEachFolderPane[1],          (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("main.view3.folder_tree.show"),                       OptionKey::TypeBoolean, &Option::mMain.mShowEachFolderPane[2],          (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("main.view4.folder_tree.show"),                       OptionKey::TypeBoolean, &Option::mMain.mShowEachFolderPane[3],          (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("main.view5.folder_tree.show"),                       OptionKey::TypeBoolean, &Option::mMain.mShowEachFolderPane[4],          (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("main.view6.folder_tree.show"),                       OptionKey::TypeBoolean, &Option::mMain.mShowEachFolderPane[5],          (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("main.view1.folder_tree.size"),                       OptionKey::TypeInteger, &Option::mMain.mEachFolderPaneSize[0],          (void *)DEF_FOLDER_PANE_SIZE           },
    { XPR_STRING_LITERAL("main.view2.folder_tree.size"),                       OptionKey::TypeInteger, &Option::mMain.mEachFolderPaneSize[1],          (void *)DEF_FOLDER_PANE_SIZE           },
    { XPR_STRING_LITERAL("main.view3.folder_tree.size"),                       OptionKey::TypeInteger, &Option::mMain.mEachFolderPaneSize[2],          (void *)DEF_FOLDER_PANE_SIZE           },
    { XPR_STRING_LITERAL("main.view4.folder_tree.size"),                       OptionKey::TypeInteger, &Option::mMain.mEachFolderPaneSize[3],          (void *)DEF_FOLDER_PANE_SIZE           },
    { XPR_STRING_LITERAL("main.view5.folder_tree.size"),                       OptionKey::TypeInteger, &Option::mMain.mEachFolderPaneSize[4],          (void *)DEF_FOLDER_PANE_SIZE           },
    { XPR_STRING_LITERAL("main.view6.folder_tree.size"),                       OptionKey::TypeInteger, &Option::mMain.mEachFolderPaneSize[5],          (void *)DEF_FOLDER_PANE_SIZE           },
    { XPR_STRING_LITERAL("main.view1.folder_tree.left_side"),                  OptionKey::TypeBoolean, &Option::mMain.mLeftEachFolderPane[0],          (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("main.view2.folder_tree.left_side"),                  OptionKey::TypeBoolean, &Option::mMain.mLeftEachFolderPane[1],          (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("main.view3.folder_tree.left_side"),                  OptionKey::TypeBoolean, &Option::mMain.mLeftEachFolderPane[2],          (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("main.view4.folder_tree.left_side"),                  OptionKey::TypeBoolean, &Option::mMain.mLeftEachFolderPane[3],          (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("main.view5.folder_tree.left_side"),                  OptionKey::TypeBoolean, &Option::mMain.mLeftEachFolderPane[4],          (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("main.view6.folder_tree.left_side"),                  OptionKey::TypeBoolean, &Option::mMain.mLeftEachFolderPane[5],          (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("main.bookmark.show_text"),                           OptionKey::TypeBoolean, &Option::mMain.mBookmarkBarText,                (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("main.bookmark.multiple_line"),                       OptionKey::TypeBoolean, &Option::mMain.mBookmarkBarMultiLine,           (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("main.drive_bar.show"),                               OptionKey::TypeBoolean, &Option::mMain.mDriveBar,                       (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("main.drive_bar.short_text"),                         OptionKey::TypeBoolean, &Option::mMain.mDriveBarShortText,              (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("main.drive_bar.multiple_line"),                      OptionKey::TypeBoolean, &Option::mMain.mDriveBarMultiLine,              (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("main.view.row_count"),                               OptionKey::TypeInteger, &Option::mMain.mViewSplitRowCount,              (void *)DEF_VIEW_SPLIT_ROW             },
    { XPR_STRING_LITERAL("main.view.column_count"),                            OptionKey::TypeInteger, &Option::mMain.mViewSplitColumnCount,           (void *)DEF_VIEW_SPLIT_COLUMN          },
    { XPR_STRING_LITERAL("main.view.ratio1"),                                  OptionKey::TypeDouble,  &Option::mMain.mViewSplitRatio[0],              (void *)new double(0.0)                },
    { XPR_STRING_LITERAL("main.view.ratio2"),                                  OptionKey::TypeDouble,  &Option::mMain.mViewSplitRatio[1],              (void *)new double(0.0)                },
    { XPR_STRING_LITERAL("main.view.ratio3"),                                  OptionKey::TypeDouble,  &Option::mMain.mViewSplitRatio[2],              (void *)new double(0.0)                },
    { XPR_STRING_LITERAL("main.view.size1"),                                   OptionKey::TypeInteger, &Option::mMain.mViewSplitSize[0],               (void *)0                              },
    { XPR_STRING_LITERAL("main.view.size2"),                                   OptionKey::TypeInteger, &Option::mMain.mViewSplitSize[1],               (void *)0                              },
    { XPR_STRING_LITERAL("main.view.size3"),                                   OptionKey::TypeInteger, &Option::mMain.mViewSplitSize[2],               (void *)0                              },
    { XPR_STRING_LITERAL("main.working_folder.#1"),                            OptionKey::TypeString,   Option::mMain.mWorkingFolder[0],               (void *)XPR_STRING_LITERAL("")         },
    { XPR_STRING_LITERAL("main.working_folder.#2"),                            OptionKey::TypeString,   Option::mMain.mWorkingFolder[1],               (void *)XPR_STRING_LITERAL("")         },
    { XPR_STRING_LITERAL("main.working_folder.#3"),                            OptionKey::TypeString,   Option::mMain.mWorkingFolder[2],               (void *)XPR_STRING_LITERAL("")         },
    { XPR_STRING_LITERAL("main.working_folder.#4"),                            OptionKey::TypeString,   Option::mMain.mWorkingFolder[3],               (void *)XPR_STRING_LITERAL("")         },
    { XPR_STRING_LITERAL("main.working_folder.#5"),                            OptionKey::TypeString,   Option::mMain.mWorkingFolder[4],               (void *)XPR_STRING_LITERAL("")         },
    { XPR_STRING_LITERAL("main.file_scrap.floating.show"),                     OptionKey::TypeBoolean, &Option::mMain.mFileScrapDrop,                  (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("main.picture_viewer.show"),                          OptionKey::TypeBoolean, &Option::mMain.mPictureViewer,                  (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("main.tip_of_today.show"),                            OptionKey::TypeBoolean, &Option::mMain.mTipOfTheToday,                  (void *)XPR_TRUE                       },
};

static const OptionKey gConfigOptionKeys[] =
{
    { XPR_STRING_LITERAL("config.mouse_click"),                                OptionKey::TypeInteger, &Option::mConfig.mMouseClick,                   (void *)MOUSE_DOUBLE_CLICK             },
    { XPR_STRING_LITERAL("config.show_system_file"),                           OptionKey::TypeBoolean, &Option::mConfig.mShowSystemAttribute,          (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.title_full_path"),                            OptionKey::TypeBoolean, &Option::mConfig.mTitleFullPath,                (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.address_full_path"),                          OptionKey::TypeBoolean, &Option::mConfig.mAddressFullPath,              (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.tooltip"),                                    OptionKey::TypeBoolean, &Option::mConfig.mTooltip,                      (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("config.tooltip_with_file_name"),                     OptionKey::TypeBoolean, &Option::mConfig.mTooltipWithFileName,          (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.file_extension_type"),                        OptionKey::TypeInteger, &Option::mConfig.mFileExtType,                  (void *)0                              },
    { XPR_STRING_LITERAL("config.new_item_popup"),                             OptionKey::TypeBoolean, &Option::mConfig.mNewItemDlg,                   (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("config.single_rename_type"),                         OptionKey::TypeBoolean, &Option::mConfig.mSingleRenameType,             (void *)SINGLE_RENAME_TYPE_DEFAULT     },
    { XPR_STRING_LITERAL("config.show_hidden_file"),                           OptionKey::TypeBoolean, &Option::mConfig.mShowHiddenAttribute,          (void *)XPR_FALSE                      },

    { XPR_STRING_LITERAL("config.rename.by_mouse"),                            OptionKey::TypeBoolean, &Option::mConfig.mRenameByMouse,                (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("config.rename.extension_type"),                      OptionKey::TypeInteger, &Option::mConfig.mRenameExtType,                (void *)RENAME_EXT_TYPE_DEFAULT        },
    { XPR_STRING_LITERAL("config.rename.batch_rename_multiple_select"),        OptionKey::TypeBoolean, &Option::mConfig.mBatchRenameMultiSel,          (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("config.rename.batch_rename_with_folder"),            OptionKey::TypeBoolean, &Option::mConfig.mBatchRenameWithFolder,        (void *)XPR_FALSE                      },

    { XPR_STRING_LITERAL("config.address_bar.show"),                           OptionKey::TypeBoolean, &Option::mConfig.mShowAddressBar,               (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("config.address_bar.url_compatible"),                 OptionKey::TypeBoolean, &Option::mConfig.mAddressBarUrl,                (void *)XPR_FALSE                      },

    { XPR_STRING_LITERAL("config.path_bar.show"),                              OptionKey::TypeBoolean, &Option::mConfig.mShowPathBar,                  (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.path_bar.real_path"),                         OptionKey::TypeBoolean, &Option::mConfig.mPathBarRealPath,              (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.path_bar.icon"),                              OptionKey::TypeBoolean, &Option::mConfig.mPathBarIcon,                  (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.path_bar.highlight"),                         OptionKey::TypeBoolean, &Option::mConfig.mPathBarHighlight,             (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("config.path_bar.highlight_color"),                   OptionKey::TypeColor,   &Option::mConfig.mPathBarHighlightColor,        (void *)(xpr_sintptr_t)DEF_PATH_BAR_HIGHLIGHT_COLOR },

    { XPR_STRING_LITERAL("config.drive_bar.show"),                             OptionKey::TypeBoolean, &Option::mConfig.mShowEachDriveBar,             (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.drive_bar.left_side"),                        OptionKey::TypeBoolean, &Option::mConfig.mDriveBarLeftSide,             (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.drive_bar.short_text"),                       OptionKey::TypeBoolean, &Option::mConfig.mDriveBarShortText,            (void *)XPR_TRUE                       },

    { XPR_STRING_LITERAL("config.activate_bar.show"),                          OptionKey::TypeBoolean, &Option::mConfig.mShowActivateBar,              (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("config.activate_bar.active_color"),                  OptionKey::TypeColor,   &Option::mConfig.mActiveViewColor,              (void *)DEF_ACTIVED_VIEW_COLOR         },

    { XPR_STRING_LITERAL("config.status_bar.show"),                            OptionKey::TypeBoolean, &Option::mConfig.mShowStatusBar,                (void *)XPR_TRUE                       },

    { XPR_STRING_LITERAL("config.info_bar.show"),                              OptionKey::TypeBoolean, &Option::mConfig.mShowInfoBar,                  (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("config.info_bar.style"),                             OptionKey::TypeInteger, &Option::mConfig.mContentsStyle,                (void *)CONTENTS_EXPLORER              },
    { XPR_STRING_LITERAL("config.info_bar.show_bookmark"),                     OptionKey::TypeBoolean, &Option::mConfig.mContentsBookmark,             (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("config.info_bar.bookmark_color"),                    OptionKey::TypeColor,   &Option::mConfig.mContentsBookmarkColor,        (void *)DEF_INFO_BAR_BOOKMARK_COLOR    },
    { XPR_STRING_LITERAL("config.info_bar.no_display_detailed_info"),          OptionKey::TypeBoolean, &Option::mConfig.mContentsNoDispDetailedInfo,   (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.info_bar.show_ARHS_file_attributes"),         OptionKey::TypeBoolean, &Option::mConfig.mContentsARHSAttribute,        (void *)XPR_FALSE                      },

    { XPR_STRING_LITERAL("config.bookmark.tooltip"),                           OptionKey::TypeBoolean, &Option::mConfig.mBookmarkTooltip,              (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("config.bookmark.expand_folder"),                     OptionKey::TypeBoolean, &Option::mConfig.mBookmarkExpandFolder,         (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.bookmark.real_path"),                         OptionKey::TypeBoolean, &Option::mConfig.mBookmarkRealPath,             (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.bookmark.fast_network_icon"),                 OptionKey::TypeBoolean, &Option::mConfig.mBookmarkFastNetIcon,          (void *)XPR_TRUE                       },

    { XPR_STRING_LITERAL("config.view1.folder_tree.background_color_type"),    OptionKey::TypeInteger, &Option::mConfig.mFolderTreeBkgndColorType[0],  (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view2.folder_tree.background_color_type"),    OptionKey::TypeInteger, &Option::mConfig.mFolderTreeBkgndColorType[1],  (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view3.folder_tree.background_color_type"),    OptionKey::TypeInteger, &Option::mConfig.mFolderTreeBkgndColorType[2],  (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view4.folder_tree.background_color_type"),    OptionKey::TypeInteger, &Option::mConfig.mFolderTreeBkgndColorType[3],  (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view5.folder_tree.background_color_type"),    OptionKey::TypeInteger, &Option::mConfig.mFolderTreeBkgndColorType[4],  (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view6.folder_tree.background_color_type"),    OptionKey::TypeInteger, &Option::mConfig.mFolderTreeBkgndColorType[5],  (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view1.folder_tree.background_color"),         OptionKey::TypeColor,   &Option::mConfig.mFolderTreeBkgndColor[0],      (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOW)      },
    { XPR_STRING_LITERAL("config.view2.folder_tree.background_color"),         OptionKey::TypeColor,   &Option::mConfig.mFolderTreeBkgndColor[1],      (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOW)      },
    { XPR_STRING_LITERAL("config.view3.folder_tree.background_color"),         OptionKey::TypeColor,   &Option::mConfig.mFolderTreeBkgndColor[2],      (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOW)      },
    { XPR_STRING_LITERAL("config.view4.folder_tree.background_color"),         OptionKey::TypeColor,   &Option::mConfig.mFolderTreeBkgndColor[3],      (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOW)      },
    { XPR_STRING_LITERAL("config.view5.folder_tree.background_color"),         OptionKey::TypeColor,   &Option::mConfig.mFolderTreeBkgndColor[4],      (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOW)      },
    { XPR_STRING_LITERAL("config.view6.folder_tree.background_color"),         OptionKey::TypeColor,   &Option::mConfig.mFolderTreeBkgndColor[5],      (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOW)      },
    { XPR_STRING_LITERAL("config.view1.folder_tree.text_color_type"),          OptionKey::TypeInteger, &Option::mConfig.mFolderTreeTextColorType[0],   (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view2.folder_tree.text_color_type"),          OptionKey::TypeInteger, &Option::mConfig.mFolderTreeTextColorType[1],   (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view3.folder_tree.text_color_type"),          OptionKey::TypeInteger, &Option::mConfig.mFolderTreeTextColorType[2],   (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view4.folder_tree.text_color_type"),          OptionKey::TypeInteger, &Option::mConfig.mFolderTreeTextColorType[3],   (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view5.folder_tree.text_color_type"),          OptionKey::TypeInteger, &Option::mConfig.mFolderTreeTextColorType[4],   (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view6.folder_tree.text_color_type"),          OptionKey::TypeInteger, &Option::mConfig.mFolderTreeTextColorType[5],   (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view1.folder_tree.text_color"),               OptionKey::TypeColor,   &Option::mConfig.mFolderTreeTextColor[0],       (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOWTEXT)  },
    { XPR_STRING_LITERAL("config.view2.folder_tree.text_color"),               OptionKey::TypeColor,   &Option::mConfig.mFolderTreeTextColor[1],       (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOWTEXT)  },
    { XPR_STRING_LITERAL("config.view3.folder_tree.text_color"),               OptionKey::TypeColor,   &Option::mConfig.mFolderTreeTextColor[2],       (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOWTEXT)  },
    { XPR_STRING_LITERAL("config.view4.folder_tree.text_color"),               OptionKey::TypeColor,   &Option::mConfig.mFolderTreeTextColor[3],       (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOWTEXT)  },
    { XPR_STRING_LITERAL("config.view5.folder_tree.text_color"),               OptionKey::TypeColor,   &Option::mConfig.mFolderTreeTextColor[4],       (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOWTEXT)  },
    { XPR_STRING_LITERAL("config.view6.folder_tree.text_color"),               OptionKey::TypeColor,   &Option::mConfig.mFolderTreeTextColor[5],       (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOWTEXT)  },
    { XPR_STRING_LITERAL("config.view1.folder_tree.inactive_highlight"),       OptionKey::TypeBoolean, &Option::mConfig.mFolderTreeHighlight[0],       (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.view2.folder_tree.inactive_highlight"),       OptionKey::TypeBoolean, &Option::mConfig.mFolderTreeHighlight[1],       (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.view3.folder_tree.inactive_highlight"),       OptionKey::TypeBoolean, &Option::mConfig.mFolderTreeHighlight[2],       (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.view4.folder_tree.inactive_highlight"),       OptionKey::TypeBoolean, &Option::mConfig.mFolderTreeHighlight[3],       (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.view5.folder_tree.inactive_highlight"),       OptionKey::TypeBoolean, &Option::mConfig.mFolderTreeHighlight[4],       (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.view6.folder_tree.inactive_highlight"),       OptionKey::TypeBoolean, &Option::mConfig.mFolderTreeHighlight[5],       (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.view1.folder_tree.inactive_highlight_color"), OptionKey::TypeColor,   &Option::mConfig.mFolderTreeHighlightColor[0],  (void *)(xpr_sintptr_t)GetSysColor(COLOR_HIGHLIGHT)   },
    { XPR_STRING_LITERAL("config.view2.folder_tree.inactive_highlight_color"), OptionKey::TypeColor,   &Option::mConfig.mFolderTreeHighlightColor[1],  (void *)(xpr_sintptr_t)GetSysColor(COLOR_HIGHLIGHT)   },
    { XPR_STRING_LITERAL("config.view3.folder_tree.inactive_highlight_color"), OptionKey::TypeColor,   &Option::mConfig.mFolderTreeHighlightColor[2],  (void *)(xpr_sintptr_t)GetSysColor(COLOR_HIGHLIGHT)   },
    { XPR_STRING_LITERAL("config.view4.folder_tree.inactive_highlight_color"), OptionKey::TypeColor,   &Option::mConfig.mFolderTreeHighlightColor[3],  (void *)(xpr_sintptr_t)GetSysColor(COLOR_HIGHLIGHT)   },
    { XPR_STRING_LITERAL("config.view5.folder_tree.inactive_highlight_color"), OptionKey::TypeColor,   &Option::mConfig.mFolderTreeHighlightColor[4],  (void *)(xpr_sintptr_t)GetSysColor(COLOR_HIGHLIGHT)   },
    { XPR_STRING_LITERAL("config.view6.folder_tree.inactive_highlight_color"), OptionKey::TypeColor,   &Option::mConfig.mFolderTreeHighlightColor[5],  (void *)(xpr_sintptr_t)GetSysColor(COLOR_HIGHLIGHT)   },
    { XPR_STRING_LITERAL("config.folder_tree.linkage"),                        OptionKey::TypeBoolean, &Option::mConfig.mFolderTreeLinkage,            (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.folder_tree.single_linkage"),                 OptionKey::TypeBoolean, &Option::mConfig.mFolderTreeLinkageOnSingle,    (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.folder_tree.item_height"),                    OptionKey::TypeBoolean, &Option::mConfig.mFolderTreeIsItemHeight,       (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.folder_tree.item_height_size"),               OptionKey::TypeInteger, &Option::mConfig.mFolderTreeItemHeight,         (void *)0                              },
    { XPR_STRING_LITERAL("config.folder_tree.custom_font"),                    OptionKey::TypeBoolean, &Option::mConfig.mFolderTreeCustomFont,         (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.folder_tree.custom_font_name"),               OptionKey::TypeString,   Option::mConfig.mFolderTreeCustomFontText,     (void *)XPR_STRING_LITERAL("")         },
    { XPR_STRING_LITERAL("config.folder_tree.init_no_expand"),                 OptionKey::TypeBoolean, &Option::mConfig.mFolderTreeInitNoExpand,       (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.folder_tree.select_delay"),                   OptionKey::TypeBoolean, &Option::mConfig.mFolderTreeSelDelay,           (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.folder_tree.select_delay_time"),              OptionKey::TypeInteger, &Option::mConfig.mFolderTreeSelDelayTime,       (void *)DEF_FLD_SEL_DELAY_MSEC         },
    { XPR_STRING_LITERAL("config.folder_tree.theme_style"),                    OptionKey::TypeBoolean, &Option::mConfig.mFolderTreeClassicThemeStyle,  (void *)XPR_FALSE                      },

    { XPR_STRING_LITERAL("config.view1.file_list.background_image"),           OptionKey::TypeBoolean, &Option::mConfig.mFileListBkgndImage[0],        (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.view2.file_list.background_image"),           OptionKey::TypeBoolean, &Option::mConfig.mFileListBkgndImage[1],        (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.view3.file_list.background_image"),           OptionKey::TypeBoolean, &Option::mConfig.mFileListBkgndImage[2],        (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.view4.file_list.background_image"),           OptionKey::TypeBoolean, &Option::mConfig.mFileListBkgndImage[3],        (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.view5.file_list.background_image"),           OptionKey::TypeBoolean, &Option::mConfig.mFileListBkgndImage[4],        (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.view6.file_list.background_image"),           OptionKey::TypeBoolean, &Option::mConfig.mFileListBkgndImage[5],        (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.view1.file_list.background_image_path"),      OptionKey::TypeString,   Option::mConfig.mFileListBkgndImagePath[0],    (void *)XPR_STRING_LITERAL("")         },
    { XPR_STRING_LITERAL("config.view2.file_list.background_image_path"),      OptionKey::TypeString,   Option::mConfig.mFileListBkgndImagePath[1],    (void *)XPR_STRING_LITERAL("")         },
    { XPR_STRING_LITERAL("config.view3.file_list.background_image_path"),      OptionKey::TypeString,   Option::mConfig.mFileListBkgndImagePath[2],    (void *)XPR_STRING_LITERAL("")         },
    { XPR_STRING_LITERAL("config.view4.file_list.background_image_path"),      OptionKey::TypeString,   Option::mConfig.mFileListBkgndImagePath[3],    (void *)XPR_STRING_LITERAL("")         },
    { XPR_STRING_LITERAL("config.view5.file_list.background_image_path"),      OptionKey::TypeString,   Option::mConfig.mFileListBkgndImagePath[4],    (void *)XPR_STRING_LITERAL("")         },
    { XPR_STRING_LITERAL("config.view6.file_list.background_image_path"),      OptionKey::TypeString,   Option::mConfig.mFileListBkgndImagePath[5],    (void *)XPR_STRING_LITERAL("")         },
    { XPR_STRING_LITERAL("config.view1.file_list.background_color_type"),      OptionKey::TypeInteger, &Option::mConfig.mFileListBkgndColorType[0],    (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view2.file_list.background_color_type"),      OptionKey::TypeInteger, &Option::mConfig.mFileListBkgndColorType[1],    (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view3.file_list.background_color_type"),      OptionKey::TypeInteger, &Option::mConfig.mFileListBkgndColorType[2],    (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view4.file_list.background_color_type"),      OptionKey::TypeInteger, &Option::mConfig.mFileListBkgndColorType[3],    (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view5.file_list.background_color_type"),      OptionKey::TypeInteger, &Option::mConfig.mFileListBkgndColorType[4],    (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view6.file_list.background_color_type"),      OptionKey::TypeInteger, &Option::mConfig.mFileListBkgndColorType[5],    (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view1.file_list.background_color"),           OptionKey::TypeColor,   &Option::mConfig.mFileListBkgndColor[0],        (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOW)      },
    { XPR_STRING_LITERAL("config.view2.file_list.background_color"),           OptionKey::TypeColor,   &Option::mConfig.mFileListBkgndColor[1],        (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOW)      },
    { XPR_STRING_LITERAL("config.view3.file_list.background_color"),           OptionKey::TypeColor,   &Option::mConfig.mFileListBkgndColor[2],        (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOW)      },
    { XPR_STRING_LITERAL("config.view4.file_list.background_color"),           OptionKey::TypeColor,   &Option::mConfig.mFileListBkgndColor[3],        (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOW)      },
    { XPR_STRING_LITERAL("config.view5.file_list.background_color"),           OptionKey::TypeColor,   &Option::mConfig.mFileListBkgndColor[4],        (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOW)      },
    { XPR_STRING_LITERAL("config.view6.file_list.background_color"),           OptionKey::TypeColor,   &Option::mConfig.mFileListBkgndColor[5],        (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOW)      },
    { XPR_STRING_LITERAL("config.view1.file_list.text_color_type"),            OptionKey::TypeInteger, &Option::mConfig.mFileListTextColorType[0],     (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view2.file_list.text_color_type"),            OptionKey::TypeInteger, &Option::mConfig.mFileListTextColorType[1],     (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view3.file_list.text_color_type"),            OptionKey::TypeInteger, &Option::mConfig.mFileListTextColorType[2],     (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view4.file_list.text_color_type"),            OptionKey::TypeInteger, &Option::mConfig.mFileListTextColorType[3],     (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view5.file_list.text_color_type"),            OptionKey::TypeInteger, &Option::mConfig.mFileListTextColorType[4],     (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view6.file_list.text_color_type"),            OptionKey::TypeInteger, &Option::mConfig.mFileListTextColorType[5],     (void *)COLOR_TYPE_DEFAULT             },
    { XPR_STRING_LITERAL("config.view1.file_list.text_color"),                 OptionKey::TypeColor,   &Option::mConfig.mFileListTextColor[0],         (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOWTEXT)  },
    { XPR_STRING_LITERAL("config.view2.file_list.text_color"),                 OptionKey::TypeColor,   &Option::mConfig.mFileListTextColor[1],         (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOWTEXT)  },
    { XPR_STRING_LITERAL("config.view3.file_list.text_color"),                 OptionKey::TypeColor,   &Option::mConfig.mFileListTextColor[2],         (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOWTEXT)  },
    { XPR_STRING_LITERAL("config.view4.file_list.text_color"),                 OptionKey::TypeColor,   &Option::mConfig.mFileListTextColor[3],         (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOWTEXT)  },
    { XPR_STRING_LITERAL("config.view5.file_list.text_color"),                 OptionKey::TypeColor,   &Option::mConfig.mFileListTextColor[4],         (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOWTEXT)  },
    { XPR_STRING_LITERAL("config.view6.file_list.text_color"),                 OptionKey::TypeColor,   &Option::mConfig.mFileListTextColor[5],         (void *)(xpr_sintptr_t)GetSysColor(COLOR_WINDOWTEXT)  },
    { XPR_STRING_LITERAL("config.view1.file_list.init_folder"),                OptionKey::TypeInteger, &Option::mConfig.mFileListInitFolderType[0],    (void *)INIT_TYPE_NONE                 },
    { XPR_STRING_LITERAL("config.view2.file_list.init_folder"),                OptionKey::TypeInteger, &Option::mConfig.mFileListInitFolderType[1],    (void *)INIT_TYPE_NONE                 },
    { XPR_STRING_LITERAL("config.view3.file_list.init_folder"),                OptionKey::TypeInteger, &Option::mConfig.mFileListInitFolderType[2],    (void *)INIT_TYPE_NONE                 },
    { XPR_STRING_LITERAL("config.view4.file_list.init_folder"),                OptionKey::TypeInteger, &Option::mConfig.mFileListInitFolderType[3],    (void *)INIT_TYPE_NONE                 },
    { XPR_STRING_LITERAL("config.view5.file_list.init_folder"),                OptionKey::TypeInteger, &Option::mConfig.mFileListInitFolderType[4],    (void *)INIT_TYPE_NONE                 },
    { XPR_STRING_LITERAL("config.view6.file_list.init_folder"),                OptionKey::TypeInteger, &Option::mConfig.mFileListInitFolderType[5],    (void *)INIT_TYPE_NONE                 },
    { XPR_STRING_LITERAL("config.view1.file_list.init_folder_path"),           OptionKey::TypeString,   Option::mConfig.mFileListInitFolder[0],        (void *)XPR_STRING_LITERAL("")         },
    { XPR_STRING_LITERAL("config.view2.file_list.init_folder_path"),           OptionKey::TypeString,   Option::mConfig.mFileListInitFolder[1],        (void *)XPR_STRING_LITERAL("")         },
    { XPR_STRING_LITERAL("config.view3.file_list.init_folder_path"),           OptionKey::TypeString,   Option::mConfig.mFileListInitFolder[2],        (void *)XPR_STRING_LITERAL("")         },
    { XPR_STRING_LITERAL("config.view4.file_list.init_folder_path"),           OptionKey::TypeString,   Option::mConfig.mFileListInitFolder[3],        (void *)XPR_STRING_LITERAL("")         },
    { XPR_STRING_LITERAL("config.view5.file_list.init_folder_path"),           OptionKey::TypeString,   Option::mConfig.mFileListInitFolder[4],        (void *)XPR_STRING_LITERAL("")         },
    { XPR_STRING_LITERAL("config.view6.file_list.init_folder_path"),           OptionKey::TypeString,   Option::mConfig.mFileListInitFolder[5],        (void *)XPR_STRING_LITERAL("")         },
    { XPR_STRING_LITERAL("config.view1.file_list.no_last_network_folder"),     OptionKey::TypeBoolean, &Option::mConfig.mFileListNoNetLastFolder[0],   (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.view2.file_list.no_last_network_folder"),     OptionKey::TypeBoolean, &Option::mConfig.mFileListNoNetLastFolder[1],   (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.view3.file_list.no_last_network_folder"),     OptionKey::TypeBoolean, &Option::mConfig.mFileListNoNetLastFolder[2],   (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.view4.file_list.no_last_network_folder"),     OptionKey::TypeBoolean, &Option::mConfig.mFileListNoNetLastFolder[3],   (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.view5.file_list.no_last_network_folder"),     OptionKey::TypeBoolean, &Option::mConfig.mFileListNoNetLastFolder[4],   (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.view6.file_list.no_last_network_folder"),     OptionKey::TypeBoolean, &Option::mConfig.mFileListNoNetLastFolder[5],   (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.file_list.size_unit"),                        OptionKey::TypeInteger, &Option::mConfig.mFileListSizeUnit,             (void *)SIZE_UNIT_DEFAULT              },
    { XPR_STRING_LITERAL("config.file_list.list_type"),                        OptionKey::TypeInteger, &Option::mConfig.mFileListListType,             (void *)LIST_TYPE_ALL                  },
    { XPR_STRING_LITERAL("config.file_list.show_parent_folder"),               OptionKey::TypeBoolean, &Option::mConfig.mFileListParentFolder,         (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.file_list.up_and_select_sub_folder"),         OptionKey::TypeBoolean, &Option::mConfig.mFileListGoUpSelSubFolder,     (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("config.file_list.custom_icon"),                      OptionKey::TypeBoolean, &Option::mConfig.mFileListCustomIcon,           (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.file_list.no_sort"),                          OptionKey::TypeBoolean, &Option::mConfig.mFileListNoSort,               (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.file_list.24_hour_time"),                     OptionKey::TypeBoolean, &Option::mConfig.mFileList24HourTime,           (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.file_list.2_year_date"),                      OptionKey::TypeBoolean, &Option::mConfig.mFileList2YearDate,            (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.file_list.show_drive"),                       OptionKey::TypeBoolean, &Option::mConfig.mFileListShowDrive,            (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.file_list.show_drive_item"),                  OptionKey::TypeBoolean, &Option::mConfig.mFileListShowDriveItem,        (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.file_list.show_drive_size"),                  OptionKey::TypeBoolean, &Option::mConfig.mFileListShowDriveSize,        (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.file_list.name_case_type"),                   OptionKey::TypeInteger, &Option::mConfig.mFileListNameCaseType,         (void *)NAME_CASE_TYPE_DEFAULT         },
    { XPR_STRING_LITERAL("config.file_list.create_text_file_and_edit"),        OptionKey::TypeBoolean, &Option::mConfig.mFileListCreateAndEditText,    (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.file_list.custom_font"),                      OptionKey::TypeBoolean, &Option::mConfig.mCustomFont,                   (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.file_list.custom_font_name"),                 OptionKey::TypeString,   Option::mConfig.mCustomFontText,               (void *)XPR_STRING_LITERAL("")         },
    { XPR_STRING_LITERAL("config.file_list.auto_column_width"),                OptionKey::TypeBoolean, &Option::mConfig.mFileListAutoColumnWidth,      (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.file_list.default_view_style"),               OptionKey::TypeInteger, &Option::mConfig.mFileListDefaultViewStyle,     (void *)VIEW_STYLE_DETAILS             },
    { XPR_STRING_LITERAL("config.file_list.default_sort"),                     OptionKey::TypeInteger, &Option::mConfig.mFileListDefaultSort,          (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.file_list.default_sort_rrder"),               OptionKey::TypeInteger, &Option::mConfig.mFileListDefaultSortOrder,     (void *)1                              },
    { XPR_STRING_LITERAL("config.file_list.save_folder_layout"),               OptionKey::TypeBoolean, &Option::mConfig.mFileListSaveFolderLayout,     (void *)SAVE_FOLDER_LAYOUT_DEFAULT     },
    { XPR_STRING_LITERAL("config.file_list.verify_folder_layout_on_exit"),     OptionKey::TypeBoolean, &Option::mConfig.mFileListExitVerifyFolderLayout, (void *)XPR_TRUE                     },
    { XPR_STRING_LITERAL("config.file_list.custom_icon_path_16"),              OptionKey::TypeString,   Option::mConfig.mFileListCustomIconFile16,     (void *)XPR_STRING_LITERAL("")         },
    { XPR_STRING_LITERAL("config.file_list.custom_icon_path_32"),              OptionKey::TypeString,   Option::mConfig.mFileListCustomIconFile32,     (void *)XPR_STRING_LITERAL("")         },
    { XPR_STRING_LITERAL("config.file_list.custom_icon_path_48"),              OptionKey::TypeString,   Option::mConfig.mFileListCustomIconFile48,     (void *)XPR_STRING_LITERAL("")         },
    { XPR_STRING_LITERAL("config.file_list.custom_icon_path_256"),             OptionKey::TypeString,   Option::mConfig.mFileListCustomIconFile256,    (void *)XPR_STRING_LITERAL("")         },
    { XPR_STRING_LITERAL("config.file_list.size_unit_single_selected"),        OptionKey::TypeInteger, &Option::mConfig.mSingleSelFileSizeUnit,        (void *)SIZE_UNIT_DEFAULT              },
    { XPR_STRING_LITERAL("config.file_list.size_unit_multiple_selected"),      OptionKey::TypeInteger, &Option::mConfig.mMultiSelFileSizeUnit,         (void *)SIZE_UNIT_DEFAULT              },
    { XPR_STRING_LITERAL("config.file_list.theme_style"),                      OptionKey::TypeBoolean, &Option::mConfig.mFileListClassicThemeStyle,    (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.file_list.grid_lines"),                       OptionKey::TypeBoolean, &Option::mConfig.mFileListGridLines,            (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.file_list.full_row_select"),                  OptionKey::TypeBoolean, &Option::mConfig.mFileListFullRowSelect,        (void *)XPR_FALSE                      },

    { XPR_STRING_LITERAL("config.thumbnail.width"),                            OptionKey::TypeInteger, &Option::mConfig.mThumbnailWidth,               (void *)DEF_THUMB_SIZE                 },
    { XPR_STRING_LITERAL("config.thumbnail.height"),                           OptionKey::TypeInteger, &Option::mConfig.mThumbnailHeight,              (void *)DEF_THUMB_SIZE                 },
    { XPR_STRING_LITERAL("config.thumbnail.save_cache"),                       OptionKey::TypeBoolean, &Option::mConfig.mThumbnailSaveCache,           (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.thumbnail.priority"),                         OptionKey::TypeInteger, &Option::mConfig.mThumbnailPriority,            (void *)THUMBNAIL_PRIORITY_BELOW_NORMAL },
    { XPR_STRING_LITERAL("config.thumbnail.load_by_extension"),                OptionKey::TypeInteger, &Option::mConfig.mThumbnailLoadByExt,           (void *)XPR_TRUE                       },

    { XPR_STRING_LITERAL("config.drive_last_folder"),                          OptionKey::TypeBoolean, &Option::mConfig.mDriveLastFolder,              (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.drive_with_shift_key"),                       OptionKey::TypeBoolean, &Option::mConfig.mDriveShiftKey,                (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.show_shell_new_menu"),                        OptionKey::TypeBoolean, &Option::mConfig.mShellNewMenu,                 (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.export_file_list_to_current_folder"),         OptionKey::TypeBoolean, &Option::mConfig.mFileListCurDir,               (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.register_shell_context_menu"),                OptionKey::TypeBoolean, &Option::mConfig.mRegShellContextMenu,          (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.single_process"),                             OptionKey::TypeBoolean, &Option::mConfig.mSingleProcess,                (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.confirm_to_exit"),                            OptionKey::TypeBoolean, &Option::mConfig.mConfirmExit,                  (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.recent_file"),                                OptionKey::TypeBoolean, &Option::mConfig.mRecentFile,                   (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("config.animation_menu"),                             OptionKey::TypeBoolean, &Option::mConfig.mAnimationMenu,                (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.standard_menu"),                              OptionKey::TypeBoolean, &Option::mConfig.mStandardMenu,                 (void *)XPR_FALSE                      },

    { XPR_STRING_LITERAL("config.tab.switch_to_new_tab"),                      OptionKey::TypeBoolean, &Option::mConfig.mTabSwitchToNewTab,            (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("config.tab.auto_fit"),                               OptionKey::TypeBoolean, &Option::mConfig.mTabAutoFit,                   (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.tab.close_on_double_click"),                  OptionKey::TypeBoolean, &Option::mConfig.mTabCloseOnDoubleClick,        (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("config.tab.show_one_tab"),                           OptionKey::TypeBoolean, &Option::mConfig.mTabShowOneTab,                (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.tab.confirm_to_close"),                       OptionKey::TypeBoolean, &Option::mConfig.mTabConfirmToClose,            (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.tab.confirm_to_close_all"),                   OptionKey::TypeBoolean, &Option::mConfig.mTabConfirmToCloseAll,         (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.tab.near_open_new_tab"),                      OptionKey::TypeBoolean, &Option::mConfig.mTabNearOpenNewTab,            (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.tab.icon"),                                   OptionKey::TypeBoolean, &Option::mConfig.mTabIcon,                      (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.tab.show_drive"),                             OptionKey::TypeBoolean, &Option::mConfig.mTabShowDrive,                 (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.tab.show_new_button"),                        OptionKey::TypeBoolean, &Option::mConfig.mTabShowNewButton,             (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.tab.drag_move"),                              OptionKey::TypeBoolean, &Option::mConfig.mTabDragMove,                  (void *)XPR_TRUE                       },

    { XPR_STRING_LITERAL("config.file_scrap.context_menu"),                    OptionKey::TypeBoolean, &Option::mConfig.mFileScrapContextMenu,         (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("config.file_scrap.save"),                            OptionKey::TypeBoolean, &Option::mConfig.mFileScrapSave,                (void *)XPR_FALSE                      },

    { XPR_STRING_LITERAL("config.clipboard.separator"),                        OptionKey::TypeString,   Option::mConfig.mClipboardSeparator,           (void *)XPR_STRING_LITERAL("\\r\\n")   },

    { XPR_STRING_LITERAL("config.working_folder.real_path"),                   OptionKey::TypeBoolean, &Option::mConfig.mWorkingFolderRealPath,        (void *)XPR_FALSE                      },

    { XPR_STRING_LITERAL("config.view_split.resize_by_ratio"),                 OptionKey::TypeBoolean, &Option::mConfig.mViewSplitByRatio,             (void *)XPR_TRUE                       },
    { XPR_STRING_LITERAL("config.view_split.single_as_actived_view"),          OptionKey::TypeBoolean, &Option::mConfig.mSingleViewSplitAsActivedView, (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.view_split.reopen_last_folder"),              OptionKey::TypeBoolean, &Option::mConfig.mViewSplitReopenLastFolder,    (void *)XPR_FALSE                      },

    { XPR_STRING_LITERAL("config.refresh.no"),                                 OptionKey::TypeBoolean, &Option::mConfig.mNoRefresh,                    (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.refresh.sort"),                               OptionKey::TypeBoolean, &Option::mConfig.mRefreshSort,                  (void *)XPR_FALSE                      },

    { XPR_STRING_LITERAL("config.file_op.external_copy"),                      OptionKey::TypeBoolean, &Option::mConfig.mExternalCopyFileOp,           (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.file_op.external_move"),                      OptionKey::TypeBoolean, &Option::mConfig.mExternalMoveFileOp,           (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.file_op.external_delete"),                    OptionKey::TypeBoolean, &Option::mConfig.mExternalDeleteFileOp,         (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.file_op.complete_flash"),                     OptionKey::TypeBoolean, &Option::mConfig.mFileOpCompleteFlash,          (void *)XPR_FALSE                      },

    { XPR_STRING_LITERAL("config.history.save"),                               OptionKey::TypeBoolean, &Option::mConfig.mSaveHistory,                  (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.history.count"),                              OptionKey::TypeInteger, &Option::mConfig.mHistoryCount,                 (void *)DEF_HISTORY                    },
    { XPR_STRING_LITERAL("config.history.backward_count"),                     OptionKey::TypeInteger, &Option::mConfig.mBackwardCount,                (void *)DEF_BACKWARD                   },
    { XPR_STRING_LITERAL("config.history.menu_count"),                         OptionKey::TypeInteger, &Option::mConfig.mHistoryMenuCount,             (void *)DEF_HISTORY_MENU               },
    { XPR_STRING_LITERAL("config.history.backward_menu_count"),                OptionKey::TypeInteger, &Option::mConfig.mBackwardMenuCount,            (void *)DEF_BACKWARD_MENU              },

    { XPR_STRING_LITERAL("config.drag_drop.drag_type"),                        OptionKey::TypeInteger, &Option::mConfig.mDragType,                     (void *)DRAG_START_DEFAULT             },
    { XPR_STRING_LITERAL("config.drag_drop.drag_distance"),                    OptionKey::TypeInteger, &Option::mConfig.mDragDist,                     (void *)DEF_DIST_DRAG                  },
    { XPR_STRING_LITERAL("config.drag_drop.drop_type"),                        OptionKey::TypeInteger, &Option::mConfig.mDropType,                     (void *)DROP_DEFAULT                   },
    { XPR_STRING_LITERAL("config.drag_drop.expand_folder_time"),               OptionKey::TypeInteger, &Option::mConfig.mDragFolderTreeExpandTime,     (void *)1                              },
    { XPR_STRING_LITERAL("config.drag_drop.scroll_time"),                      OptionKey::TypeInteger, &Option::mConfig.mDragScrollTime,               (void *)1                              },
    { XPR_STRING_LITERAL("config.drag_drop.default_file_op"),                  OptionKey::TypeInteger, &Option::mConfig.mDragDefaultFileOp,            (void *)DRAG_FILE_OP_DEFAULT           },
    { XPR_STRING_LITERAL("config.drag_drop.no_display_contents"),              OptionKey::TypeBoolean, &Option::mConfig.mDragNoContents,               (void *)XPR_FALSE                      },

    { XPR_STRING_LITERAL("config.tray.show"),                                  OptionKey::TypeBoolean, &Option::mConfig.mTray,                         (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.tray.show_on_close"),                         OptionKey::TypeBoolean, &Option::mConfig.mTrayOnClose,                  (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.tray.restore_init_folder"),                   OptionKey::TypeBoolean, &Option::mConfig.mTrayRestoreInitFolder,        (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.tray.minimize"),                              OptionKey::TypeBoolean, &Option::mConfig.mTrayOnMinmize,                (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.tray.restore_by_one_click"),                  OptionKey::TypeBoolean, &Option::mConfig.mTrayOneClick,                 (void *)XPR_FALSE                      },

    { XPR_STRING_LITERAL("config.global_hotkey.enable"),                       OptionKey::TypeBoolean, &Option::mConfig.mLauncher,                     (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.global_hotkey.show_tray"),                    OptionKey::TypeBoolean, &Option::mConfig.mLauncherTray,                 (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.global_hotkey.windows_startup"),              OptionKey::TypeBoolean, &Option::mConfig.mLauncherWinStartup,           (void *)XPR_FALSE                      },
    { XPR_STRING_LITERAL("config.global_hotkey.key"),                          OptionKey::TypeInteger, &Option::mConfig.mLauncherGlobalHotKey,         (void *)XPR_FALSE                      },

    { XPR_STRING_LITERAL("config.update_check.enable"),                        OptionKey::TypeInteger, &Option::mConfig.mUpdateCheckEnable,            (void *)XPR_TRUE                       },

    { XPR_STRING_LITERAL("config.language"),                                   OptionKey::TypeString,   Option::mConfig.mLanguage,                     (void *)XPR_STRING_LITERAL("English")  },
};

void Option::Main::clearView(void)
{
}

Option::Main::Tab::Tab(void)
    : mDefaultFolderLayout(XPR_NULL), mComputerFolderLayout(XPR_NULL), mVirtualFolderLayout(XPR_NULL)
{
}

Option::Main::Tab::~Tab(void)
{
    XPR_SAFE_DELETE(mDefaultFolderLayout);
    XPR_SAFE_DELETE(mComputerFolderLayout);
    XPR_SAFE_DELETE(mVirtualFolderLayout);
}

Option::Main::View::View(void)
    : mCurTab(0)
{
}

Option::Main::View::~View(void)
{
    clear();
}

void Option::Main::View::clear(void)
{
    Tab *sTab;
    TabDeque::iterator sIterator;

    FXFILE_STL_FOR_EACH(sIterator, mTabDeque)
    {
        sTab = *sIterator;
        XPR_ASSERT(sTab != XPR_NULL);

        XPR_SAFE_DELETE(sTab);
    }

    mTabDeque.clear();

    mCurTab = 0;
}

Option::Main   Option::mMain;
Option::Config Option::mConfig;

Option::Option(void)
    : mObserver(XPR_NULL)
{
}

Option::~Option(void)
{
    xpr_sint_t i;
    xpr_sint_t sKeyCount;
    const OptionKey *sOptionKey;

    sKeyCount = XPR_COUNT_OF(gMainOptionKeys);
    for (i = 0; i < sKeyCount; ++i)
    {
        sOptionKey = gMainOptionKeys + i;

        switch (sOptionKey->mType)
        {
        case OptionKey::TypeDouble:
            {
                xpr_double_t *sDefValue = (xpr_double_t *)sOptionKey->mDefValue;
                XPR_SAFE_DELETE(sDefValue);
                break;
            }

        case OptionKey::TypeRect:
            {
                CRect *sDefValue = (CRect *)sOptionKey->mDefValue;
                XPR_SAFE_DELETE(sDefValue);
                break;
            }
        }
    }
}

void Option::setObserver(OptionObserver *aObserver)
{
    mObserver = aObserver;
}

void Option::copyConfig(Config &aConfig) const
{
    aConfig = mConfig;
}

void Option::setConfig(const Config &aConfig)
{
    mConfig = aConfig;
}

void Option::notifyConfig(void)
{
    if (XPR_IS_NOT_NULL(mObserver))
    {
        mObserver->onChangedConfig(*this);
    }
}

void Option::initDefault(void)
{
    fxfile::base::ConfFileEx sConfFile;
    loadMainOption(sConfFile);
    loadConfigOption(sConfFile);
}

void Option::initDefaultConfigOption(void)
{
    fxfile::base::ConfFileEx sConfFile;
    loadConfigOption(sConfFile);
}

void Option::loadMainOption(fxfile::base::ConfFileEx &aConfFile)
{
    xpr_sint_t         i, j, k;
    xpr_tchar_t        sKey[0xff] = {0};
    xpr_tchar_t        sFolderLayoutKey[0xff] = {0};
    ConfFile::Section *sSection;
    const xpr_tchar_t *sValue;
    Main::Tab         *sTab;

    sSection = aConfFile.findSection(kMainSection);

    loadOptionKeys(aConfFile, sSection, gMainOptionKeys, XPR_COUNT_OF(gMainOptionKeys));

    // 'C:' -> 'C:\\'
    for (i = 0; i < MAX_WORKING_FOLDER; ++i)
    {
        if (_tcslen(mMain.mWorkingFolder[i]) == 2 && mMain.mWorkingFolder[i][1] == XPR_STRING_LITERAL(':'))
            _tcscat(mMain.mWorkingFolder[i], XPR_STRING_LITERAL("\\"));
    }

    // last tab
    for (i = 0; i < MAX_VIEW_SPLIT; ++i)
    {
        _stprintf(sKey, kCurTabKey, i + 1);

        mMain.mView[i].mCurTab = aConfFile.getValueI(sSection, sKey, 0);

        for (j = 0; ; ++j)
        {
            _stprintf(sKey, kTabPathKey, i + 1, j + 1);

            sValue = aConfFile.getValueS(sSection, sKey, XPR_NULL);
            if (sValue == XPR_NULL)
                break;

            sTab = new Main::Tab;

            // path
            sTab->mPath = sValue;

            // backward list
            for (k = 0; ; ++k)
            {
                _stprintf(sKey, kBackwardKey, i + 1, j + 1, k + 1);

                sValue = aConfFile.getValueS(sSection, sKey, XPR_NULL);
                if (XPR_IS_NULL(sValue))
                    break;

                sTab->mBackwardList.push_back(sValue);
            }

            // forward list
            for (k = 0; ; ++k)
            {
                _stprintf(sKey, kForwardKey, i + 1, j + 1, k + 1);

                sValue = aConfFile.getValueS(sSection, sKey, XPR_NULL);
                if (XPR_IS_NULL(sValue))
                    break;

                sTab->mForwardList.push_back(sValue);
            }

            // history list
            for (k = 0; ; ++k)
            {
                _stprintf(sKey, kHistoryKey, i + 1, j + 1, k + 1);

                sValue = aConfFile.getValueS(sSection, sKey, XPR_NULL);
                if (XPR_IS_NULL(sValue))
                    break;

                sTab->mHistoryList.push_back(sValue);
            }

            // default folder layout
            sTab->mDefaultFolderLayout = new FolderLayout;
            _stprintf(sFolderLayoutKey, kTabFolderLayoutDefaultKey, i + 1, j + 1);

            if (FolderLayoutManager::loadFromConfFile(*sTab->mDefaultFolderLayout, aConfFile, sSection, sFolderLayoutKey) == XPR_FALSE)
            {
                XPR_SAFE_DELETE(sTab->mDefaultFolderLayout);
            }

            // computer folder layout
            sTab->mComputerFolderLayout = new FolderLayout;
            _stprintf(sFolderLayoutKey, kTabFolderLayoutComputerKey, i + 1, j + 1);

            if (FolderLayoutManager::loadFromConfFile(*sTab->mComputerFolderLayout, aConfFile, sSection, sFolderLayoutKey) == XPR_FALSE)
            {
                XPR_SAFE_DELETE(sTab->mComputerFolderLayout);
            }

            // virtual folder layout
            sTab->mVirtualFolderLayout = new FolderLayout;
            _stprintf(sFolderLayoutKey, kTabFolderLayoutVirtualKey, i + 1, j + 1);

            if (FolderLayoutManager::loadFromConfFile(*sTab->mVirtualFolderLayout, aConfFile, sSection, sFolderLayoutKey) == XPR_FALSE)
            {
                XPR_SAFE_DELETE(sTab->mVirtualFolderLayout);
            }

            mMain.mView[i].mTabDeque.push_back(sTab);
        }
    }
}

void Option::loadConfigOption(fxfile::base::ConfFileEx &aConfFile)
{
    xpr_sint_t         i;
    xpr_tchar_t        sKey[0xff] = {0};
    ConfFile::Section *sSection;

    sSection = aConfFile.findSection(kConfigSection);

    xpr_sint_t sMajorVer = 0;
    xpr_sint_t sMinorVer = 0;
    xpr_sint_t sBuildNumber = 0;

    const xpr_tchar_t *sAppVer = aConfFile.getValueS(sSection, kVersionKey, XPR_NULL);
    if (XPR_IS_NOT_NULL(sAppVer))
        _stscanf(sAppVer, XPR_STRING_LITERAL("%d.%d.%d"), &sMajorVer, &sMinorVer, &sBuildNumber);

    loadOptionKeys(aConfFile, sSection, gConfigOptionKeys, XPR_COUNT_OF(gConfigOptionKeys));

    if (mConfig.mFolderTreeItemHeight < MIN_FOLDER_TREE_HEIGHT) mConfig.mFolderTreeItemHeight = MIN_FOLDER_TREE_HEIGHT;
    if (mConfig.mFolderTreeItemHeight > MAX_FOLDER_TREE_HEIGHT) mConfig.mFolderTreeItemHeight = MAX_FOLDER_TREE_HEIGHT;

    if (mConfig.mFolderTreeSelDelayTime < MIN_FLD_SEL_DELAY_MSEC) mConfig.mFolderTreeSelDelayTime = DEF_FLD_SEL_DELAY_MSEC;
    if (mConfig.mFolderTreeSelDelayTime > MAX_FLD_SEL_DELAY_MSEC) mConfig.mFolderTreeSelDelayTime = DEF_FLD_SEL_DELAY_MSEC;

    if (mConfig.mFileListCustomIconFile16[0] == 0)
        _tcscpy(mConfig.mFileListCustomIconFile16, XPR_STRING_LITERAL("%fxfile%\\custom_image_list_16.bmp"));

    if (mConfig.mFileListCustomIconFile32[0] == 0)
        _tcscpy(mConfig.mFileListCustomIconFile32, XPR_STRING_LITERAL("%fxfile%\\custom_image_list_32.bmp"));

    if (mConfig.mFileListCustomIconFile48[0] == 0)
        _tcscpy(mConfig.mFileListCustomIconFile48, XPR_STRING_LITERAL("%fxfile%\\custom_image_list_48.bmp"));

    if (mConfig.mFileListCustomIconFile256[0] == 0)
        _tcscpy(mConfig.mFileListCustomIconFile256, XPR_STRING_LITERAL("%fxfile%\\custom_image_list_256.bmp"));

    SizeFormat::validSizeUnit(mConfig.mFileListSizeUnit);

    for (i = 0; i < MAX_VIEW_SPLIT; ++i)
    {
        if (_tcslen(mConfig.mFileListInitFolder[i]) == 2 && mConfig.mFileListInitFolder[i][1] == XPR_STRING_LITERAL(':'))
            _tcscat(mConfig.mFileListInitFolder[i], XPR_STRING_LITERAL("\\"));
    }

    if (mConfig.mDragDist < MIN_DIST_DRAG) mConfig.mDragDist = DEF_DIST_DRAG;
    if (mConfig.mDragDist > MAX_DIST_DRAG) mConfig.mDragDist = DEF_DIST_DRAG;

    if (mConfig.mDragFolderTreeExpandTime < MIN_EXPAND_FOLDER_MSEC) mConfig.mDragFolderTreeExpandTime = DEF_EXPAND_FOLDER_MSEC;
    if (mConfig.mDragFolderTreeExpandTime > MAX_EXPAND_FOLDER_MSEC) mConfig.mDragFolderTreeExpandTime = DEF_EXPAND_FOLDER_MSEC;

    if (mConfig.mDragScrollTime < MIN_DRAG_SCROLL_MSEC) mConfig.mDragScrollTime = DEF_DRAG_SCROLL_MSEC;
    if (mConfig.mDragScrollTime > MAX_DRAG_SCROLL_MSEC) mConfig.mDragScrollTime = DEF_DRAG_SCROLL_MSEC;

    if (mConfig.mHistoryCount < MIN_HISTORY) mConfig.mHistoryCount = DEF_HISTORY;
    if (mConfig.mHistoryCount > MAX_HISTORY) mConfig.mHistoryCount = DEF_HISTORY;

    if (mConfig.mBackwardCount < MIN_BACKWARD) mConfig.mBackwardCount = DEF_BACKWARD;
    if (mConfig.mBackwardCount > MAX_BACKWARD) mConfig.mBackwardCount = DEF_BACKWARD;

    if (mConfig.mHistoryMenuCount < MIN_HISTORY_MENU) mConfig.mHistoryMenuCount = DEF_HISTORY_MENU;
    if (mConfig.mHistoryMenuCount > MAX_HISTORY_MENU) mConfig.mHistoryMenuCount = DEF_HISTORY_MENU;

    if (mConfig.mBackwardMenuCount < MIN_BACKWARD_MENU) mConfig.mBackwardMenuCount = DEF_BACKWARD_MENU;
    if (mConfig.mBackwardMenuCount > MAX_BACKWARD_MENU) mConfig.mBackwardMenuCount = DEF_BACKWARD_MENU;

    SizeFormat::validSizeUnit(mConfig.mSingleSelFileSizeUnit);
    SizeFormat::validSizeUnit(mConfig.mMultiSelFileSizeUnit);
}

void Option::saveMainOption(fxfile::base::ConfFileEx &aConfFile) const
{
    xpr_sint_t         i, j, k;
    xpr_tchar_t        sKey[0xff] = {0};
    xpr_tchar_t        sFolderLayoutKey[0xff] = {0};
    ConfFile::Section *sSection;
    Main::Tab         *sTab;
    Main::TabDeque::const_iterator    sTabIterator;
    Main::HistoryList::const_iterator sHistoryIterator;

    sSection = aConfFile.addSection(kMainSection);
    XPR_ASSERT(sSection != XPR_NULL);

    xpr_tchar_t sAppVer[0xff] = {0};
    getAppVer(sAppVer);

    aConfFile.setValueS(sSection, kVersionKey, sAppVer);

    saveOptionKeys(aConfFile, sSection, gMainOptionKeys, XPR_COUNT_OF(gMainOptionKeys));

    for (i = 0; i < MAX_VIEW_SPLIT; ++i)
    {
        _stprintf(sKey, kCurTabKey, i + 1);

        aConfFile.setValueI(sSection, sKey, (xpr_sint_t)mMain.mView[i].mCurTab);

        sTabIterator = mMain.mView[i].mTabDeque.begin();
        for (j = 0; sTabIterator != mMain.mView[i].mTabDeque.end(); ++sTabIterator, ++j)
        {
            sTab = *sTabIterator;
            XPR_ASSERT(sTab != XPR_NULL);

            _stprintf(sKey, kTabPathKey, i + 1, j + 1);

            aConfFile.setValueS(sSection, sKey, sTab->mPath);

            sHistoryIterator = sTab->mBackwardList.begin();
            for (k = 0; sHistoryIterator != sTab->mBackwardList.end(); ++sHistoryIterator, ++k)
            {
                const xpr::string &sTabPath = *sHistoryIterator;

                _stprintf(sKey, kBackwardKey, i + 1, j + 1, k + 1);

                aConfFile.setValueS(sSection, sKey, sTabPath);
            }

            sHistoryIterator = sTab->mForwardList.begin();
            for (k = 0; sHistoryIterator != sTab->mForwardList.end(); ++sHistoryIterator, ++k)
            {
                const xpr::string &sTabPath = *sHistoryIterator;

                _stprintf(sKey, kForwardKey, i + 1, j + 1, k + 1);

                aConfFile.setValueS(sSection, sKey, sTabPath);
            }

            sHistoryIterator = sTab->mHistoryList.begin();
            for (k = 0; sHistoryIterator != sTab->mHistoryList.end(); ++sHistoryIterator, ++k)
            {
                const xpr::string &sTabPath = *sHistoryIterator;

                _stprintf(sKey, kHistoryKey, i + 1, j + 1, k + 1);

                aConfFile.setValueS(sSection, sKey, sTabPath);
            }

            if (XPR_IS_NOT_NULL(sTab->mDefaultFolderLayout))
            {
                _stprintf(sFolderLayoutKey, kTabFolderLayoutDefaultKey, i + 1, j + 1);

                FolderLayoutManager::saveToConfFile(*sTab->mDefaultFolderLayout, aConfFile, sSection, sFolderLayoutKey);
            }

            if (XPR_IS_NOT_NULL(sTab->mComputerFolderLayout))
            {
                _stprintf(sFolderLayoutKey, kTabFolderLayoutComputerKey, i + 1, j + 1);

                FolderLayoutManager::saveToConfFile(*sTab->mComputerFolderLayout, aConfFile, sSection, sFolderLayoutKey);
            }

            if (XPR_IS_NOT_NULL(sTab->mVirtualFolderLayout))
            {
                _stprintf(sFolderLayoutKey, kTabFolderLayoutVirtualKey, i + 1, j + 1);

                FolderLayoutManager::saveToConfFile(*sTab->mVirtualFolderLayout, aConfFile, sSection, sFolderLayoutKey);
            }
        }
    }
}

void Option::saveConfigOption(fxfile::base::ConfFileEx &aConfFile) const
{
    ConfFile::Section *sSection;

    sSection = aConfFile.addSection(kConfigSection);
    XPR_ASSERT(sSection != XPR_NULL);

    xpr_tchar_t sAppVer[0xff] = {0};
    getAppVer(sAppVer);

    aConfFile.setValueS(sSection, XPR_STRING_LITERAL("version"), sAppVer);

    saveOptionKeys(aConfFile, sSection, gConfigOptionKeys, XPR_COUNT_OF(gConfigOptionKeys));
}

static void loadOptionKeys(fxfile::base::ConfFileEx &aConfFile, ConfFile::Section *aSection, const OptionKey *aOptionKeys, xpr_sint_t aKeyCount)
{
    xpr_sint_t i;
    const OptionKey *sOptionKey;

    for (i = 0; i < aKeyCount; ++i)
    {
        sOptionKey = aOptionKeys + i;

        switch (sOptionKey->mType)
        {
        case OptionKey::TypeBoolean:
            {
                xpr_bool_t *sValue    = (xpr_bool_t *)sOptionKey->mValue;
                xpr_bool_t  sDefValue = (xpr_bool_t)(xpr_sintptr_t)sOptionKey->mDefValue;

                *sValue = aConfFile.getValueB(aSection,
                                              sOptionKey->mKey,
                                              sDefValue);
                break;
            }

        case OptionKey::TypeInteger:
            {
                xpr_sint_t *sValue    = (xpr_sint_t *)sOptionKey->mValue;
                xpr_sint_t  sDefValue = (xpr_sint_t)(xpr_sintptr_t)sOptionKey->mDefValue;

                *sValue = aConfFile.getValueI(aSection,
                                              sOptionKey->mKey,
                                              sDefValue);
                break;
            }

        case OptionKey::TypeDouble:
            {
                xpr_double_t *sValue    = (xpr_double_t *)sOptionKey->mValue;
                xpr_double_t *sDefValue = (xpr_double_t *)sOptionKey->mDefValue;

                *sValue = aConfFile.getValueF(aSection,
                                              sOptionKey->mKey,
                                              *sDefValue);
                break;
            }

        case OptionKey::TypeString:
            {
                xpr_tchar_t *sValue    = (xpr_tchar_t *)sOptionKey->mValue;
                xpr_tchar_t *sDefValue = (xpr_tchar_t *)sOptionKey->mDefValue;

                _tcscpy(sValue, aConfFile.getValueS(aSection,
                                                    sOptionKey->mKey,
                                                    sDefValue));
                break;
            }

        case OptionKey::TypeRect:
            {
                CRect *sValue    = (CRect *)sOptionKey->mValue;
                CRect *sDefValue = (CRect *)sOptionKey->mDefValue;

                *sValue = aConfFile.getValueR(aSection,
                                              sOptionKey->mKey,
                                              *sDefValue);
                break;
            }

        case OptionKey::TypeColor:
            {
                COLORREF *sValue    = (COLORREF *)sOptionKey->mValue;
                COLORREF  sDefValue = (COLORREF)(xpr_sintptr_t)sOptionKey->mDefValue;

                *sValue = aConfFile.getValueC(aSection,
                                              sOptionKey->mKey,
                                              sDefValue);
                break;
            }
        }
    }
}

static void saveOptionKeys(fxfile::base::ConfFileEx &aConfFile, ConfFile::Section *aSection, const OptionKey *aOptionKeys, xpr_sint_t aKeyCount)
{
    xpr_sint_t i;
    const OptionKey *sOptionKey;

    for (i = 0; i < aKeyCount; ++i)
    {
        sOptionKey = aOptionKeys + i;

        switch (sOptionKey->mType)
        {
        case OptionKey::TypeBoolean:
            {
                xpr_bool_t *sValue = (xpr_bool_t *)sOptionKey->mValue;

                aConfFile.setValueB(aSection,
                                    sOptionKey->mKey,
                                    *sValue);
                break;
            }

        case OptionKey::TypeInteger:
            {
                xpr_sint_t *sValue = (xpr_sint_t *)sOptionKey->mValue;

                aConfFile.setValueI(aSection,
                                    sOptionKey->mKey,
                                    *sValue);
                break;
            }

        case OptionKey::TypeDouble:
            {
                xpr_double_t *sValue = (xpr_double_t *)sOptionKey->mValue;

                aConfFile.setValueF(aSection,
                                    sOptionKey->mKey,
                                    *sValue);
                break;
            }

        case OptionKey::TypeString:
            {
                xpr_tchar_t *sValue = (xpr_tchar_t *)sOptionKey->mValue;

                aConfFile.setValueS(aSection,
                                    sOptionKey->mKey,
                                    sValue);
                break;
            }

        case OptionKey::TypeRect:
            {
                CRect *sValue = (CRect *)sOptionKey->mValue;

                aConfFile.setValueR(aSection,
                                    sOptionKey->mKey,
                                    *sValue);
                break;
            }

        case OptionKey::TypeColor:
            {
                COLORREF *sValue = (COLORREF *)sOptionKey->mValue;

                aConfFile.setValueC(aSection,
                                    sOptionKey->mKey,
                                    *sValue);
                break;
            }
        }
    }
}
} // namespace fxfile
