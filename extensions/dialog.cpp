
#include "dialog.h"
#include "LocalSub.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    config = new QSettings("localtrans.ini", QSettings::IniFormat);
    config->setIniCodec("UTF-8");
    jpfile = config->value("jpdic","jpdic.txt").toString();
    zhfile = config->value("zhdic","zhdic.txt").toString();
    config->setValue("jpdic",jpfile);
    config->setValue("zhdic",zhfile);
    config->sync();
    //qDebug()<<jpfile<<zhfile;
    wigglyWidget = new WigglyWidget;

    lineEdit = new QLineEdit;
    lineEdit->setStyleSheet("background-color: rgb(102, 102, 102);"); //color: rgb(102, 102, 102);
    lineEdit->setVisible(false);

    pb1 = new QPushButton();
    pb1->setVisible(false);
    connect(pb1,&QPushButton::clicked,wigglyWidget, &WigglyWidget::show);
    //connect(pb1,&QPushButton::clicked,pb1, &QPushButton::hide);

    QCheckBox * cb1 = new QCheckBox();
    cb1->setChecked(true);
    connect(cb1,&QCheckBox::stateChanged,wigglyWidget, &WigglyWidget::setHideShow);
    //cb1->setStyleSheet("background-color: red;color: rgb(102, 102, 102);selection-background-color: rgb(102, 102, 102);");

    QRadioButton *rb1 = new QRadioButton();
    rb1->setStyleSheet("background-color: rgb(102, 102, 102);"); //color: rgb(102, 102, 102);



    layout = new QVBoxLayout(this);
    layout->addWidget(lineEdit);
    layout->addWidget(wigglyWidget);

    hl2=new QHBoxLayout(this);
    //hl2->addWidget(lineEdit2);
    //hl2->addWidget(pb1);
    //hl2->addWidget(lineEdit);
    //layout->addLayout(hl2);

    connect(lineEdit, &QLineEdit::textChanged, this, &Dialog::overScreen);
    connect(lineEdit, &QLineEdit::textChanged, wigglyWidget, &WigglyWidget::setText);
    connect(wigglyWidget, &WigglyWidget::setMainXY, this, &Dialog::resizedialog); //set xy

    //初始位置
    move(QRect(QApplication::desktop()->availableGeometry(this)).bottomRight().x()/3,QRect(QApplication::desktop()->availableGeometry(this)).bottomRight().y());
    //setWindowTitle(tr("Wiggly"));
    resize(360, 145); //-280 -335
    lineEdit->setText(QString::fromLocal8Bit("Bishojo Game 美少女游戏"));


    //透明度
    //setWindowOpacity(0.2);
    //透明
    setAttribute(Qt::WA_TranslucentBackground, true);//需要设置
//    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_PaintOnScreen);
    //无边框
    setWindowFlag(Qt::FramelessWindowHint);
    //置顶
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint); // | Qt::Tool);
    show();


    //右键菜单

    //创建弹出菜单对象
    pMenu = new QMenu(this);//pMenu 为类成员变量
    strokeMenu = new QMenu(this);

    {
    QAction *pAction = new QAction(pMenu);
    pAction->setText("font color");//设置文字
    //pAction->setIcon(QIcon(":/new/prefix1/forbidPNG"));//设置图标
    pMenu->addAction(pAction);//action添加到menu中
    connect(pAction,SIGNAL(triggered()),wigglyWidget,SLOT(setfontColor()));//关联事件响应函数，选择菜单中的action后，触发槽函数执行
    }
    {
    QAction *pAction = new QAction(pMenu);
    pAction->setText("font");//设置文字
    //pAction->setIcon(QIcon(":/new/prefix1/forbidPNG"));//设置图标
    pMenu->addAction(pAction);//action添加到menu中
    connect(pAction,SIGNAL(triggered()),wigglyWidget,SLOT(setfont()));//关联事件响应函数，选择菜单中的action后，触发槽函数执行
    }


    {
    QAction *pAction = new QAction(pMenu);
    pAction->setText("stroke");//设置文字
    //pAction->setIcon(QIcon(":/new/prefix1/forbidPNG"));//设置图标
    pMenu->addAction(pAction);//action添加到menu中
    pAction->setMenu(strokeMenu);
    //connect(pAction,SIGNAL(triggered()),this,SLOT(fontChooser()));//关联事件响应函数，选择菜单中的action后，触发槽函数执行
    }
    {
    QAction *pAction = new QAction(strokeMenu);
    pAction->setText("show/hide");//设置文字
    pAction->setCheckable(true);
    pAction->setChecked(true);
    //pAction->setIcon(QIcon(":/new/prefix1/forbidPNG"));//设置图标
    strokeMenu->addAction(pAction);//action添加到menu中
    connect(pAction,SIGNAL(triggered()),wigglyWidget,SLOT(setstrokeShow()));//关联事件响应函数，选择菜单中的action后，触发槽函数执行
    }
    {
    QAction *pAction = new QAction(strokeMenu);
    pAction->setText("stroke width");//设置文字
    //pAction->setIcon(QIcon(":/new/prefix1/forbidPNG"));//设置图标
    strokeMenu->addAction(pAction);//action添加到menu中
    connect(pAction,SIGNAL(triggered()),wigglyWidget,SLOT(setstrokeWidth()));//关联事件响应函数，选择菜单中的action后，触发槽函数执行
    }
    {
    QAction *pAction = new QAction(strokeMenu);
    pAction->setText("stroke color");//设置文字
    //pAction->setIcon(QIcon(":/new/prefix1/forbidPNG"));//设置图标
    strokeMenu->addAction(pAction);//action添加到menu中
    connect(pAction,SIGNAL(triggered()),wigglyWidget,SLOT(strokeColorChooser()));//关联事件响应函数，选择菜单中的action后，触发槽函数执行
    }
    {
    QAction *pAction = new QAction(strokeMenu);
    pAction->setText("auto color");//设置文字
    pAction->setCheckable(true);
    pAction->setChecked(true);
    //pAction->setIcon(QIcon(":/new/prefix1/forbidPNG"));//设置图标
    strokeMenu->addAction(pAction);//action添加到menu中
    connect(pAction,SIGNAL(triggered()),wigglyWidget,SLOT(setautoStrokeColor()));//关联事件响应函数，选择菜单中的action后，触发槽函数执行
    }
