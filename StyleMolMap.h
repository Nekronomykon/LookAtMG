#ifndef Style_MolMapping_h__
#define Style_MolMapping_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <vtkType.h>
#include <vtkColor.h>

class StyleMolMapping // ???
{
public:
  virtual ~StyleMolMapping() {}

  // whatever
  bool QueryAtomTypeColor(vtkIdType /*idType*/, vtkColor3d * /*pCol*/ = nullptr);
  // true  --> Ok
  // false --> if color for idType is not found,
  //           and *pCol (if valid) is set up using the fallback
};

bool StyleMolMapping::QueryAtomTypeColor(vtkIdType idType, vtkColor3d *pCol)
{
  return false; // STUB
}

#endif // !Style_MolMapping_h__
