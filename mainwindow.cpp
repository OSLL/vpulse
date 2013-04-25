#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createActions();
    QGroupBox* leftBox = createGroupBox();
    QGroupBox* rightBox = createGroupBoxRight();

    setMinimumSize(400,400);

    QSize windowSize = size();
    int width = windowSize.width();
    int height = windowSize.height();

    QWidget* CentralWidget = new QWidget(this);
    //CentralWidget->setMinimumSize(300,200);
    int boxWidH2 = height - 150;
    int boxWidH1 = 50;
    CentralWidget->setGeometry(10,boxWidH1,width - 10,boxWidH2);

    memo = new QTextEdit(this);
    memo->setGeometry(40,boxWidH1+boxWidH2+ 20,width-80,height-(boxWidH1+boxWidH2+ 20+ 20) );
    memo->show();
    memo->append("Programm started.");

    QHBoxLayout* CentralLayout = new QHBoxLayout();
    CentralWidget->setLayout(CentralLayout);

    CentralLayout->addWidget(leftBox);
    CentralLayout->addWidget(rightBox);

    setWindowTitle("Pulse recognition by video");
    QDesktopWidget* desktop = QApplication::desktop();

    int w = desktop->width();
    int h = desktop->height();




    int x = (w - width)/2;
    int y = (h - height)/2;
    move(x,y);

    sliderHint = new QLabel(this);
    sliderHint->setAutoFillBackground(true);
    sliderHint->setGeometry(0,0,25,15);
    sliderHint->setPalette(QPalette(QColor(255,0,0,100)));
    sliderHint->hide();

    VideoReader* Curr_video=new VideoReader();
    Pr1=new processor();

}

void MainWindow::createActions(void)
{

    my_menuBar = new QMenuBar(this);
    //fileMenu:
    newAct = new QAction(this);
    newAct->setIcon(QIcon(QPixmap("new.png")));
    newAct->setShortcut(QKeySequence("Ctrl+N"));
    newAct->setText(QString("New"));
    QObject::connect(newAct,SIGNAL(triggered()),this,SLOT(newval()));
    openAct = new QAction(this);
    openAct->setIcon(QIcon(QPixmap("open.png")));
    openAct->setShortcut(QKeySequence("Ctrl+O"));
    openAct->setText(QString("Open"));
    QObject::connect(openAct,SIGNAL(triggered()),this,SLOT(openfile()));
    saveAct = new QAction(this);
    saveAct->setIcon(QIcon(QPixmap("save.png")));
    saveAct->setShortcut(QKeySequence("Ctrl+S"));
    saveAct->setText(QString("Save"));
    QObject::connect(saveAct,SIGNAL(triggered()),this,SLOT(savefile()));
    exitAct = new QAction(this);
    exitAct->setIcon(QIcon(QPixmap("exit.png")));
    exitAct->setShortcut(QKeySequence("Ctrl+X"));
    exitAct->setText(QString("Exit"));
    QObject::connect(exitAct,SIGNAL(triggered()),this,SLOT(close()));


    fileMenu = new QMenu(this);

    my_menuBar->addMenu(fileMenu);
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    //fileMenu->setWindowFlags(Qt::FramelessWindowHint);
    fileMenu->setAttribute(Qt::WA_TranslucentBackground);
    fileMenu->setStyleSheet("QMenu{background:rgba(255,0,0,50%);}");
    //fileMenu->popup(QCursor::pos());
    //fileMenu->setGeometry(0,0,60,70);
    fileMenu->setTitle("File");
    fileMenu->setAutoFillBackground(true);
    fileMenu->show();
    //Data menu


}

QGroupBox* MainWindow::createGroupBox(void)
{


    //QRadioButton* CamB;
    //QRadioButton* FileB;
    QGroupBox* grb1 = new QGroupBox();
    QVBoxLayout* layout = new QVBoxLayout();
    grb1->setLayout(layout);


     frL = new QSlider(Qt::Horizontal);
    frL->setAutoFillBackground(true);
    frL->setMinimum(20);
    frL->setMaximum(400);
    QObject::connect(frL,SIGNAL(sliderMoved(int)),this,SLOT(frLchanged(int)));
    QObject::connect(frL,SIGNAL(sliderPressed()),this,SLOT(showHintLb()));
    QObject::connect(frL,SIGNAL(sliderReleased()),this,SLOT(hideHintLb()));

     frH = new QSlider(Qt::Horizontal);
    frH->setAutoFillBackground(true);
    frH->setMinimum(30);
    frH->setMaximum(410);
    QObject::connect(frH,SIGNAL(sliderMoved(int)),this,SLOT(frHchanged(int)));
    QObject::connect(frH,SIGNAL(sliderPressed()),this,SLOT(showHintLb()));
    QObject::connect(frH,SIGNAL(sliderReleased()),this,SLOT(hideHintLb()));

     ampFact = new QSlider(Qt::Horizontal);
    ampFact->setAutoFillBackground(true);
    ampFact->setMinimum(0);
    ampFact->setMaximum(100);
    QObject::connect(ampFact,SIGNAL(sliderMoved(int)),this,SLOT(Ampchanged(int)));
    QObject::connect(ampFact,SIGNAL(sliderPressed()),this,SLOT(showHintLb()));
    QObject::connect(ampFact,SIGNAL(sliderReleased()),this,SLOT(hideHintLb()));

     frHL= new QLabel(tr("High frequency"));
     frLL= new QLabel(tr("Low frequency"));
     ampL= new QLabel(tr("Amplification factor"));

    layout->addWidget(frLL);
    layout->addWidget(frL);
    layout->addWidget(frHL);
    layout->addWidget(frH);
    layout->addWidget(ampL);
    layout->addWidget(ampFact);
    layout->setAlignment(Qt::AlignCenter);

    QPlastiqueStyle* style = new QPlastiqueStyle();
    grb1->setStyle(style);
    grb1->show();
    return(grb1);
}

