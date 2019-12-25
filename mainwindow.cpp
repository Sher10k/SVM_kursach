#include "mainwindow.h"
#include "ui_mainwindow.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // Button click 
    connect( ui->addLearningPoint, SIGNAL(clicked()), this, SLOT(pointLearningParser()) );
    connect( ui->addUserPoint, SIGNAL(clicked()), this, SLOT(pointUserParser()) );
    connect( ui->calculSVM, SIGNAL(clicked()), this, SLOT(calculateSVM()) );
    
    // Zeroing number of map
    num_p_L = 0;
    num_p_U = 0;
    
    // Update plot
    presetPlot();
    
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
void MainWindow::presetPlot()
{
    // Create plot white color
    plot = cv::Mat::ones( height, width, CV_8UC3 );
    line_mask = cv::Mat::ones( height, width, CV_8UC3 );
    for ( size_t i = 0; i < plot.total(); i++ )
    {
        plot.at< cv::Vec3b >(int(i)) = cv::Vec3b( 255, 255, 255 );
        line_mask.at< cv::Vec3b >(int(i)) = cv::Vec3b( 255, 255, 255 );
    }
    
    // Add axis
    // axis X1
    cv::line( plot, cv::Point(0,0), cv::Point(350,0), cv::Scalar(0,0,255), 2, cv::LINE_8, 0 );
    // axis X2
    cv::line( plot, cv::Point(0,0), cv::Point(0,350), cv::Scalar(0,255,0), 2, cv::LINE_8, 0 );
    
    for ( auto i : L_points )
    {
        cv::circle( plot, cv::Point(i.second.x, i.second.y), 
                    2, (i.second.z > 0) ? cv::Scalar(0,0,255) : cv::Scalar(255,0,0), 
                    -1, cv::LINE_8, 0 );
    }
    for ( auto i : U_points )
    {
        cv::circle( plot, cv::Point(i.second.x, i.second.y), 
                    2, cv::Scalar(0,0,0), -1, cv::LINE_8, 0 );
    }
    
    cv::flip( plot, plot, 0);
    
    //cv::addWeighted( line_mask, 0.2, plot, 1.0, 0.0, plot );
    
    cv::putText( plot, "X1", cv::Point(340, 360), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,0,255), 2 );
    cv::putText( plot, "X2", cv::Point(10, 25), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,255,0), 2 );
    
    cv::cvtColor( plot, plot, cv::COLOR_BGR2RGB );
    //cv::resize( plot, plot, cv::Size(height, width), 0, 0, cv::INTER_LINEAR );
    q_plot = QImage( plot.data, 
                     plot.cols, 
                     plot.rows, 
                     int(plot.step),
                     QImage::Format_RGB888 );
    ui->label_plot->setPixmap( QPixmap::fromImage(q_plot));
}
//void MainWindow::paintPoint( QString &x1, QString &x2 )
//{
//    cv::flip( plot, plot, 0); // flip for correct axis
    
//    cv::circle( plot, 
//                cv::Point(x1.toInt(), x2.toInt()), 
//                2, cv::Scalar(255,0,0), -1, cv::LINE_8, 0 );
    
//    cv::flip( plot, plot, 0); // re-flip for correct axis
//    q_plot = QImage( plot.data, 
//                     plot.cols, 
//                     plot.rows, 
//                     int(plot.step),
//                     QImage::Format_RGB888 );
//    ui->label_plot->setPixmap( QPixmap::fromImage(q_plot));
//}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::pointLearningParser()
{
    QString str_x1_L = ui->line_x1_L->text();
    QString str_x2_L = ui->line_x2_L->text();
    QString str_Y_L = ui->line_Y_L->text();
    
    if ( str_x1_L.length() && str_x2_L.length() && str_Y_L.length() )   // Check empty lineEdit
    {
        L_points.insert( std::make_pair( num_p_L, cv::Point3i(str_x1_L.toInt(), str_x2_L.toInt(), str_Y_L.toInt()) ) );
        ui->LP->appendPlainText( QString("X[") + QString::number(num_p_L) + 
                                 QString("]( x1 | x2 ) = ") + str_x1_L + 
                                 QString(" | ") + str_x2_L + QString(",  ") +
                                 QString("Y = ") + str_Y_L );
        presetPlot();
        num_p_L ++;
    }
}
void MainWindow::pointUserParser()
{
    QString str_x1 = ui->line_x1->text();
    QString str_x2 = ui->line_x2->text();
    
    if ( str_x1.length() && str_x2.length() )   // Check empty lineEdit
    {
        U_points.insert( std::make_pair( num_p_U, cv::Point3i(str_x1.toInt(), str_x2.toInt(), 0) ) );
        ui->UP->appendPlainText( QString("X[") + QString::number(num_p_U) + 
                                 QString("]( x1 | x2 ) = ") + str_x1 + 
                                 QString(" | ") + str_x2 );
        presetPlot();
        num_p_U ++;
    }
}

