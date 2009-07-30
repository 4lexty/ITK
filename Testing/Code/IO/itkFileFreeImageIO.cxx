/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFileFreeImageIO.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkFileFreeImageIO.h"
#include "itksys/RegularExpression.hxx"
#include <string.h>
#include <cstdlib>

namespace itk {

FileFreeImageIO
::FileFreeImageIO()
{
}

FileFreeImageIO
::~FileFreeImageIO()
{
}

bool
FileFreeImageIO
::CanReadFile(const char* filename)
{
  std::string fileDescription(filename);
  itksys::RegularExpression findPrefix("^FileFreeIO::");
  if (findPrefix.find(filename))
    {
    return true;
    }
  else
    {
    return false;
    }
}

void
FileFreeImageIO
::ReadImageInformation()
{

  // type
  // dimension
  // size
  // spacing
  // origin
  // direction
  std::vector<unsigned int> sizeVector;
  std::vector<double> spacingVector;
  std::vector<double> originVector;
  std::vector<std::vector<double> > directionVector;
  std::string fileDescription(this->GetFileName());

  itksys::RegularExpression findSize("Size=([^:$]*)");

  if (findSize.find(this->GetFileName()))
    {
    std::vector<std::string> tempVector;
    std::cout << "Size is: " << findSize.match(1) << std::endl; 
    this->SplitString(findSize.match(1), ",", tempVector);
    for (std::vector<std::string>::size_type i = 0; i < tempVector.size(); i++)
      {
      sizeVector.push_back(atoi(tempVector[i].c_str()));
      }
    }

  itksys::RegularExpression findSpacing("Spacing=([^:$]*)");
  if (findSpacing.find(this->GetFileName()))
    {
    std::vector<std::string> tempVector;
    std::cout << "Spacing is: " << findSpacing.match(1) << std::endl; 
    this->SplitString(findSpacing.match(1), ",", tempVector);
    for (std::vector<std::string>::size_type i = 0; i < tempVector.size(); i++)
      {
      spacingVector.push_back(atof(tempVector[i].c_str()));
      }
    }

  itksys::RegularExpression findOrigin("Origin=([^:$]*)");
  if (findOrigin.find(this->GetFileName()))
    {
    std::vector<std::string> tempVector;
    std::cout << "Origin is: " << findOrigin.match(1) << std::endl; 
    this->SplitString(findOrigin.match(1), ",", tempVector);
    for (std::vector<std::string>::size_type i = 0; i < tempVector.size(); i++)
      {
      originVector.push_back(atof(tempVector[i].c_str()));
      }
    }

  itksys::RegularExpression findDirection("Direction=([^:$]*)");
  if (findDirection.find(this->GetFileName()))
    {
    std::vector<std::string> tempVector;
    std::cout << "Direction is: " << findDirection.match(1) << std::endl; 
    this->SplitString(findDirection.match(1), ",", tempVector);
    std::vector<std::string>::size_type ii = 0;
    directionVector.resize(sizeVector.size());
    for (std::vector<std::string>::size_type i = 0; i < tempVector.size() / 2; i++)
      {
      for (std::vector<std::string>::size_type j = 0; j < tempVector.size() / 2; j++)
        {
        
        directionVector[i].push_back(atof(tempVector[ii].c_str()));
        ii++;
        }
      }
    }
  if (originVector.size() == 0)
    {
    for (std::vector<std::string>::size_type i = 0; i < sizeVector.size(); i++)
      {
      originVector.push_back(0.0);
      }
    }

  if (spacingVector.size() == 0)
    {
    for (std::vector<std::string>::size_type i = 0; i < sizeVector.size(); i++)
      {
      spacingVector.push_back(1.0);
      }
    }

  if (directionVector.size() == 0)
    {
    directionVector.resize(sizeVector.size());
    for (std::vector<std::string>::size_type i = 0; i < sizeVector.size(); i++)
      {
      for (std::vector<std::string>::size_type j = 0; j < sizeVector.size(); j++)
        {
        if (i == j)
          {
          directionVector[i].push_back(1.0);
          }
        else
          {
          directionVector[i].push_back(0.0);
          }
        }
      }
    }

  this->SetNumberOfDimensions(sizeVector.size());
  this->SetNumberOfComponents(1);
  this->SetPixelType(SCALAR);
  this->SetComponentType(UCHAR);
  for (std::vector<std::string>::size_type i = 0; i < sizeVector.size(); i++)
    {
    this->SetDimensions(i, sizeVector[i]);
    this->SetSpacing(i, spacingVector[i]);
    this->SetOrigin(i, originVector[i]);
    this->SetDirection(i, directionVector[i]);
    }

  std::cout << "Size in bytes: " << this->GetImageSizeInPixels() << std::endl;
  std::cout << "ReadInformation complete!" << std::endl;
}

// Read from the generated image
void
FileFreeImageIO
::Read(void *buffer)
{
  memset(buffer, (unsigned char) 175, this->GetImageSizeInBytes());
}

bool
FileFreeImageIO
::CanWriteFile(const char* )
{
  return false;
}

void
FileFreeImageIO
::WriteImageInformation()
{
}

void
FileFreeImageIO
::Write(const void *)
{
}

void
FileFreeImageIO
::SplitString (const std::string &text,
               const std::string &separators,
               std::vector<std::string> &words)
{
  const std::string::size_type n = text.length();
  std::string::size_type start = text.find_first_not_of(separators);
  while (start < n)
    {
    std::string::size_type stop = text.find_first_of(separators, start);
    if (stop > n) stop = n;
    words.push_back(text.substr(start, stop - start));
    start = text.find_first_not_of(separators, stop+1);
    }
}

void
FileFreeImageIO
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace itk
