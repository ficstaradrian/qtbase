/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtCore/QtCore>
#include <QtTest/QtTest>

#include <math.h>

#ifdef Q_OS_SYMBIAN
# include <e32std.h>
typedef RMutex NativeMutexType;
void NativeMutexInitialize(NativeMutexType *mutex)
{
    mutex->CreateLocal();
}
void NativeMutexDestroy(NativeMutexType *mutex)
{
    mutex->Close();
}
void NativeMutexLock(NativeMutexType *mutex)
{
    mutex->Wait();
}
void NativeMutexUnlock(NativeMutexType *mutex)
{
    mutex->Signal();
}
#elif defined(Q_OS_UNIX)
#  include <pthread.h>
#  include <errno.h>
typedef pthread_mutex_t NativeMutexType;
void NativeMutexInitialize(NativeMutexType *mutex)
{
    pthread_mutex_init(mutex, NULL);
}
void NativeMutexDestroy(NativeMutexType *mutex)
{
    pthread_mutex_destroy(mutex);
}
void NativeMutexLock(NativeMutexType *mutex)
{
    pthread_mutex_lock(mutex);
}
void NativeMutexUnlock(NativeMutexType *mutex)
{
    pthread_mutex_unlock(mutex);
}
#elif defined(Q_OS_WIN)
#  define _WIN32_WINNT 0x0400
#  include <windows.h>
typedef CRITICAL_SECTION NativeMutexType;
void NativeMutexInitialize(NativeMutexType *mutex)
{
    InitializeCriticalSection(mutex);
}
void NativeMutexDestroy(NativeMutexType *mutex)
{
    DeleteCriticalSection(mutex);
}
void NativeMutexLock(NativeMutexType *mutex)
{
    EnterCriticalSection(mutex);
}
void NativeMutexUnlock(NativeMutexType *mutex)
{
    LeaveCriticalSection(mutex);
}
#endif

class tst_QMutex : public QObject
{
    Q_OBJECT

    int threadCount;

public:
    // barriers for the contended tests
    static QSemaphore semaphore1, semaphore2, semaphore3, semaphore4;

    tst_QMutex()
    {
        // at least 2 threads, even on single cpu/core machines
        threadCount = qMax(2, QThread::idealThreadCount());
        qDebug("thread count: %d", threadCount);
    }

private slots:
    void noThread_data();
    void noThread();

    void constructionNative();
    void uncontendedNative();
    void constructionQMutex();
    void uncontendedQMutex();
    void uncontendedQMutexLocker();

    void contendedNative_data();
    void contendedQMutex_data() { contendedNative_data(); }
    void contendedQMutexLocker_data() { contendedNative_data(); }

    void contendedNative();
    void contendedQMutex();
    void contendedQMutexLocker();
};

QSemaphore tst_QMutex::semaphore1;
QSemaphore tst_QMutex::semaphore2;
QSemaphore tst_QMutex::semaphore3;
QSemaphore tst_QMutex::semaphore4;

void tst_QMutex::noThread_data()
{
    QTest::addColumn<int>("t");

    QTest::newRow("noLock") << 1;
    QTest::newRow("QMutex") << 3;
    QTest::newRow("QMutexLocker") << 4;
}

void tst_QMutex::noThread()
{
    volatile int count = 0;
    const int N = 5000000;
    QMutex mtx;

    QFETCH(int, t);
    switch(t) {
        case 1:
            QBENCHMARK {
                count = 0;
                for (int i = 0; i < N; i++) {
                    count++;
                }
            }
            break;
        case 3:
            QBENCHMARK {
                count = 0;
                for (int i = 0; i < N; i++) {
                    mtx.lock();
                    count++;
                    mtx.unlock();
                }
            }
            break;
        case 4:
            QBENCHMARK {
                count = 0;
                for (int i = 0; i < N; i++) {
                    QMutexLocker locker(&mtx);
                    count++;
                }
            }
            break;
    }
    QCOMPARE(int(count), N);
}

void tst_QMutex::constructionNative()
{
    QBENCHMARK {
        NativeMutexType mutex;
        NativeMutexInitialize(&mutex);
        NativeMutexDestroy(&mutex);
    }
}

