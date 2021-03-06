//
// Copyright (c) 2012 Leon Lee author. All rights reserved.
//
// Use of this source code is governed by a GPLv3 license that can be
// found in the LICENSE file.

#ifndef __XPR_GUID_H__
#define __XPR_GUID_H__ 1
#pragma once

#include "xpr_types.h"
#include "xpr_dlsym.h"
#include "xpr_string.h"

namespace xpr
{
class XPR_DL_API Guid
{
public:
    Guid(void);
    Guid(const Guid &aGuid);
    virtual ~Guid(void);

public:
    xpr_rcode_t generate(void);
    void        clear(void);
    xpr_sint_t  compare(const Guid &aGuid2) const;
    xpr_bool_t  test(const Guid &aGuid2) const;
    xpr_bool_t  none(void) const;

public:
    xpr_bool_t  toString(xpr::string &aString, xpr_bool_t aWithBrace = XPR_TRUE) const;
    xpr_bool_t  fromString(const xpr::string &aString);

public:
    xpr_size_t  getBufferSize(void) const;
    xpr_bool_t  toBuffer(xpr_byte_t *aBuffer) const;
    xpr_bool_t  fromBuffer(const xpr_byte_t *aBuffer);

public:
    Guid& operator = (const Guid &aGuid);

protected:
    xpr_uint32_t mData1;
    xpr_uint16_t mData2;
    xpr_uint16_t mData3;
    xpr_uint8_t  mData4[8];
};

XPR_INLINE bool operator== (const Guid &aGuid1, const Guid &aGuid2)
{
    return (aGuid1.compare(aGuid2) == 0) ? true : false;
}

XPR_INLINE bool operator!= (const Guid &aGuid1, const Guid &aGuid2)
{
    return (aGuid1.compare(aGuid2) != 0) ? true : false;
}

XPR_INLINE bool operator< (const Guid &aGuid1, const Guid &aGuid2)
{
    return (aGuid1.compare(aGuid2) < 0) ? true : false;
}

XPR_INLINE bool operator> (const Guid &aGuid1, const Guid &aGuid2)
{
    return (aGuid1.compare(aGuid2) > 0) ? true : false;
}

const Guid GuidNone;
} // namespace xpr

#endif // __XPR_GUID_H__
