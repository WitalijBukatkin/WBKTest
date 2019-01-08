#include <QtWidgets>
#include "fragen.h"

class edittest : public QWidget
{
    Q_OBJECT
    QString passwd="";
    bool neu;
    QListWidget* ListFragen;
    QTreeWidget* ListAntworten;
    QLineEdit* nameEdit;
    QList<Fragen> frage;
    Fragen t;

public:
    QString input;

    edittest(QWidget* pwgt=0) : QWidget(pwgt)
    {
        setWindowTitle("WBK Test - Редактор тестов - "+QFileInfo(input).baseName());
        setMinimumSize(QSize(700,600));
        setMaximumSize(QSize(700,600));

        QVBoxLayout* LayoutAll = new QVBoxLayout; // Вся форма
            QHBoxLayout* LayoutName=new QHBoxLayout;
                LayoutName->addWidget(new QLabel("Название теста:"));
                nameEdit = new QLineEdit("Новый тест");
                LayoutName->addWidget(nameEdit);
            LayoutAll->addLayout(LayoutName);
            QHBoxLayout* LayoutTable=new QHBoxLayout;
                QVBoxLayout* LayoutFragen = new QVBoxLayout;
                    QHBoxLayout* LayoutFragenTitle = new QHBoxLayout;
                        LayoutFragenTitle->addWidget(new QLabel("Список вопросов:"));
                        QPushButton* Button_NeuFrage=new QPushButton("+");
                        QPushButton* Button_DelFrage=new QPushButton("-");
                        LayoutFragenTitle->addWidget(Button_DelFrage);
                        LayoutFragenTitle->addWidget(Button_NeuFrage);
                    LayoutFragen->addLayout(LayoutFragenTitle);
                    ListFragen=new QListWidget(); //Список вопросов
                    LayoutFragen->addWidget(ListFragen);
                LayoutTable->addLayout(LayoutFragen);
                QVBoxLayout* LayoutAntworte = new QVBoxLayout;
                    QHBoxLayout* LayoutAntworteTitle = new QHBoxLayout;
                        QPushButton* Button_NeuAntwort=new QPushButton("+");
                        QPushButton* Button_DelAntwort=new QPushButton("-");
                    LayoutAntworteTitle->addWidget(new QLabel("Список ответов:"));
                    LayoutAntworteTitle->addWidget(Button_DelAntwort);
                    LayoutAntworteTitle->addWidget(Button_NeuAntwort);
                LayoutAntworte->addLayout(LayoutAntworteTitle);
                    ListAntworten=new QTreeWidget(); //Список ответов
                    ListAntworten->setHeaderLabel("True - верно / False - Неверно");
                LayoutAntworte->addWidget(ListAntworten);
                        QHBoxLayout* LayoutButton = new QHBoxLayout;
                        QPushButton* Button_Exit=new QPushButton("Выход");
                        QPushButton* Button_Next=new QPushButton("Продолжить");
                    LayoutButton->addWidget(Button_Exit);
                    LayoutButton->addWidget(Button_Next);
                LayoutAntworte->addLayout(LayoutButton);
            LayoutTable->addLayout(LayoutAntworte);
        LayoutAll->addLayout(LayoutTable);
        setLayout(LayoutAll);

        QObject::connect(Button_NeuFrage, SIGNAL(clicked()), SLOT(addItemListFragen()));
        QObject::connect(Button_DelFrage, SIGNAL(clicked()), SLOT(delItemListFragen()));
        QObject::connect(Button_NeuAntwort, SIGNAL(clicked()), SLOT(addItemListAntworten()));
        QObject::connect(Button_DelAntwort, SIGNAL(clicked()), SLOT(delItemListAntworten()));

        QObject::connect(ListFragen, SIGNAL(itemSelectionChanged()), SLOT(selectItemFragen()));
        QObject::connect(ListFragen, SIGNAL(itemChanged(QListWidgetItem*)), SLOT(changedItemFragen()));
        QObject::connect(ListAntworten, SIGNAL(itemChanged(QTreeWidgetItem*,int)), SLOT(changedItemAntworten()));

        QObject::connect(Button_Next, SIGNAL(clicked()), SLOT(clickNextButton()));
        QObject::connect(Button_Exit, SIGNAL(clicked()), SLOT(clickExitButton()));
    }

    void read()
    {
        if(!QFile::exists(input)) {neu=1;addItemListFragen();return;}

        bool bOk; passwd = QInputDialog::getText(0, "", "Введите пароль:",
                                                QLineEdit::Password, "", &bOk);
        if(!bOk) {neu=1; addItemListFragen(); return;}

        QFile file(input);

        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::information(0,"","Файл не удалось открыть\nСоздаю новый файл...");
            neu=1; addItemListFragen(); return;
        }

        QDataStream in(&file); QString string;

        in>>string; nameEdit->setText(string);

        in>>string; if(string.toUtf8()!=QCryptographicHash::hash(passwd.toUtf8(),QCryptographicHash::Md5).toHex())
        {
            QMessageBox::critical(0,"","Пароль не верный!\nСоздаю новый файл...");
            file.close();
            neu=1; addItemListFragen(); return;
        }

