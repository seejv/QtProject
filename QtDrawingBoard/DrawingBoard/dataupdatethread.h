#ifndef DATAUPDATETHREAD_H
#define DATAUPDATETHREAD_H

#include <QObject>
#include <QThread>
#include <QPixmap>
#include <QVector>

class DataUpdateThread : public QObject
{
    Q_OBJECT
public:
    explicit DataUpdateThread(QObject *parent = nullptr);



signals:


public slots:
    void onUpdateData(QPixmap *pix, QVector<QLine> lines);

};

#endif // DATAUPDATETHREAD_H
