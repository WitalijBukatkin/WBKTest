#pragma once

class Fragen{
    public:
        QString frage;
        QStringList antworten;
        QList<int> antRicht; //Верные ответы

        void sort()
        {
            std::sort(antworten.begin(),antworten.end(),sortrand);
        }

    private:
        static bool sortrand(const QString &s1, const QString &s2){
             return (s1>s2)?qrand()%2:qrand()%2;
        }
};
