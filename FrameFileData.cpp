#include "FrameFileData.h"

#include <QFile>
#include <QFileInfo>
#include <QByteArray>
#include <QMessageBox>

#include <vtkCMLMoleculeReader.h>
#include <vtkGaussianCubeReader.h>
#include <vtkGaussianCubeReader2.h>
#include <vtkXYZMolReader.h>
#include <vtkXYZMolReader2.h>
#include <vtkPDBReader.h>

#include "AcquireFileCML.h"
#include "AcquireFileCUBE.h"
#include "AcquireFileEXTOUT.h"
#include "AcquireFileHIN.h"
#include "AcquireFileMGP.h"
#include "AcquireFileMOL2.h"
#include "AcquireFilePDB.h"
#include "AcquireFileSUM.h"
#include "AcquireFileWFN.h"
#include "AcquireFileWFX.h"
#include "AcquireFileXYZ.h"

#include <vtkBMPWriter.h>
#include <vtkTIFFWriter.h>
#include <vtkPNGWriter.h>
#include <vtkJPEGWriter.h>
#include <vtkPostScriptWriter.h>

#include "ResetCursor.h"

const FileFormat FrameFileData::formatsIO[] = {
    /////////////////////////////// Simple single structure file //////////////////////////////////
    FileFormat("XMol XYZ", ".xyz", &ReadStructureFile<vtkXYZMolReader2>::FromPathTo),
    FileFormat("Chemical Markup Language", ".cml", &ReadStructureFile<vtkCMLMoleculeReader>::FromPathTo),
    FileFormat("Brookhaven PDB", ".pdb", &ReadStructureFile<vtkPDBReader>::FromPathTo),
    FileFormat("Alchemy 2000 molecule", ".mol2", &ReadStructureFile<AcquireFileMOL2>::FromPathTo),
    FileFormat("HyperChem input", ".hin", &ReadStructureFile<AcquireFileHIN>::FromPathTo),
    //////////////////////////////// QTAIM data file //////////////////////////////////////////////
    FileFormat("Gaussian CUBE", ".cube", &ReadStructureFile<AcquireFileCUBE>::FromPathTo),
    FileFormat("Wavefunction", ".wfn", &ReadStructureFile<AcquireFileWFN>::FromPathTo),
    FileFormat("Wavefunction extended", ".wfx", &ReadStructureFile<AcquireFileWFX>::FromPathTo),
    ////////////////////////// AIMAll results file (NYI) //////////////////////////////////////////
    FileFormat("AIMAll summary", ".sum", &ReadStructureFile<AcquireFileSUM>::FromPathTo),
    FileFormat("AIMAll molecular graph", ".mgp", &ReadStructureFile<AcquireFileMGP>::FromPathTo),
    FileFormat("AIMAll extended output", ".extout", &ReadStructureFile<AcquireFileEXTOUT>::FromPathTo),
    /////////////////////// Image files for VTK graph to save /////////////////////////////////////
    FileFormat("Bitmap image", ".bmp", nullptr, &FrameFileData::saveImageFileBMP),
    FileFormat("JPEG image", ".jpeg", nullptr, &FrameFileData::saveImageFileJPEG),
    FileFormat("Portable Network Graphics image", ".png", nullptr, &FrameFileData::saveImageFilePNG),
    FileFormat("Post Script", ".ps", nullptr, &FrameFileData::saveImageFilePS),
    FileFormat("Portable Documet Format", ".pdf", nullptr, &FrameFileData::saveImageFilePDF),
    FileFormat("Tagged Image Format", ".tiff", nullptr, &FrameFileData::saveImageFileTIFF),
    FileFormat()};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
