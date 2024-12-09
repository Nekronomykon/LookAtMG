#ifndef AcquireFile_Base_h__
#define AcquireFile_Base_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <memory>

#include <vtkStringArray.h>

#include <vtkNew.h>
#include <vtkSmartPointer.h>

#include "SourceOfMolecule.h"

#include "ImplPathName.h"

struct MoleculeFactors
{
  constexpr static const double BohrFromAngstrom = 1.889726125907782;
  constexpr static const double AngstromFromBohr = 0.52917721054482;
};

class AcquireFileBase
    : public SourceOfMolecule,
      public ImplPathName<AcquireFileBase>,
      public MoleculeFactors
{
public:
  static AcquireFileBase *New();
  vtkTypeMacro(AcquireFileBase, SourceOfMolecule);
  void PrintSelf(ostream &os, vtkIndent indent) override;
  //
  vtkIdType GetNumberOfAtoms() const;
  vtkIdType resetNumberOfAtoms(vtkIdType /*id*/ = 0L);
  //
  String getTitle() const;
  String resetTitle(String /*name*/ = String());
  String &Title();

protected:
  explicit AcquireFileBase(int /* nOuts */ = 1);
  ~AcquireFileBase() override = default;
  //
  vtkIdType &NumberOfAtoms();

  // ----------------------------------------------------------------------------------------------------
  // To be overriden to read information stored in the (file) stream
  // ----------------------------------------------------------------------------------------------------
  int RequestInformation(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  virtual int ReadSizesFrom(InputFile & /*inp*/);
  // ----------------------------------------------------------------------------------------------------
  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  virtual int ReadDataFrom(InputFile & /*inp*/, Molecule * /*ptrMol*/);
  virtual int OnReadDataComplete(Molecule * /* ptrMol */);
  // ----------------------------------------------------------------------------------------------------

private:
  String title_ = String();
  vtkIdType numAtoms_ = -1;
  vtkNew<vtkStringArray> nameAtoms_;

private:
  AcquireFileBase(const AcquireFileBase &) = delete;
  void operator=(const AcquireFileBase &) = delete;
};

template <class Reader>
struct ReadStructureFile
{

  template <class Host>
  static inline bool CheckStructureAt(const Host &dest)
  {
    return bool(dest.getMolecule()->GetNumberOfAtoms() > 0);
  }
  // ==========================================================================================
  template <class Host>
  static bool FromPathTo(Path path, Host &dest)
  {
    // vtkSmartPointer<Reader> read(Reader::New());
    vtkNew<Reader> read;
    // if (!read)
    //  return false;
    read->resetPath(path);
    if (!read->hasPath() || !read->isRegular())
      return false;
    Molecule *pMol = dest.getMolecule();
    read->SetOutput(pMol);
    dest.clearAll();
    read->Update();
    // pMol->DeepCopy(read->GetOutput());
    bool bRes(pMol->GetNumberOfAtoms() > 0);
    // if (bRes)
    //   dest.setModified(false); // success in de-novo read...
    return CheckStructureAt<Host>(dest);
  }
};

#endif // !AcquireFile_Base_h__
