#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <vector>

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
    
private slots:
    void on_pushButton_clicked();
    void pointLearningParser();
    void pointUserParser();
    void calculateSVM();
    
    void on_CUP_clicked();
    
    void on_CLP_clicked();
    
    void on_SLP_clicked();
    
    void on_LLP_clicked();
    
private:
    Ui::MainWindow *ui;
    
    cv::Mat plot;
    //cv::Mat plot_mirror;
    
    QImage q_plot;
    //QImage q_plot_mirror;
    
    std::map<int, cv::Point3i> L_points;
    std::map<int, cv::Point3i> U_points;
    
    void paintPoint( QString &, QString & );
   
};
#endif // MAINWINDOW_H
