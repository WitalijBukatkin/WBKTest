#include <QApplication>
#include <QMessageBox>
#include "wbktest.h"
#include "ngdialog.h"
#include "edittest.h"
#include "starttest.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug()<<a.arguments();

    if(argc>1)
    {
        edittest editwindow; wbktest testwindow;
        InputDialog* pInputDialog = new InputDialog;
        pInputDialog->exec();
        switch (pInputDialog->result())
        {
            case QDialog::Accepted:
            {
                testwindow.input=argv[0];
                if(testwindow.read()) testwindow.show();
                else QMessageBox::critical(0,"","Не могу открыть файл!");
                break;
            }
            case QDialog::Rejected:
                return 0;
            default:
                editwindow.input=argv[0];
                editwindow.read(); editwindow.show();
        }
        delete pInputDialog;
    }

    while(1)
    {
        edittest editwindow; wbktest testwindow;
        starttest* startwindow = new starttest;
        startwindow->exec();
        switch(startwindow->result())
        {
            case 2:
            {
                editwindow.input=startwindow->file;
                editwindow.read(); editwindow.show();
                break;
            }
            case 3:
            {
                InputDialog* pInputDialog = new InputDialog;
                pInputDialog->exec();
                switch (pInputDialog->result())
                {
                    case QDialog::Accepted:
                    {
                        testwindow.input=startwindow->file;
                        if(testwindow.read()) testwindow.show();
                        else QMessageBox::critical(0,"","Не могу открыть файл!");
                        break;
                    }
                    case QDialog::Rejected:
                        return 0;
                    default:
                        editwindow.input=startwindow->file;
                        editwindow.read(); editwindow.show();
                }
                delete pInputDialog;
                break;
            }
            default:
                return 0;
        }
        delete startwindow;
        a.exec();
    }
    return a.exec();
}
