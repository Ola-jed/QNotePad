#include "notepad.hpp"
#include "ui_notepad.h"

Notepad::Notepad(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Notepad)
{
    ui->setupUi(this);
    connect(ui->quit,&QPushButton::clicked,this,&Notepad::onQuit);
    connect(ui->newFile,&QPushButton::clicked,this,&Notepad::onNewFile);
    connect(ui->openFile,&QPushButton::clicked,this,&Notepad::onOpenFile);
    connect(ui->saveFile,&QPushButton::clicked,this,&Notepad::onSaveFile);
    connect(ui->textEdit,&QTextEdit::textChanged,this,&Notepad::onTextChanged);
    connect(ui->textEdit,&QTextEdit::textChanged,this,&Notepad::onAutoSave);
    connect(ui->colorText,&QPushButton::clicked,this,&Notepad::onColorChanged);
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
        }
        else
        {
            QMessageBox::information(this,"Nouveau Fichier","Le fichier a bien été créé");
            ui->label->setText(fileName);
            fichier.close();
        }
    }
}

void Notepad::onOpenFile()
{
    fileName = QFileDialog::getOpenFileName(this);
    if(fileName.isEmpty())
    {
        QMessageBox::warning(this,"Nouveau Fichier","Choisir fichier valide");
    }
    else
    {
        QFile fichier{fileName};
        if((!fichier.open(QIODevice::ReadWrite)))
        {
            QMessageBox::critical(this,"Nouveau Fichier","Impossible d'ouvrir le fichier");
        }
        else
        {
            ui->label->setText(fileName);
            ui->textEdit->setText(" ");
            // Reading the file line by line and storing int the textEdit.
            QTextStream in{&fichier};
            QString tempLine;
            while(!in.atEnd())
            {
                tempLine = in.readLine();
                ui->textEdit->append(tempLine);
            }
        }
        fichier.close();
    }
}

void Notepad::onSaveFile()
{
    if(fileName.isEmpty()) // If this is a new file
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
            }
            else
            {
                ui->label->setText(fileName);
                fichier.close();
            }
        }
    }
    QFile fich{fileName};
    if(fich.open(QIODevice::ReadWrite|QFile::Truncate))
    {
        auto labelFont = ui->label->font();
        labelFont.setWeight(QFont::Normal);
        ui->label->setFont(labelFont);
        QTextStream out{&fich};
        out << ui->textEdit->toPlainText() << Qt::endl;
        fich.close();
        isSaved = true;
        QMessageBox::information(this,"Sauvegarde","Sauvegarde Réussie");
    }
    else
    {
        QMessageBox::critical(this,"Sauvegarde","Impossible de sauvegarder");
    }
}

void Notepad::onQuit()
{
    if(((!fileName.isEmpty()) && !isSaved) || (fileName.isEmpty() && !(ui->textEdit->toPlainText().isEmpty())))
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
    auto labelFont = ui->label->font();
    labelFont.setWeight(99);
    ui->label->setFont(labelFont);
}

void Notepad::onColorChanged() // Get the color and set the color in the textEdit.
{
    QColor chosenColor = QColorDialog::getColor("Choisir une couleur");
    QString colorToSet = QString::number(chosenColor.red())+","+QString::number(chosenColor.green())+","+QString::number(chosenColor.blue());
    setStyleSheet("QTextEdit{color:rgb("+colorToSet+")}");
}

void Notepad::onAutoSave()
{
    if((ui->autoSaveCheckBox->isChecked()) && (!fileName.isEmpty()))
    {
        QFile fich{fileName};
        if(fich.open(QIODevice::ReadWrite|QFile::Truncate))
        {
            auto labelFont = ui->label->font();
            labelFont.setWeight(QFont::Normal);
            ui->label->setFont(labelFont);
            QTextStream out{&fich};
            out << ui->textEdit->toPlainText() << Qt::endl;
            isSaved = true;
        }
        fich.close();
    }
}

Notepad::~Notepad()
{
    delete ui;
}
