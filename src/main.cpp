#include "notepad.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("QNotePad");
    QCoreApplication::setOrganizationName("QNotePad");
    Notepad w;
    // We will check if the program receives some files
    if(argc > 1)
    {
        for(auto i = 1;i < argc;i++)
        {
            w.onOpenFile(argv[i]);
        }
    }
    w.show();
    return QApplication::exec();
}