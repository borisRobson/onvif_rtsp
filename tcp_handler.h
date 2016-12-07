#ifndef TCP_HANDLER_H
#define TCP_HANDLER_H

#include <QThread>
#include <QObject>
#include <QProcess>

class tcp_handler : public QObject
{
    Q_OBJECT
public:
    tcp_handler(QObject *parent = 0);
    //void start();

public slots:
    void start();

signals:


private :

    QString process;
};

class tcp_listener : public QThread
{
    Q_OBJECT
public:
    tcp_listener(int ID = 0, QObject *parent = 0);
    void run();
private:
    QProcess *tcpProcess;
};

#endif // TCP_HANDLER_H
