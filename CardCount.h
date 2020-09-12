#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CardCount.h"
#include<qpoint>
#include <map>
#include<opencv2/opencv.hpp>
#include<QGuiApplication>
#include<qscreen.h>
#include<qdebug.h>
#include<qdir.h>
#include<QTimerEvent>
#include <opencv2\imgproc\types_c.h>
#include<qstring.h>
#include<QMenu>
#include<Qpushbutton>
#include<qsettings.h>
#include<algorithm>
#include<QTime>
#include<QMessagebox>

//全局变量
extern cv::Rect myputting;

class CardCount : public QMainWindow
{
    Q_OBJECT

signals:
	//void save_setInfo();  // 鼠标右键信号

public:
    CardCount(QWidget *parent = Q_NULLPTR);
	void  mousePressEvent(QMouseEvent *event);
	void  mouseMoveEvent(QMouseEvent *event);
	void setCardVector();
	void slotGrabFullScreen(QString fileName);
	void timerEvent(QTimerEvent *);
	void settingMenuShow();  //右键显示菜单
	cv::Rect rectSelect();
	void synchro_seIinfo(bool setOrSave);
	std::vector<int> getPoisition();
	void makeMask();
	void makeMask2(cv::Mat myputting_get, std::vector<int>poisition_v);
	void updataCardVector(int val);
	cv::Mat diff_img(cv::Mat new_img, cv::Mat old_img);
	std::vector<int> getPoisition_2(cv::Mat myputting_img,int val);
	void detect_card(cv::Mat img, std::vector<int>poisition_v);

	QPoint winStartPos;
	std::map<std::string, int> card_data;
	int timerId;

	cv::Rect myplaying;
	cv::Rect playing2;
	cv::Rect playing3;
	bool first_flag = false;
	bool start_on = false;
	bool state_playing_2;
	bool state_playing_3;
	bool makemask_stat = false;

private:
    Ui::CardCountClass ui;
};
