/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkTranslationTransformTest.cxx
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

#include <iostream>

#include "itkTranslationTransform.h"
#include "itkPhysicalImage.h"
#include "vnl/vnl_vector_fixed.h"

typedef  itk::Vector<double,2>     VectorType;


void PrintVector( const VectorType & v )
{
  for( unsigned int i=0; i<VectorType::VectorDimension; i++)
  {
    std::cout << v[i] << ", ";
  }
  std::cout << std::endl;
}


int main(
    int argc,
    char *argv[])
{


    int any = 0;       // Any errors detected in testing?

    VectorType                   vector2;

    int i;

    /* FIXME: This code exercises most of the methods but doesn't
       actually check that the results are correct. */
    any = 0;
    std::cout << "The TranslationTransform class is still being implemented"
              << std::endl;

    /* Create a 2D identity transformation and show its parameters */
    itk::TranslationTransform<double,2> id2;
    vector2 = id2.GetOffset();
        std::cout << "Vector from instantiating an identity transform:"
              << std::endl;
    PrintVector( vector2 );
    
    /* Create and show a simple 2D transform from given parameters */
    vector2[0] = 5;
    vector2[1] = 6;
    itk::TranslationTransform<double,2> aff2(vector2);
    for (i = 0; i < 2; i++) 
    {
        vector2[i]    = 0.0;
    }
    std::cout << "Instantiation of a given 2D transform:" 
              << std::endl << aff2;

    itk::TranslationTransform<double,2> inverse2;
    inverse2 = aff2.Inverse();
    std::cout << "Inverse of the given transform:"
              << std::endl << inverse2;

    /* Set parameters of a 2D transform */
    vector2[0] = 2;
    vector2[1] = 1;
    aff2.SetOffset(vector2);
    for (i = 0; i < 2; i++) 
    {
        vector2[i]    = 0.0;
    }
   
    vector2 = aff2.GetOffset();
    std::cout << "Setting the offset in an existing  transform:"
              << std::endl;
    PrintVector( vector2 );

    /* Try composition of two transformations */
    aff2.Compose(aff2);
    std::cout << "Result of a composition:"
              << std::endl << aff2;

    /* Compose with a translation */
    VectorType trans;
    trans[0] = 1;
    trans[1] = 2;
    aff2.Translate(trans);
    std::cout << "Result of a translation:"
              << std::endl << aff2;

        /* Transform a point */
    itk::Point<double, 2> u2, v2;
    u2[0] = 3;
    u2[1] = 5;
    v2 = aff2.Transform(u2);
    std::cout << "Transform a point:" << std::endl
              << v2[0] << " , " << v2[1] << std::endl;

    /* Back transform a point */
    v2 = aff2.BackTransform(u2);
    std::cout << "Back transform a point:" << std::endl
              << v2[0] << " , " << v2[1] << std::endl;

    /* Transform a vnl_vector */
    vnl_vector_fixed<double, 2> x2, y2;
    x2[0] = 1;
    x2[1] = 2;
    y2 = aff2.Transform(x2);
    std::cout << "Transform a vnl_vector:" << std::endl
              << y2[0] << " , " << y2[1] << std::endl;

    /* Back transform a vector */
    y2 = aff2.BackTransform(x2);
    std::cout << "Back transform a vnl_vector:" << std::endl
              << y2[0] << " , " << y2[1] << std::endl;

    /* Transform a vector */
    itk::Vector<double, 2> u3, v3;
    u3[0] = 3;
    u3[1] = 5;
    v3 = aff2.Transform(u3);
    std::cout << "Transform a vector:" << std::endl
              << v3[0] << " , " << v3[1] << std::endl;

    /* Back transform a vector */
    v3 = aff2.BackTransform(u3);
    std::cout << "Back transform a vector :" << std::endl
              << v3[0] << " , " << v3[1] << std::endl;

    /* Transform a Covariant vector */
    itk::Vector<double, 2> u4, v4;
    u4[0] = 3;
    u4[1] = 5;
    v4 = aff2.Transform(u4);
    std::cout << "Transform a Covariant vector:" << std::endl
              << v4[0] << " , " << v4[1] << std::endl;

    /* Back transform a vector */
    v4 = aff2.BackTransform(u4);
    std::cout << "Back transform a vector :" << std::endl
              << v4[0] << " , " << v4[1] << std::endl;

    return any;
}
