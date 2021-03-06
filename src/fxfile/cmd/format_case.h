//
// Copyright (c) 2014 Leon Lee author. All rights reserved.
//
//   homepage: http://www.flychk.com
//   e-mail:   mailto:flychk@flychk.com
//
// Use of this source code is governed by a GPLv3 license that can be
// found in the LICENSE file.

#ifndef __FXFILE_FORMAT_CASE_H__
#define __FXFILE_FORMAT_CASE_H__ 1
#pragma once

#include "format.h"

namespace fxfile
{
namespace cmd
{
class FormatCase : public Format
{
    typedef Format super;

public:
    FormatCase(void);
    FormatCase(CaseTargetType aTargetType, CaseType aCaseType);
    FormatCase(CaseTargetType aTargetType, CaseType aCaseType, const xpr::string &aSkipChars);
    virtual ~FormatCase(void);

public:
    virtual void rename(RenameContext &aContext) const;

    virtual xpr_bool_t parseXml(const base::XmlReader &aXmlReader, base::XmlReader::Element *aElement);
    virtual xpr_bool_t write(base::XmlWriter &aXmlWriter) const;

    virtual Format *newClone(void) const;

public:
    inline CaseTargetType getTargetType(void) const { return mTargetType; }
    inline CaseType getCaseType(void) const { return mCaseType; }
    inline const xpr::string &getSkipChars(void) const { return mSkipChars; }

public:
    static xpr_bool_t canParseXml(const xpr::string &aElementName);

    static Format *newObject(void);

private:
    static void convertUpperAtFirstChar(xpr::string &aName, const xpr::string &aSkipChars);
    static void convertUpperAtFirstCharOnEveryWord(xpr::string &aName, const xpr::string &aSkipChars);

    static void toString(const CaseTargetType &aTargetType, xpr::string &aString);
    static void toString(const CaseType &aCaseType, xpr::string &aString);
    static void toType(const xpr::string &aString, CaseTargetType &aTargetType);
    static void toType(const xpr::string &aString, CaseType &aCaseType);

private:
    CaseTargetType mTargetType;
    CaseType       mCaseType;
    xpr::string    mSkipChars;
};
} // namespace cmd
} // namespace fxfile

#endif // __FXFILE_FORMAT_CASE_H__
