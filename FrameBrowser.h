#ifndef Frame_Browser_h__
#define Frame_Browser_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "NamedColors.h"
#include <QLineEdit>
//
#include "ViewFileSystem.h"
#include "ViewFileHistory.h"
#include "ViewBrowse.h"

#include "FrameFileData.h"

#include <memory>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class FrameBrowser;
}
QT_END_NAMESPACE

class FrameBrowser : public QMainWindow
{
    Q_OBJECT

public:
    explicit FrameBrowser(QWidget *parent = nullptr);
    ~FrameBrowser() override;

    static FrameBrowser *createNewFrame(QWidget * /*parent*/ = nullptr);
    FrameFileData *getView() const;
    // dock view access:
    // ========================================================================
    ViewFileSystem *getViewFiles(void) const { return files_; }
    // ------------------------------------------------------------------------
    ViewFilesTree *getFileTree() const { return this->getViewFiles()->getFileTree(); }
    ViewFilesList *getFileList() const { return this->getViewFiles()->getFileList(); }
    // ========================================================================
    ViewBrowse *getBrowse() const { return browse_; }
    // ------------------------------------------------------------------------
    ViewListPathString *getWorkspace(void) const { return this->getBrowse()->getWorkspace(); }
    ViewFileContent *getContent() const { return this->getBrowse()->getContent(); }
    // ========================================================================
    ViewFileHistory *getHistory() const { return history_; }
    // ------------------------------------------------------------------------
    // ========================================================================
    //
    void provideFilePath(const QString & /* sPath */);
    template <typename Iter>
    void provideFilePaths(Iter from, Iter to)
    {
        if (from == to)
            return;
        QString sLoad = QString(*from);
        do
        {
            this->addPath(QString(*from));
        } while (++from != to);
        //
        this->getView()->loadFile(sLoad);
    }
    //
    void loadSettings();
    void saveSettings();

protected slots:
    void on_actionExit__triggered(void);
    void on_actionOpen__triggered(void);
    void on_actionClose__triggered(void);
    void on_actionSaveAs__triggered(void);
    void on_actionAboutQt__triggered(void);
    void on_actionNewFile__triggered(void);
    void on_actionAboutThis__triggered(void);
    void on_actionProperties__triggered(void);
    void on_actionFullScreen__triggered(void);
    void on_actionNewBrowser__triggered(void);
    void on_actionStyleMolFast__triggered(void);
    void on_actionStyleMolBalls__triggered(void);
    void on_actionStyleMolSpace__triggered(void);
    void on_actionStyleMolSticks__triggered(void);
    void on_actionProjectReset__triggered(void);
    void on_actionProjectOrthogonal__triggered(void);
    void on_actionProjectPerspective__triggered(void);
    void on_actionBackgroundColor__triggered(void);

protected:
    void closeEvent(QCloseEvent * /*event*/) override;

protected:
    void addPath(const QString & /*sPath*/);
    void updateUi(Ui::FrameBrowser * /*pUI*/);

private slots:
    void setBgColorByName(const QString & /*name*/);
    void setBgColorRed(void);
    void setBgColorGreen(void);
    void setBgColorBlue(void);

private:
    void setupDockingViews(void);
    static void setupMenuActions(Ui::FrameBrowser * /*p*/);
    void setupToolBars(void);
    //
    void toggleFullScreen(void);
    //
    std::unique_ptr<Ui::FrameBrowser> ptrUI_;
    // Tool views
    QPointer<NamedColors> named_colors_;
    QPointer<QLineEdit> edit_red_;
    QPointer<QLineEdit> edit_green_;
    QPointer<QLineEdit> edit_blue_;
    // docking views:
    bool isFullScreen_ = false;
    QPointer<ViewFileSystem> files_;
    QPointer<ViewBrowse> browse_;
    QPointer<ViewFileHistory> history_;
};
#endif // Frame_Browser_h__
