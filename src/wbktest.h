#include <QtWidgets>
#include "fragen.h"

class wbktest : public QWidget
{
    Q_OBJECT
    QLabel* labelFrage;
    QTreeWidget* antwortWidget;
    QLabel* progressRange;
    QProgressBar* progressBar;
    bool c=0; int userScore=0, frageScore=0;
    QList<Fragen> frage; Fragen t;

public:
    QString input, username, usergroup;

    wbktest(QWidget* pwgt=0) : QWidget(pwgt)
    {
        setMinimumSize(QSize(700,600));
        setMaximumSize(QSize(700,600));

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
        setLayout(mainLayout);

        QObject::connect(buttonExit, SIGNAL(clicked()), SLOT(clickExitButton()));
        QObject::connect(buttonNext, SIGNAL(clicked()), SLOT(clickNextButton()));
    }

    bool read()
    {
        qsrand (QDateTime::currentMSecsSinceEpoch());

        QFile file(input);
        if(!file.open(QIODevice::ReadOnly)) return 0;

        QDataStream in(&file); QString string;

        in>>string; setWindowTitle("WBK Test - " + string); in>>string;

        for(int num=0;!in.atEnd();num++)
        {
            frage<<t;

            in>>frage[num].frage;
            in>>frage[num].antworten;

            frage[num].sort();

            for(int i=0;i<frage[num].antworten.count();i++){
                if(frage[num].antworten[i][0]=="+") frage[num].antRicht<<i;
                frage[num].antworten[i].remove(0,1);
            }
        }
        std::sort(frage.begin(), frage.end(), sortfragen);
        clickNextButton();
        return 1;
    }

private:
    static bool sortfragen(const Fragen &s1, const Fragen &s2){
        if(!s1.frage.isEmpty())
            return (s1.frage<s2.frage)?qrand()%2:qrand()%2;
        else return false;
    }
    void testExit()
    {
        QMessageBox::about(0, "Результаты теста" , QString("<h1><u>Оценка <i>"+
                             QString::number((userScore*10/frage.count()==10)?5:
                             (userScore*10/frage.count()>=7)?4:
                             (userScore*10/frage.count()>=5)?3:2)+"<i></u></h1>")+
                             QString("Верно: ")+QString::number(userScore)+QString("/")+QString::number(frage.count()));

        QFile file(QDir::homePath()+"/Documents/result.txt");
        if(file.open(QIODevice::Append | QIODevice::Text)){
            QTextStream writeStream(&file);
            writeStream<<"Name: "<<username<<" | Group: "<<usergroup
                       <<" | "<<QString("True: ")+QString::number(userScore)+QString("/")+QString::number(frage.count())+"\n";
        }
        close();
    }
private slots:
    void clickExitButton()
    {
        QMessageBox* MB=new QMessageBox(QMessageBox::Information,
                "", "Выйти?",
                QMessageBox::Yes | QMessageBox::No);
        int n=MB->exec(); delete MB;
        if(n==QMessageBox::Yes) {
            close();
        }
    }
    void clickNextButton()
    {
        if(c==1){
            bool z=1;
            for(int i=0;i<frage[frageScore].antRicht.count(); i++)
              if(antwortWidget->topLevelItem(frage[frageScore].antRicht[i])->checkState(0)==Qt::Unchecked) {z=0;break;}
            if(z==1) userScore++;
            if(frageScore<frage.count()-1) frageScore++; else testExit();
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
};
