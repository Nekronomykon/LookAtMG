#ifndef AcquireFile_MOL2_h__
#define AcquireFile_MOL2_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "FileFormat.h"
#include "AcquireFileBase.h"

#include <vtkNew.h>
#include <vtkSmartPointer.h>

class AcquireFileMOL2
    : public AcquireFileBase
{
public:
  static AcquireFileMOL2 *New();
  vtkTypeMacro(AcquireFileMOL2, AcquireFileBase);
  void PrintSelf(ostream &os, vtkIndent indent) override;

  // ----------------------------------------------------------------------------------------------------
  vtkIdType& NumberOfBonds();
  vtkIdType& NumberOfFragments();
  vtkIdType GetNumberOfBonds()const;
  vtkIdType GetNumberOfFragments()const;

protected:
  explicit AcquireFileMOL2();
  ~AcquireFileMOL2() override = default;

  // ----------------------------------------------------------------------------------------------------
  // To be overriden to read information stored in the (file) stream
  // ----------------------------------------------------------------------------------------------------
  // int RequestInformation(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  int ReadSizesFrom(InputFile & /*inp*/) override;
  // ----------------------------------------------------------------------------------------------------
  // int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  int ReadDataFrom(InputFile & /*inp*/, Molecule * /* pMol */) override;
  // int OnReadDataComplete(Molecule * /* ptrMol */) override;
  // ----------------------------------------------------------------------------------------------------

private:
  AcquireFileMOL2(const AcquireFileMOL2 &) = delete;
  void operator=(const AcquireFileMOL2 &) = delete;

  private:
  vtkIdType numberOfBonds_ = 0;
  vtkIdType numberOfFragments_ = -1;
};

using ANewMOL2Parser = vtkNew<AcquireFileMOL2>;
using AFileParserMOL2 = vtkSmartPointer<AcquireFileMOL2>;

template <class Host>
bool ParseMOL2FileTo(Path a_path, Host &host)
{
  ANewMOL2Parser read;
  read->resetPath(a_path);
  read->SetOutput(host.getMolecule());
  read->Update();
  host.resetTitle(read->getTitle());
  return bool(host.getMolecule()->GetNumberOfAtoms() > 0);
}

#endif // !AcquireFile_MOL2_h__
