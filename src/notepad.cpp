#include "notepad.hpp"

Notepad::Notepad(QWidget *parent)
    : QMainWindow(parent)
{
    newFile    = new QPushButton("New");
    openFile   = new QPushButton("Open");
    saveFile   = new QPushButton("Save");
    quit       = new QPushButton("Quit");
    colorText  = new QPushButton("Color");
    openFile   = new QPushButton("Open");
    label      = new QLabel("File");
    textEdit   = new QPlainTextEdit(this);
    autoSaveCheckBox        = new QCheckBox("AutoSave",this);
    QHBoxLayout *hboxLayout = new QHBoxLayout();
    hboxLayout->addWidget(label);
    hboxLayout->addWidget(newFile);
    hboxLayout->addWidget(openFile);
    hboxLayout->addWidget(saveFile);
    hboxLayout->addWidget(colorText);
    hboxLayout->addWidget(quit);
    hboxLayout->addWidget(autoSaveCheckBox);
    QVBoxLayout *vboxLayout = new QVBoxLayout();
    vboxLayout->addLayout(hboxLayout);
    vboxLayout->addWidget(textEdit);
    setLayout(vboxLayout);
    auto central = new QWidget;
    central->setLayout(vboxLayout);
    setCentralWidget(central);
    setStyleSheet("QPushButton{background-color: rgb(28, 49, 80);color:#f32e69;}QLabel{color:#27fff8;}");
    connect(quit,&QPushButton::clicked,this,&Notepad::onQuit);
    connect(newFile,&QPushButton::clicked,this,&Notepad::onNewFile);
    connect(openFile,&QPushButton::clicked,this,&Notepad::onOpenFile);
    connect(saveFile,&QPushButton::clicked,this,&Notepad::onSaveFile);
    connect(textEdit,&QPlainTextEdit::textChanged,this,&Notepad::onTextChanged);
    connect(textEdit,&QPlainTextEdit::textChanged,this,&Notepad::onAutoSave);
    connect(colorText,&QPushButton::clicked,this,&Notepad::onColorChanged);
}

void Notepad::onNewFile()
{
    fileName = QFileDialog::getSaveFileName(this);
    if(fileName.isEmpty())
    {
        QMessageBox::warning(this,"Nouveau Fichier","Entrer un nom valide");
    }
    else
    {
        QFile fichier{fileName};
        if((!fichier.open(QIODevice::ReadWrite)))
        {
            QMessageBox::critical(this,"Nouveau Fichier","Impossible de créer le fichier");
            return;
        }
        else
        {
            QMessageBox::information(this,"Nouveau Fichier","Le fichier a bien été créé");
            label->setText(fileName);
        }
    }
}

void Notepad::onOpenFile()
{
    fileName = QFileDialog::getOpenFileName(this);
    if(fileName.isEmpty())
    {
        QMessageBox::warning(this,"Nouveau Fichier","Choisir fichier valide");
        return;
    }
    else
    {
        QFile fichier{fileName};
        if((!fichier.open(QIODevice::ReadWrite)))
        {
            QMessageBox::critical(this,"Nouveau Fichier","Impossible d'ouvrir le fichier");
            return;
        }
        else
        {
            label->setText(fileName);
            textEdit->setPlainText(" ");
            // Reading the file line by line and storing int the textEdit.
            QTextStream in{&fichier};
            QString tempLine;
            while(!in.atEnd())
            {
                tempLine = in.readLine();
                textEdit->appendPlainText(tempLine);
            }
        }
    }
}

void Notepad::onSaveFile()
{
    if(fileName.isEmpty() && !(textEdit->toPlainText().isEmpty())) // If this is a new file
    {
        fileName = QFileDialog::getSaveFileName(this);
        if(fileName.isEmpty())
        {
            QMessageBox::warning(this,"Nouveau Fichier","Entrer un nom valide");
        }
        else
        {
            QFile fichier{fileName};
            if((!fichier.open(QIODevice::ReadWrite)))
            {
                QMessageBox::critical(this,"Sauvegarder","Impossible de sauvegarder le fichier");
                return;
            }
            else
            {
                label->setText(fileName);
            }
        }
    }
    else if((!fileName.isEmpty()) && !(textEdit->toPlainText().isEmpty()))
    {
        QFile fich{fileName};
        if(fich.open(QIODevice::ReadWrite|QFile::Truncate))
        {
            auto labelFont = label->font();
            labelFont.setWeight(QFont::Normal);
            label->setFont(labelFont);
            QTextStream out{&fich};
            out << textEdit->toPlainText() << Qt::endl;
            isSaved = true;
            QMessageBox::information(this,"Sauvegarde","Sauvegarde Réussie");
        }
        else
        {
            QMessageBox::critical(this,"Sauvegarde","Impossible de sauvegarder");
            return;
        }
    }
}

void Notepad::onQuit()
{
    if(((!fileName.isEmpty()) && !isSaved && !(textEdit->toPlainText().isEmpty()))||(fileName.isEmpty() && !(textEdit->toPlainText().isEmpty())))
    {
        auto reply = QMessageBox::question(this, "Quitter", "Voulez vous sauvegarder ?",QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes)
        {
            onSaveFile();
        }
    }
    qApp->quit();
}

void Notepad::onTextChanged() // Set the weight of the filename to bold to show that the file isn't saved.
{
    auto labelFont = label->font();
    labelFont.setWeight(99);
    label->setFont(labelFont);
}

void Notepad::onColorChanged() // Get the color and set the color in the textEdit.
{
    QColor chosenColor = QColorDialog::getColor("Choisir une couleur");
    QString colorToSet = QString::number(chosenColor.red())+","+QString::number(chosenColor.green())+","+QString::number(chosenColor.blue());
    setStyleSheet("QPlainTextEdit{color:rgb("+colorToSet+")}");
}

void Notepad::onAutoSave()
{
    if((autoSaveCheckBox->isChecked()) && (!fileName.isEmpty()))
    {
        QFile fich{fileName};
        if(fich.open(QIODevice::ReadWrite|QFile::Truncate))
        {
            auto labelFont = label->font();
            labelFont.setWeight(QFont::Normal);
            label->setFont(labelFont);
            QTextStream out{&fich};
            out << textEdit->toPlainText() << Qt::endl;
            isSaved = true;
        }
    }
}

Notepad::~Notepad()
{

}
