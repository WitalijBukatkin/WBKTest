#include <QtWidgets>

class InputDialog : public QDialog {
    Q_OBJECT
private:
    QLineEdit* pUserName;
    QLineEdit* pUserGroup;

public:
    InputDialog(QWidget* pwgt = 0) : QDialog(pwgt)
    {
        pUserName = new QLineEdit;
        pUserGroup  = new QLineEdit;

        QPushButton* okButton     = new QPushButton("Пройти тест");
        QPushButton* editButton = new QPushButton("Редактировать");
        QPushButton* cancelButton = new QPushButton("Отмена");

        QGridLayout* ptopLayout = new QGridLayout;
        ptopLayout->addWidget(new QLabel("Ваше имя:"), 0, 0);
        ptopLayout->addWidget(new QLabel("Ваша группа"), 1, 0);
        ptopLayout->addWidget(pUserName, 0, 1);
        ptopLayout->addWidget(pUserGroup, 1, 1);
        ptopLayout->addWidget(okButton, 2, 0, 1, 0);
        ptopLayout->addWidget(cancelButton, 3, 0);
        ptopLayout->addWidget(editButton, 3, 1);
        setLayout(ptopLayout);

        connect(okButton, SIGNAL(clicked()), SLOT(okButtonClick()));
        connect(editButton,SIGNAL(clicked()), SLOT(editButtonClick()));
        connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
    }
    QString UserName() const
    {
        return pUserName->text();
    }
    QString UserGroup() const
    {
        return pUserGroup->text();
    }
private slots:
    void okButtonClick(){
        if(UserName().count()==0 || UserGroup().count()==0){
            QMessageBox::critical(0, "" , "Поля должны быть заполнены!");
            return;
        }
        this->accept();
    }
    void editButtonClick(){this->done(3); this->close();}
};