void tst_QMutex::uncontendedNative()
{
    NativeMutexType mutex;
    NativeMutexInitialize(&mutex);
    QBENCHMARK {
        NativeMutexLock(&mutex);
        NativeMutexUnlock(&mutex);
    }
    NativeMutexDestroy(&mutex);
}

void tst_QMutex::constructionQMutex()
{
    QBENCHMARK {
        QMutex mutex;
        Q_UNUSED(mutex);
    }
}

void tst_QMutex::uncontendedQMutex()
{
    QMutex mutex;
    QBENCHMARK {
        mutex.lock();
        mutex.unlock();
    }
}

void tst_QMutex::uncontendedQMutexLocker()
{
    QMutex mutex;
    QBENCHMARK {
        QMutexLocker locker(&mutex);
    }
}

void tst_QMutex::contendedNative_data()
{
    QTest::addColumn<int>("iterations");
    QTest::addColumn<int>("msleepDuration");
    QTest::addColumn<bool>("use2mutexes");

    QTest::newRow("baseline")               <<    0 <<  -1 << false;

    QTest::newRow("no msleep, 1 mutex")     << 1000 <<  -1 << false;
    QTest::newRow("no msleep, 2 mutexes")   << 1000 <<  -1 << true;
    QTest::newRow("msleep(0), 1 mutex")     << 1000 <<   0 << false;
    QTest::newRow("msleep(0), 2 mutexes")   << 1000 <<   0 << true;
    QTest::newRow("msleep(1), 1 mutex")     <<   10 <<   1 << false;
    QTest::newRow("msleep(1), 2 mutexes")   <<   10 <<   1 << true;
    QTest::newRow("msleep(2), 1 mutex")     <<   10 <<   2 << false;
    QTest::newRow("msleep(2), 2 mutexes")   <<   10 <<   2 << true;
    QTest::newRow("msleep(10), 1 mutex")    <<   10 <<  10 << false;
    QTest::newRow("msleep(10), 2 mutexes")  <<   10 <<  10 << true;
}

class NativeMutexThread : public QThread
{
    NativeMutexType *mutex1, *mutex2;
    int iterations, msleepDuration;
    bool use2mutexes;
public:
    bool done;
    NativeMutexThread(NativeMutexType *mutex1, NativeMutexType *mutex2, int iterations, int msleepDuration, bool use2mutexes)
        : mutex1(mutex1), mutex2(mutex2), iterations(iterations), msleepDuration(msleepDuration), use2mutexes(use2mutexes), done(false)
    { }
    void run() {
        forever {
            tst_QMutex::semaphore1.release();
            tst_QMutex::semaphore2.acquire();
            if (done)
                break;
            for (int i = 0; i < iterations; ++i) {
                NativeMutexLock(mutex1);
                if (use2mutexes)
                    NativeMutexLock(mutex2);
                if (msleepDuration >= 0)
                    msleep(msleepDuration);
                if (use2mutexes)
                    NativeMutexUnlock(mutex2);
                NativeMutexUnlock(mutex1);

                QThread::yieldCurrentThread();
            }
            tst_QMutex::semaphore3.release();
            tst_QMutex::semaphore4.acquire();
        }
    }
};

void tst_QMutex::contendedNative()
{
    QFETCH(int, iterations);
    QFETCH(int, msleepDuration);
    QFETCH(bool, use2mutexes);

    NativeMutexType mutex1, mutex2;
    NativeMutexInitialize(&mutex1);
    NativeMutexInitialize(&mutex2);

    QVector<NativeMutexThread *> threads(threadCount);
    for (int i = 0; i < threads.count(); ++i) {
        threads[i] = new NativeMutexThread(&mutex1, &mutex2, iterations, msleepDuration, use2mutexes);
        threads[i]->start();
    }

    QBENCHMARK {
        semaphore1.acquire(threadCount);
        semaphore2.release(threadCount);
        semaphore3.acquire(threadCount);
        semaphore4.release(threadCount);
    }

    for (int i = 0; i < threads.count(); ++i)
        threads[i]->done = true;
    semaphore1.acquire(threadCount);
    semaphore2.release(threadCount);
    for (int i = 0; i < threads.count(); ++i)
        threads[i]->wait();
    qDeleteAll(threads);

    NativeMutexDestroy(&mutex1);
    NativeMutexDestroy(&mutex2);
}

