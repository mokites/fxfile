//
// Copyright (c) 2001-2013 Leon Lee author. All rights reserved.
//
//   homepage: http://www.flychk.com
//   e-mail:   mailto:flychk@flychk.com
//
// Use of this source code is governed by a GPLv3 license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "fxb_ini_file.h"

namespace fxb
{
#define MAX_LINE            5120
#define MAX_SECTION         128
#define MAX_KEY             128
#define MAX_VALUE           300
#define MAX_COMMENT         2048

class IniFile::Key
{
public:
    std::tstring mName;
    std::tstring mValue;
};

class IniFile::Section
{
public:
    Section(void)
        : mMaxNameLen(0)
    {
    }

    ~Section(void)
    {
        Key *sKey;
        KeyList::const_iterator sIterator;

        sIterator = mKeyList.begin();
        for (; sIterator != mKeyList.end(); ++sIterator)
        {
            sKey = *sIterator;

            XPR_SAFE_DELETE(sKey);
        }

        mKeyList.clear();
        mKeyMap.clear();
    }

public:
    std::tstring mName;
    KeyList      mKeyList;
    KeyMap       mKeyMap;
    xpr_size_t   mMaxNameLen;
};

class IniFile::Comment
{
public:
    std::tstring mComment;
};

IniFile::SectionIterator::SectionIterator(void)
{
}

IniFile::Section* IniFile::SectionIterator::operator* () const
{
    return *mIterator;
}

IniFile::SectionIterator& IniFile::SectionIterator::operator++ ()
{
    ++mIterator;

    return *this;
}

IniFile::SectionIterator IniFile::SectionIterator::operator++ (xpr_sint_t)
{
    SectionIterator sIterator = *this;

    ++mIterator;

    return sIterator;
}

bool IniFile::SectionIterator::operator== (const IniFile::SectionIterator &aIterator) const
{
    return (mIterator == aIterator.mIterator) ? true : false;
}

bool IniFile::SectionIterator::operator!= (const IniFile::SectionIterator &aIterator) const
{
    return (mIterator != aIterator.mIterator) ? true : false;
}

IniFile::KeyIterator::KeyIterator(void)
{
}

IniFile::Key* IniFile::KeyIterator::operator* () const
{
    return *mIterator;
}

IniFile::KeyIterator& IniFile::KeyIterator::operator++ ()
{
    ++mIterator;

    return *this;
}

IniFile::KeyIterator IniFile::KeyIterator::operator++ (xpr_sint_t)
{
    KeyIterator sIterator = *this;

    ++mIterator;

    return sIterator;
}

bool IniFile::KeyIterator::operator== (const IniFile::KeyIterator &aIterator) const
{
    return (mIterator == aIterator.mIterator) ? true : false;
}

bool IniFile::KeyIterator::operator!= (const IniFile::KeyIterator &aIterator) const
{
    return (mIterator != aIterator.mIterator) ? true : false;
}

IniFile::IniFile(const xpr_tchar_t *aIniPath)
{
    if (XPR_IS_NOT_NULL(aIniPath))
        mPath = aIniPath;
}

IniFile::~IniFile(void)
{
    clear();
}

void IniFile::clear(void)
{
    clearSection();
    clearComment();
}

const xpr_tchar_t *IniFile::getPath(void) const
{
    return mPath.c_str();
}

void IniFile::setPath(const xpr_tchar_t *aIniPath)
{
    if (XPR_IS_NOT_NULL(aIniPath))
        mPath = aIniPath;
}

xpr_bool_t IniFile::readFile(void)
{
    xpr_rcode_t sRcode;
    xpr::FileIo sFileIo;
    xpr_ssize_t sReadLen;

    sRcode = sFileIo.open(mPath.c_str(), xpr::FileIo::OpenModeReadOnly);
    if (XPR_RCODE_IS_ERROR(sRcode))
        return XPR_FALSE;

    xpr::TextFileReader sTextFileReader(sFileIo);

    if (sTextFileReader.detectEncoding() == xpr::CharSetNone)
        sTextFileReader.setEncoding(xpr::CharSetMultiBytes);

    xpr_tchar_t  sLine[MAX_LINE + 1] = {0};
    Section     *sSection = XPR_NULL;
    xpr_tchar_t *sLineRef;
    xpr_tchar_t *sSeparator;
    xpr_tchar_t *sKey;
    xpr_tchar_t *sValue;

    do
    {
        sRcode = sTextFileReader.readLine(sLine, MAX_LINE, &sReadLen);
        if (XPR_RCODE_IS_ERROR(sRcode) || sReadLen < 0)
            break;

        sLineRef = sLine;

        if (*sLineRef == 0)
            continue;

        if (_istprint(*sLineRef) == 0)
        {
            sFileIo.close();
            return XPR_FALSE;
        }

        while (_istspace(*sLineRef) > 0)
        {
            ++sLineRef;
        }

        switch (*sLineRef)
        {
        case '[':
            sSeparator = _tcschr(sLineRef, ']');
            if (XPR_IS_NOT_NULL(sSeparator))
            {
                sSeparator[0] = '\0';

                sSection = addSection(sLineRef + 1);
            }
            break;

        case ';':
        case '#':
            {
                setComment(sLineRef);
                break;
            }

        default:
            sSeparator = _tcschr(sLineRef, '=');
            if (XPR_IS_NOT_NULL(sSeparator))
            {
                if (sSeparator > sLineRef)
                {
                    sKey   = sLineRef;
                    sValue = sSeparator + 1;

                    --sSeparator;

                    while (_istspace(*sSeparator) > 0 && sSeparator > sLineRef)
                    {
                        --sSeparator;
                    }

                    ++sSeparator;

                    *sSeparator = 0;

                    while (_istspace(*sValue) > 0)
                    {
                        ++sValue;
                    }

                    while (_istspace(*(sLine + sReadLen - 1)) > 0)
                    {
                        *(sLine + sReadLen - 1) = 0;
                        --sReadLen;
                    }

                    if (sSection != XPR_NULL)
                    {
                        setValue(sSection, sKey, sValue);
                    }
                    else
                    {
                        sSection = addSection(XPR_STRING_LITERAL(""));
                        XPR_ASSERT(sSection != XPR_NULL);

                        setValue(sSection, sKey, sValue);
                    }
                }
            }
            break;
        }
    } while (true);

    sFileIo.close();

    return mSectionList.empty() ? XPR_FALSE : XPR_TRUE;
}

xpr_bool_t IniFile::writeFile(xpr::CharSet aCharSet) const
{
    xpr_rcode_t sRcode;
    xpr_sint_t sOpenMode;
    xpr::FileIo sFileIo;

    sOpenMode = xpr::FileIo::OpenModeCreate | xpr::FileIo::OpenModeTruncate | xpr::FileIo::OpenModeWriteOnly;
    sRcode = sFileIo.open(mPath.c_str(), sOpenMode);
    if (XPR_RCODE_IS_ERROR(sRcode))
        return XPR_FALSE;

    xpr::TextFileWriter sTextFileWriter(sFileIo);
    sTextFileWriter.setEncoding(aCharSet);
    sTextFileWriter.setEndOfLine(xpr::TextFileWriter::kWinStyle);
    sTextFileWriter.writeBom();

    if (mCommentList.empty() == false)
    {
        Comment *sComment;
        CommentList::const_iterator sIterator;

        sIterator = mCommentList.begin();
        for (; sIterator != mCommentList.end(); ++sIterator)
        {
            sComment = *sIterator;

            sTextFileWriter.writeFormatLine(XPR_STRING_LITERAL("# %s"), sComment->mComment.c_str());
        }

        sTextFileWriter.writeFormatLine(XPR_STRING_LITERAL(""));
    }

    if (mSectionList.empty() == false)
    {
        Section     *sSection;
        Key         *sKey;
        xpr_tchar_t  sFormat[0xff];
        SectionList::const_iterator sIterator;
        KeyList::const_iterator sKeyIterator;

        sIterator = mSectionList.begin();
        for (; sIterator != mSectionList.end(); ++sIterator)
        {
            sSection = *sIterator;

            sTextFileWriter.writeFormatLine(XPR_STRING_LITERAL("[%s]"), sSection->mName.c_str());

            if (sSection->mMaxNameLen > 0)
            {
                _stprintf(sFormat, XPR_STRING_LITERAL("%%-%ds = %%s"), sSection->mMaxNameLen);
            }
            else
            {
                _tcscpy(sFormat, XPR_STRING_LITERAL("%s = %s"));
            }

            sKeyIterator = sSection->mKeyList.begin();
            for (; sKeyIterator != sSection->mKeyList.end(); ++sKeyIterator)
            {
                sKey = *sKeyIterator;

                if (sSection->mMaxNameLen > 0)
                {
                    sTextFileWriter.writeFormatLine(sFormat, sKey->mName.c_str(), sKey->mValue.c_str());
                }
                else
                {
                }
            }

            sTextFileWriter.writeFormatLine(XPR_STRING_LITERAL(""));
        }
    }

    sFileIo.close();

    return XPR_TRUE;
}

IniFile::Section *IniFile::findSection(const xpr_tchar_t *aSection) const
{
    if (XPR_IS_NULL(aSection))
        return XPR_NULL;

    Section *sSection;
    SectionList::const_iterator sIterator;

    sIterator = mSectionList.begin();
    for (; sIterator != mSectionList.end(); ++sIterator)
    {
        sSection = *sIterator;

        if (sSection->mName.compare(aSection) == 0)
            return sSection;
    }

    return XPR_NULL;
}

IniFile::Key *IniFile::findKey(const Section *aSection, const xpr_tchar_t *aKey) const
{
    if (XPR_IS_NULL(aKey))
        return XPR_NULL;

    XPR_ASSERT(aSection != XPR_NULL);

    KeyMap::const_iterator sIterator = aSection->mKeyMap.find(aKey);
    if (sIterator == aSection->mKeyMap.end())
        return XPR_NULL;

    Key *sKey = sIterator->second;

    return sKey;
}

IniFile::Key *IniFile::findKey(const xpr_tchar_t *aSection, const xpr_tchar_t *aKey) const
{
    Section *sSection = findSection(aSection);
    if (XPR_IS_NULL(sSection))
        return XPR_NULL;

    return findKey(sSection, aKey);
}

IniFile::Section *IniFile::addSection(const xpr_tchar_t *aSection)
{
    Section *sSection = findSection(aSection);
    if (XPR_IS_NULL(sSection))
    {
        sSection = new Section;
        sSection->mName = aSection;

        mSectionList.push_back(sSection);
    }

    return sSection;
}

const xpr_tchar_t *IniFile::getSectionName(const Section *aSection) const
{
    XPR_ASSERT(aSection != XPR_NULL);

    return aSection->mName.c_str();
}

xpr_size_t IniFile::getSectionCount(void) const
{
    return mSectionList.size();
}

xpr_size_t IniFile::getKeyCount(const Section *aSection) const
{
    XPR_ASSERT(aSection != XPR_NULL);

    return aSection->mKeyList.size();
}

xpr_size_t IniFile::getKeyCount(const xpr_tchar_t *aSection) const
{
    Section *sSection = findSection(aSection);
    if (XPR_IS_NULL(sSection))
        return 0;

    return sSection->mKeyList.size();
}

IniFile::SectionIterator IniFile::beginSection(void) const
{
    SectionIterator sIterator;
    sIterator.mIterator = mSectionList.begin();

    return sIterator;
}

IniFile::SectionIterator IniFile::endSection(void) const
{
    SectionIterator sIterator;
    sIterator.mIterator = mSectionList.end();

    return sIterator;
}

IniFile::KeyIterator IniFile::beginKey(Section *aSection) const
{
    XPR_ASSERT(aSection != XPR_NULL);

    KeyIterator sIterator;
    sIterator.mSection  = aSection;
    sIterator.mIterator = aSection->mKeyList.begin();

    return sIterator;
}

IniFile::KeyIterator IniFile::endKey(Section *aSection) const
{
    XPR_ASSERT(aSection != XPR_NULL);

    KeyIterator sIterator;
    sIterator.mSection  = aSection;
    sIterator.mIterator = aSection->mKeyList.end();

    return sIterator;
}

const xpr_tchar_t *IniFile::getKeyName(const Key *aKey) const
{
    XPR_ASSERT(aKey != XPR_NULL);

    return aKey->mName.c_str();
}

const xpr_tchar_t *IniFile::getValue(const Key *aKey, const xpr_tchar_t *aDefValue) const
{
    if (XPR_IS_NULL(aKey))
        return aDefValue;

    return aKey->mValue.c_str();
}

const xpr_tchar_t *IniFile::getValue(const Section *aSection, const xpr_tchar_t *aKey, const xpr_tchar_t *aDefValue) const
{
    if (XPR_IS_NULL(aSection))
        return aDefValue;

    Key *sKey = findKey(aSection, aKey);
    if (XPR_IS_NULL(sKey))
        return aDefValue;

    return sKey->mValue.c_str();
}

const xpr_tchar_t *IniFile::getValue(const xpr_tchar_t *aSection, const xpr_tchar_t *aKey, const xpr_tchar_t *aDefValue) const
{
    Section *sSection = findSection(aSection);
    if (XPR_IS_NULL(sSection))
        return aDefValue;

    Key *sKey = findKey(sSection, aKey);
    if (XPR_IS_NULL(sKey))
        return aDefValue;

    return sKey->mValue.c_str();
}

xpr_bool_t IniFile::setValue(Key *aKey, const xpr_tchar_t *aValue)
{
    if (XPR_IS_NULL(aValue))
        return XPR_FALSE;

    XPR_ASSERT(aKey != XPR_NULL);

    aKey->mValue = aValue;

    return XPR_TRUE;
}

xpr_bool_t IniFile::setValue(Section *aSection, const xpr_tchar_t *aKey, const xpr_tchar_t *aValue)
{
    if (XPR_IS_NULL(aKey) || XPR_IS_NULL(aValue))
        return XPR_FALSE;

    XPR_ASSERT(aSection != XPR_NULL);

    Key *sKey = findKey(aSection, aKey);
    if (XPR_IS_NOT_NULL(sKey))
    {
        sKey->mValue = aValue;
    }
    else
    {
        sKey = new Key;
        XPR_ASSERT(sKey != XPR_NULL);

        sKey->mName  = aKey;
        sKey->mValue = aValue;

        xpr_size_t sNameLen = _tcslen(aKey);
        if (sNameLen > aSection->mMaxNameLen)
            aSection->mMaxNameLen = sNameLen;

        aSection->mKeyList.push_back(sKey);
        aSection->mKeyMap[aKey] = sKey;
    }

    return XPR_TRUE;
}

xpr_bool_t IniFile::setValue(const xpr_tchar_t *aSection, const xpr_tchar_t *aKey, const xpr_tchar_t *aValue)
{
    if (XPR_IS_NULL(aSection) || XPR_IS_NULL(aKey) || XPR_IS_NULL(aValue))
        return XPR_FALSE;

    Section *sSection = findSection(aSection);
    if (XPR_IS_NULL(sSection))
    {
        sSection = new Section;
        sSection->mName = aSection;

        mSectionList.push_back(sSection);
    }

    return setValue(sSection, aKey, aValue);
}

void IniFile::clearKey(Section *aSection, const xpr_tchar_t *aKey)
{
    XPR_ASSERT(aSection != XPR_NULL);

    Key *sKey;
    KeyList::const_iterator sIterator;

    sIterator = aSection->mKeyList.begin();
    for (; sIterator != aSection->mKeyList.end(); ++sIterator)
    {
        sKey = *sIterator;

        if (sKey->mName.compare(aKey) == 0)
        {
            aSection->mKeyList.erase(sIterator);
            aSection->mKeyMap.erase(sKey->mName);

            XPR_SAFE_DELETE(sKey);

            break;
        }
    }
}

void IniFile::clearKey(const xpr_tchar_t *aSection, const xpr_tchar_t *aKey)
{
    if (XPR_IS_NULL(aSection) || XPR_IS_NULL(aKey))
        return;

    Section *sSection = findSection(aSection);
    if (XPR_IS_NULL(sSection))
        return;

    clearKey(sSection, aKey);
}

void IniFile::clearSection(const xpr_tchar_t *aSection)
{
    if (XPR_IS_NULL(aSection))
        return;

    Section *sSection;
    SectionList::const_iterator sIterator;

    sIterator = mSectionList.begin();
    for (; sIterator != mSectionList.end(); ++sIterator)
    {
        sSection = *sIterator;

        if (sSection->mName.compare(aSection) == 0)
        {
            XPR_SAFE_DELETE(sSection);

            mSectionList.erase(sIterator);
            break;
        }
    }
}

void IniFile::clearSection(void)
{
    Section *sSection;
    SectionList::const_iterator sIterator;

    sIterator = mSectionList.begin();
    for (; sIterator != mSectionList.end(); ++sIterator)
    {
        sSection = *sIterator;

        XPR_SAFE_DELETE(sSection);
    }

    mSectionList.clear();
}

xpr_size_t IniFile::getCommentCount(void)
{
    return mCommentList.size();
}

void IniFile::setComment(const xpr_tchar_t *aComment)
{
    Comment *sComment = new Comment;
    sComment->mComment = aComment;

    mCommentList.push_back(sComment);
}

const xpr_tchar_t *IniFile::getComment(const Comment *aComment) const
{
    XPR_ASSERT(aComment != XPR_NULL);

    return aComment->mComment.c_str();
}

void IniFile::clearComment(void)
{
    Comment *sComment;
    CommentList::iterator sIterator;

    sIterator = mCommentList.begin();
    for (; sIterator != mCommentList.end(); ++sIterator)
    {
        sComment = *sIterator;

        XPR_SAFE_DELETE(sComment);
    }

    mCommentList.clear();
}
} // namespace fxb
