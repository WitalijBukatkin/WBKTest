#include "allwindow.h"
#include <QCryptographicHash>

QWidget* editWindow;
QListWidget* ListFragen;
QTreeWidget* ListAntworten;
QLineEdit* nameEdit;
static QList<Fragen> frage;
static Fragen t;
QString passwd;
static QString input;
bool neu=0;

void addItemListFragen()
{
    QListWidgetItem* item = new QListWidgetItem(ListFragen);
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

void сhangedItemFragen()
{
    frage[ListFragen->currentRow()].frage=ListFragen->currentItem()->text();
}

void сhangedItemAntworten()
{
    frage[ListFragen->currentRow()].antworten[ListAntworten->currentIndex().row()]=ListAntworten->currentItem()->text(0);
    frage[ListFragen->currentRow()].antRicht.clear();
    for(int i=0;i<frage[ListFragen->currentRow()].antworten.count();i++)
       if(ListAntworten->topLevelItem(i)->checkState(0)==Qt::Checked) frage[ListFragen->currentRow()].antRicht<<i;
}

int opentest()
{
    bool bOk;
    passwd = QInputDialog::getText(0, "", "Введите пароль:",
                                    QLineEdit::Password, "", &bOk);
    if(!bOk) return 0;

    QFile file(input); QString fortfrage;
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0,"","Файл не удалось открыть");
        delete editWindow;
        startwindow();
        return 0;
    }
    nameEdit->setText(QString(QByteArray::fromBase64(file.readLine())));

    if(QByteArray::fromBase64(file.readLine())!=QCryptographicHash::hash(passwd.toUtf8(),QCryptographicHash::Md5))
    {
        QMessageBox::information(0,"","Пароль не верный!");
        file.close();
        return 0;
    }

    fortfrage=QByteArray::fromBase64(file.readLine()); //Генерация 1-го вопроса
    for(int num=0;!file.atEnd(); num++){
        frage<<t;

        frage[num].frage=fortfrage; ListFragen->addItem(frage[num].frage);
        frage[num].antworten<<QByteArray::fromBase64(file.readLine()); //Чтение первого ответа

        for(int i=0;frage[num].antworten[i][0]=="-"||frage[num].antworten[i][0]=="+";i++)
            frage[num].antworten<<QByteArray::fromBase64(file.readLine()); // Пока - или + читаем ответы

        fortfrage=frage[num].antworten[frage[num].antworten.count()-1]; //Вытаскиваем след. вопрос
        frage[num].antworten.removeAt(frage[num].antworten.count()-1); //Удаляем вопрос из списка ответов

        for(int i=0;i<frage[num].antworten.count();i++){ //Поиск правильных ответов
            if(frage[num].antworten[i][0]=="+") frage[num].antRicht<<i;
            frage[num].antworten[i].remove(0,1);
        }
    }
    ListFragen->setCurrentRow(ListFragen->count()-1);
    return 1;
}

void clickNextButton()
{
    QFile file(input);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(0,"","Файл не удалось открыть");
        delete editWindow;
        return;
    }
    if(nameEdit->text().count()!=0)
        file.write(nameEdit->text().toUtf8().toBase64()+"\n");
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

    file.write(QCryptographicHash::hash(passwd.toUtf8(),QCryptographicHash::Md5).toBase64()+"\n");
    for(int i=0; i<frage.count(); i++)
    {
        file.write(frage[i].frage.toUtf8().toBase64()+"\n");
        for(int j=0;j<frage[i].antworten.count();j++)
        {
            bool s=0;
            for(int si=0; si<frage[i].antRicht.count(); si++)
               if(frage[i].antRicht[si]==j) {s=1;break;}
            frage[i].antworten[j]=QString((s)?"+":"-")+frage[i].antworten[j];
            file.write(QString(frage[i].antworten[j]).toUtf8().toBase64()+"\n");
        }
    }
    file.close();
    QMessageBox::information(0,"","Тест сохранен!");
    delete editWindow;
    startwindow();
}

void clickExitButton()
{
    QMessageBox* MB=new QMessageBox(QMessageBox::Information,
            "", "Сохранить изменения?",
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    int n=MB->exec(); delete MB;
    if(n==QMessageBox::Yes) {
        clickNextButton();
    }
    if(n==QMessageBox::No) {
        delete editWindow;
        startwindow();
    }
}

void edittestwindow(QString src){
    input=src;
    QStringList split=input.split("/");
    QString filename=split.at(split.count()-1);

    if(!QFile::exists(input)) neu=1;

    editWindow=new QWidget;
    editWindow->setWindowTitle("WBK Test - Редактор тестов - "+filename);
    editWindow->setWindowIcon(QIcon(":/images/logo.png"));
    editWindow->setMinimumSize(QSize(700,600));
    editWindow->setMaximumSize(QSize(700,600));
    editWindow->setPalette(Qt::white);

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
                if(neu) addItemListFragen();
                else if(!opentest()) {qDebug()<<"H";return;}
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
                if(neu)
                    addItemListAntworten();
                else
                    selectItemFragen();

            LayoutAntworte->addWidget(ListAntworten);
                    QHBoxLayout* LayoutButton = new QHBoxLayout;
                    QPushButton* Button_Exit=new QPushButton("Выход");
                    QPushButton* Button_Next=new QPushButton("Продолжить");
                LayoutButton->addWidget(Button_Exit);
                LayoutButton->addWidget(Button_Next);
            LayoutAntworte->addLayout(LayoutButton);

        LayoutTable->addLayout(LayoutAntworte);

    LayoutAll->addLayout(LayoutTable);

    editWindow->setLayout(LayoutAll);

    QObject::connect(Button_NeuFrage, &QPushButton::clicked, addItemListFragen);
    QObject::connect(Button_DelFrage, &QPushButton::clicked, delItemListFragen);
    QObject::connect(Button_NeuAntwort, &QPushButton::clicked, addItemListAntworten);
    QObject::connect(Button_DelAntwort, &QPushButton::clicked, delItemListAntworten);


    QObject::connect(ListFragen, &QListWidget::itemSelectionChanged, selectItemFragen);
    QObject::connect(ListFragen, &QListWidget::itemChanged, сhangedItemFragen);

    QObject::connect(ListAntworten, &QTreeWidget::itemChanged, сhangedItemAntworten);

    QObject::connect(Button_Next, &QPushButton::clicked, clickNextButton);
    QObject::connect(Button_Exit, &QPushButton::clicked, clickExitButton);

    editWindow->show();
}
