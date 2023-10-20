/*
 * =====================================================================================
 *
 *       Filename:  ThreadedWorker.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/12/2023 04:25:51 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include <QObject>
#include <QThread>
#ifndef THREAD_WORKER_H
#define THREAD_WORKER_H 
class ThreadedWorker : public QObject
{
    Q_OBJECT
public:
    explicit ThreadedWorker(QObject *parent = 0);
signals:
    void finished(ThreadedWorker* worker);
    void resultReady(const QString &result);    
    void updateLedMatrix(void);
public slots:
    void doWork();
    
protected:
    QThread workerThread_;
};
#endif
