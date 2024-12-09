#include "FrameBrowser.h"
#include "./ui_FrameBrowser.h"

#include <QToolBar>
#include <QSettings>
#include <QDockWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QColorDialog>

#include <vtkVersion.h>

#include "DialogFileProperties.h"

namespace // anonymous for setting keys
{
    static inline QString nameNewFile(void)
    {
        return QStringLiteral("File");
    }
    //
    ///////////////////////////////////////////////////////////////////////
    //
    static inline QString keyRecentFiles(void)
    {
        return QStringLiteral("RecentFiles");
    }
    //
    static inline QString keyRecentFile(void)
    {
        return QStringLiteral("RecentFile");
    }
    //
    static inline QString keyBackgroundColor(void)
    {
        return QStringLiteral("BackgroundColor");
    }
    //
    static inline QString keyGeometry(void)
    {
        return QStringLiteral("Geometry");
    }
    //
    static inline QString keyState(void)
    {
        return QStringLiteral("State");
    }
    //
    static inline QString keyFile(void)
    {
        return QStringLiteral("File");
    }
    //
    ///////////////////////////////////////////////////////////////////////
}; // namespace // anonymous for setting different string keys
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FrameBrowser::FrameBrowser(QWidget *parent)
    : QMainWindow(parent),
      ptrUI_(std::make_unique<Ui::FrameBrowser>()),
      files_(new ViewFileSystem(this)),
      browse_(new ViewBrowse(this)),
      named_colors_(new NamedColors(this)),
      edit_red_(new QLineEdit(this)),
      edit_green_(new QLineEdit(this)),
      edit_blue_(new QLineEdit(this))
{
    ptrUI_->setupUi(this);

    // this-readSettings();
    setupMenuActions(ptrUI_.get());
    this->setupToolBars();
    this->setupDockingViews();
    //
    this->loadSettings();
}

FrameBrowser::~FrameBrowser()
{
    // delete ui;
}

FrameFileData *FrameBrowser::getView() const { return ptrUI_.get()->view_; }

