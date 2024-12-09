#ifndef QVTK_Widget_h__
#define QVTK_Widget_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

// For now, only wraps the QVTKOpenGLNativeWidget
#include <QVTKOpenGLNativeWidget.h>
using QVTKWidget = QVTKOpenGLNativeWidget;

#endif // !QVTK_Widget_h__