QString FrameFileData::getFilterStringOpen()
{
    const QString sAll("All files (*.*)");
    QString sKnown("All appropriate formats (");
    QString sExt;
    QString sFmt;
    for (const auto &fmt : formatsIO)
    {
        if (!fmt.isToLoad())
            continue;
        //
        QString sMask(fmt.getMask().c_str());
        sKnown += sMask;
        sKnown += ' ';
        QString sOne(";;");
        sOne += fmt.getName().c_str();
        sOne += " files (";
        sOne += sMask;
        sOne += ')';
        sFmt += sOne;
    }
    QString sRes;
    *sKnown.rbegin() = ')';
    sRes = sKnown;
    sRes += sFmt;
    sRes += ";;";
    sRes += sAll;
    return sRes;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
QString FrameFileData::getFilterStringSave()
{
    const QString sAll("All files (*.*)");
    QString sKnown("All appropriate formats (");
    QString sExt;
    QString sFmt;
    for (const auto &fmt : formatsIO)
    {
        if (!fmt.isToSave())
            continue;
        //
        QString sMask(fmt.getMask().c_str());
        sKnown += sMask;
        sKnown += ' ';
        QString sOne(";;");
        sOne += fmt.getName().c_str();
        sOne += " files (";
        sOne += sMask;
        sOne += ')';
        sFmt += sOne;
    }
    // TO THINK ON!!!
    QString sRes;
    *sKnown.rbegin() = ')';
    sRes = sKnown;
    sRes += sFmt;
    sRes += ";;";
    sRes += sAll;
    return sRes;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
FileFormat FrameFileData::FindInputFormat(String ext)
{
    FileFormat fres;
    for (const auto &fmt : formatsIO)
    {
        if (!fmt.isToLoad())
            continue;
        if (fmt.isMaskEqual(ext))
        {
            fres = fmt;
            break;
        }
    }
    return fres;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
FileFormat FrameFileData::FindOutputFormat(String ext)
{
    FileFormat fres;
    for (const auto &fmt : formatsIO)
    {
        if (!fmt.isToSave())
            continue;
        if (fmt.isMaskEqual(ext))
        {
            fres = fmt;
            break;
        }
    }
    return fres;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
FrameFileData::FrameFileData(QWidget *parent) : QTabWidget(parent),
                                                viewElements_(new TableElements(this)),
                                                editRawSource_(new EditMarkLines(this)),
                                                viewMolecule_(new ViewMolecule(this))
{
    // this->setupFileView();
    this->setTabPosition(QTabWidget::South);
    this->setDocumentMode(false);

    // Setup minimal view:
    viewElements_->setObjectName("Elements");
    idViewElements_ = this->addTab(viewElements_, tr("Elements"));
    //
    editRawSource_->setObjectName("Source");
    idEditRawSource_ = this->addTab(editRawSource_, tr("Source"));
    //
    viewMolecule_->setObjectName("Molecule");
    viewMolecule_->getMapMolecule()->SetInputData(this->getMolecule());
    idViewMolecule_ = this->addTab(viewMolecule_, tr("Molecule"));

    //
    // this->setModeTextEdit();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
bool FrameFileData::resetInputFormat(FileFormat f)
{
    bool bRes = f.isToLoad();
    formatInp_ = bRes ? f : FileFormat();
    return bRes;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
bool FrameFileData::resetOutputFormat(FileFormat f)
{
    bool bRes = f.isToSave();
    formatInp_ = bRes ? f : FileFormat();
    return bRes;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameFileData::setModeTextView()
{
    editRawSource_->setReadOnly(true);
    /*
    this->setCurrentWidget(editRawSource_);
    if (idViewMolecule_ != -1)
    {
        this->removeTab(idViewMolecule_);
        idViewMolecule_ = -1;
    }
    */
    mode_ = ModeText;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameFileData::setModeTextEdit()
{
    editRawSource_->setReadOnly(false);
    /*
    this->setCurrentWidget(editRawSource_);
    if (idViewMolecule_ != -1)
    {
        this->removeTab(idViewMolecule_);
        idViewMolecule_ = -1;
    }
    */
    mode_ = ModeText;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameFileData::setModeMolecular()
{
    /*
    if (!viewMolecule_)
    {
        viewMolecule_ = new ViewMolecule(this);
        viewMolecule_->setObjectName("Structure");
    }
    if (idViewMolecule_ == -1)
    {
        idViewMolecule_ = this->addTab(viewMolecule_, tr("Structure"));
    }
    this->viewMolecule();
    */
    mode_ = ModeMolecule;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
bool FrameFileData::loadSourceFile(void)
{
    QString sPath(this->getPath().c_str());
    return this->loadSourceFileFrom(sPath);
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
bool FrameFileData::loadSourceFileFrom(const QString &sPath)
{
    bool bOk = editRawSource_->loadPath(sPath);
    if (bOk)
    {
        this->setCurrentWidget(editRawSource_);
    }
    else
    {
        int nRes = QMessageBox::question(this, tr("Loading a file:"),
                                         tr("Cannot read text file:\n%1\nCreate an empty new one with this name?").arg(sPath)
                                         // , Qt::StandartButtons
        );
        if (nRes)
            return false;
    }
    editRawSource_->setReadOnly(bOk);
    return bOk;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
bool FrameFileData::loadFile(const QString &sPath)
{
    ResetCursor cursor;
    QFileInfo fi(sPath);

    Path path = fi.filesystemAbsoluteFilePath();
    if (!fs::exists(path) || !fs::is_regular_file(path))
        return false;
    FileFormat fInp = FrameFileData::FindInputFormat(path.extension().string());
    if (!fInp.isToLoad())
        return false;
    this->setModeMolecular();
    bool bHasStr = fInp.implementRead(*this, path);
    if (bHasStr)
    {
        this->resetInputFormat(fInp);
        RenderTheWindow rw(this->getViewMolecule()->getRenderWindow());
    }
    this->loadSourceFile();
    return bHasStr;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameFileData::clearContent()
{
    this->resetTitle();
    mol_->Initialize();
    //
    editRawSource_->clear();
    this->setModeTextEdit();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameFileData::clearAll()
{
    this->clearContent();
    this->resetPath(); // clearPath()
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
bool FrameFileData::isModified() const
{
    return isModified_; // || editRawSource_->isWindowModified()
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
bool FrameFileData::setModified(bool bSet)
{
    std::swap(bSet, isModified_);
    return bSet;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
bool FrameFileData::hasTitle()const
{
    return !bool(viewMolecule_->getEditTitle()->text().isEmpty());
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
String FrameFileData::getTitle() const
{
    auto bytes = viewMolecule_->getEditTitle()->text().toLatin1();
    String title(bytes.data());
    return title;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameFileData::resetTitle(String title)
{
    auto bytes = title.c_str();
    viewMolecule_->getEditTitle()->setText(QString(bytes));
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameFileData::pathToTitle(void)
{
    String sTitle("From:");
    sTitle += this->getPath().string();
    this->resetTitle(sTitle);
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Saving graphs
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
bool FrameFileData::saveImageFileBMP(Path path)
{
    bool bRes(mol_->GetNumberOfAtoms() > 0);
    if (bRes)
    {
        vtkNew<vtkBMPWriter> write;
        write->SetFileName(path.c_str());
        // going further:
        this->getViewMolecule()->exportImageTo(write);
    }
    return bRes;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
bool FrameFileData::saveImageFilePNG(Path path)
{
    bool bRes(mol_->GetNumberOfAtoms() > 0);
    if (bRes)
    {
        vtkNew<vtkPNGWriter> write;
        write->SetFileName(path.c_str());
        // going further:
        this->getViewMolecule()->exportImageTo(write);
        write->Write();
    }
    return bRes;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
bool FrameFileData::saveImageFilePDF(Path path)
{
    bool bRes(mol_->GetNumberOfAtoms() > 0);
    if (bRes)
    {
        QMessageBox::information(this,
                                 QString("VTK --> PDF Export"),
                                 QString("Not yet supported, but I'm still trying to fill the gap.\nNow you try the PostScript export\nand ps2pdf further"));
        // going further:
    }
    return bRes;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
bool FrameFileData::saveImageFilePS(Path path)
{
    bool bRes(mol_->GetNumberOfAtoms() > 0);
    if (bRes)
    {
        vtkNew<vtkPostScriptWriter> write;
        write->SetFileName(path.c_str());
        // going further:
        this->getViewMolecule()->exportImageTo(write);
        write->Write();
    }
    return bRes;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
bool FrameFileData::saveImageFileJPEG(Path path)
{
    bool bRes(mol_->GetNumberOfAtoms() > 0);
    if (bRes)
    {
        vtkNew<vtkJPEGWriter> write;
        write->SetFileName(path.c_str());
        // going further:
        this->getViewMolecule()->exportImageTo(write, false);
        write->Write();
    }
    return bRes;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
bool FrameFileData::saveImageFileTIFF(Path path)
{
    bool bRes(mol_->GetNumberOfAtoms() > 0);
    if (bRes)
    {
        vtkNew<vtkTIFFWriter> write;
        write->SetFileName(path.c_str());
        // going further:
        this->getViewMolecule()->exportImageTo(write);
        write->Write();
    }
    return bRes;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