void MainWindow::calculateSVM()
{
    cv::Mat trainingDataMat = cv::Mat::zeros( int(L_points.size()), 2, CV_32F );
    cv::Mat labelsMat = cv::Mat::zeros( int(L_points.size()), 1, CV_32SC1 );
    
    for ( int k = 0, i = 0; i < int(trainingDataMat.total()); i += 2, k++ )
    {
        trainingDataMat.at< float >(i) = L_points.at(k).x;
        trainingDataMat.at< float >(i + 1) = L_points.at(k).y;
    }
    for ( int i = 0; i < int(L_points.size()); i++ )
    {
        labelsMat.at< int >(i) = L_points.at(i).z;
    }
//    std::cout << "trainingDataMat = " << trainingDataMat << std::endl;
//    std::cout << "labelsMat = " << labelsMat << std::endl;
    
    // Train the SVM
    cv::Ptr< cv::ml::SVM > svm = cv::ml::SVM::create();
    svm->setType( cv::ml::SVM::C_SVC );
    svm->setKernel( cv::ml::SVM::LINEAR );
    svm->setTermCriteria( cv::TermCriteria( cv::TermCriteria::MAX_ITER, 100, 1e-6) );
    svm->train( trainingDataMat, cv::ml::ROW_SAMPLE, labelsMat );
    
    cv::Vec3b green(0,255,0), blue(255,0,0);
    for (int i = 0; i < line_mask.rows; i++)
    {
        for (int j = 0; j < line_mask.cols; j++)
        {
            cv::Mat sampleMat = (cv::Mat_< float >(1,2) << j,i);
            float response = svm->predict(sampleMat);
            if (response > 0.0f)
                line_mask.at< cv::Vec3b >(i,j)  = green;
            else 
                line_mask.at< cv::Vec3b >(i,j)  = blue;
        }
    }
    
    // Support vectors
    cv::Mat sv = svm->getUncompressedSupportVectors();
//    std::cout << "sv = " << sv << std::endl;
    cv::cvtColor( plot, plot, cv::COLOR_RGB2BGR );
    cv::flip( plot, plot, 0);
    for ( int i = 0; i < sv.rows; i++ )
    {
        cv::circle( plot, cv::Point(sv.row(i)), 
                    5, cv::Scalar(0,0,0), 2, cv::LINE_8, 0 );
    }
    cv::flip( plot, plot, 0);
    cv::flip( line_mask, line_mask, 0); // flip for correct axis
    
    cv::addWeighted( line_mask, -0.2, plot, 1.0, 0.0, plot );
    cv::cvtColor( plot, plot, cv::COLOR_BGR2RGB );
    q_plot = QImage( plot.data, 
                     plot.cols, 
                     plot.rows, 
                     int(plot.step),
                     QImage::Format_RGB888 );
    ui->label_plot->setPixmap( QPixmap::fromImage(q_plot));
    
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_CLP_clicked()
{
    L_points.clear();
    ui->LP->clear();
    num_p_L = 0;
    presetPlot();
}
void MainWindow::on_CUP_clicked()
{
    U_points.clear();
    ui->UP->clear();
    num_p_U = 0;
    presetPlot();
}

void MainWindow::on_LLP_clicked()
{
    // Open file for read
    //QFile file("../SVM_gui/TestPoints.txt");
    QFile file( path_file );
    if( !file.open(QIODevice::ReadOnly))
        QMessageBox::information( nullptr, "info", file.errorString() );
    QTextStream in(&file);
    QString list_point = in.readAll();
    
    // File parser
    int comma_num = 0;
    cv::Point3i temp_point = cv::Point3i(0,0,0);
    std::string temp_str = "";
    for ( QChar &i : list_point ) 
    {
        if ( i == ",")
        {
            switch (comma_num) 
            {
            case 0:
                temp_point.x = std::stoi( temp_str );
                break;
            case 1:
                temp_point.y = std::stoi( temp_str );
                break;
            }
            temp_str.clear();
            comma_num ++;
        }
        else if ( i == "\n" )
        {
            temp_point.z = std::stoi( temp_str );
            L_points.insert( std::make_pair( num_p_L, temp_point ) );
            ui->LP->appendPlainText( QString("X[") + QString::number(num_p_L) + 
                                     QString("]( x1 | x2 ) = ") + QString::number(temp_point.x) + 
                                     QString(" | ") + QString::number(temp_point.y) + QString(",  ") +
                                     QString("Y = ") + QString::number(temp_point.z) );
            num_p_L ++;
            temp_str.clear();
            comma_num = 0;
        }
        else if( i != " " ) temp_str += char( i.cell() );
    }
    file.close();
    presetPlot();
}
void MainWindow::on_SLP_clicked()
{
    // Open file for write
    QFile file( path_file );
    if( !file.open(QIODevice::WriteOnly))
        QMessageBox::information( nullptr, "info", file.errorString() );
    QTextStream out(&file);
    
    // Write points
    for ( auto i : L_points )
    {
        out << i.second.x << "," << i.second.y << "," << i.second.z << endl;
    }
    file.close();
    presetPlot();
}


