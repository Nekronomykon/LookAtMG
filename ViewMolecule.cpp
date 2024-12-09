#include "ViewMolecule.h"
#include "./ui_ViewMolecule.h"

#include "RenderVTKWindow.h"
#include "ImageWriter.h"

#include <vtkWindowToImageFilter.h>
#include <vtkNamedColors.h>
#include <vtkProperty.h>

#include <QColor>
#include <QMessageBox>

ViewMolecule::ViewMolecule(QWidget *parent)
    : QWidget(parent),
      ptrUI_(std::make_unique<Ui::ViewMolecule>()),
      nameBgColor_("alice_blue")
{
    ptrUI_->setupUi(this);
    //
    mapMol_->UseFastSettings(); //
    // mapMol_->SetRenderBonds(false);
    actorMol_->SetMapper(mapMol_);
    //
    vtkProperty *pProp(actorMol_->GetProperty());
    pProp->SetDiffuse(0.75);
    pProp->SetSpecular(0.75);
    pProp->SetSpecularPower(20.0);
    //
    ///////////////////////////////////////////////////////////////////////////////////////////
    //
    renderMol_->AddActor(actorMol_);
    wndOpenGL_->AddRenderer(renderMol_);
    auto *pView = ptrUI_->view_;
    pView->setRenderWindow(wndOpenGL_.Get());
    //
    ///////////////////////////////////////////////////////////////////////////////////////////
    //
    this->applyBackgroundColor(true);
}

ViewMolecule::~ViewMolecule() {}

bool ViewMolecule::setBgColorRed(double val)
{
    bool bRes(val >= 0.0 && val <= 1.0);
    if (bRes)
    {
        colorBg_.SetRed(val);
        this->applyBackgroundColor();
    }
    return bRes;
}

bool ViewMolecule::setBgColorGreen(double val)
{
    bool bRes(val >= 0.0 && val <= 1.0);
    if (bRes)
    {
        colorBg_.SetGreen(val);
        this->applyBackgroundColor();
    }
    return bRes;
}

bool ViewMolecule::setBgColorBlue(double val)
{
    bool bRes(val >= 0.0 && val <= 1.0);
    if (bRes)
    {
        colorBg_.SetBlue(val);
        this->applyBackgroundColor();
    }
    return bRes;
}

bool ViewMolecule::applyBackgroundColor(bool bLoadNamed)
{
    bool bRes = false;
    if (bLoadNamed && !nameBgColor_.empty())
    {
        vtkNew<vtkNamedColors> colors;
        colorBg_ = colors->GetColor3d(nameBgColor_);
        bRes = true;
    }
    renderMol_->SetBackground(colorBg_.GetData());
    wndOpenGL_->Render();
    //
    return bRes;
}

void ViewMolecule::changeColorUnnamed(const QColor &col)
{
    vtkColor3d bg((double)col.red() / 255, (double)col.green() / 255, (double)col.blue() / 255);
    //
    vtkNew<vtkNamedColors> named; // how to find a name?
    nameBgColor_ = String();      // user color

    // named->FindColorName(bg);
    colorBg_ = bg;
    this->applyBackgroundColor();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void ViewMolecule::setBgColorNamed(String sName)
{
    nameBgColor_ = sName;
    this->applyBackgroundColor(true);
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
QColor ViewMolecule::getBgQColor() const
{
    return QColor(colorBg_.GetRed() * 255, colorBg_.GetGreen() * 255, colorBg_.GetBlue() * 255);
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
QLineEdit *ViewMolecule::getEditTitle() const { return ptrUI_->title_; }
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
QVTKWidget *ViewMolecule::getViewStructure() const { return ptrUI_->view_; }
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
vtkRenderWindow *ViewMolecule::getRenderWindow() const { return wndOpenGL_; }
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
bool ViewMolecule::exportImageTo(ImageWriter *pIW, bool bAlpha)
{
    if (!pIW)
        return false;

    RenderVTKWindow pRW(wndOpenGL_);
    //
    vtkNew<vtkWindowToImageFilter> w2if;
    w2if->SetFixBoundary(true);
    // w2if->SetScale(4); --> how to scale label fonts???  to think on...
    //
    w2if->SetInput(pRW);
    if (bAlpha)
        w2if->SetInputBufferTypeToRGBA();
    else
        w2if->SetInputBufferTypeToRGB();
    w2if->ReadFrontBufferOff();
    w2if->Update();

    pIW->SetInputConnection(w2if->GetOutputPort());
    // pIW->Write();
    //
    return true;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
