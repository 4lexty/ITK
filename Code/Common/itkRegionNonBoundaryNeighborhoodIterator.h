/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkRegionNonBoundaryNeighborhoodIterator.h
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
#ifndef __itkRegionNonBoundaryNeighborhoodIterator_h
#define __itkRegionNonBoundaryNeighborhoodIterator_h

#include "itkRegionNeighborhoodIterator.h"

namespace itk {

/**
 * \class RegionNonBoundaryNeighborhoodIterator
 * \brief Subclass of NeighborhoodIterator designed for fast iteration
 * over the non-boundary pixels of an itk::Image region of interest.
 *
 * RegionNonBoundaryNeighborhoodIterator is the simple iterator
 * optimized for fast iteration of a pixel neighborhood over an
 * image's non-boundary pixels.  It performs no bounds checking and does
 * not handle boundary conditions on dereferencing.  The non-boundary pixels
 * are determined at construction from the region information contained
 * in the image.
 * 
 * \sa NeighborhoodIterator
 * \sa RegionBoundaryNeighborhoodIterator
 * \sa SmartRegionNeighborhoodIterator
 * \sa Neighborhood
 * \sa NeighborhoodAlgorithm
 */
 
template<class TImage,
  class TAllocator
    = NeighborhoodAllocator<ITK_TYPENAME TImage::InternalPixelType *>,
  class TDerefAllocator
    = NeighborhoodAllocator<ITK_TYPENAME TImage::PixelType>
  >
class ITK_EXPORT RegionNonBoundaryNeighborhoodIterator
  : public RegionNeighborhoodIterator<TImage, TAllocator, TDerefAllocator>
{
public:

  /** 
   * Standard "Self" & Superclass typedef support.
   */
  typedef RegionNonBoundaryNeighborhoodIterator Self;
  typedef RegionNeighborhoodIterator<TImage, TAllocator,
    TDerefAllocator> Superclass;

  /**
   * Extract image type information.
   */
  typedef typename Superclass::InternalPixelType InternalPixelType;
  typedef typename Superclass::PixelType PixelType;
  enum {Dimension = Superclass::Dimension };
  
  /**
   * Some common itk object typedefs
   */
  typedef typename Superclass::ImageType ImageType;
  typedef typename Superclass::RegionType RegionType;
  typedef typename Superclass::SizeType SizeType;
  typedef typename Superclass::NeighborhoodType NeighborhoodType;
  typedef typename Superclass::IndexType IndexType;
  typedef typename Superclass::OffsetType OffsetType;

  /**
   * Scalar data type typedef support
   */
  typedef typename Superclass::ScalarValueType ScalarValueType;

  /**
   * Default constructor
   */
  RegionNonBoundaryNeighborhoodIterator() {};

  /**
   * Copy constructor
   */
  RegionNonBoundaryNeighborhoodIterator(const Self& other)
    : RegionNeighborhoodIterator<TImage, TAllocator, TDerefAllocator>(other)
  {}
  
  /**
   * Assignment operator
   */
  Self &operator=(const Self& orig)
  {
    Superclass::operator=(orig);
    return *this;
  }
  
  /**
  * Constructor establishes a neighborhood of iterators of a specified
  * dimension to walk a particular image and a particular region of
  * that image.
  */
  RegionNonBoundaryNeighborhoodIterator(const SizeType &, ImageType *,
                                        const RegionType &);

  /**
   * Return an iterator for the beginning of the region.
   */
  Self Begin() const;

  /**
   * Return an iterator for the end of the region.
   */
  Self End() const; 

  /**
   * 
   */
  void SetEnd()
  {    m_EndPointer = this->End().operator[](this->Size()>>1);  }
  
  /**
   *
   */
  void SetToBegin()
  {    *this = this->Begin();  }

};
  
} // namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkRegionNonBoundaryNeighborhoodIterator.txx"
#endif

#endif 

