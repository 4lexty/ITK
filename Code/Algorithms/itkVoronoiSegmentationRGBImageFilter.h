/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVoronoiSegmentationRGBImageFilter.h
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

#ifndef _itkVoronoiSegmentationRGBImageFilter_h
#define _itkVoronoiSegmentationRGBImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkVoronoi2DDiagram.h"
#include "itkImage.h"

namespace itk
{

template <class TInputImage, class TOutputImage>
class VoronoiSegmentationRGBImageFilter:
public ImageToImageFilter<TInputImage,TOutputImage>
{
/** \class VoronoiSegmentationRGBImageFilter
 * 
 * Perform the segmentation of 2D images (RGB image) by Voronoi Diagram.
 * Used as a node of the segmentation toolkits.
 * The parameters here are: 
 * 1. the estimation of the statistics of the object. (mean and std.)
 * 2. the tolerance for the classification. (around the mean ans std. estimated value).
 * The parameters can also be automatically set by given a prior, as a binary image.
 *
 *
 * Detail information about this algorithm can be found in:
 *  " Semi-automated color segmentation of anatomical tissue,"
 *   C. Imelinska, M. Downes, and W. Yuan  
 *  Computerized Medical Imaging and Graphics, Vor.24, pp 173-180, 2000.
 *
 * the input image should be in the format of:
 * itkImage<itkVector<PixelType,3>, 2>.
 */
public:
  /**
   * Standard "Self" typedef.
   */
  typedef VoronoiSegmentationRGBImageFilter       Self;

  /** 
   * Smart pointer typedef support.
   */
  typedef SmartPointer <Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /**
   * Standard "Superclass" typedef.
   */
  typedef ImageToImageFilter<TInputImage,TOutputImage>   Superclass;


  /**
   * Run-time type information (and related methods).
   */
  itkTypeMacro(VoronoiSegmentationRGBImageFilter,ImageToImageFilter);

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);

  enum {ImageDimension = TInputImage::ImageDimension };
  

  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;
  typedef typename TInputImage::IndexType IndexType;
  typedef typename TInputImage::SizeType SizeType;
  typedef typename TInputImage::RegionType RegionType;
  typedef typename TInputImage::PixelType PixelType;
  typedef Voronoi2DDiagram<double> VoronoiDiagram;
  
  typedef typename VoronoiDiagram::PointType PointType;
  typedef typename VoronoiDiagram::Cell Cell;
  typedef typename VoronoiDiagram::CellPointer CellPointer;
  typedef typename VoronoiDiagram::Pointer VoronoiPointer;
  typedef typename Cell::PointIdIterator PointIdIterator;

  typedef typename VoronoiDiagram::SeedsType SeedsType;
  typedef typename VoronoiDiagram::SeedsIterator SeedsIterator;
  typedef typename VoronoiDiagram::NeighborIdIterator NeighborIdIterator;
  typedef typename VoronoiDiagram::EdgeIterator EdgeIterator;
  typedef typename VoronoiDiagram::FortuneEdgeInfo EdgeInfo;
  typedef std::vector<PointType> PointTypeVector;
  typedef std::deque<PointType> PointTypeDeque;
  typedef itk::Image<bool,2>  BinaryObjectImage;
  typedef typename BinaryObjectImage::Pointer  BinaryObjectImagePointer;
  typedef Vector<float,6> RGBHCVPixel;
  typedef Image<RGBHCVPixel> RGBHCVImage;

 /* for output the drawing of Voronoi Diagram */ 
  typedef itk::Image<unsigned char,2>  VDImage; 
  typedef typename VDImage::Pointer  VDImagePointer; 
    
  /**
   * Set the initial Number of Seeds for VD
   */
  itkSetMacro(NumberOfSeeds, int);
  /**
   * Get the Number of Seeds for VD
   */
  itkGetMacro(NumberOfSeeds, int);

  /**
   * Set the smallest region to be divided
   */
  itkSetMacro(MinRegion, int);

  /**
   * Get the smallest region to be divided
   */
  itkGetMacro(MinRegion, int);

  /**
   * Set the number of iterations to run (0: run until no more to divide);
   */
  itkSetMacro(Steps, int);

  /**
   * Get the number of iterations to run (0: run until no more to divide);
   */
  itkGetMacro(Steps, int);

