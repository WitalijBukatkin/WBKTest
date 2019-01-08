#include "allwindow.h"
static QString input;

QWidget* window;

void clicked_Botton_Neu()
{
    input=QFileDialog::getSaveFileName(0, "Сохранение файла теста",QDir::homePath()+"/Documents"+"/neutest","*.wst");
    if(input.isEmpty()) {QMessageBox::information(0, "", "He выбрано место сохранения файла!"); return;}
    delete window;
    if(QFile::exists(input)) QFile::remove(input);
    edittestwindow(input);
}

void clicked_Botton_Open()
{
    input=QFileDialog::getOpenFileName(0, "Открытие файла теста", QDir::homePath()+"/Documents", "*.wst");
    if(input.isEmpty()) {QMessageBox::information(0, "", "He выбран файл!");return;}
    delete window;
    testwindow(input);
}

void startwindow(){
    input="";
    window=new QWidget;
    window->setWindowTitle("WBK Test - Начало");
    window->setWindowIcon(QIcon(":/images/logo.png"));
    window->setMinimumSize(QSize(600,450));
    window->setMaximumSize(QSize(600,450));
    window->setPalette(Qt::white);

    QPushButton* Button_Neu=new QPushButton("Новый");
    Button_Neu->setFixedSize(230, 45);
    QPushButton* Button_Open=new QPushButton("Открыть");
    Button_Open->setFixedSize(230,45);

    QLabel* LabelMitLogo=new QLabel;
    LabelMitLogo->setPixmap(QPixmap(":/images/logo.png").scaled(600, 400));

    QHBoxLayout* layoutH = new QHBoxLayout;
    QVBoxLayout* layoutV = new QVBoxLayout;
    layoutH->addWidget(Button_Neu);
    layoutH->addWidget(Button_Open);
    layoutV->addWidget(LabelMitLogo);
    layoutV->addLayout(layoutH);

    window->setLayout(layoutV);
    window->show();

    QObject::connect(Button_Neu, &QPushButton::clicked, clicked_Botton_Neu);
    QObject::connect(Button_Open, &QPushButton::clicked, clicked_Botton_Open);
}
