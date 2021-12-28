#ifndef QNOTEPAD_LINENUMBER_HPP
#define QNOTEPAD_LINENUMBER_HPP

#include <QWidget>
#include "editor.hpp"

class LineNumber : public QWidget
{
public:
    explicit LineNumber(Editor *editor);
    [[nodiscard]] QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Editor *editor;
};

#endif //QNOTEPAD_LINENUMBER_HPP
