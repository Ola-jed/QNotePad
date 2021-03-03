#include "notepad.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Notepad w;
    // We will check if the program receives a file
    if(argc > 1)
    {
        QString paramFile{argv[1]};
        w.onOpenFile(paramFile);
    }
    w.show();
    return a.exec();
}