class QMutexThread : public QThread
{
    QMutex *mutex1, *mutex2;
    int iterations, msleepDuration;
    bool use2mutexes;
public:
    bool done;
    QMutexThread(QMutex *mutex1, QMutex *mutex2, int iterations, int msleepDuration, bool use2mutexes)
        : mutex1(mutex1), mutex2(mutex2), iterations(iterations), msleepDuration(msleepDuration), use2mutexes(use2mutexes), done(false)
    { }
    void run() {
        forever {
            tst_QMutex::semaphore1.release();
            tst_QMutex::semaphore2.acquire();
            if (done)
                break;
            for (int i = 0; i < iterations; ++i) {
                mutex1->lock();
                if (use2mutexes)
                    mutex2->lock();
                if (msleepDuration >= 0)
                    msleep(msleepDuration);
                if (use2mutexes)
                    mutex2->unlock();
                mutex1->unlock();

                QThread::yieldCurrentThread();
            }
            tst_QMutex::semaphore3.release();
            tst_QMutex::semaphore4.acquire();
        }
    }
};

void tst_QMutex::contendedQMutex()
{
    QFETCH(int, iterations);
    QFETCH(int, msleepDuration);
    QFETCH(bool, use2mutexes);

    QMutex mutex1, mutex2;

    QVector<QMutexThread *> threads(threadCount);
    for (int i = 0; i < threads.count(); ++i) {
        threads[i] = new QMutexThread(&mutex1, &mutex2, iterations, msleepDuration, use2mutexes);
        threads[i]->start();
    }

    QBENCHMARK {
        semaphore1.acquire(threadCount);
        semaphore2.release(threadCount);
        semaphore3.acquire(threadCount);
        semaphore4.release(threadCount);
    }

    for (int i = 0; i < threads.count(); ++i)
        threads[i]->done = true;
    semaphore1.acquire(threadCount);
    semaphore2.release(threadCount);
    for (int i = 0; i < threads.count(); ++i)
        threads[i]->wait();
    qDeleteAll(threads);
}

class QMutexLockerThread : public QThread
{
    QMutex *mutex1, *mutex2;
    int iterations, msleepDuration;
    bool use2mutexes;
public:
    bool done;
    QMutexLockerThread(QMutex *mutex1, QMutex *mutex2, int iterations, int msleepDuration, bool use2mutexes)
        : mutex1(mutex1), mutex2(mutex2), iterations(iterations), msleepDuration(msleepDuration), use2mutexes(use2mutexes), done(false)
    { }
    void run() {
        forever {
            tst_QMutex::semaphore1.release();
            tst_QMutex::semaphore2.acquire();
            if (done)
                break;
            for (int i = 0; i < iterations; ++i) {
                {
                    QMutexLocker locker1(mutex1);
                    QMutexLocker locker2(use2mutexes ? mutex2 : 0);
                    if (msleepDuration >= 0)
                        msleep(msleepDuration);
                }

                QThread::yieldCurrentThread();
            }
            tst_QMutex::semaphore3.release();
            tst_QMutex::semaphore4.acquire();
        }
    }
};

void tst_QMutex::contendedQMutexLocker()
{
    QFETCH(int, iterations);
    QFETCH(int, msleepDuration);
    QFETCH(bool, use2mutexes);

    QMutex mutex1, mutex2;

    QVector<QMutexLockerThread *> threads(threadCount);
    for (int i = 0; i < threads.count(); ++i) {
        threads[i] = new QMutexLockerThread(&mutex1, &mutex2, iterations, msleepDuration, use2mutexes);
        threads[i]->start();
    }

    QBENCHMARK {
        semaphore1.acquire(threadCount);
        semaphore2.release(threadCount);
        semaphore3.acquire(threadCount);
        semaphore4.release(threadCount);
    }

    for (int i = 0; i < threads.count(); ++i)
        threads[i]->done = true;
    semaphore1.acquire(threadCount);
    semaphore2.release(threadCount);
    for (int i = 0; i < threads.count(); ++i)
        threads[i]->wait();
    qDeleteAll(threads);
}

QTEST_MAIN(tst_QMutex)
#include "tst_qmutex.moc"