 /**
  * Get the number of Seeds before adding new seeds;
  */
  itkGetMacro(LastStepSeeds, int);

  itkGetMacro(NumberOfSeedsToAdded, int); 

  void SetMeanPercentError(double x[6]);
  void SetVarPercentError(double x[6]);


  /*
   * maximum value of the RGB, needed for color space coversions.
   * default as 8 bit per channel, if it is different, need to be
   * set before doing anything.
   */
  itkSetMacro(MaxValueOfRGB,double);
  itkGetMacro(MaxValueOfRGB,double);

  /**
   * stuff need to be take care of before segmentation
   */
  void InitializeSegment(void);

  /**
   * take a prior from other segmentation node, should be an
   * binary object.
   */
  void TakeAPrior(BinaryObjectImage* aprior);
  
  /**
   * Perform the segmentation.
   */
  void ExcuteSegment(void);

  /**
   * Perform the segmentation.
   */
  void ExcuteSegmentOneStep(void);

  /**
   * Make the output binary result as boundary. 
   */
  void MakeSegmentBoundary(void);

  /*
   * set the three channels to test the mean and var respectivley
   * 0:red, 1:green, 2:blue, 3:hue, 4:chroma, 5:value
   */
  void SetTestMean(unsigned int t1,unsigned int t2,unsigned int t3){
    m_TestMean[0] = t1;
    m_TestMean[1] = t2;
    m_TestMean[2] = t3;
  }
  void SetTestVar(unsigned int t1,unsigned int t2,unsigned int t3){
    m_TestVar[0] = t1;
    m_TestVar[1] = t2;
    m_TestVar[2] = t3;
  }

  VoronoiPointer GetVoronoiDiagram(void){ return m_WorkingVD; }; 
    
  /** 
   * Normally not used, the seeds are set randomly. 
   * in case that need set customized seeds: 
   * use SetSeeds methods after InitializeSegment. 
   */ 
  void SetSeeds(int num, SeedsIterator begin){ 
    m_NumberOfSeeds = num; 
    m_WorkingVD->SetSeeds(num,begin); 
  }; 
    
  PointType getSeed(int SeedID){ return m_WorkingVD->getSeed(SeedID); }; 
    
    
  void DrawDiagram(VDImagePointer result,unsigned char incolor, 
  unsigned char outcolor,unsigned char boundcolor); 
    
  void BeforeNextStep(void); 
    
protected:
  VoronoiSegmentationRGBImageFilter();
  ~VoronoiSegmentationRGBImageFilter();

private:
  double m_Mean[6];
  double m_Var[6];  //actually it is the STD of the object. (sqrt(Var)).
  double m_MeanTolerance[6];
  double m_VarTolerance[6];
  double m_MeanPercentError[6];
  double m_VarPercentError[6];
  SizeType m_Size;
  int m_NumberOfSeeds;
  int m_MinRegion;
  int m_Steps;
  int m_LastStepSeeds;
  int m_NumberOfSeedsToAdded;
  int m_NumberOfBoundary;
  std::vector<int> m_NumberOfPixels;
  std::vector<unsigned char> m_Label;
  int m_StepsRuned;
  double m_MaxValueOfRGB;
  unsigned int m_TestMean[3];
  unsigned int m_TestVar[3];

  typename InputImageType::Pointer m_InputImage;
  typename OutputImageType::Pointer m_OutputImage;
  typename RGBHCVImage::Pointer m_WorkingImage;
  typename VoronoiDiagram::Pointer m_WorkingVD;

  std::vector<PointType> m_SeedsToAdded;

	// private methods:
	// classify all the voronoi cells as interior or exterior or boundary
  void ClassifyDiagram(void);

	// generate the seeds to be added. (by divide the boundary cells)
  void GenerateAddingSeeds(void);

	// compute the statistics of the pixels inside the polygon.
  void GetStats(PointTypeDeque vertlist, double *savemean, double *savevar, int *num);

	// draw a straight line to the output image.
  void drawLine(PointType p1,PointType p2);


  //used for drawing the intermedia Voronoi Diagram. 
  void drawVDline(VDImagePointer result,PointType p1,PointType p2, unsigned char color); 
};

}//end namespace


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVoronoiSegmentationRGBImageFilter.txx"
#endif

#endif




