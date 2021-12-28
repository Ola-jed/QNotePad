#ifndef QNOTEPAD_EDITOR_HPP
#define QNOTEPAD_EDITOR_HPP

#include <QPlainTextEdit>

class Editor : public QPlainTextEdit
{
Q_OBJECT
public:
    explicit Editor(QWidget *parent = nullptr);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
protected:
    void resizeEvent(QResizeEvent *event) override;
private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);
private:
    QWidget *lineNumberArea;
};

#endif //QNOTEPAD_EDITOR_HPP
