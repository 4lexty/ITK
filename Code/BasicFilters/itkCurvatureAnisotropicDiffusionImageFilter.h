/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkCurvatureAnisotropicDiffusionImageFilter.h
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
#ifndef __itkCurvatureAnisotropicDiffusionImageFilter_h
#define __itkCurvatureAnisotropicDiffusionImageFilter_h

#include "itkNeighborhoodOperator.h"
#include "itkImage.h"
#include "itkRegionBoundaryNeighborhoodIterator.h"
#include "itkRegionNonBoundaryNeighborhoodIterator.h"
#include "itkAnisotropicDiffusionImageFilter.h"

namespace itk
{

template <class TInnerProduct,  class TIterator>
struct ITK_EXPORT AnisoDiffuseCurve2D : public DiffusionStrategy
{
  AnisoDiffuseCurve2D() {}
  AnisoDiffuseCurve2D(float c) : DiffusionStrategy(c) {}
  virtual void operator()(void *, void *);
};

template <class TInnerProduct,  class TIterator>
struct ITK_EXPORT AnisoDiffuseCurveND : public DiffusionStrategy
{
  AnisoDiffuseCurveND() {}
  AnisoDiffuseCurveND(float c) : DiffusionStrategy(c) {}
  virtual void operator()(void *, void *);
};
  
/**
 * \class CurvatureAnisotropicDiffusionImageFilter
 *
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT CurvatureAnisotropicDiffusionImageFilter :
    public AnisotropicDiffusionImageFilter< TInputImage, TOutputImage >
{
public:
  /**
   * Standard "Self" & Superclass typedef.
   */
  typedef CurvatureAnisotropicDiffusionImageFilter Self;
  typedef AnisotropicDiffusionImageFilter<TInputImage, TOutputImage> Superclass;

  /**
   * Extract some information from the image types.  Dimensionality
   * of the two images is assumed to be the same.
   */
  typedef typename Superclass::OutputPixelType OutputPixelType;
  typedef typename Superclass::OutputInternalPixelType OutputInternalPixelType;
  typedef typename Superclass::InputPixelType InputPixelType;
  typedef typename Superclass::InputInternalPixelType InputInternalPixelType;
  enum { ImageDimension = Superclass::ImageDimension };
  
  /**
   * Image typedef support
   */
  typedef typename Superclass::InputImageType  InputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;

  /** 
   * Smart pointer typedef support 
   */
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /**
   * Run-time type information (and related methods)
   */
  itkTypeMacro(CurvatureAnisotropicDiffusionImageFilter,
               AnisotropicDiffusionImageFilter);
  
  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);

protected:
  CurvatureAnisotropicDiffusionImageFilter() {}
  virtual ~CurvatureAnisotropicDiffusionImageFilter() {}
  CurvatureAnisotropicDiffusionImageFilter(const Self&) {}
  void operator=(const Self&) {}

  virtual UpdateStrategy *GetUpdateStrategy()
  { return new UpdateStrategyScalar<OutputImageType, OutputImageType>;}
  virtual DiffusionStrategy *GetDiffusionStrategy()
  {
    typedef RegionNonBoundaryNeighborhoodIterator<OutputImageType> RNI;
    typedef RegionBoundaryNeighborhoodIterator<OutputImageType> RBI;
    typedef NeighborhoodAlgorithm::IteratorInnerProduct<RNI,
      NeighborhoodOperator<OutputPixelType, ImageDimension> > SNIP;
    typedef NeighborhoodAlgorithm::BoundsCheckingIteratorInnerProduct<RBI,
      NeighborhoodOperator<OutputPixelType, ImageDimension> > SBIP;
    
    if (ImageDimension == 2)
      {
        return new CompositeDiffusionStrategy(
                                    new AnisoDiffuseCurve2D<SNIP, RNI>(),
                                    new AnisoDiffuseCurve2D<SBIP, RBI>(),
                                    this->GetConductanceParameter());
      }
    else
      {
        return new CompositeDiffusionStrategy(
                                    new AnisoDiffuseCurveND<SNIP, RNI>(),
                                    new AnisoDiffuseCurveND<SBIP, RBI>(),
                                    this->GetConductanceParameter());
      }
  }

  virtual CopyStrategy *GetCopyStrategy()
  { return new CopyStrategyScalar<InputImageType, OutputImageType>;  }
};
  
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkCurvatureAnisotropicDiffusionImageFilter.txx"
#endif

#endif
