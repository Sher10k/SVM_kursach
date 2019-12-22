#include "mainwindow.h"

// Qt
#include <QApplication>

// Opencv 4.1.0
//#include <opencv2/opencv.hpp>

int main(int argc, char *argv[])
{
    /*** READ CONFGURATION ***/
    const cv::String keys =
        "{help h usage ? |                | print this message   }"
        "{config         |TestPoints.txt  | path to test points  }"
        ;
    cv::CommandLineParser parser(argc, argv, keys);
    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }
    std::string filename = parser.get< cv::String >("config");
    
    QApplication a(argc, argv);
    MainWindow w;
    w.path_file = QString::fromUtf8( filename.c_str() );
    w.show();
    return a.exec();
}
