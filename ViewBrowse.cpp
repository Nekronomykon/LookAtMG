#include "ViewBrowse.h"

ViewBrowse::ViewBrowse(QWidget *parent)
    : QSplitter(Qt::Vertical, parent),
      wkspace_(new ViewListPathString(this)),
      content_(new ViewFileContent(this))
{
}