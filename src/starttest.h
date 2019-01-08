#include <QtWidgets>

class starttest : public QDialog {
    Q_OBJECT
public:
    QString file;
    starttest(QWidget* pwgt = 0) : QDialog(pwgt)
    {
        setWindowTitle("WBK Test - Начало");
        setMinimumSize(QSize(500,400));
        setMaximumSize(QSize(500,400));

        QPushButton* Button_Neu=new QPushButton("Новый");
        Button_Neu->setFixedSize(170, 34);
        QPushButton* Button_Open=new QPushButton("Открыть");
        Button_Open->setFixedSize(170,34);

        QLabel* LabelMitLogo=new QLabel;
        LabelMitLogo->setPixmap(QPixmap(":/images/logo.png").scaled(500, 400));
        QHBoxLayout* layoutH = new QHBoxLayout;
        QVBoxLayout* layoutV = new QVBoxLayout;
        layoutH->addWidget(Button_Neu);
        layoutH->addWidget(Button_Open);
        layoutV->addWidget(LabelMitLogo);
        layoutV->addLayout(layoutH);
        setLayout(layoutV);

        QObject::connect(Button_Neu, SIGNAL(clicked()), SLOT(clicked_Botton_Neu()));
        QObject::connect(Button_Open, SIGNAL(clicked()), SLOT(clicked_Botton_Open()));
    }
private slots:
    void clicked_Botton_Neu()
    {
        file=QFileDialog::getSaveFileName(0, "Сохранение файла теста",QDir::homePath()+"/Documents"+"/neutest","*.wst");
        if(file.isEmpty()) {QMessageBox::information(0, "", "He выбрано место сохранения файла!"); return;}
        if(QFile::exists(file)) QFile::remove(file);
        done(2);
    }

    void clicked_Botton_Open()
    {
        file=QFileDialog::getOpenFileName(0, "Открытие файла теста", QDir::homePath()+"/Documents", "*.wst");
        if(file.isEmpty()) {QMessageBox::information(0, "", "He выбран файл!");return;}
        done(3);
    }
};