        for(int num=0;!in.atEnd();num++)
        {
            frage<<t;

            in>>frage[num].frage;
            QListWidgetItem* item=new QListWidgetItem;
            item->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled|Qt::ItemIsSelectable);
            item->setText(frage[num].frage);
            ListFragen->addItem(item);

            in>>frage[num].antworten;
            for(int i=0;i<frage[num].antworten.count();i++){
                if(frage[num].antworten[i][0]=="+") frage[num].antRicht<<i;
                frage[num].antworten[i].remove(0,1);
            }
        }
        ListFragen->setCurrentRow(ListFragen->count()-1);
        selectItemFragen();
    }
private slots:
    void addItemListFragen()
    {
        QListWidgetItem* item = new QListWidgetItem;
        item->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        item->setText("Новый вопрос");
        ListFragen->addItem(item); frage<<t;
        ListFragen->setCurrentItem(item);
        frage[ListFragen->currentRow()].frage="Новый вопрос";
    }

    void delItemListFragen()
    {
        if(ListFragen->count()!=1)
        {
            int row=ListFragen->currentRow()-1;
            if(row==-1) row=0;
            frage.removeAt(ListFragen->currentRow());
            delete ListFragen->currentItem();
            ListFragen->setCurrentRow(row);
        }
    }

    void addItemListAntworten()
    {
        QTreeWidgetItem* item = new QTreeWidgetItem;
        item->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsSelectable);
        item->setCheckState(0,Qt::Unchecked);
        item->setText(0,"Новый ответ");
        ListAntworten->addTopLevelItem(item);
        ListAntworten->setCurrentItem(item);
        frage[ListFragen->currentRow()].antworten<<"Новый oтвет";
    }

    void delItemListAntworten()
    {
        frage[ListFragen->currentRow()].antworten.removeAt(ListAntworten->currentIndex().row());
        delete ListAntworten->currentItem();
    }

    void selectItemFragen()
    {
        ListAntworten->clear();
        for(int j=0;j<frage[ListFragen->currentRow()].antworten.count();j++)
        {
            QTreeWidgetItem* item = new QTreeWidgetItem;
            item->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsSelectable);
            bool s=0;
            for(int si=0; si<frage[ListFragen->currentRow()].antRicht.count(); si++)
                if(frage[ListFragen->currentRow()].antRicht[si]==j) {s=1;break;}
            item->setCheckState(0,(s==0)?Qt::Unchecked:Qt::Checked);
            item->setText(0,frage[ListFragen->currentRow()].antworten[j]);
            ListAntworten->addTopLevelItem(item);
            ListAntworten->setCurrentItem(item);
        }
    }

    void changedItemFragen()
    {
        frage[ListFragen->currentRow()].frage=ListFragen->currentItem()->text();
    }

    void changedItemAntworten()
    {
        frage[ListFragen->currentRow()].antworten[ListAntworten->currentIndex().row()]=ListAntworten->currentItem()->text(0);
        frage[ListFragen->currentRow()].antRicht.clear();
        for(int i=0;i<frage[ListFragen->currentRow()].antworten.count();i++)
           if(ListAntworten->topLevelItem(i)->checkState(0)==Qt::Checked) frage[ListFragen->currentRow()].antRicht<<i;
    }

    void clickNextButton()
    {
        QFile file(input);
        if(!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::critical(0,"","Файл не удалось открыть");
            close();
        }

        QDataStream out(&file);

        if(nameEdit->text().count()!=0)
            out<<nameEdit->text();
        else
        {
            QMessageBox::information(0,"","Введите имя теста!");
            return;
        }
        if(neu==1)
        {
            bool bOk; passwd = QInputDialog::getText(0, "", "Введите пароль:",
                                        QLineEdit::Password, "", &bOk);
            if(!bOk){
                passwd="";
                QMessageBox::information(0,"","Пустой пароль!");
            }
        }

        out<<QString(QCryptographicHash::hash(passwd.toUtf8(),QCryptographicHash::Md5).toHex());

        for(int i=0; i<frage.count(); i++)
        {
            for(int j=0;j<frage[i].antworten.count();j++)
            {
                bool s=0;
                for(int si=0; si<frage[i].antRicht.count(); si++)
                   if(frage[i].antRicht[si]==j) {s=1;break;}
                frage[i].antworten[j]=QString((s)?"+":"-")+frage[i].antworten[j];
            }
            out<<frage[i].frage;
            out<<frage[i].antworten;
        }
        file.close();
        QMessageBox::information(0,"","Тест сохранен!");
        close();
    }

    void clickExitButton()
    {
        QMessageBox* MB=new QMessageBox(QMessageBox::Information,
                "", "Сохранить изменения?",
                QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        int n=MB->exec(); delete MB;
        if(n==QMessageBox::Yes)
            clickNextButton();
        if(n==QMessageBox::No)
            close();
    }
};
