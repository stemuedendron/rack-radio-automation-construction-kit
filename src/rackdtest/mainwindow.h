/*
    Copyright (C) 2011, Steffen Müller.
    All rights reserved.

    This file is part of r.a.c.k. radio automation construction kit

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Author: Steffen Müller
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QBasicTimer>

class RackdClient;
class QSpinBox;
class QTextEdit;
class QLabel;
class RPeakMeter;
class QGraphicsScene;
class QGraphicsView;
class QFrame;


class MainWindow : public QWidget
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);



protected:

    void resizeEvent(QResizeEvent *);

private slots:

    void togglePeakmeter();

    void connectToServer();
    void sendPass();
    void meterEnable(bool ok);
    void loadStream();
    void waveForm();

    void waveForm1();

    void play();
    void stop();
    void unloadStream();
    void dropConnection();
    void setPosition(int pos);

    //slots for response signals:
    void passWordOK(bool ok);
    void streamLoaded(quint32 handle, quint32 time);
    void position(quint8 device, quint32 handle, quint32 position, quint16 leftLevel, quint16 rightLevel);
    void waveFormGenerated(quint32 handle, QImage waveform);

    void waveFormGenerated1(quint32 handle, QList<QImage> waveforms);

    void streamUnloaded(quint32 handle);

private:

    RackdClient *m_rackdClient;
    QSpinBox *m_sb;
    QTextEdit *m_log;
    QLabel *m_labelTime;

    RPeakMeter *m_peakMeter;

    QGraphicsScene *m_scene;
    QGraphicsView *m_view;
    QFrame *m_playCursor;

    quint32 m_handle;
    quint32 m_time;

};

#endif // MAINWINDOW_H
