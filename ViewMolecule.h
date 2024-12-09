#ifndef View_Molecule_h__
#define View_Molecule_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "ImageWriter.h"
#include "MapMolecule.h"
#include "QVTKWidget.h"

#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkGenericOpenGLRenderWindow.h>

#include <vtkActor.h>
#include <vtkRenderer.h>

#include <vtkColor.h>
#include <vtkNew.h>

#include <memory>
#include <string>
using String = std::string;

#include <QLineEdit>

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class ViewMolecule;
}
QT_END_NAMESPACE

class ViewMolecule
    : public QWidget
{
    Q_OBJECT

public:
    explicit ViewMolecule(QWidget * /*parent*/ = nullptr);
    virtual ~ViewMolecule();
    //
    MapOfMolecule *getMapMolecule() const { return mapMol_.Get(); }
    //
    vtkColor3d getBgColor() const { return colorBg_; }
    QColor getBgQColor(void) const;
    //
    double getBgColorRed() const { return colorBg_.GetRed(); }
    double getBgColorGreen() const { return colorBg_.GetGreen(); }
    double getBgColorBlue() const { return colorBg_.GetGreen(); }
    //
    bool setBgColorRed(double /*val*/);
    bool setBgColorGreen(double /*val*/);
    bool setBgColorBlue(double /*val*/);
    //
    QLineEdit *getEditTitle() const;
    QVTKWidget *getViewStructure() const;
    vtkRenderWindow *getRenderWindow() const;
    //
    bool exportImageTo(ImageWriter * /*pIW*/, bool /*bAlpha*/ = true);

public slots:
    void changeColorUnnamed(const QColor &col);
    void setBgColorNamed(String /*name*/);

protected:
    bool applyBackgroundColor(bool /*bLoadNamed*/ = false);

private:
    std::unique_ptr<Ui::ViewMolecule> ptrUI_;
    ANewMapOfMolecule mapMol_;
    //
    String nameBgColor_;
    vtkColor3d colorBg_;
    vtkNew<vtkRenderer> renderBg_;
    //
    vtkNew<vtkGenericOpenGLRenderWindow> wndOpenGL_;
    vtkNew<vtkRenderer> renderMol_;
    vtkNew<vtkActor> actorMol_;
    vtkNew<vtkRenderWindowInteractor> iren;
    //
};

class RenderTheWindow
{
public:
    explicit RenderTheWindow(vtkRenderWindow *pInit) : ptr_(pInit) {}
    ~RenderTheWindow()
    {
        if (ptr_)
            ptr_->Render();
    }
    operator vtkRenderWindow *() const { return ptr_; }
    vtkRenderWindow *operator->() const { return ptr_; }
    bool operator!() const { return bool(ptr_ == nullptr); }

private:
    RenderTheWindow() = delete;
    RenderTheWindow(const RenderTheWindow &) = delete;
    vtkRenderWindow *ptr_ = nullptr;
};

#endif // View_Molecule_h__
