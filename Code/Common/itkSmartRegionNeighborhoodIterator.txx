/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSmartRegionNeighborhoodIterator.txx
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
#ifndef _itkSmartRegionNeighborhoodIterator_txx
#define _itkSmartRegionNeighborhoodIterator_txx
namespace itk {

template<class TImage, class TAllocator, class TBoundaryCondition,
    class TDerefAllocator>
SmartRegionNeighborhoodIterator<TImage, TAllocator, TBoundaryCondition,
  TDerefAllocator>
::SmartRegionNeighborhoodIterator(const Self& orig)
  : NeighborhoodIterator<TImage, TAllocator, TDerefAllocator>(orig)
{
  m_InnerBoundsLow = orig.m_InnerBoundsLow;
  m_InnerBoundsHigh = orig.m_InnerBoundsHigh;
  for (unsigned int i = 0; i < Dimension; ++i)
    {
      m_InBounds[i] = orig.m_InBounds[i];
    }
  m_InternalBoundaryCondition = orig.m_InternalBoundaryCondition;

  // Check to see if the default boundary conditions have been
  // overridden.
  if ( orig.m_BoundaryCondition ==
       (ImageBoundaryConditionPointerType)&orig.m_InternalBoundaryCondition ) 
    {
      this->ResetBoundaryCondition();
    }
  else m_BoundaryCondition = orig.m_BoundaryCondition;        
}

template<class TImage, class TAllocator, class TBoundaryCondition,
    class TDerefAllocator>
SmartRegionNeighborhoodIterator<TImage, TAllocator, TBoundaryCondition,
  TDerefAllocator> &  
SmartRegionNeighborhoodIterator<TImage, TAllocator, TBoundaryCondition,
  TDerefAllocator>
::operator=(const Self& orig)
{
  Superclass::operator=(orig);
  m_InnerBoundsLow = orig.m_InnerBoundsLow;
  m_InnerBoundsHigh = orig.m_InnerBoundsHigh;
  for (unsigned int i = 0; i < Dimension; ++i)
    {
      m_InBounds[i] = orig.m_InBounds[i];
    }

  // Check to see if the default boundary conditions
  // have been overridden.
  if (orig.m_BoundaryCondition ==
      (ImageBoundaryConditionPointerType) &orig.m_InternalBoundaryCondition ) 
    {
      this->ResetBoundaryCondition();
    }
  else m_BoundaryCondition = orig.m_BoundaryCondition;
  return *this;
}
  
template<class TImage, class TAllocator, class TBoundaryCondition,
  class TDerefAllocator>
bool
SmartRegionNeighborhoodIterator<TImage, TAllocator, TBoundaryCondition,
  TDerefAllocator>
::InBounds()
{ 
  bool ans = true;
  for (unsigned int i=0; i<Dimension; i++)
    {
      if (m_Loop[i] < m_InnerBoundsLow[i] || m_Loop[i] >= m_InnerBoundsHigh[i])
        {
          m_InBounds[i] = ans = false;
        }
    }
  return ans;
}

template<class TImage, class TAllocator, class TBoundaryCondition,
  class TDerefAllocator>
typename SmartRegionNeighborhoodIterator<TImage, TAllocator,
  TBoundaryCondition, TDerefAllocator>::NeighborhoodType
SmartRegionNeighborhoodIterator<TImage, TAllocator, TBoundaryCondition,
  TDerefAllocator>
::GetNeighborhood()
{
  register unsigned int i;
  OffsetType OverlapLow, OverlapHigh, temp, offset;
  bool flag;

  const Iterator _end = this->end();
  NeighborhoodType ans;
  typename NeighborhoodType::Iterator ans_it;
  Iterator this_it;

  ans.SetRadius( this->GetRadius() );
  
  if (InBounds())
    {
      for (ans_it = ans.begin(), this_it = this->begin();
           this_it < _end; ans_it++, this_it++)
        { *ans_it = **this_it; }
    }
  else
    {
      // Calculate overlap & initialize index
      for (i=0; i<Dimension; i++)
        {
          OverlapLow[i] = m_InnerBoundsLow[i] - m_Loop[i];
          OverlapHigh[i]=
            this->GetSize(i) - ( (m_Loop[i]+2) - m_InnerBoundsHigh[i] );
          temp[i] = 0;
        }

      // Iterate through neighborhood
      for (ans_it = ans.begin(), this_it = this->begin();
           this_it < _end; ans_it++, this_it++)
        {
          flag = true;
          
          // Is this pixel in bounds?
          for (i=0; i<Dimension; ++i)
            {
              if (m_InBounds[i]) offset[i] = 0; // this dimension in bounds
              else  // part of this dimension spills out of bounds
                {
                  if (temp[i] < OverlapLow[i])
                    {
                      flag = false;
                      offset[i] = OverlapLow[i] - temp[i];
                    }
                  else if ( OverlapHigh[i] < temp[i] )
                    {
                      flag = false;
                      offset[i] =  OverlapHigh[i] - temp[i];
                    }
                  else offset[i] = 0;
                }
            }
          
          if (flag) *ans_it = **this_it;
          else *ans_it = m_BoundaryCondition->operator()(temp, offset, this);
          
          for (i=0; i<Dimension; ++i)  // Update index
            {
              temp[i]++;
              if ( temp[i] == this->GetSize(i) ) temp[i]= 0;
              else break;
            }
        } 
    }
  return ans;
}

template<class TImage, class TAllocator, class TBoundaryCondition,
  class TDerefAllocator>
void
SmartRegionNeighborhoodIterator<TImage, TAllocator, TBoundaryCondition,
  TDerefAllocator>
::SetNeighborhood(NeighborhoodType &N)
{
  register unsigned int i;
  OffsetType OverlapLow, OverlapHigh, temp;
  bool flag;
      
  const Iterator _end = this->end();
  Iterator this_it;
  typename  NeighborhoodType::Iterator N_it;
  
  if (InBounds())
    {
      for (N_it = N.begin(), this_it = begin(); this_it < _end;
           this_it++, N_it++)
        {
          **this_it = *N_it;
        }  
    }
  else
    {
      // Calculate overlap & initialize index
      for (i=0; i<Dimension; i++)
        {
          OverlapLow[i] =m_InnerBoundsLow[i] - m_Loop[i];
          OverlapHigh[i]=
            this->GetSize(i) - (m_Loop[i]-m_InnerBoundsHigh[i])-1;
          temp[i] = 0;
        }
      
      // Iterate through neighborhood
      for (N_it = N.begin(), this_it = this->begin();
           this_it < _end; N_it++, this_it++)
        {
          flag = true;
          for (i=0; i<Dimension; ++i)
            {
              if (!m_InBounds[i] && ((temp[i] < OverlapLow[i])
                                     || (temp[i] >= OverlapHigh[i])) )
                {
                  flag=false;
                  break;
                }
            }
          
          if (flag)
            {
              **this_it = *N_it;
            }
          
          for (i=0; i<Dimension; ++i)  // Update index
            {
              temp[i]++;
              if ( temp[i] == this->GetSize(i) ) temp[i]= 0;
              else break;
            }
        }
      
    }
}

template<class TImage, class TAllocator, class TBoundaryCondition,
  class TDerefAllocator>
void SmartRegionNeighborhoodIterator<TImage, TAllocator, TBoundaryCondition,
  TDerefAllocator>
::PrintSelf(std::ostream &os, Indent indent) const
{
  unsigned int i;
  os << indent << "SmartRegionNeighborhoodIterator { this = " << this
     << ", m_InnerBoundsLow = { ";
  for (i = 0; i<Dimension; i++) os << m_InnerBoundsLow[i] << " ";
  os << "}, m_InnerBoundsHigh = { ";
  for (i = 0; i<Dimension; i++) os << m_InnerBoundsHigh[i] << " ";
  os << "} }" << std::endl;
  Superclass::PrintSelf(os, indent.GetNextIndent());
}

template<class TImage, class TAllocator, class TBoundaryCondition,
  class TDerefAllocator>
void SmartRegionNeighborhoodIterator<TImage, TAllocator, TBoundaryCondition,
  TDerefAllocator>
::SetBound(const SizeType& size)
{
  SizeType radius  = this->GetRadius();
  const unsigned long *offset     = m_Image->GetOffsetTable();
  const IndexType imageRRStart  = m_Image->GetRequestedRegion().GetIndex();
  SizeType imageRRSize = m_Image->GetRequestedRegion().GetSize();
  SizeType imageBufferSize = m_Image->GetBufferedRegion().GetSize();

  // Set the bounds and the wrapping offsets. Inner bounds are the loop
  // indicies where the iterator will begin to overlap the edge of the image
  // requested region.
  for (unsigned int i=0; i<Dimension; ++i)
    {
      m_Bound[i]          = m_StartIndex[i]+size[i];
      m_InnerBoundsHigh[i]= imageRRStart[i] + ( imageRRSize[i] - radius[i] );
      m_InnerBoundsLow[i] = imageRRStart[i] + radius[i];
      m_WrapOffset[i]     = (imageBufferSize[i] - (m_Bound[i]
                              - m_StartIndex[i])) * offset[i];
    }
}

template<class TImage, class TAllocator, class TBoundaryCondition,
  class TDerefAllocator>
SmartRegionNeighborhoodIterator<TImage, TAllocator, TBoundaryCondition,
  TDerefAllocator> 
SmartRegionNeighborhoodIterator<TImage, TAllocator, TBoundaryCondition,
  TDerefAllocator> 
::Begin() const 
{
  //Copy the current iterator
  Self it( *this );

  // Set the position to the m_BeginOffset
  it.SetLocation( this->m_StartIndex );

  return it;
}

template<class TImage, class TAllocator, class TBoundaryCondition,
  class TDerefAllocator>
SmartRegionNeighborhoodIterator<TImage, TAllocator, TBoundaryCondition,
  TDerefAllocator> 
SmartRegionNeighborhoodIterator<TImage, TAllocator, TBoundaryCondition,
  TDerefAllocator> 
::End() const
{
  IndexType endIndex;
  
  // Copy the current iterator
  Self it( *this );

  // Calculate the end index
  for (unsigned int i = 0; i< Dimension; ++i)
    {
      endIndex.m_Index[i] = m_Bound[i] -1;
    }

  // Set the position to the m_BeginOffset
  it.SetLocation( endIndex );

  ++it;

  return it;
}


} // end namespace itk

#endif