QGroupBox* MainWindow::createGroupBoxRight()
{
    QGroupBox* grb1 = new QGroupBox();
    QVBoxLayout* layout = new QVBoxLayout();
    grb1->setLayout(layout);

        CamB = new QRadioButton();
        FileB = new QRadioButton();
        CamB->setCheckable(true);
        FileB->setCheckable(true);
     CamL = new QLabel(tr("Camera"));
     FileL = new QLabel(tr("File"));
     QObject::connect(CamB,SIGNAL(clicked()),this,SLOT(source_changed()));
     QObject::connect(FileB,SIGNAL(clicked()),this,SLOT(source_changed()));

     StartBt = new QPushButton(tr("Start"));
     StartBt->setFixedHeight(50);
     StartBt->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
     QObject::connect(StartBt,SIGNAL(clicked()),this,SLOT(work()));


     layout->addWidget(CamL);
     layout->addWidget(CamB);
     layout->addWidget(FileL);
     layout->addWidget(FileB);
     layout->addWidget(StartBt);

     QPlastiqueStyle* style = new QPlastiqueStyle();
     grb1->setStyle(style);
     grb1->show();
     return(grb1);
}

void MainWindow::work(void)
{
    qDebug("work");
    if(Curr_video==NULL){
    Curr_video=new VideoReader();
    Pr1=new processor();
    }else{
        delete(Curr_video);
        delete(Pr1);
        Curr_video=new VideoReader();
        Pr1=new processor();
    }
    if(frLow<frHigh){
        qDebug("%f,%f,%f,%d",frLow,frHigh,aFact,source);
        //filename_in = "palm.MOV";
        frLow = frLow/60.0;
        frHigh = frHigh/60.0;
    Curr_video->CVReadVideoRT(filename_get.toAscii().data(),Pr1,frLow,frHigh,aFact,source,memo);
    }else{
        memo->append("Error! Low frequency is more than Hight!");
    }
}

void MainWindow::source_changed(void)
{
    if(sender()==CamB)
    {
        //FileB->setChecked(true);
        emit send_source(1);
        source = 1;
        //qDebug("source =2");
    }
    else
    {
        //CamB->setChecked(true);
        emit send_source(2);
        source = 2;
        //qDebug("source =1");
    }
}

void MainWindow::showHintLb(void)
{
    QSlider* tmp = (QSlider*)sender();
    int x = tmp->geometry().x()+ (int)((float)tmp->geometry().width()*(float)tmp->sliderPosition()/(float)tmp->maximum());
    int y = tmp->geometry().y()+50;
    sliderHint->move(x,y);
    sliderHint->setText(QString::number(tmp->sliderPosition()));
    sliderHint->show();
}

void MainWindow::hideHintLb(void)
{
    sliderHint->hide();
}

void MainWindow::Ampchanged(int val)
{
    QSlider* tmp = (QSlider*)sender();
    int x = tmp->geometry().x()+ (int)((float)tmp->geometry().width()*(float)tmp->sliderPosition()/(float)tmp->maximum());
    int y = tmp->geometry().y()+50;
    sliderHint->move(x,y);
    sliderHint->setText(QString::number(tmp->sliderPosition()));
    aFact = val;
    emit send_amp(val);
}

void MainWindow::frLchanged(int val)
{
    QSlider* tmp = (QSlider*)sender();
    int x = tmp->geometry().x()+ (int)((float)tmp->geometry().width()*(float)tmp->sliderPosition()/(float)tmp->maximum());
    int y = tmp->geometry().y()+50;
    sliderHint->move(x,y);
    sliderHint->setText(QString::number(tmp->sliderPosition()));
    frLow= val;
    emit send_frL(val);
}

void MainWindow::frHchanged(int val)
{
    QSlider* tmp = (QSlider*)sender();
    int x = tmp->geometry().x()+ (int)((float)tmp->geometry().width()*(float)tmp->sliderPosition()/(float)tmp->maximum());
    int y = tmp->geometry().y()+50;
    sliderHint->move(x,y);
    sliderHint->setText(QString::number(tmp->sliderPosition()));
    frHigh= val;
    emit send_frH(val);
}

void MainWindow::openfile(void)
{
    QString s = QFileDialog::getOpenFileName(this,tr("Open file"),QDir::currentPath(),tr("(*.*)"));
    filename_get = s;
    filename_in= &s.toAscii().data()[0];
    memo->append("File opened:");
    memo->append(filename_get.toAscii().data());
}

void MainWindow::savefile(void)
{
    QString s = QFileDialog::getSaveFileName(this,tr("Save file"),QDir::currentPath(),tr("DATA (*.*)"));
    filename_out=  s.toAscii().data();
    memo->append("File saved as:");
    memo->append(filename_out);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newval(void)
{

}