/*****/
    {
    QAction *pAction = new QAction(pMenu);
    pAction->setText("reload text");//设置文字
    pAction->setToolTip("reloads");
    //pAction->setIcon(QIcon(":/new/prefix1/forbidPNG"));//设置图标
    pMenu->addAction(pAction);//action添加到menu中
    connect(pAction,SIGNAL(triggered()),this,SLOT(stringReloads()));//关联事件响应函数，选择菜单中的action后，触发槽函数执行
    }
    {
    QAction *pAction = new QAction(pMenu);
    pAction->setText("texts Chooser");//设置文字
    pAction->setToolTip("jpdic.txt and zhdic.txt");
    //pAction->setIcon(QIcon(":/new/prefix1/forbidPNG"));//设置图标
    pMenu->addAction(pAction);//action添加到menu中
    connect(pAction,SIGNAL(triggered()),this,SLOT(mapChooser()));//关联事件响应函数，选择菜单中的action后，触发槽函数执行
    }
    {
    pAction = new QAction(pMenu);
    pAction->setText("background");//设置文字
    pAction->setCheckable(true);
    pAction->setChecked(false);
    //pAction->setIcon(QIcon(":/new/prefix1/forbidPNG"));//设置图标
    pMenu->addAction(pAction);//action添加到menu中
    connect(pAction,SIGNAL(triggered()),this,SLOT(BGset()));//关联事件响应函数，选择菜单中的action后，触发槽函数执行
    }
    {
    QAction *pAction = new QAction(pMenu);
    pAction->setText("debug");//设置文字
    pAction->setCheckable(true);
    pAction->setChecked(false);
    //pAction->setIcon(QIcon(":/new/prefix1/forbidPNG"));//设置图标
    pMenu->addAction(pAction);//action添加到menu中
    connect(pAction,SIGNAL(triggered()),this,SLOT(setDebug()));//关联事件响应函数，选择菜单中的action后，触发槽函数执行
    }
    {
    QAction *pAction = new QAction(pMenu);
    pAction->setText("Hide/Show");//设置文字
    pAction->setCheckable(true);
    pAction->setChecked(true);
    //pAction->setIcon(QIcon(":/new/prefix1/forbidPNG"));//设置图标
    pMenu->addAction(pAction);//action添加到menu中
    connect(pAction,SIGNAL(triggered()),wigglyWidget,SLOT(widgetVis()));//关联事件响应函数，选择菜单中的action后，触发槽函数执行
    }

    //主菜单添加子菜单
    pMenu->addMenu(strokeMenu);


    //创建一个QSyStemTrayIcon的对象.
    QSystemTrayIcon *m_trayIcon = new QSystemTrayIcon();
    //设置图标.
    m_trayIcon->setIcon(QIcon("G.ico"));
    //设置右键菜单.
    m_trayIcon->setContextMenu(pMenu);
    m_trayIcon->show();

    connect(this,&Dialog::jOverScreen,this, &Dialog::overScreen);
}
void Dialog::stringReloads()
{
    loadText();
}
void Dialog::mapChooser()
{
    QString jpdicNew = QFileDialog::getOpenFileName(NULL,QString::fromLocal8Bit("选择日文文本"),".","(*.txt);(*.*)");
    if(jpdicNew!=""){
        jpfile = jpdicNew;
    }
    QString zhdicNew = QFileDialog::getOpenFileName(NULL,QString::fromLocal8Bit("选择中文文本"),".","(*.txt);(*.*)");
    if(zhdicNew!=""){
        zhfile = zhdicNew;
    }
    qDebug()<<jpfile<<zhfile;
    config->setValue("jpdic",jpfile);
    config->setValue("zhdic",zhfile);
    config->sync();
    loadText();
}

