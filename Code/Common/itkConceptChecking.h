/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkConceptChecking.h
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
#ifndef _itkConceptChecking_h
#define _itkConceptChecking_h

#include "itkMacro.h"

/**
 * Concept checking based on paper
 * "Concept Checking: Binding Parametric Polymorphism in C++"
 * by Jeremy Siek and Andrew Lumsdaine, University of Notre Dame.
 *
 * ITK_CLASS_REQUIRES macros adapted from BOOST libraries (www.boost.org).
 * Several standard concepts also adapted from BOOST.
 */

// Don't do concept checks for MSVC++.
#if !defined(ITK_CONCEPT_CHECKING) || defined(_MSC_VER)

// Define fake concept specification macros.
// Concept checks will not be done.

#define ITK_FUNCTION_REQUIRES(type_var, concept)
#define ITK_FUNCTION_REQUIRES2(type_var1, type_var2, concept)
#define ITK_FUNCTION_REQUIRES3(type_var1, type_var2, type_var3, concept)
#define ITK_FUNCTION_REQUIRES4(type_var1, type_var2, type_var3, type_var4, concept)

#define ITK_CLASS_REQUIRES(type_var, concept)
#define ITK_CLASS_REQUIRES2(type_var1, type_var2, concept)
#define ITK_CLASS_REQUIRES3(type_var1, type_var2, type_var3, concept)
#define ITK_CLASS_REQUIRES4(type_var1, type_var2, type_var3, type_var4, concept)

#else

// Define real concept specification macros.
// Concept checks will be done.

