/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    cxxArrayType.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include "cxxTypes.h"

#include <strstream>

namespace _cxx_
{


/**
 * Retrieve what kind of Type this is.
 */
RepresentationType
ArrayType
::GetRepresentationType() const
{
  return ArrayType_id;
}


/**
 * Try to cast the given Type to an ArrayType.  If this returns, the
 * pointer will be valid.  If the cast is not allowed, an exception is
 * thrown.
 */
ArrayType* ArrayType::SafeDownCast(Type* t)
{
  ArrayType* result = dynamic_cast<ArrayType*>(t);
  if(!result) { throw TypeDownCastException(t, ArrayType_id); }
  return result;
}


/**
 * Try to cast the given Type to an ArrayType.  If this returns, the
 * pointer will be valid.  If the cast is not allowed, an exception is
 * thrown.
 */
const ArrayType* ArrayType::SafeDownCast(const Type* t)
{
  const ArrayType* result = dynamic_cast<const ArrayType*>(t);
  if(!result) { throw TypeDownCastException(t, ArrayType_id); }
  return result;
}


String ArrayType::GenerateName(const String& indirection,
                               bool isConst, bool isVolatile) const
{
  std::strstream length;
  length << m_Length << std::ends;
  String lengthStr = length.str();
  String indirect = "";
  if(indirection.length() > 0)
    {
    indirect = "("+indirection+")";
    }
  return m_ElementType.GenerateName("", isConst, isVolatile)+indirection+"["+lengthStr+"]";
}


/**
 * Constructor takes the type of the elements in the array, and the length.
 */
ArrayType
::ArrayType(const CvQualifiedType& in_elementType, unsigned long in_length):
  m_ElementType(in_elementType),
  m_Length(in_length)
{
}

} // namespace _cxx_
