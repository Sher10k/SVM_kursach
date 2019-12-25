#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <vector>
#include <iostream>
#include <set>

//Qt
#include <QtCore>
#include <QtGui>
#include <QMainWindow>
#include <QWidget>
#include <QGraphicsScene>
#include <QImage>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

// Opencv 4.1.0
//#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/ml.hpp>


///////////////////////////////////////////////////////////////////////////////////////////////////
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

///////////////////////////////////////////////////////////////////////////////////////////////////
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString path_file = "";
    
private slots:
    void pointLearningParser();
    void pointUserParser();
    void calculateSVM();
    
    void on_CUP_clicked();
    
    void on_CLP_clicked();
    
    void on_SLP_clicked();
    
    void on_LLP_clicked();
    
private:
    Ui::MainWindow *ui;
    
    int width = 380, height = 380;
    cv::Mat plot, line_mask;
    QImage q_plot;
    
    std::map<int, cv::Point3i> L_points;
    std::map<int, cv::Point3i> U_points;
    int num_p_L, num_p_U;
    
    //void paintPoint( QString &, QString & );
    void presetPlot();
   
};
#endif // MAINWINDOW_H
