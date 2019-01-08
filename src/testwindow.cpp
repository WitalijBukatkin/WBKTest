#include "allwindow.h"
#include <QDialog>

static QString input;

QWidget* testWindow;
QWidget* userDateWindow;
QLabel* labelFrage;
QTreeWidget* antwortWidget;

QLabel* timeProgress;
QLabel* progressRange;

QProgressBar* progressBar;

QLineEdit* nameLineEdit;
QLineEdit* groupLineEdit;

QList<Fragen> frage; Fragen t;

int userScore=0;
int userAntRect=0;
int frageScore=0;
QString usergroup="";
QString userName="";

bool sortfragen(const Fragen &s1, const Fragen &s2){
    if(!s1.frage.isEmpty())
        return (s1.frage<s2.frage)?qrand()%2:qrand()%2;
    else return false;
}

int readfile()
{
    qsrand (QDateTime::currentMSecsSinceEpoch());

    QFile file(input); QString fortfrage;
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0,"","Файл не удалось открыть");
        delete testWindow;
        startwindow();
        return 0;
    }
    testWindow->setWindowTitle("WBK Test - " + QString(QByteArray::fromBase64(file.readLine())));

    file.readLine();

    fortfrage=QByteArray::fromBase64(file.readLine()); //Генерация 1-го вопроса
    for(int num=0;!file.atEnd(); num++)
    {
        frage<<t;
        frage[num].frage=fortfrage;
        frage[num].antworten<<QByteArray::fromBase64(file.readLine()); //Чтение первого ответа

        for(int i=0;frage[num].antworten[i][0]=="-"||frage[num].antworten[i][0]=="+";i++)
            frage[num].antworten<<QByteArray::fromBase64(file.readLine()); // Пока - или + читаем ответы

        fortfrage=frage[num].antworten[frage[num].antworten.count()-1]; //Вытаскиваем след. вопрос
        frage[num].antworten.removeAt(frage[num].antworten.count()-1); //Удаляем вопрос из списка ответов

        frage[num].sort();

        for(int i=0;i<frage[num].antworten.count();i++){ //Поиск правильных ответов
            if(frage[num].antworten[i][0]=="+") frage[num].antRicht<<i;
            frage[num].antworten[i].remove(0,1);
        }
    }

    std::sort(frage.begin(), frage.end(), sortfragen);

    return 1;
}

static void clickExitButton()
{
    QMessageBox* MB=new QMessageBox(QMessageBox::Information,
            "", "Выйти?",
            QMessageBox::Yes | QMessageBox::No);
    int n=MB->exec(); delete MB;
    if(n==QMessageBox::Yes) {
        delete testWindow;
        startwindow();
    }
}

void clickNextButtonUserDate()
{
    if(nameLineEdit->text().count()==0 || groupLineEdit->text().count()==0){
        QMessageBox::warning(0, "" , "Поля должны быть заполнены!");
        return;
    }
    userName=nameLineEdit->text();
    usergroup=groupLineEdit->text();
    delete userDateWindow;
    testWindow->show();
}

void clickEditButton()
{
    delete userDateWindow;
    edittestwindow(input);
}

void userDate()
{
    userDateWindow=new QWidget;
    userDateWindow->setMinimumSize(QSize(400,220));
    userDateWindow->setMaximumSize(QSize(400,220));
    userDateWindow->setPalette(Qt::white);

    QVBoxLayout* alleLayout = new QVBoxLayout;

    nameLineEdit = new QLineEdit;
    groupLineEdit  = new QLineEdit;

    alleLayout->addWidget(new QLabel("Ваше имя:"));
    alleLayout->addWidget(nameLineEdit);
    alleLayout->addWidget(new QLabel("Ваша группа:"));
    alleLayout->addWidget(groupLineEdit);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
        QPushButton* exitButton= new QPushButton("Выход");
        QPushButton* nextButton = new QPushButton("Продолжить");
        QPushButton* editButton = new QPushButton("Редактировать");
        buttonLayout->addWidget(editButton);
        buttonLayout->addWidget(exitButton);
    alleLayout->addWidget(nextButton);
    alleLayout->addLayout(buttonLayout);

    userDateWindow->setWindowFlag(Qt::FramelessWindowHint);

    QObject::connect(exitButton, &QPushButton::clicked, clickExitButton);
    QObject::connect(editButton, &QPushButton::clicked, clickEditButton);
    QObject::connect(nextButton, &QPushButton::clicked, clickNextButtonUserDate);

    userDateWindow->setLayout(alleLayout);
    userDateWindow->show();
}

void testExit()
{
    QMessageBox::warning(0, "Результаты теста" ,
                         QString("Верно: ")+QString::number(userScore)+QString("/")+QString::number(frage.count()));
    QFile file(QDir::homePath()+"/Documents/result.txt");

    if(file.open(QIODevice::Append | QIODevice::Text)){
        QTextStream writeStream(&file);
        writeStream<<"Name: "<<userName<<" | Group: "<<usergroup
                   <<" | "<<QString("True: ")+QString::number(userScore)+QString("/")+QString::number(frage.count())+"\n";
    }

    delete testWindow;
    startwindow();
}

static void clickNextButton()
{ static bool c=0;

    if(c==1){
        bool z=1;
        for(int i=0;i<frage[frageScore].antRicht.count(); i++)
          if(antwortWidget->topLevelItem(frage[frageScore].antRicht[i])->checkState(0)==Qt::Unchecked) {z=0;break;}
        if(z==1) userScore++;
        if(frageScore<frage.count()-2) frageScore++; else {testExit();return;}
    }

    labelFrage->setText(frage[frageScore].frage);
    antwortWidget->clear();
    for(int j=0;j<frage[frageScore].antworten.count();j++)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem;
        item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsSelectable);
        item->setCheckState(0, Qt::Unchecked);
        item->setText(0,frage[frageScore].antworten[j]);
        antwortWidget->addTopLevelItem(item);
    }

    progressRange->setText(QString("   Вопрос: ")+QString::number(frageScore+1)+QString("/")+QString::number(frage.count()));
    progressBar->setValue((100/frage.count())*(frageScore));

    c=1;
}

void testwindow(QString src)
{
    input=src;

    testWindow=new QWidget;
    testWindow->setWindowIcon(QIcon(":/images/logo.png"));
    testWindow->setMinimumSize(QSize(700,600));
    testWindow->setMaximumSize(QSize(700,600));
    testWindow->setPalette(Qt::white);

    labelFrage=new QLabel;
    labelFrage->setFixedSize(700,100);

    antwortWidget = new QTreeWidget;
    antwortWidget->setHeaderHidden(1);

    progressRange=new QLabel;

    QPushButton* buttonNext=new QPushButton("Продолжить");
    QPushButton* buttonExit=new QPushButton("Выход");

    progressBar=new QProgressBar;

    QVBoxLayout* mainLayout=new QVBoxLayout;
    mainLayout->addWidget(labelFrage);
    mainLayout->addWidget(antwortWidget);
    mainLayout->addWidget(progressBar);
    QHBoxLayout* indicationLayout=new QHBoxLayout;
        indicationLayout->addWidget(progressRange);
        indicationLayout->addWidget(buttonExit);
        indicationLayout->addWidget(buttonNext);
    mainLayout->addLayout(indicationLayout);
    testWindow->setLayout(mainLayout);

    if(!readfile()) return;
    clickNextButton();

    userDate();

    QObject::connect(buttonExit, &QPushButton::clicked, clickExitButton);
    QObject::connect(buttonNext, &QPushButton::clicked, clickNextButton);
}
