#ifndef View_StructureFile_h__
#define View_StructureFile_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "ViewElements.h"
#include "EditMarkLines.h"
#include "ViewMolecule.h"

#include "FileFormat.h"

#include <QPointer>

#include "ImplPathName.h"
#include <QTabWidget>

#include "Molecule.h"

#include <vtkSmartPointer.h>
#include <vtkNew.h>

class FrameFileData;

typedef TagFileFormat<FrameFileData> FileFormat;

class FrameFileData
    : public QTabWidget,
      public ImplPathName<FrameFileData>
{
    Q_OBJECT
public:
    explicit FrameFileData(QWidget * /*parent*/ = nullptr);
    ~FrameFileData() override = default;
    // Filters for any case
    static QString getFilterStringOpen(void);
    static QString getFilterStringSave(void);
    // Modes
    void setModeTextView();
    void setModeTextEdit();
    void setModeMolecular();
    // views
    int getIdTabElements() const { return idViewElements_; }
    ViewElements *getViewElements() const { return viewElements_; }
    ViewElements *viewElements()
    {
        if (idViewElements_ < 0 || !viewElements_)
            return nullptr;
        this->setCurrentIndex(idViewElements_);
        return viewElements_;
    }
    int getIdTabSource() const { return idEditRawSource_; }
    EditMarkLines *getEditSource() const { return editRawSource_; }
    EditMarkLines *editSource()
    {
        if (idEditRawSource_ < 0 || !editRawSource_)
            return nullptr;
        this->setCurrentIndex(idEditRawSource_);
        return editRawSource_;
    }
    int getIdTabMolecule() const { return idViewMolecule_; }
    ViewMolecule *getViewMolecule() const { return viewMolecule_; }
    ViewMolecule *viewMolecule()
    {
        if (idViewMolecule_ < 0 || !viewMolecule_)
            return nullptr;
        this->setCurrentIndex(idViewMolecule_);
        return viewMolecule_;
    }
    //
    void clearContent(void);
    void clearAll(void);
    //
    bool isModified() const;
    bool setModified(bool /*bSet*/ = true);
    //
    bool hasTitle() const;
    String getTitle() const;
    void resetTitle(String /*new_title*/ = String());
    void pathToTitle(void);
    //
    bool loadSourceFileFrom(const QString & /*sPath*/);
    bool loadSourceFile(void);
    //
    bool loadFile(const QString & /*sPath*/);
    //
    const FileFormat &getFormatInput() const
    {
        return formatInp_;
    }
    bool resetInputFormat(FileFormat /*f*/ = FileFormat());
    const FileFormat &getFormatOutput() const
    {
        return formatOut_;
    }
    bool resetOutputFormat(FileFormat /*f*/ = FileFormat());
    //
    Molecule *getMolecule() const { return mol_; }
    //
    static FileFormat FindInputFormat(String ext);
    static FileFormat FindOutputFormat(String ext);
    //

protected:
    enum ViewFileMode
    {
        ModeUnusable = -1,
        ModeText = 0,
        ModeMolecule = 1
    };
    static const FileFormat formatsIO[];

    // format file writer
    bool saveImageFileBMP(Path path);
    bool saveImageFilePNG(Path path);
    bool saveImageFilePS(Path path);
    bool saveImageFilePDF(Path path);
    bool saveImageFileJPEG(Path path);
    bool saveImageFileTIFF(Path path);

private:
    bool isModified_ = false;
    Path path_ = Path();
    ViewFileMode mode_ = ModeUnusable;
    //
    ANewMolecule mol_;
    //
    FileFormat formatInp_ = FileFormat();
    FileFormat formatOut_ = FileFormat();
    //
    QPointer<ViewElements> viewElements_;
    int idViewElements_ = -1;
    QPointer<EditMarkLines> editRawSource_;
    int idEditRawSource_ = -1;
    QPointer<ViewMolecule> viewMolecule_;
    int idViewMolecule_ = -1;
};

#endif // View_StructureFile_h__