#define ITK_FUNCTION_REQUIRES(type_var, concept) \
  void (::itk::Concepts::concept <type_var>::* var##type_var##concept)() = &::itk::Concepts::concept<type_var>::constraints; \
  ::itk::Concepts::ignore_unused_variable(var##type_var##concept)

#define ITK_FUNCTION_REQUIRES2(type_var1, type_var2, concept) \
  void (::itk::Concepts::concept <type_var1,type_var2>::* var##type_var1##type_var2##concept)() = &::itk::Concepts::concept<type_var1,type_var2>::constraints; \
  ::itk::Concepts::ignore_unused_variable(var##type_var1##type_var2##concept)

#define ITK_FUNCTION_REQUIRES3(type_var1, type_var2, type_var3, concept) \
  void (::itk::Concepts::concept <type_var1,type_var2,type_var3>::* var##type_var1##type_var2##type_var3##concept)() = &::itk::Concepts::concept<type_var1,type_var2,type_var3>::constraints; \
  ::itk::Concepts::ignore_unused_variable(var##type_var1##type_var2##type_var3##concept)

#define ITK_FUNCTION_REQUIRES4(type_var1, type_var2, type_var3, type_var4, concept) \
  void (::itk::Concepts::concept <type_var1,type_var2,type_var3,type_var4>::* var##type_var1##type_var2##type_var3##type_var4##concept)() = &::itk::Concepts::concept<type_var1,type_var2,type_var3,type_var4>::constraints; \
  ::itk::Concepts::ignore_unused_variable(var##type_var1##type_var2##type_var3##type_var4##concept)

#define ITK_CLASS_REQUIRES(type_var, concept) \
  typedef void (::itk::Concepts::concept <type_var>::* func##type_var##concept)(); \
  template <func##type_var##concept> \
  struct concept_checking_##type_var##concept { }; \
  typedef concept_checking_##type_var##concept< \
    &::itk::Concepts::concept <type_var>::constraints> \
    concept_checking_typedef_##type_var##concept

#define ITK_CLASS_REQUIRES2(type_var1, type_var2, concept) \
  typedef void (::itk::Concepts::concept <type_var1,type_var2>::* func##type_var1##type_var2##concept)(); \
  template <func##type_var1##type_var2##concept> \
  struct concept_checking_##type_var1##type_var2##concept { }; \
  typedef concept_checking_##type_var1##type_var2##concept< \
    &::itk::Concepts::concept <type_var1,type_var2>::constraints> \
    concept_checking_typedef_##type_var1##type_var2##concept

#define ITK_CLASS_REQUIRES3(type_var1, type_var2, type_var3, concept) \
  typedef void (::itk::Concepts::concept <type_var1,type_var2,type_var3>::* func##type_var1##type_var2##type_var3##concept)(); \
  template <func##type_var1##type_var2##type_var3##concept> \
  struct concept_checking_##type_var1##type_var2##type_var3##concept { }; \
  typedef concept_checking_##type_var1##type_var2##type_var3##concept< \
    &::itk::Concepts::concept <type_var1,type_var2,type_var3>::constraints>  \
  concept_checking_typedef_##type_var1##type_var2##type_var3##concept

#define ITK_CLASS_REQUIRES4(type_var1, type_var2, type_var3, type_var4, concept) \
  typedef void (::itk::Concepts::concept <type_var1,type_var2,type_var3,type_var4>::* func##type_var1##type_var2##type_var3##type_var4##concept)(); \
  template <func##type_var1##type_var2##type_var3##type_var4##concept> \
  struct concept_checking_##type_var1##type_var2##type_var3##type_var4##concept { }; \
  typedef concept_checking_##type_var1##type_var2##type_var3##type_var4##concept< \
    &::itk::Concepts::concept <type_var1,type_var2,type_var3,type_var4>::constraints>  \
    concept_checking_typedef_##type_var1##type_var2##type_var3##type_var4##concept

#endif

namespace itk
{
namespace Concepts
{

/**
 * Concept checks may require a variable to be declared but not used.
 * This function can be called with the variable to prevent the compiler
 * warning.
 */
template <typename T>
inline void ignore_unused_variable(const T&)
{
}


/*@{
 * Utility class to guarantee a unique type.
 */
template <typename T> struct UniqueTypeForType {};
template <int> struct UniqueTypeFor_int {};
template <unsigned int> struct UniqueTypeFor_unsigned_int {};
//@}

/**
 * Concept checks may require that an expression be convertible to bool.
 * Passing the expression to this function will enforce this requirement.
 */
template <class T>
void require_boolean_expr(const T& t)
{
  bool x = t;
  ignore_unused_variable(x);
}


/**
 * Concept that requires a default constructor.
 */
template <typename T>
struct DefaultConstructorConcept
{
  void constraints()
    {
      T a;
      ignore_unused_variable(a);
    }
};


/**
 * Concept that requires a copy constructor.
 */
template <class T>
struct CopyConstructorConcept
{
  void constraints()
    {
      T b(a);
    }
  void const_constraints(const T& b)
    {
      T c(b); // require copy constructor to take a const argument.
      ignore_unused_variable(c);
    }
  T a;
};


/**
 * Concept that requires an assignment operator.
 */
template <typename T>
struct AssignmentOperatorConcept
{
  void constraints()
    {
      a = a;
      const_constraints(a);
    }
  void const_constraints(const T& b)
    {
      a = b; // require const argument to assignment operator.
    }
  T a;
};


/**
 * Concept that requires math operators +,-,+=,-=, =.
 */
template <typename T>
struct AdditiveOperatorsConcept
{
  void constraints()
    {
      // Require that the operators exist, and that their results can
      // convert back to type T in an assignment.
      a = b + b;
      a = b - b;
      a += b;
      a -= b;
      const_constraints(b);
    }
  void const_constraints(const T& c)
    {
      // Require that operators work with const argument types.
      a = c + c;
      a = c - c;
      a += c;
      a -= c;
    }
  T a, b;
};


/**
 * Concept that requires math operators *,/,*=,/=, =.
 */
template <typename T>
struct MultiplicativeOperatorsConcept
{
  void constraints()
    {
      // Require that the operators exist, and that their results can
      // convert back to type T in an assignment.
      a = b * b;
      a = b / b;
      a *= b;
      a /= b;
      const_constraints(b);
    }
  void const_constraints(const T& c)
    {
      // Require that operators work with const argument types.
      a = c * c;
      a = c / c;
      a *= c;
      a /= c;      
    }
  T a, b;
};


/**
 * Concept that requires comparison operators <, >, <=, >=, ==, !=.
 */
template <class T>
struct ComparisonOperatorsConcept
{
  void constraints()
    {
      // Require that the operators exist and that they have a result
      // that is convertible to bool.
      require_boolean_expr(a < b);
      require_boolean_expr(a > b);
      require_boolean_expr(a <= b);
      require_boolean_expr(a >= b);
      require_boolean_expr(a == b);
      require_boolean_expr(a != b);
      const_constraints(a, b);
    }
  void const_constraints(const T& c, const T& d)
    {
      // Require that the operators can be applied to const arguments.
      require_boolean_expr(c < d);
      require_boolean_expr(c > d);
      require_boolean_expr(c <= d);
      require_boolean_expr(c >= d);
      require_boolean_expr(c == d);
      require_boolean_expr(c != d);
    }
  T a, b;
};


/**
 * Concept that requires a conversion from its first type to its second.
 */
template <typename T1, typename T2>
struct ConvertibleConcept
{
  void constraints()
    {
      T2 b = a;
      ignore_unused_variable(b);
    }
  T1 a;
};


/**
 * Concept that requires two types to be exactly the same.
 */
template <typename T1, typename T2>
struct SameTypeConcept
{
  void constraints()
    {
      UniqueTypeForType<T1> x = UniqueTypeForType<T2>();
      ignore_unused_variable(x);
    }
};


/**
 * Concept that requires two dimension values to be the same.
 */
template <unsigned int D1, unsigned int D2>
struct SameDimensionConcept
{
  void constraints()
    {
      UniqueTypeFor_unsigned_int<D1> x = UniqueTypeFor_unsigned_int<D2>();
      ignore_unused_variable(x);
    }
};


/**
 * Concept that requires the standard "Self", "Pointer", and "ConstPointer"
 * typedefs in a class.
 */
template <class T>
struct itkStandardTypedefsConcept
{
  void constraints()
    {
      typedef typename T::Self Self;
      typedef typename T::Pointer Pointer;
      typedef typename T::ConstPointer ConstPointer;
    }
};


/**
 * Concept that requires that a type models the itk::IndexedContainerInterface.
 */
template <class T>
struct IndexedContainerInterfaceConcept
{
  void constraints()
    {
      typedef typename T::Self Self;
      typedef typename T::Pointer Pointer;
      typedef typename T::ConstPointer ConstPointer;
      typedef typename T::ElementIdentifier ElementIdentifier;
      typedef typename T::Element Element;
      typedef typename T::Iterator Iterator;
      typedef typename T::ConstIterator ConstIterator;
      
      Element& e1 = a.ElementAt(element_identifier);
      Element& e2 = a.CreateElementAt(element_identifier);
      a.SetElement(element_identifier, element);
      a.InsertElement(element_identifier, element);
      a.CreateIndex(element_identifier);
      a.DeleteIndex(element_identifier);
      itr = a.Begin();
      itr = a.End();
      a.Reserve(element_identifier);
      a.Squeeze();
      const_constraints(a);
      
      ignore_unused_variable(e1);
      ignore_unused_variable(e2);
    }
  void const_constraints(const T& b)
    {
      element = b.GetElement(element_identifier);
      require_boolean_expr(b.IndexExists(element_identifier));
      require_boolean_expr(b.GetElementIfIndexExists(element_identifier, &element));
      c_itr = a.Begin();
      c_itr = a.End();
      ul = b.Size();
    }
  T a;
  typename T::ElementIdentifier element_identifier;
  typename T::Element element;
  typename T::Iterator itr;
  typename T::ConstIterator c_itr;
  unsigned long ul;
};


} // namespace Concepts
} // namespace itk

#endif
