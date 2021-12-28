#include "linenumber.hpp"

LineNumber::LineNumber(Editor *editor) : QWidget(editor), editor(editor)
{
}

QSize LineNumber::sizeHint() const
{
    return {editor->lineNumberAreaWidth(), 0};
}

void LineNumber::paintEvent(QPaintEvent *event)
{
    editor->lineNumberAreaPaintEvent(event);
}