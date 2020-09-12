#include "CardCount.h"
#include <QtWidgets/QApplication>
cv::Rect myputting;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CardCount w;
	w.setWindowOpacity(1);
	w.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	w.setAttribute(Qt::WA_TranslucentBackground);
    w.show();
    return a.exec();
}
