/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef WIGGLYWIDGET_H
#define WIGGLYWIDGET_H

#include <QBasicTimer>
#include <QWidget>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QApplication>
#include <QColorDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QDebug>
class WigglyWidget : public QWidget
{
    Q_OBJECT

public:
    WigglyWidget(QWidget *parent = nullptr);
    int screenX=1366;//1888;
    QFont subFont;
    int y_init = 60; //y区域上边界60，不固定 fontheight+20
    int dy_font = 30; //y字体间距，不固定30
    int dx_screen = 60; //x区域边界，固定60
    int dy_window = 80; //y窗口总边界，固定60
    QString text;
    QColor fontColor;

    bool strokeShow =true;
    bool autoStrokeColor=true;
    QColor strokeColor;
    int strokeWidth = 1;

    bool widvis = true;

    void ReSize();
    void setWText(std::wstring newText){setText(QString::fromStdWString(newText));}
signals:
    void setMainXY(int x,int y);
public slots:
    void widgetVis(){widvis=!widvis;setVisible(widvis);}
    void setfontColor(){
        QColor rgb = QColorDialog::getColor(fontColor);
        if(rgb.isValid()) fontColor=rgb;
        if(autoStrokeColor){ strokeColor.setRgb((rgb.red()<128)?255:0,(rgb.green()<128)?255:0,(rgb.blue()<128)?255:0);
        strokeColor=strokeColor.darker();}
    }
    void setfont(){
        bool ok;
        QFont fontback = QFontDialog::getFont(&ok, subFont);
        if(ok){ fontback.setBold(true);subFont=fontback;}
    }
    void strokeColorChooser(){
        QColor rgb = QColorDialog::getColor(fontColor);
        if(rgb.isValid()) strokeColor=rgb;
    }
    void setstrokeShow(){strokeShow=!strokeShow;}
    void setautoStrokeColor(){autoStrokeColor=!autoStrokeColor;}
    void setstrokeWidth(){bool ok=false;
         int wid = QInputDialog::getInt(this,QString::fromLocal8Bit("设置描边线宽"),QString::fromLocal8Bit("轮廓宽度（默认1）"),strokeWidth,1,100,1,&ok);
         if(ok) strokeWidth=wid;
    }

    void setText(const QString &newText){text = newText;ReSize();}
    void setHideShow(int flagShow){ flagShow?show():hide();}

protected:
    void enterEvent(QEvent *) override{}
    void leaveEvent(QEvent *) override{}
protected:
    void paintEvent(QPaintEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

private:
    QBasicTimer timer;
    int step;
};
//! [0]

#endif