FrameBrowser *FrameBrowser::createNewFrame(QWidget *parent)
{
    FrameBrowser *pRes = new FrameBrowser;
    if (parent)
    {
    }
    else
    {
    }
    // pRes->show();
    return pRes;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void FrameBrowser::provideFilePath(const QString &sPath)
{
    this->addPath(sPath);
    this->getView()->loadFile(sPath);
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void FrameBrowser::addPath(const QString &sPath)
{
    /*
    Path pathFull = std::make_full_path(sPath.toLocal8Bit().data());
    // Workspace should contain unique and existent path names
    // probably with content operations
    auto *pWk = this->getWorkspace();
    assert(pWk);
    auto hItem = pWk->findPath(pathFull);
    if (!pWk->isValidItem(hItem))
        hItem = pWk->addItem(pathFull);
        //
    auto* pHist = this->getHistory();;
    // History is to contain the sequence of file loadings with their results...
    pHist->addItem(pathFull);
    */
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void FrameBrowser::loadSettings()
{
    QSettings settings; //(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    this->restoreGeometry(settings.value(keyGeometry(), QByteArray()).toByteArray());
    this->restoreState(settings.value(keyState(), QByteArray()).toByteArray());
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void FrameBrowser::saveSettings()
{
    QSettings settings; //(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue(keyGeometry(), this->saveGeometry());
    settings.setValue(keyState(), this->saveState());
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void FrameBrowser::setupDockingViews()
{
    QDockWidget *pF(new QDockWidget(tr("Files"), this));
    pF->setObjectName("dockFiles");
    pF->setWidget(files_);
    this->addDockWidget(Qt::LeftDockWidgetArea, pF);
    QDockWidget *pW(new QDockWidget(tr("Workspace"), this));
    pW->setObjectName("dockWorkspace");
    pW->setWidget(browse_);
    this->tabifyDockWidget(pF, pW);
    // a suite...
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void FrameBrowser::setupMenuActions(Ui::FrameBrowser *p)
{
    // :: New ::
    const QIcon iconNew(QIcon::fromTheme("document-new", QIcon(":/images/NewDocument.png")));
    p->actionNewFile_->setIcon(iconNew);
    p->actionNewFile_->setShortcuts(QKeySequence::New);
    p->actionNewFile_->setStatusTip(tr("Recreate an empty data file"));

    const QIcon iconNewWindow(QIcon::fromTheme("window-new", QIcon(":/images/NewWindow.png")));
    p->actionNewBrowser_->setIcon(iconNewWindow);
    p->actionNewBrowser_->setShortcut(QKeySequence(Qt::SHIFT | Qt::CTRL | Qt::Key_N));
    p->actionNewBrowser_->setStatusTip(tr("Start a new top-level application window"));

    // :: Close ::

    const QIcon iconClose(QIcon::fromTheme("window-close", QIcon(":/images/CloseFrame.png")));
    p->actionClose_->setIcon(iconClose);
    p->actionClose_->setShortcut(QKeySequence::Close);
    p->actionClose_->setStatusTip(tr("Close this top-level application window"));

    const QIcon iconExit(QIcon::fromTheme("application-exit", QIcon(":/images/Exit.png")));
    p->actionExit_->setIcon(iconExit);
    p->actionExit_->setShortcuts(QKeySequence::Quit);
    p->actionExit_->setStatusTip(tr("Close all windows and exit the application"));

    // :: File ::

    const QIcon iconOpen(QIcon::fromTheme("document-open", QIcon(":/images/Open.png")));
    p->actionOpen_->setIcon(iconOpen);
    p->actionOpen_->setShortcuts(QKeySequence::Open);
    p->actionOpen_->setStatusTip(tr("Open a file from disk"));

    const QIcon iconReload(QIcon::fromTheme("document-revert", QIcon(":/images/Reload.png")));
    p->actionReload_->setIcon(iconReload);
    p->actionReload_->setShortcuts(QKeySequence::Refresh);
    p->actionReload_->setStatusTip(tr("Reload content from the currently loaded file"));

    const QIcon iconDetach(":/images/Detach.png");
    // QIcon::fromTheme("document-detach", QIcon(":/images/Detach.png")); // clear path and set modified
    p->actionDetach_->setIcon(iconDetach);
    p->actionDetach_->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_D));
    p->actionDetach_->setStatusTip(tr("Make the current content not associated with the current path"));

    const QIcon iconSaveAs(QIcon::fromTheme("document-save-as", QIcon(":/images/SaveAs.png")));
    p->actionSaveAs_->setIcon(iconSaveAs);
    p->actionSaveAs_->setShortcuts(QKeySequence::SaveAs);
    p->actionSaveAs_->setStatusTip(tr("Save the document under a newly set path"));

    const QIcon iconSave(QIcon::fromTheme("document-save", QIcon(":/images/Save.png")));
    p->actionSave_->setIcon(iconSave);
    p->actionSave_->setShortcuts(QKeySequence::Save);
    p->actionSave_->setStatusTip(tr("Save the document to disk"));

    const QIcon iconExport(QIcon::fromTheme("document-export", QIcon(":/images/Export.png")));
    p->actionSaveCopy_->setIcon(iconExport);
    p->actionSaveCopy_->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_S));
    p->actionSaveCopy_->setStatusTip(tr("Export a file to disk based on the current data"));

    // :: View ::

    const QIcon iconFullScreen(QIcon::fromTheme("view-fullscreen", QIcon(":/images/FullScreen.png")));
    p->actionFullScreen_->setIcon(iconFullScreen);
    p->actionFullScreen_->setShortcuts(QKeySequence::FullScreen);
    p->actionFullScreen_->setStatusTip(tr("Toggle the full-screen view of the window"));

    const QIcon iconBgColor(":/images/BgColor.png");
    p->actionBackgroundColor_->setIcon(iconBgColor);
    p->actionBackgroundColor_->setStatusTip(tr("Select a background color for the graphic scene view"));

    // :: Help ::

    const QIcon iconAbout(QIcon::fromTheme("help-about", QIcon(":/images/About.png")));
    p->actionAboutThis_->setIcon(iconAbout);
    p->actionAboutThis_->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F1));
    p->actionAboutThis_->setStatusTip(tr("Show the concise information about the application"));

    const QIcon iconAboutQt(QIcon::fromTheme("system-help", QIcon(":/images/AboutKit.png")));
    p->actionAboutQt_->setIcon(iconAboutQt);
    p->actionAboutQt_->setShortcut(QKeySequence(Qt::SHIFT | Qt::CTRL | Qt::Key_F1));
    p->actionAboutQt_->setStatusTip(tr("Show the concise Qt information on the kit used for the application"));

    // :: Edit ::
    const QIcon iconUndo(QIcon::fromTheme("edit-undo", QIcon(":/images/Undo.png")));
    p->actionUndo_->setIcon(iconUndo);
    p->actionUndo_->setShortcut(QKeySequence::Undo);

    const QIcon iconRedo(QIcon::fromTheme("edit-redo", QIcon(":/images/Redo.png")));
    p->actionRedo_->setIcon(iconRedo);
    p->actionRedo_->setShortcut(QKeySequence::Redo);

    const QIcon iconCut(QIcon::fromTheme("edit-cut", QIcon(":/images/Cut.png")));
    p->actionCut_->setIcon(iconCut);
    p->actionCut_->setShortcuts(QKeySequence::Cut);

    const QIcon iconCopy(QIcon::fromTheme("edit-copy", QIcon(":/images/Copy.png")));
    p->actionCopy_->setIcon(iconCopy);
    p->actionCopy_->setShortcuts(QKeySequence::Copy);

    const QIcon iconPaste(QIcon::fromTheme("edit-paste", QIcon(":/images/Paste.png")));
    p->actionPaste_->setIcon(iconPaste);
    p->actionPaste_->setShortcuts(QKeySequence::Paste);

    const QIcon iconClear(QIcon::fromTheme("edit-clear", QIcon(":/images/Clear.png")));
    p->actionClear_->setIcon(iconClear);
    p->actionClear_->setShortcut(QKeySequence::Delete);

    const QIcon iconClearAll( // QIcon::fromTheme("edit-clear-all",
        QIcon(":/images/ClearAll.png"));
    p->actionClearAll_->setIcon(iconClearAll);
    p->actionClearAll_->setShortcut(QKeySequence(Qt::SHIFT | Qt::CTRL | Qt::Key_Delete));

    const QIcon iconClearOthers( // QIcon::fromTheme("edit-clear-others",
        QIcon(":/images/ClearOthers.png"));
    p->actionClearOthers_->setIcon(iconClearOthers);
    p->actionClearOthers_->setShortcut(QKeySequence(Qt::SHIFT | Qt::ALT | Qt::Key_Delete));

    const QIcon iconProjectReset(":/images/ProjectReset.png");
    p->actionProjectReset_->setIcon(iconProjectReset);

    const QIcon iconProjectOrtho(":/images/ProjectOrthogonal.png");
    p->actionProjectOrthogonal_->setIcon(iconProjectOrtho);

    const QIcon iconProjectPersp(":/images/ProjectPerspective.png");
    p->actionProjectPerspective_->setIcon(iconProjectPersp);

    const QIcon iconLabelAtoms(":/images/LabelAtom.png");
    p->actionLabelAtoms_->setIcon(iconLabelAtoms);

    const QIcon iconViewBonds(":/images/ViewBond.png");
    p->actionViewBonds_->setIcon(iconViewBonds);

    const QIcon iconLabelBonds(":/images/LabelBond.png");
    p->actionLabelBonds_->setIcon(iconLabelBonds);

    const QIcon iconStyleFast(":/images/StyleFast.png");
    p->actionStyleMolFast_->setIcon(iconStyleFast);

    const QIcon iconStyleBall(":/images/StyleBall.png");
    p->actionStyleMolBalls_->setIcon(iconStyleBall);

    const QIcon iconStyleSpace(":/images/StyleSpace.png");
    p->actionStyleMolSpace_->setIcon(iconStyleSpace);

    const QIcon iconStyleStick(":/images/StyleStick.png");
    p->actionStyleMolSticks_->setIcon(iconStyleStick);

    /*
    const QIcon iconImport(QIcon::fromTheme("document-import", QIcon(":/images/Import.png")));
    actionImport_->setIcon(iconImport);
    // actionSaveAs_->setShortcuts(QKeySequence::SaveAs);
    actionSaveAs_->setStatusTip(tr("Load data from a disk file"));

    // is it required, or [Export] could be good enough?
    const QIcon iconSaveCopyAs(QIcon::fromTheme("document-save-copy-as", QIcon(":/images/SaveCopyAs.png")));
    actionSaveCopyAs_->setIcon(iconSaveAs);
    // actionSaveCopyAs_->setShortcuts(QKeySequence::SaveAs);
    actionSaveAs_->setStatusTip(tr("Save the document copy under a new path"));

    const QIcon iconProps(QIcon::fromTheme("document-properties", QIcon(":/images/Properties.png")));
    actionProperties_->setIcon(iconProps);

    const QIcon iconOptions(QIcon::fromTheme("preferences-system", QIcon(":/images/Options.png")));
    actionOptions_->setIcon(iconOptions);


    const QIcon iconViewStr(":/images/ViewStr.png");
    actionViewStructure_->setIcon(iconViewStr);
    frameStr_->setTabIcon(0, iconViewStr);
    */

    const QIcon iconEditSrc(":/images/EditSrc.png");
    // actionViewSource_->setIcon(iconEditSrc);
    p->view_->setTabIcon(p->view_->getIdTabSource(), iconEditSrc);
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void FrameBrowser::setupToolBars()
{
    // :: Main
    QToolBar *pToolBar = this->addToolBar(QString("ToolsMain"));
    pToolBar->setObjectName("ToolsMain");
    pToolBar->addAction(ptrUI_->actionClose_);
    pToolBar->addAction(ptrUI_->actionExit_);
    pToolBar->addAction(ptrUI_->actionOpen_);
    pToolBar->addAction(ptrUI_->actionSave_);
    // pToolBar->addWidget(dropDownMenu)
    // :: Edit
    QToolBar *pToolEdit = this->addToolBar(QString("ToolsEdit"));
    pToolEdit->setObjectName("ToolsEdit");
    pToolEdit->addAction(ptrUI_->actionUndo_);
    pToolEdit->addAction(ptrUI_->actionRedo_);
    pToolEdit->addSeparator();
    pToolEdit->addAction(ptrUI_->actionCut_);
    pToolEdit->addAction(ptrUI_->actionCopy_);
    pToolEdit->addAction(ptrUI_->actionPaste_);
    pToolEdit->addSeparator();
    pToolEdit->addAction(ptrUI_->actionClear_);
    pToolEdit->addAction(ptrUI_->actionClearAll_);
    pToolEdit->addAction(ptrUI_->actionClearOthers_);
    // :: BgColor
    named_colors_->setMaximumWidth(125);
    connect(named_colors_, &NamedColors::currentTextChanged,
            this, &FrameBrowser::setBgColorByName);
    //
    edit_red_->setMaximumWidth(100);
    edit_red_->setPlaceholderText(tr("[red]"));
    this->setBgColorRed();
    connect(edit_red_, &QLineEdit::editingFinished,
            this, &FrameBrowser::setBgColorRed);
    //
    edit_green_->setMaximumWidth(100);
    edit_green_->setPlaceholderText(tr("[green]"));
    this->setBgColorGreen();
    connect(edit_green_, &QLineEdit::editingFinished,
            this, &FrameBrowser::setBgColorGreen);
    //
    edit_blue_->setMaximumWidth(100);
    edit_blue_->setPlaceholderText(tr("[blue]"));
    this->setBgColorBlue();
    connect(edit_blue_, &QLineEdit::editingFinished,
            this, &FrameBrowser::setBgColorBlue);

    QToolBar *pColorBar = this->addToolBar(QString("ToolsColorBg"));
    pColorBar->setObjectName("ToolsColorBg");
    pColorBar->addAction(ptrUI_->actionBackgroundColor_);
    pColorBar->addSeparator();
    pColorBar->insertWidget(ptrUI_->actionBackgroundColor_, named_colors_);
    pColorBar->addSeparator();
    pColorBar->insertWidget(ptrUI_->actionBackgroundColor_, edit_red_);
    pColorBar->insertWidget(ptrUI_->actionBackgroundColor_, edit_green_);
    pColorBar->insertWidget(ptrUI_->actionBackgroundColor_, edit_blue_);

    const QString nameTBMol("ToolsMolecule");
    QToolBar *pBarMol = this->addToolBar(nameTBMol);
    pBarMol->setObjectName(nameTBMol);
    pBarMol->addAction(ptrUI_->actionProjectReset_);
    pBarMol->addSeparator();
    pBarMol->addAction(ptrUI_->actionProjectOrthogonal_);
    pBarMol->addAction(ptrUI_->actionProjectPerspective_);
    pBarMol->addSeparator();
    pBarMol->addAction(ptrUI_->actionStyleMolFast_);
    pBarMol->addAction(ptrUI_->actionStyleMolSticks_);
    pBarMol->addAction(ptrUI_->actionStyleMolBalls_);
    pBarMol->addAction(ptrUI_->actionStyleMolSpace_);
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void FrameBrowser::updateUi(Ui::FrameBrowser *pUI)
{
}
void FrameBrowser::closeEvent(QCloseEvent *event)
{
    this->saveSettings();
    auto *pView = this->getView();
    if (!pView->isModified()
        // || pView->trySaveModified()
    )
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void FrameBrowser::toggleFullScreen(void)
{
    if (isFullScreen_)
    { // restoring
        this->loadSettings();
    }
    else
    { // setup full-screen view
        this->saveSettings();
    }
    //
    isFullScreen_ = !isFullScreen_;
    ptrUI_->actionFullScreen_->setChecked(isFullScreen_);
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameBrowser::on_actionBackgroundColor__triggered(void)
{
    auto *pView = this->getView()->viewMolecule();
    QColorDialog::ColorDialogOptions opts = QColorDialog::DontUseNativeDialog
        // | QColorDialog::NoEyeDropperButton << for Qt > 6.6 >>
        // | QColorDialog::NoButtons
        ;

    QColor colNew = QColorDialog::getColor(pView ? pView->getBgQColor() : QColor(),
                                           pView,
                                           QString(tr("Graph backgound color")),
                                           opts);
    if (colNew.isValid())
        pView->changeColorUnnamed(colNew);
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameBrowser::setBgColorByName(const QString &name)
{
    if (!name.indexOf('-'))
    {
        this->on_actionBackgroundColor__triggered();
        return;
    }
    int nSC = name.indexOf(';');
    QByteArray bytes(name.toLatin1());
    if (nSC > 0)
        bytes[nSC] = 0;
    auto *pGraph = this->getView()->viewMolecule();
    if (!pGraph)
        return;
    pGraph->setBgColorNamed(String(bytes.data()));
    // load color RGB in edits
    edit_red_->setText(QString("%1").arg(pGraph->getBgColorRed()));
    edit_green_->setText(QString("%1").arg(pGraph->getBgColorGreen()));
    edit_blue_->setText(QString("%1").arg(pGraph->getBgColorBlue()));
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameBrowser::setBgColorRed(void)
{
    auto *pGraph = this->getView()->viewMolecule();
    if (!pGraph)
    {
        edit_red_->clear();
    }
    else
    {
        QString sText(edit_red_->text());
        bool bOk(!sText.isEmpty());
        if (bOk)
        {
            double val = sText.toDouble(&bOk);
            if (bOk)
                bOk = pGraph->setBgColorRed(val);
        }
        if (!bOk)
            edit_red_->setText(QString("%1").arg(pGraph->getBgColorRed()));
    }
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameBrowser::setBgColorGreen(void)
{
    auto *pGraph = this->getView()->viewMolecule();
    if (!pGraph)
    {
        edit_green_->clear();
    }
    else
    {
        QString sText(edit_green_->text());
        bool bOk(!sText.isEmpty());
        if (bOk)
        {
            double val = sText.toDouble(&bOk);
            if (bOk)
                bOk = pGraph->setBgColorGreen(val);
        }
        if (!bOk)
            edit_green_->setText(QString("%1").arg(pGraph->getBgColorGreen()));
    }
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameBrowser::setBgColorBlue(void)
{
    auto *pGraph = this->getView()->viewMolecule();
    if (!pGraph)
    {
        edit_blue_->clear();
    }
    else
    {
        QString sText(edit_blue_->text());
        bool bOk(!sText.isEmpty());
        if (bOk)
        {
            double val = sText.toDouble(&bOk);
            if (bOk)
                bOk = pGraph->setBgColorBlue(val);
        }
        if (!bOk)
            edit_blue_->setText(QString("%1").arg(pGraph->getBgColorBlue()));
    }
}

//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameBrowser::on_actionExit__triggered(void)
{
    qApp->closeAllWindows();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameBrowser::on_actionAboutQt__triggered(void)
{
    qApp->aboutQt();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameBrowser::on_actionClose__triggered(void)
{
    this->close();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameBrowser::on_actionNewBrowser__triggered(void)
{
    if (!this->isFullScreen())
        this->saveSettings();
    else
        this->toggleFullScreen();
    FrameBrowser *pBrowse = FrameBrowser::createNewFrame(this);
    pBrowse->show();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameBrowser::on_actionNewFile__triggered(void)
{
    auto *pView = this->getView();
    if (!pView->isModified()
        // || pView->trySaveModified()
    )
        pView->clearAll(); // renew the content
    pView->setModeTextEdit();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameBrowser::on_actionAboutThis__triggered(void)
{
    QString message(tr("<center><b> S q D r v </b></center><br>This example demonstrates how to write single document interface (<b>SDI</b>) applications using Qt and VTK<br><br>Qt version %1-%2 from https://wwww.qt.io<br>VTK version %3 from https://vtk.org/"));

    QMessageBox::about(this, tr("About the application"),
                       message.arg(QT_VERSION_STR).arg(QT_VERSION).arg(vtkVersion::GetVTKVersionFull()));
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameBrowser::on_actionOpen__triggered(void)
{
    QString sCaption(tr("Open file(s)"));
    QString sDir; // (this->getView()->getPath());
    QString sFilter = FrameFileData::getFilterStringOpen();
    QString sFilterSel; // (getView()->getFormatInput().);
    QFileDialog::Options optionsOpen = QFileDialog::DontUseCustomDirectoryIcons |
                                       QFileDialog::DontUseNativeDialog;

    auto sPaths = QFileDialog::getOpenFileNames(this,
                                                sCaption,
                                                sDir,
                                                sFilter, &sFilterSel, optionsOpen);
    this->provideFilePaths(sPaths.begin(), sPaths.end());
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameBrowser::on_actionSaveAs__triggered(void)
{
    QString sCaption(tr("Save as file"));
    QString sDir(this->getView()->getPath().c_str());
    QString sFilter = FrameFileData::getFilterStringSave();
    QString sFilterSel; // (getView()->getFormatInput().);
    QFileDialog::Options optionsOpen = QFileDialog::DontUseCustomDirectoryIcons |
                                       QFileDialog::DontUseNativeDialog;

    QString sPath = QFileDialog::getSaveFileName(this,
                                                 sCaption,
                                                 sDir,
                                                 sFilter, &sFilterSel, optionsOpen);
    // this->provideFilePaths(sPaths.begin(), sPaths.end());
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameBrowser::on_actionProperties__triggered(void)
{
    const auto *pView = this->getView();
    if (!pView || !pView->hasPath())
        return;
    DialogFileProperties dlgFile(pView->getPath(), this);
    int kRes = dlgFile.exec();
    if (kRes == QDialog::Accepted)
    {
        // load newly changed data
        // this->updateUi();
    }
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameBrowser::on_actionProjectReset__triggered(void)
{
    auto *pView = this->getView();
    if (!pView)
        return;
    auto *pMW = pView->viewMolecule();
    if (!pMW)
        return;
    auto *pMap = pMW->getMapMolecule();
    if (!pMap)
        return;

    // now changing settings
    // if(!Projection::IsDefault( ? ? ? )) return;
    RenderTheWindow rw(pMW->getRenderWindow());
    if (!rw)
        return;
    else
    {
        MolMapRefresh mmrf(pMap);
    }
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameBrowser::on_actionProjectOrthogonal__triggered(void)
{
    auto *pView = this->getView();
    if (!pView)
        return;
    auto *pMW = pView->viewMolecule();
    if (!pMW)
        return;
    auto *pMap = pMW->getMapMolecule();
    if (!pMap)
        return;

    // now changing settings
    // if(!Projection::IsOrthogonal( ? ? ? )) return;
    RenderTheWindow rw(pMW->getRenderWindow());
    if (!rw)
        return;
    else
    {
        MolMapRefresh mmrf(pMap);
    }
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameBrowser::on_actionProjectPerspective__triggered(void)
{
    auto *pView = this->getView();
    if (!pView)
        return;
    auto *pMW = pView->viewMolecule();
    if (!pMW)
        return;
    auto *pMap = pMW->getMapMolecule();
    if (!pMap)
        return;

    // now changing settings
    // if(!Projection::IsPerspective( ? ? ? )) return;
    RenderTheWindow rw(pMW->getRenderWindow());
    if (!rw)
        return;
    else
    {
        MolMapRefresh mmrf(pMap);
    }
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameBrowser::on_actionStyleMolFast__triggered(void)
{
    auto *pView = this->getView();
    if (!pView)
        return;
    auto *pMW = pView->viewMolecule();
    if (!pMW)
        return;
    auto *pMap = pMW->getMapMolecule();
    if (!pMap)
        return;

    // now changing settings
    // if(!StyleMoleculeFast::IsAppliedTo(pMap)) return;
    RenderTheWindow rw(pMW->getRenderWindow());
    if (!rw)
        return;
    else
    {
        MolMapRefresh mmrf(pMap);
        mmrf->UseFastSettings();
    }
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameBrowser::on_actionStyleMolBalls__triggered(void)
{
    auto *pView = this->getView();
    if (!pView)
        return;
    auto *pMW = pView->viewMolecule();
    if (!pMW)
        return;
    auto *pMap = pMW->getMapMolecule();
    if (!pMap)
        return;

    // now changing settings
    // if(!StyleMoleculeBallSticks::IsAppliedTo(pMap)) return;
    RenderTheWindow rw(pMW->getRenderWindow());
    if (!rw)
        return;
    else
    {
        MolMapRefresh mmrf(pMap);
        mmrf->UseBallAndStickSettings();
    }
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameBrowser::on_actionStyleMolSticks__triggered(void)
{
    auto *pView = this->getView();
    if (!pView)
        return;
    auto *pMW = pView->viewMolecule();
    if (!pMW)
        return;
    auto *pMap = pMW->getMapMolecule();
    if (!pMap)
        return;

    // now changing settings
    // if(!StyleMoleculeSticks::IsAppliedTo(pMap)) return;
    RenderTheWindow rw(pMW->getRenderWindow());
    if (!rw)
        return;
    else
    {
        MolMapRefresh mmrf(pMap);
        mmrf->UseLiquoriceStickSettings();
    }
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameBrowser::on_actionStyleMolSpace__triggered(void)
{
    auto *pView = this->getView();
    if (!pView)
        return;
    auto *pMW = pView->viewMolecule();
    if (!pMW)
        return;
    auto *pMap = pMW->getMapMolecule();
    if (!pMap)
        return;

    // now changing settings
    // if(!StyleMoleculeVDW::IsAppliedTo(pMap)) return;
    RenderTheWindow rw(pMW->getRenderWindow());
    if (!rw)
        return;
    else
    {
        MolMapRefresh mmrf(pMap);
        mmrf->UseVDWSpheresSettings();
    }
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void FrameBrowser::on_actionFullScreen__triggered(void)
{
    this->toggleFullScreen();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
