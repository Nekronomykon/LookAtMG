#ifndef View_Browse_h__
#define View_Browse_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "ViewWorkspace.h"
#include "ViewListPathString.h"
#include "ViewFileContent.h"

#include <QSplitter>
#include <QPointer>

class ViewBrowse
    : public QSplitter
{
    Q_OBJECT
public:
    explicit ViewBrowse(QWidget * /*parent*/ = nullptr);
    ~ViewBrowse() override = default;

    ViewListPathString *getWorkspace() const { return wkspace_; }        
    ViewFileContent* getContent() const { return content_; }

private:
    QPointer<ViewListPathString> wkspace_;
    QPointer<ViewFileContent> content_;
};

#endif // View_Browse_h__
