#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QPixmap>
#include <QFileDialog>
#include <QTextStream>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPlastiqueStyle>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QString>
#include <QComboBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QDesktopWidget>
#include <QSize>

//#include "Conf.h"
#include "videoreader.h"
//#include "processor.h"

class VideoReader;
class processor;

namespace Ui {
    class MainWindow;
}

//using namespace cv;
//using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    Ui::MainWindow *ui;

private:
    //temporary:
    int source;
    float frLow;
    float frHigh;
    float aFact;
    // end temporary

    VideoReader* Curr_video;
    processor* Pr1;

    QGroupBox* createGroupBox();
    QGroupBox* createGroupBoxRight();

    QMenuBar* my_menuBar;
    QMenu* fileMenu;
    //QMenu* dataMenu;

    //file menu
    QAction* newAct;
    QAction* openAct;
    QAction* saveAct;
    QAction* exitAct;

    //data menu
    //QAction* inputdata;
    //QAction* sortdataUp;
    //QAction* sortdataDown;

    //sortexecutor* sort;
    //InputWindow* inp_form;
    QTextEdit* memo;
    QSlider* frL;
    QSlider* frH;
    QSlider* ampFact;
    QPushButton* StartBt;
    QLabel* frHL;
    QLabel* frLL;
    QLabel* ampL;
    QLabel* PulseL;
    QLabel* CamL;
    QLabel* FileL;
    QRadioButton* CamB;
    QRadioButton* FileB;
    QCheckBox* PrintFrames;

    char* filename_in;
    char* filename_out;
    QString filename_get;

    QLabel* sliderHint;



    void createActions(void);
    //void createGroupBox(void);

signals:
    void send_source(int source_id);
    void send_amp(int amp);
    void send_frH(int val);
    void send_frL(int val);
    //
private slots:
    void source_changed(void);
    void Ampchanged(int val);
    void frLchanged(int val);
    void frHchanged(int val);
    void work(void);
    //=======
    void openfile(void);
    void savefile(void);
    void newval(void);
    void showHintLb(void);
    void hideHintLb(void);
    //void print_Data(void);
};

#endif // MAINWINDOW_H
