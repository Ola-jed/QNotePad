#include <QPainter>
#include <QTextBlock>
#include "editor.hpp"
#include "linenumber.hpp"

Editor::Editor(QWidget *parent) :
        QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumber(this);
    connect(this, &Editor::blockCountChanged, this, &Editor::updateLineNumberAreaWidth);
    connect(this, &Editor::updateRequest, this, &Editor::updateLineNumberArea);
    connect(this, &Editor::cursorPositionChanged, this, &Editor::highlightCurrentLine);
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

int Editor::lineNumberAreaWidth()
{
    auto digits = 1;
    auto max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }
    return 4 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
}

void Editor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void Editor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
    {
        lineNumberArea->scroll(0, dy);
    }
    else
    {
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect()))
    {
        updateLineNumberAreaWidth(0);
    }
}

void Editor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);
    auto const cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void Editor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;
        auto lineColor = QColor(0xfcefcd);
        lineColor.setAlpha(50);
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    setExtraSelections(extraSelections);
}

void Editor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPen pen(0xf5f5f5);
    pen.setWidth(4);
    QPainter painter{lineNumberArea};
    painter.fillRect(event->rect(), 0x0f4851);
    QTextBlock block = firstVisibleBlock();
    auto blockNumber = block.blockNumber();
    auto top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    auto bottom = top + qRound(blockBoundingRect(block).height());
    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            auto const number = QString::number(blockNumber + 1);
            painter.setPen(pen);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignCenter,
                             number);
        }
        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}