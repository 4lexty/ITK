/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkGaussianOperator.h
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
#ifndef __itkGaussianOperator_h
#define __itkGaussianOperator_h

#include "itkNeighborhoodOperator.h"
#include <math.h>
namespace itk {

/**
 * \class GaussianOperator
 * \brief A NeighborhoodOperator whose coefficients are a one
 * dimensional, discrete Gaussian kernel.
 *
 * GaussianOperator can be used to perform Gaussian blurring
 * by taking its inner product with to a Neighborhood
 * (NeighborhooIterator) that is swept across an image region.
 * It is a directional operator.  N successive applications
 * oriented along each dimensional direction will effect separable,
 * efficient, N-D Gaussian blurring of an image region.
 *
 * GaussianOperator takes two parameters:
 
 * (1) The floating-point variance of the desired Gaussian function.
 
 * (2) The "maximum error" allowed in the discrete Gaussian
 * function.  "Maximum errror" is defined as the difference between the area
 * under the discrete Gaussian curve and the area under the continuous
 * Gaussian. Maximum error affects the Gaussian operator size. Care should
 * be taken not to make this value too small relative to the variance
 * lest the operator size become unreasonably large.
 *
 * References:
 * The Gaussian kernel contained in this operator was described
 * by Tony Lindeberg (Discrete Scale-Space Theory and the Scale-Space
 * Primal Sketch.  Dissertation. Royal Institute of Technology, Stockholm,
 * Sweden. May 1991.).
 *
 * \sa NeighborhoodOperator
 * \sa NeighborhoodIterator
 * \sa Neighborhood
 *
 */
template<unsigned int VDimension=2,
  class TAllocator = NeighborhoodAllocator<float> >
class ITK_EXPORT GaussianOperator
  : public NeighborhoodOperator<float, VDimension, TAllocator>
{
public:
  /**
   * Standard "Self" typedef support.
   */
  typedef GaussianOperator Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef NeighborhoodOperator<float, VDimension, TAllocator>  Superclass;

  /**
   * Constructor.
   */
  GaussianOperator() : m_Variance(1), m_MaximumError(.01) { }

  /**
   * Copy constructor
   */
  GaussianOperator(const Self &other)
    : NeighborhoodOperator<float, VDimension, TAllocator>(other)
  {
    m_Variance = other.m_Variance;
    m_MaximumError = other.m_MaximumError;
  }

  /**
   * Assignment operator
   */
  Self &operator=(const Self &other)
  {
    Superclass::operator=(other);
    m_Variance = other.m_Variance;
    m_MaximumError = other.m_MaximumError;
    return *this;
  }
  
  /**
   * Sets the desired variance of the Gaussian kernel.
   */
  void SetVariance(const float &variance)
  {  m_Variance = variance;  }

  /**
   * Sets the desired maximum error of the gaussian approximation.  Maximum
   * error is the difference between the area under the discrete Gaussian curve
   * and the area under the continuous Gaussian. Maximum error affects the
   * Gaussian operator size.
   */
  void SetMaximumError(const float &max_error)
  {
    if (m_MaximumError >= 1 || m_MaximumError <= 0)
      {
        throw ExceptionObject();
      }
    
    m_MaximumError = max_error;
  }
  
  /**
   * Returns the variance of the Gaussian (scale) for the operator.
   */
  float GetVariance()
  {  return m_Variance;  }

  /**
   * Returns the maximum error of the gaussian approximation.  Maximum error is 
   * the difference between the area under the discrete Gaussian curve and the
   * area under the continuous Gaussian. Maximum error affects the Gaussian
   * operator size.
   */
  float GetMaximumError()
  {    return m_MaximumError;  }

  /**
   * Prints some debugging information.
   */
  virtual void PrintSelf(std::ostream &os, Indent i) const
  {
    os << i << "GaussianOperator { this=" << this
       << ", m_Variance = " << m_Variance
       << ", m_MaximumError = " << m_MaximumError
       << "} "  << std::endl;
    Superclass::PrintSelf(os, i.GetNextIndent());
  }
  
protected:
  typedef std::vector<float> CoefficientVector;

  /**
   * Returns the value of the modified Bessel function I0(x) at a point x >= 0.
   */
  float ModifiedBesselI0(float);

  /**
   * Returns the value of the modified Bessel function I1(x) at a point x,
   * x real. 
   */
  float ModifiedBesselI1(float);

  /**
   * Returns the value of the modified Bessel function Ik(x) at a point x>=0,
   * where k>=2.
   */
  float ModifiedBesselI(int, float);

  /**
   * Calculates operator coefficients.
   */
  CoefficientVector GenerateCoefficients();

  /**
   * Arranges coefficients spatially in the memory buffer.
   */
  void Fill(const CoefficientVector& coeff)
  {    this->FillCenteredDirectional(coeff);  }

private:
  /**
   * Desired variance of the discrete Gaussian function.
   */
  float m_Variance;
  
  /**
   * Difference between the areas under the curves of the continuous and
   * discrete Gaussian functions.
   */
  float m_MaximumError;
  
};

} // namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkGaussianOperator.txx"
#endif

#endif