void Dialog::BGset()
{
    //5.13 不能工作
    if(pAction->isChecked())
    {
        //透明
        setAttribute(Qt::WA_TranslucentBackground, false);//需要设置
    //    setAttribute(Qt::WA_OpaquePaintEvent);
        setAttribute(Qt::WA_PaintOnScreen,false);
    }
    else{
        //透明
        setAttribute(Qt::WA_TranslucentBackground, true);//需要设置
    //    setAttribute(Qt::WA_OpaquePaintEvent);
        setAttribute(Qt::WA_PaintOnScreen,true);
    }
}

//void Dialog::fontColorChooser(){
//    QColor color = QColorDialog::getRgba(wigglyWidget->getfontColor().rgba());
//    if(color.isValid())
//        wigglyWidget->setfontColor(color);
//    //qDebug()<<color;
//}

//void Dialog::fontChooser()
//{
//    bool ok;
//    QFont fontback = QFontDialog::getFont(&ok, wigglyWidget->getfont());

////    qDebug()<<"test2"<<fontdia;
//    if(ok){
//        fontback.setBold(true);
//        //qDebug()<<"test2"<<wigglyWidget->getfont()<<endl<<fontback;
//        wigglyWidget->setfont(fontback); //seems bug
//        wigglyWidget->ReSize();
//    }
//}

void Dialog::resizedialog(float x, float y)
{
    resize(x+20,y);
}
//void Dialog::enterEvent(QEvent *)
//{
//    qDebug()<<"鼠标移入";
//}
//void Dialog::leaveEvent(QEvent *)
//{
//    qDebug()<<"鼠标移出";
//}
void Dialog::mousePressEvent(QMouseEvent *event)
{
    //当鼠标左键点击时.
    if (event->button() == Qt::LeftButton)
    {
        m_move = true;
        //记录鼠标的世界坐标.
        m_startPoint = event->globalPos();
        //记录窗体的世界坐标.
        m_windowPoint = this->frameGeometry().topLeft();
    }
    if (event->button() == Qt::RightButton){
        //下面执行弹出操作
                pMenu->popup(event->globalPos());
    }
}
void Dialog::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        //移动中的鼠标位置相对于初始位置的相对位置.
        QPoint relativePos = event->globalPos() - m_startPoint;

        //然后移动窗体即可.
        //this->move(m_windowPoint + relativePos );

        //方法1：
                QDesktopWidget* desktop = QApplication::desktop();
                QRect windowRect(desktop->availableGeometry(this));
                QRect widgetRect(this->geometry());
        //以下是防止窗口拖出可见范围外
        QPoint point=m_windowPoint + relativePos ;
                //左边
                if (point.x() <= 0)
                {
                    point = QPoint(0,point.y());
                }
                //右边
                int y = windowRect.bottomRight().y() - this->size().height();
                if (point.y() >= y && widgetRect.topLeft().y() >= y)
                {
                    point = QPoint(point.x(),y);
                }
                //上边
                if (point.y() <= 0)
                {
                    point = QPoint(point.x(),0);
                }
                //下边
                int x = windowRect.bottomRight().x() - this->size().width();
                if (point.x() >= x && widgetRect.topLeft().x() >= x)
                {
                    point = QPoint(x,point.y());
                }
                move(point);




                //方法2：
                //可以通过判断QRect windowRect是否包含（contains） QRect widgetRect 再移动
                //这里没有给出代码
    }
}
void Dialog::overScreen()
{

    m_windowPoint = this->frameGeometry().topLeft();
    //方法1：
            QDesktopWidget* desktop = QApplication::desktop();
            QRect windowRect(desktop->availableGeometry(this));
            QRect widgetRect(this->geometry());

            //qDebug()<<QRect(QApplication::desktop()->availableGeometry(this)).bottomRight().x()<<QRect(QApplication::desktop()->availableGeometry(this)).bottomRight().y();

    //以下是防止窗口拖出可见范围外
    QPoint point=m_windowPoint;
            //左边
            if (point.x() <= 0)
            {
                point = QPoint(0,point.y());
            }
            //右边
            int y = windowRect.bottomRight().y() - this->size().height();
            if (point.y() >= y && widgetRect.topLeft().y() >= y)
            {
                point = QPoint(point.x(),y);
            }
            //上边
            if (point.y() <= 0)
            {
                point = QPoint(point.x(),0);
            }
            //下边
            int x = windowRect.bottomRight().x() - this->size().width();
            if (point.x() >= x && widgetRect.topLeft().x() >= x)
            {
                point = QPoint(x,point.y());
            }
            move(point);
}
void Dialog::inputUpdate(std::wstring newText)
{
    wigglyWidget->setWText(newText);
    wigglyWidget->ReSize();
    emit jOverScreen();
}

void Dialog::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        //改变移动状态.
        m_move = false;
    }
}
