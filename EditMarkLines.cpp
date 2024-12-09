#include "EditMarkLines.h"

#include "BarLines.h"

#include <QFile>
#include <QPainter>
#include <QTextBlock>
#include <QTextStream>
#include <QGuiApplication>

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
EditMarkLines::EditMarkLines(QWidget *parent)
    : QPlainTextEdit(parent), barLines_(new BarLines(this))
{
  QFont fontFx("Monospace", 10, -1, false);
  fontFx.setFixedPitch(true);
  this->setFont(fontFx);
  //
  connect(this, &EditMarkLines::blockCountChanged,
          this, &EditMarkLines::updateLineNumberAreaWidth);
  connect(this, &EditMarkLines::updateRequest,
          this, &EditMarkLines::updateLineNumberArea);
  connect(this, &EditMarkLines::cursorPositionChanged,
          this, &EditMarkLines::highlightCurrentLine);

  this->updateLineNumberAreaWidth(0);
  this->highlightCurrentLine();
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
int EditMarkLines::lineNumberAreaWidth()
{
  int digits = 4;
  int max = std::max(1, this->blockCount());
  while (max >= 10)
  {
    max /= 10;
    ++digits;
  }
  /// digits += 3;

  int space = 4 + this->fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

  return space;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void EditMarkLines::updateLineNumberAreaWidth(int /* newBlockCount */)
{
  this->setViewportMargins(this->lineNumberAreaWidth(), 0, 0, 0);
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void EditMarkLines::updateLineNumberArea(const QRect &rect, int dy)
{
  if (dy)
    barLines_->scroll(0, dy);
  else
    barLines_->update(0, rect.y(), barLines_->width(), rect.height());

  if (rect.contains(this->viewport()->rect()))
    this->updateLineNumberAreaWidth(0);
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void EditMarkLines::resizeEvent(QResizeEvent *e)
{
  QPlainTextEdit::resizeEvent(e);

  QRect cr = this->contentsRect();
  barLines_->setGeometry(QRect(cr.left(), cr.top(), this->lineNumberAreaWidth(), cr.height()));
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void EditMarkLines::highlightCurrentLine()
{
  QList<QTextEdit::ExtraSelection> extraSelections; // eh??

  QTextEdit::ExtraSelection selection;
  bool bReadOnly = this->isReadOnly();
  // how to make these colors theme-darkness-dependent (Linux)?
  // background:
  QColor lineColor(bReadOnly ? (Qt::gray) : (Qt::yellow));
  lineColor = lineColor.lighter(100);

  selection.format.setBackground(lineColor);
  // how to adjust font color?
  // selection.format.setColorBlaBlah...
  selection.format.setProperty(QTextFormat::FullWidthSelection, true);
  selection.cursor = this->textCursor();
  barLines_->resetActiveID(selection.cursor.blockNumber());

  selection.cursor.clearSelection();
  extraSelections.append(selection);
  this->setExtraSelections(extraSelections);
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @name  //
/// @brief //
/// @param //
//
void EditMarkLines::lineNumberAreaPaintEvent(QPaintEvent *event)
{
  QPainter painter(barLines_);
  painter.fillRect(event->rect(), Qt::lightGray);

  QTextBlock block = this->firstVisibleBlock();
  int blockNumber = block.blockNumber();
  int top = qRound(this->blockBoundingGeometry(block).translated(contentOffset()).top());
  int bottom = top + qRound(this->blockBoundingRect(block).height());

  while (block.isValid() && top <= event->rect().bottom())
  {
    // if (block.text().isEmpty())
    //  continue;
    if (block.isVisible() && bottom >= event->rect().top())
    {
      QString sText;
      QString sNumber = QString::number(blockNumber + 1);
      if (blockNumber == barLines_->getActiveID())
        sText += '[';
      else
        sText += ' ';
      sText += sNumber;
      if (blockNumber == barLines_->getActiveID())
        sText += ']';
      painter.setPen(Qt::black);
      int kx = this->fontMetrics().horizontalAdvance(QLatin1Char('9'));
      painter.drawText(kx, top, barLines_->width() - kx, this->fontMetrics().height(),
                       Qt::AlignLeft, sText);
    }

    block = block.next();
    top = bottom;
    bottom = top + qRound(this->blockBoundingRect(block).height());
    ++blockNumber;
  }
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool EditMarkLines::loadPath(const QString &sPath)
{
  QFile file(sPath);
  if (!file.open(QFile::ReadOnly | QFile::Text))
    return false;
  QTextStream in(&file);
  this->setPlainText(in.readAll());
  return true;
}
