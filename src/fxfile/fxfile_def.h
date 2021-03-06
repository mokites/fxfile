//
// Copyright (c) 2013 Leon Lee author. All rights reserved.
//
//   homepage: http://www.flychk.com
//   e-mail:   mailto:flychk@flychk.com
//
// Use of this source code is governed by a GPLv3 license that can be
// found in the LICENSE file.

#ifndef __FXFILE_DEF_H__
#define __FXFILE_DEF_H__ 1
#pragma once

namespace fxfile
{
#define MIN_HISTORY               (10)
#define DEF_HISTORY               (100)
#define MAX_HISTORY               (1000)
#define MIN_HISTORY_MENU          (5)
#define DEF_HISTORY_MENU          (10)
#define MAX_HISTORY_MENU          (30)

#define MIN_BACKWARD              (10)
#define DEF_BACKWARD              (100)
#define MAX_BACKWARD              (1000)
#define MIN_BACKWARD_MENU         (5)
#define DEF_BACKWARD_MENU         (10)
#define MAX_BACKWARD_MENU         (30)

#define DEF_RECENT_FILE_LIST_MENU (10)

#define MIN_DIST_DRAG             (8)
#define DEF_DIST_DRAG             (8)
#define MAX_DIST_DRAG             (100)

#define MIN_EXPAND_FOLDER_MSEC    (500)
#define DEF_EXPAND_FOLDER_MSEC    (1000)
#define MAX_EXPAND_FOLDER_MSEC    (10000)

#define MIN_DRAG_SCROLL_MSEC      (100)
#define DEF_DRAG_SCROLL_MSEC      (200)
#define MAX_DRAG_SCROLL_MSEC      (5000)

#define MIN_FLD_SEL_DELAY_MSEC    (0)
#define DEF_FLD_SEL_DELAY_MSEC    (700)
#define MAX_FLD_SEL_DELAY_MSEC    (5000)

#define MAX_WORKING_FOLDER        (5)

#define MAX_FONT_TEXT             (0xff)

#define MIN_THUMB_SIZE            (50)
#define DEF_THUMB_SIZE            (100)
#define MAX_THUMB_SIZE            (300)

#define DEF_FOLDER_PANE_SIZE      (200)

#define MIN_FOLDER_TREE_HEIGHT    (10)
#define MAX_FOLDER_TREE_HEIGHT    (50)

#define MAX_CLIP_SEPARATOR        (30)

#define MAX_ACCEL                 (200)

#define MAX_VIEW_SPLIT_ROW        (2)
#define MAX_VIEW_SPLIT_COLUMN     (3)
#define MAX_VIEW_SPLIT            (MAX_VIEW_SPLIT_ROW * MAX_VIEW_SPLIT_COLUMN)
#define DEF_VIEW_SPLIT_ROW        (1)
#define DEF_VIEW_SPLIT_COLUMN     (1)
#define MIN_PANE_SIZE             (50)
#define SPLITTER_SIZE             (2)

#define UPDATE_CHECK_TIME_MIN     (1)
#define UPDATE_CHECK_TIME_DEF     (24)
#define UPDATE_CHECK_TIME_MAX     (365 * 24)

enum FolderType
{
    FOLDER_DEFAULT = 0,
    FOLDER_COMPUTER,
    FOLDER_VIRTUAL,
};

enum
{
    RENAME_EXT_TYPE_DEFAULT = 0,
    RENAME_EXT_TYPE_KEEP,
    RENAME_EXT_TYPE_SEL_EXCEPT_FOR_EXT,
};

enum
{
    MOUSE_ONE_CLICK = 0,
    MOUSE_DOUBLE_CLICK,
};

enum
{
    EXT_TYPE_HIDE = 0,
    EXT_TYPE_ALWAYS,
    EXT_TYPE_KNOWN,
};

enum
{
    SINGLE_RENAME_TYPE_DEFAULT = 0,
    SINGLE_RENAME_TYPE_BY_POPUP,
    SINGLE_RENAME_TYPE_BATCH_RENAME,
};

enum
{
    LIST_TYPE_ALL = 0,
    LIST_TYPE_FOLDER,
    LIST_TYPE_FILE,
};

enum
{
    CASE_TYPE_ORIGINAL = 0,
    CASE_TYPE_UPPER,
    CASE_TYPE_LOWER,
};

enum
{
    CONTENTS_NONE = 0,
    CONTENTS_EXPLORER,
    CONTENTS_BASIC,
};

enum
{
    INIT_TYPE_NONE = 0,
    INIT_TYPE_INIT_FOLDER,
    INIT_TYPE_LAST_FOLDER,
    INIT_TYPE_LAST_TAB,
};

enum
{
    COLOR_TYPE_DEFAULT = 0,
    COLOR_TYPE_CUSTOM,
    COLOR_TYPE_FILTERING,
    COLOR_TYPE_FILE_LIST,
};

enum
{
    FILTER_COLOR_TYPE_NONE = 0,
    FILTER_COLOR_TYPE_TEXT,
    FILTER_COLOR_TYPE_BKGND,
};

enum
{
    NAME_CASE_TYPE_DEFAULT = 0,
    NAME_CASE_TYPE_UPPER,
    NAME_CASE_TYPE_LOWER,
};

enum
{
    SAVE_FOLDER_LAYOUT_NONE = 0,
    SAVE_FOLDER_LAYOUT_DEFAULT,
    SAVE_FOLDER_LAYOUT_EACH_FOLDER,
};

enum
{
    VIEW_STYLE_DETAILS = 0,
    VIEW_STYLE_LIST,
    VIEW_STYLE_TILES,
    VIEW_STYLE_CONTENT,
    VIEW_STYLE_THUMBNAIL,
    VIEW_STYLE_SMALL_ICONS = 20,
    VIEW_STYLE_MEDIUM_ICONS,
    VIEW_STYLE_LARGE_ICONS,
    VIEW_STYLE_EXTRA_LARGE_ICONS,
};

enum
{
    SIZE_UNIT_DEFAULT = 0,
    SIZE_UNIT_AUTO,
    SIZE_UNIT_CUSTOM,
    SIZE_UNIT_NONE,
    SIZE_UNIT_BYTE = 10,
    SIZE_UNIT_KB,
    SIZE_UNIT_MB,
    SIZE_UNIT_GB,
    SIZE_UNIT_TB,
    SIZE_UNIT_PB,
    SIZE_UNIT_EB,
    SIZE_UNIT_ZB,
    SIZE_UNIT_YB,
};

enum
{
    DRAG_START_NONE = 0,
    DRAG_START_DEFAULT,
    DRAG_START_CTRL,
    DRAG_START_DIST,
};

enum
{
    DROP_NONE = 0,
    DROP_DEFAULT,
};

enum
{
    DRAG_FILE_OP_DEFAULT = 0,
    DRAG_FILE_OP_MOVE,
    DRAG_FILE_OP_COPY,
    DRAG_FILE_OP_LINK,
};

#define DEF_ACTIVED_VIEW_COLOR       (RGB(48,192,48))
#define DEF_INFO_BAR_BOOKMARK_COLOR  (RGB(0,0,255))
#define DEF_PATH_BAR_HIGHLIGHT_COLOR (::GetSysColor(COLOR_ACTIVECAPTION))
} // namespace fxfile

#endif // __FXFILE_DEF_H__
