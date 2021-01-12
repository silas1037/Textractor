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

#include "wigglywidget.h"

#include <QFontMetrics>
#include <QPainter>
#include <QTimerEvent>
#include <QDebug>
//! [0]
WigglyWidget::WigglyWidget(QWidget *parent)
    : QWidget(parent), step(0)
{
    //setBackgroundRole(QPalette::Midlight);
    setBackgroundRole(QPalette::Dark); //QPalette::Dark
    setAttribute(Qt::WA_OpaquePaintEvent); //正确的设置：透明字背景
    setStyleSheet("color: rgb(255, 255, 255);");
    //setAutoFillBackground(true);
    setWindowOpacity(1.0);

    subFont = QFont("黑体",40,QFont::Bold);
    //newFont.setPointSize(newFont.pointSize() + 10);
    setFont(subFont);
    timer.start(60, this);
    screenX=QRect(QApplication::desktop()->availableGeometry(this)).bottomRight().x()-30;

    fontColor = QColor(200,200,200);
}


void WigglyWidget::enterEvent(QEvent *)
{
    ReSize();
//    qDebug()<<"鼠标移入";
}
void WigglyWidget::leaveEvent(QEvent *)
{
//    qDebug()<<"鼠标移出";
}

//! [1]
void WigglyWidget::paintEvent(QPaintEvent * /* event */)
{
    //频繁绘制不需要窗口尺寸绘制
    //qDebug()<<subFont;
    QFontMetrics metrics(subFont); //font()
//    int x = (width() - metrics.horizontalAdvance(text)) / 2;
//    int y = (height() + metrics.ascent() - metrics.descent()) / 2; //一行字的高度 = paint.descent - paint.ascent
    int x = 0;
    int y = y_init; //(height() + metrics.ascent() - metrics.descent()) / 2;
    //QColor color;
    QPainter painter(this);
    setFont(subFont);
    for (int i = 0; i < text.size(); ++i) {
        painter.setPen(fontColor);
        painter.drawText(x, y , //sineTable[index] * //y - (( metrics.height()) / 400)
                         QString(text[i]));
        x += metrics.horizontalAdvance(text[i]);
        if (x > screenX-dx_screen){
            x = 0;
            y += metrics.ascent() - metrics.descent()+dy_font; //
        }
    }

    //may
    //painter.setBackgroundMode(Qt::TransparentMode); //OpaqueMode

}
void WigglyWidget::ReSize()
{
    //Textractor 均不能工作
    screenX=QRect(QApplication::desktop()->availableGeometry(this)).bottomRight().x()-30;


    QFontMetrics metrics(subFont);
    //qDebug()<<"y0"<<metrics.ascent() - metrics.descent();
    y_init = metrics.ascent() - metrics.descent()+20;

    int x = 0;
    int y = y_init; //
    bool FlagWidthFull = false;
    for (int i = 0; i < text.size(); ++i) {
        x += metrics.horizontalAdvance(text[i]);
        if (x > screenX-dx_screen){ //
            FlagWidthFull=true;
            x = 0;
            y += metrics.ascent() - metrics.descent()+dy_font; //
        }
    }
    emit setMainXY(FlagWidthFull?screenX:x,y+dy_window); //145
}

//void WigglyWidget::setMainXY(int x,int y)
//{

//}

void WigglyWidget::setText(const QString &newText="")
{
    //赋值后仅需要窗口尺寸绘制
    text = newText;
    ReSize();
}
void WigglyWidget::setWText(std::wstring newText)
{
    //赋值后仅需要窗口尺寸绘制
    text = QString::fromStdWString(newText);
    setText(text);
}
void WigglyWidget::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timer.timerId()) {
        ++step;
        update();
    } else {
        QWidget::timerEvent(event);
    }
//! [6]
}


void WigglyWidget::setHideShow(int flagShow)
{
    //qDebug()<<flagShow;
    if(flagShow){
        show();
        //qDebug()<<"show";
    }
    else{
        hide();
        //qDebug()<<"hide";
    }
}
