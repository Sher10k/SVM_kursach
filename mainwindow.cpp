#include "mainwindow.h"
#include "ui_mainwindow.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // Button click 
    connect( ui->addLearningPoint, SIGNAL(clicked()), this, SLOT(pointLearningParser()) );
    connect( ui->addUserPoint, SIGNAL(clicked()), this, SLOT(pointUserParser()) );
    
    // Create plot
    //plot = cv::imread( "Beginner-Class-Hierarchy.jpg" );
    plot = cv::Mat::eye( 380, 380, CV_8UC3 );
    for ( size_t i = 0; i < plot.total(); i++ )
        plot.at< cv::Vec3b >(int(i)) = cv::Vec3b( 255, 255, 255 );
    
    // Add axis
    // axis X1
    cv::line( plot, cv::Point(0,0), cv::Point(350,0), cv::Scalar(0,0,255), 2, cv::LINE_8, 0 );
    // axis X2
    cv::line( plot, cv::Point(0,0), cv::Point(0,350), cv::Scalar(0,255,0), 2, cv::LINE_8, 0 );
    
    cv::flip( plot, plot, 0);
    
    cv::putText( plot, "X1", cv::Point(340, 360), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,0,255), 2 );
    cv::putText( plot, "X2", cv::Point(10, 25), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,255,0), 2 );
    
    cv::cvtColor( plot, plot, cv::COLOR_BGR2RGB );
    //cv::resize( plot, plot, cv::Size(380, 380), 0, 0, cv::INTER_LINEAR );
    q_plot = QImage( plot.data, 
                     plot.cols, 
                     plot.rows, 
                     int(plot.step),
                     QImage::Format_RGB888 );
    ui->label_plot->setPixmap( QPixmap::fromImage(q_plot));
    

    
//    for (int x = 50; x < 60; ++x) {
//        for (int y = 100; y < 110; ++y) {
//            q_plot.setPixel(x, y, qRgb(255, 0, 0));
//        }
//    }
//    ui->label_plot->setPixmap( QPixmap::fromImage(q_plot));
    
//    ui->plainTextEdit->appendPlainText( QString("P( x | y ) = ") + QString::number(p.x()) + 
//                                        QString(" | ") + QString::number(p.y()) );
    
}

///////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::~MainWindow()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::paintPoint( QString &x1, QString &x2 )
{
    cv::flip( plot, plot, 0); // flip for correct axis
    
    cv::circle( plot, 
                cv::Point(x1.toInt(), x2.toInt()), 
                2, cv::Scalar(255,0,0), -1, cv::LINE_8, 0 );
    
    cv::flip( plot, plot, 0); // re-flip for correct axis
    q_plot = QImage( plot.data, 
                     plot.cols, 
                     plot.rows, 
                     int(plot.step),
                     QImage::Format_RGB888 );
    ui->label_plot->setPixmap( QPixmap::fromImage(q_plot));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::pointLearningParser()
{
    QString str_x1_L = ui->line_x1_L->text();
    QString str_x2_L = ui->line_x2_L->text();
    QString str_Y_L = ui->line_Y_L->text();
    L_points.insert( std::make_pair( 0, cv::Point3i(str_x1_L.toInt(), str_x2_L.toInt(), str_Y_L.toInt()) ) );
    
    if ( str_x1_L.length() && str_x2_L.length() )   // Check empty lineEdit
    {
        ui->LP->appendPlainText( QString("X( x1 | x2 ) = ") + str_x1_L + 
                                 QString(" | ") + str_x2_L + QString(",  ") +
                                 QString("Y = ") + str_Y_L );
        //paintPoint( str_x1_L, str_x2_L );
    }
}
void MainWindow::pointUserParser()
{
    QString str_x1 = ui->line_x1->text();
    QString str_x2 = ui->line_x2->text();
    
    U_points.insert( std::make_pair( 0, cv::Point3i(str_x1.toInt(), str_x2.toInt(), 0) ) );
    
    if ( str_x1.length() && str_x2.length() )   // Check empty lineEdit
    {
        ui->UP->appendPlainText( QString("X( x1 | x2 ) = ") + str_x1 + 
                                 QString(" | ") + str_x2 );
        paintPoint( str_x1, str_x2 );
    }
}

void MainWindow::calculateSVM()
{
    
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_CUP_clicked()
{
    U_points.clear();
    ui->UP->clear();
}
void MainWindow::on_CLP_clicked()
{
    L_points.clear();
    ui->LP->clear();
}

void MainWindow::on_LLP_clicked()
{
    QFile file("../SVM_gui/TestPoints.txt");
    if( !file.open(QIODevice::ReadOnly))
        QMessageBox::information( nullptr, "info", file.errorString() );
    QTextStream in(&file);
    
    QString list_point = in.readAll();
    
    for ( auto i : list_point ) 
    {
        ui->LP->appendPlainText( i );
    }
    
    //ui->LP->appendPlainText( in.readAll() );
}
void MainWindow::on_SLP_clicked()
{
    
}



void MainWindow::on_pushButton_clicked()
{
    
}


