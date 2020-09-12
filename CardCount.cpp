#include "CardCount.h"
#include<QMouseEvent>
#pragma execution_character_set("UTF-8")


//鼠标左键按下窗口时响应	
void  CardCount::mousePressEvent(QMouseEvent *event)
{
	if (event->buttons() == Qt::LeftButton)
	{
		winStartPos = event->globalPos() - this->frameGeometry().topLeft();  //记录窗口此时的坐标
	}
}

//鼠标移动时响应
void  CardCount::mouseMoveEvent(QMouseEvent *event)
{
	QPoint EndPos;
	if (event->buttons() == Qt::LeftButton)
	{
		EndPos = event->globalPos() - winStartPos;
		this->move(EndPos);
	}
}

//牌的字典
void CardCount::setCardVector()
{
	//std::map<char, int> card_data;
	card_data["14"] = 1;
	card_data["15"] = 1;
	card_data["1"] = 4;
	card_data["2"] = 4;
	card_data["3"] = 4;
	card_data["4"] = 4;
	card_data["5"] = 4;
	card_data["6"] = 4;
	card_data["7"] = 4;
	card_data["8"] = 4;
	card_data["9"] = 4;
	card_data["10"] = 4;
	card_data["11"] = 4;
	card_data["12"] = 4;
	card_data["13"] = 4;

}

//牌的字典更新
void CardCount::updataCardVector(int val)
{
	if (val != 0)
	{
		card_data[std::to_string(val)]--;
	}
	ui.label_king->setText(QString::fromStdString(std::to_string(card_data["15"])));  // 大王
	ui.label_king1->setText(QString::fromStdString(std::to_string(card_data["14"])));
	ui.label_a->setText(QString::fromStdString(std::to_string(card_data["1"])));
	ui.label_2->setText(QString::fromStdString(std::to_string(card_data["2"])));
	ui.label_3->setText(QString::fromStdString(std::to_string(card_data["3"])));
	ui.label_4->setText(QString::fromStdString(std::to_string(card_data["4"])));
	ui.label_5->setText(QString::fromStdString(std::to_string(card_data["5"])));
	ui.label_6->setText(QString::fromStdString(std::to_string(card_data["6"])));
	ui.label_7->setText(QString::fromStdString(std::to_string(card_data["7"])));
	ui.label_8->setText(QString::fromStdString(std::to_string(card_data["8"])));
	ui.label_9->setText(QString::fromStdString(std::to_string(card_data["9"])));
	ui.label_10->setText(QString::fromStdString(std::to_string(card_data["10"])));
	ui.label_j->setText(QString::fromStdString(std::to_string(card_data["11"])));
	ui.label_q->setText(QString::fromStdString(std::to_string(card_data["12"])));
	ui.label_k->setText(QString::fromStdString(std::to_string(card_data["13"])));

}

//截全屏
void CardCount::slotGrabFullScreen(QString fileName)
{
	QScreen *screen = QGuiApplication::primaryScreen();
	QDir dir;
	if (!dir.exists("pic")) 
	{
		dir.mkdir("pic");
	}
	QString filePathName = "pic/" + fileName;
	filePathName += ".jpg";
	if (!screen->grabWindow(0).save(filePathName, "jpg"))
	{
		qDebug() << "save full screen failed" << endl;
	}
}


//获得牌的边界位置
std::vector<int> CardCount::getPoisition()
{
	slotGrabFullScreen("current");
	cv::Mat img_current;
	img_current = cv::imread("pic/current.jpg");
	cv::cvtColor(img_current, img_current, CV_BGR2GRAY);
	threshold(img_current, img_current, 200, 255, CV_THRESH_BINARY);
	cv::Mat myputting_img, canny_myputting;
	myputting_img = img_current(myputting);
	//cv::imshow("target000", img_current);
	int pos_left;
	for (int i = 0; i < myputting_img.cols/2; i++)
	{
		try 
		{
			if (myputting_img.at<uchar>(myputting_img.rows / 2, i) == 255)
			{
				pos_left = i;
				break;
			}
		}
		catch (...)
		{
		}
		
	}
	int pos_right;
	for (int i = myputting_img.cols-1; i > myputting_img.cols / 2; i--)
	{
		try
		{
			if (myputting_img.at<uchar>(myputting_img.rows / 2, i) == 255)
			{
				pos_right = i;
				break;
			}
		}
		catch (...)
		{
		}
	}
	int pos_top;
	for (int i = 0; i < myputting_img.rows / 2; i++)
	{
		try
		{
			if (myputting_img.at<uchar>(i, myputting_img.cols / 2) == 255)
			{
				pos_top = i;
				break;
			}
		}
		catch (...)
		{
		}
	}
	std::vector<int>poisition_vector;
	poisition_vector.push_back(pos_left);
	poisition_vector.push_back(pos_right);
	poisition_vector.push_back(pos_top);
	return poisition_vector;
}

//获得牌的边缘2
std::vector<int> CardCount::getPoisition_2(cv::Mat myputting_img,int val)
{
	//val=0 右边；val=1 左边
	std::vector<int>poisition_vector_def;
	poisition_vector_def.push_back(20);
	poisition_vector_def.push_back(40);
	poisition_vector_def.push_back(30);
	poisition_vector_def.push_back(50);
	int pos_left;
	for (int i = 0; i < myputting_img.cols; i++)
	{
		try
		{
			if (myputting_img.at<uchar>(90, i) == 255)
			{
				pos_left = i;
				break;
			}
		}
		catch (...)
		{
			return  poisition_vector_def;
		}
	}
	int pos_right;

	for (int i = (myputting_img.cols - 1); i > 0; i--)
	{
		try
		{
			if (myputting_img.at<uchar>(70, i) == 255)
			{
				pos_right = i;
				break;
			}
		}
		catch (...)
		{
			return  poisition_vector_def;
		}
		
	}
	
	int poc_temp = pos_right - 54;
	if (poc_temp < 0)
	{
		if (val == 0)
		{
			poc_temp = 252;
		}
		else
		{
			poc_temp = 20;
		}
	}
	int pos_top;
	for (int i = 0; i < myputting_img.rows / 2; i++)
	{
		try
		{
			if (myputting_img.at<uchar>(i, poc_temp) == 255)
			{
				pos_top = i;
				break;
			}
		}
		catch (...)
		{
			return  poisition_vector_def;
		}
	}
	int pos_botton;
	for (int i = (myputting_img.rows-1); i > 0; i--)
	{
		try
		{
			if (myputting_img.at<uchar>(i, poc_temp) == 255)
			{
				pos_botton = i;
				break;
			}
		}
		catch (...)
		{
			return  poisition_vector_def;
		}
	}
	std::vector<int>poisition_vector;
	poisition_vector.push_back(pos_left);
	poisition_vector.push_back(pos_right);
	poisition_vector.push_back(pos_top);
	poisition_vector.push_back(pos_botton);
	return poisition_vector;
}

//制作模板
void CardCount::makeMask()
{
	std::vector<int>poisition_v;
	poisition_v = getPoisition();
	int total_width = poisition_v[1] - poisition_v[0];
	int num = (total_width - 118) / 36 + 1;
	cv::Mat muban_img;
	cv::Mat img_current, myputting_get;
	img_current = cv::imread("pic/current.jpg");
	myputting_get = img_current(myputting);
	for (size_t i = 0; i < num; i++)
	{
		cv::Rect muban_cut = cv::Rect(poisition_v[0] + i * 36.5, poisition_v[2], 35, 44);
		try
		{
			muban_img = myputting_get(muban_cut);
		}
		catch (...)
		{
			ui.button_start->clicked();
			QMessageBox::critical(this, "错误", "发生异常，请重新开始");
			return;
		}
		QString filename = "pic/makeMask/";
		QTime *time = new QTime();
		QString strTime;
		strTime = time->currentTime().toString("hhmmss");
		filename += (strTime + "_" + QString::number(i) + ".jpg");
		std::string filename_1 = filename.toStdString();
		cv::imwrite(filename_1, muban_img);
	}
}

//制作模板2
void CardCount::makeMask2(cv::Mat myputting_get, std::vector<int>poisition_v)
{
	int total_width = poisition_v[1] - poisition_v[0];
	int num = (total_width - 74) / 20 + 1;
	cv::Mat muban_img;
	for (size_t i = 0; i < num; i++)
	{
		cv::Rect muban_cut = cv::Rect(poisition_v[0] + i * 22, poisition_v[2], 20, 28);
		try
		{
			muban_img = myputting_get(muban_cut);
		}
		catch (...)
		{
			ui.button_start->clicked();
			QMessageBox::critical(this, "错误", "发生异常，请重新开始");
			return;
		}
		QString filename = "pic/makeMask/s-";
		QTime *time = new QTime();
		QString strTime;
		strTime = time->currentTime().toString("hhmmss");
		filename += (strTime +"_"+ QString::number(i)+ ".jpg");
		std::string filename_1 = filename.toStdString();
		cv::imwrite(filename_1, muban_img);
	}
}

//图像差分（负值赋零）
cv::Mat CardCount::diff_img(cv::Mat new_img, cv::Mat old_img)
{
	int height = new_img.rows;
	int width = new_img.cols;
	cv::Mat temp_img(height, width,new_img.type());
	int sum = 0;
	for (int row = 0; row < height; row++) 
	{
		for (int col = 0; col < width; col++)
		{
			int a = new_img.at<uchar>(row, col) - old_img.at<uchar>(row, col);
			if (a < 0)
			{
				a = 0;
			}
			temp_img.at<uchar>(row, col) = a;
		}
	}
	return temp_img;
}

//定时器不停的截图
void CardCount::timerEvent(QTimerEvent *)
{
	slotGrabFullScreen("second");
	cv::Mat img_second;
	img_second = cv::imread("pic/second.jpg");
	cv::Mat img_second_gray;
	cv::cvtColor(img_second, img_second_gray, CV_BGR2GRAY);
	cv::Mat putting_img,test_img,center_img;
	putting_img = img_second_gray(myputting);
	threshold(img_second_gray, test_img, 200, 255, CV_THRESH_BINARY_INV);
	cv::Rect center_rect(img_second_gray.cols / 3, img_second_gray.rows / 3, img_second_gray.cols / 3, img_second_gray.rows / 3);
	center_img = test_img(center_rect);
	//cv::imshow("target", center_img);
	cv::Scalar tempVal_center = cv::mean(center_img);
	float matMean_center = tempVal_center.val[0];
	//qDebug() << matMean_center;
	if (matMean_center < 100)
	{
		return;
	}
	cv::GaussianBlur(putting_img, putting_img, cv::Size(3, 3), 2, 2);//高斯滤波
	dilate(putting_img, putting_img, cv::Mat());//膨胀
	erode(putting_img, putting_img, cv::Mat());//腐蚀
	std::vector<int>poisition_v;
	poisition_v = getPoisition();
	//qDebug() << test_img.at<uchar>(test_img.cols / 2, 90);
	int total_width = poisition_v[1] - poisition_v[0];
	//qDebug() << total_width;
	if (test_img.at<uchar>(test_img.cols / 2, 90) == 255)
	{
		setCardVector();
		updataCardVector(0);
		first_flag = true;
		return;
	}
	int num = (total_width - 118) / 36 + 1;
	if (num == 20)
	{
		setCardVector();
		updataCardVector(0);
		first_flag = true;
	}
	if (first_flag)
	{
		int num = (total_width - 118) / 36 + 1;
		if (num < 17 )
		{
			first_flag = true;
			setCardVector();
			updataCardVector(0);
			return;
		}
		cv::Mat image_matched, temp_match, match_img;
		for (size_t i = 0; i < num; i++)
		{
			cv::Rect muban_cut = cv::Rect(poisition_v[0] + i * 36.5, poisition_v[2], 35, 44);
			try
			{
				match_img = putting_img(muban_cut);
			}
			catch (...)
			{
				ui.button_start->clicked();
				return;
			}
			std::vector<double>minvalScore;
			for (size_t j = 0; j < 28; j++)
			{
				std::string fileName = "pic/muban/";
				fileName += std::to_string(j);
				fileName += ".jpg";
				temp_match = cv::imread(fileName);
				cv::cvtColor(temp_match, temp_match, CV_BGR2GRAY);
				//threshold(temp_match, temp_match, 150, 255, CV_THRESH_BINARY_INV);
				cv::GaussianBlur(temp_match, temp_match, cv::Size(3, 3), 2, 2);//高斯滤波
				dilate(temp_match, temp_match, cv::Mat());//膨胀
				erode(temp_match, temp_match, cv::Mat());//腐蚀
				//cv::imshow("target", match_img);
				//cv::waitKey(0);
				cv::matchTemplate(match_img, temp_match, image_matched, cv::TM_CCOEFF_NORMED);
				double minVal, maxVal;
				cv::Point minLoc, maxLoc;
				//寻找最佳匹配位置
				cv::minMaxLoc(image_matched, &minVal, &maxVal, &minLoc, &maxLoc);
				minvalScore.push_back(maxVal);
				//cv::imshow("target", temp_match);
				//cv::waitKey(0);
				//qDebug() << "****"<< maxVal << endl;
			}
			int score = max_element(minvalScore.begin(), minvalScore.end()) - minvalScore.begin();
			if (score == 26)
			{
				updataCardVector(15);
			}
			else
			{
				updataCardVector(score / 2 + 1);
			}
		}
		first_flag = false;
		if (makemask_stat)
		{
			makeMask();
		}
		
	}
	else
	{
		cv::Mat playing_img_2, playing_img_2_first,playing_img_3, playing_img_3_first;
		cv::Mat playing_img_2_differ, playing_img_3_differ;
		playing_img_2 = img_second_gray(playing2);
		playing_img_3 = img_second_gray(playing3);

		threshold(playing_img_2, playing_img_2_first, 220, 255, CV_THRESH_BINARY);
		threshold(playing_img_3, playing_img_3_first, 220, 255, CV_THRESH_BINARY);
		cv::Scalar tempVal_2 = cv::mean(playing_img_2_first);
		float matMean_2 = tempVal_2.val[0];
		cv::Scalar tempVal_3 = cv::mean(playing_img_3_first);
		float matMean_3 = tempVal_3.val[0];
		qDebug() << matMean_2;
		if(matMean_2<15)
		{
			state_playing_2 = true;
		}
		else if(state_playing_2)
		{
			state_playing_2 = false;
			cv::imshow("target0", playing_img_2_first);
			//qDebug() << "1 号玩家在出牌" << endl;
			//cv::imshow("target0", playing_img_2);
			QTime dieTime = QTime::currentTime().addMSecs(500);  //延时3s
			while (QTime::currentTime() < dieTime)
				QCoreApplication::processEvents(QEventLoop::AllEvents, 100);  //使得延时不会阻塞线程
			slotGrabFullScreen("second");
			cv::Mat img_second;
			img_second = cv::imread("pic/second.jpg");
			cv::Mat img_second_gray;
			cv::cvtColor(img_second, img_second_gray, CV_BGR2GRAY);
			
			cv::Mat playing_img_2_copy;
			playing_img_2_copy = img_second_gray(playing2);
			threshold(playing_img_2_copy, playing_img_2_copy, 220, 255, CV_THRESH_BINARY);
			std::vector<int>poisition_v = getPoisition_2(playing_img_2_copy,0);
			int total_heigh = poisition_v[3] - poisition_v[2];
			if (total_heigh > 105 || total_heigh < 95)
			{
				state_playing_3 = true;
				return;
			}
			if (makemask_stat)
			{
				makeMask2(img_second(playing2), poisition_v);
			}
			detect_card(img_second(playing2), poisition_v);
			state_playing_3 = true;
			return;
		}
		//if (img_h_3 > 105 || img_h_3 < 95)
		//{
		//	state_playing_3 = true;
		//}
		//if (playing_img_3_first.at<uchar>(93, 10) == 255)  //198->桌面的颜色  230->白色的牌
		if (matMean_3 < 15)
		{
			state_playing_3 = true;
		}
		else if (state_playing_3)
		{
			//qDebug() << "2 号玩家在出牌" << endl;
			cv::imshow("target01", playing_img_3_first);
			state_playing_3 = false;
			QTime dieTime = QTime::currentTime().addMSecs(500);  //延时2s
			while (QTime::currentTime() < dieTime)
				QCoreApplication::processEvents(QEventLoop::AllEvents, 100);  //使得延时不会阻塞线程
			slotGrabFullScreen("second");
			cv::Mat img_second;
			img_second = cv::imread("pic/second.jpg");
			cv::cvtColor(img_second, img_second_gray, CV_BGR2GRAY);
			cv::Mat playing_img_2_copy;
			playing_img_2_copy = img_second_gray(playing3);
			threshold(playing_img_2_copy, playing_img_2_copy, 220, 255, CV_THRESH_BINARY);


			std::vector<int>poisition_v = getPoisition_2(playing_img_2_copy,1);
			int total_heigh = poisition_v[3] - poisition_v[2];
			if (total_heigh > 105 || total_heigh < 95)
			{
				return;
			}
			if (makemask_stat)
			{
				makeMask2(img_second(playing3), poisition_v);
			}
			detect_card(img_second(playing3), poisition_v);
			return;
		}
	}
}

//出牌识别
void CardCount::detect_card(cv::Mat img, std::vector<int>poisition_v)
{
	int total_width = poisition_v[1] - poisition_v[0];
	int num = (total_width - 73) / 20 + 1;
	//qDebug() << "total_width = "<<total_width;
	//qDebug() << "num = " << num;
	cv::Mat muban_img;
	cv::cvtColor(img, img, CV_BGR2GRAY);
	cv::GaussianBlur(img, img, cv::Size(3, 3), 2, 2);//高斯滤波
	dilate(img, img, cv::Mat());//膨胀
	erode(img, img, cv::Mat());//腐蚀

	cv::Mat image_matched, temp_match, match_img;
	for (size_t i = 0; i < num; i++)
	{
		cv::Rect muban_cut = cv::Rect(poisition_v[0] + i * 22, poisition_v[2], 20, 28);
		try
		{
			match_img = img(muban_cut);
		}
		catch (...)
		{
			ui.button_start->clicked();
			return;
		}
		std::vector<double>minvalScore;
		for (size_t j = 0; j < 28; j++)
		{
			std::string fileName = "pic/muban1/";
			fileName += std::to_string(j);
			fileName += ".jpg";
			temp_match = cv::imread(fileName);
			cv::cvtColor(temp_match, temp_match, CV_BGR2GRAY);
			cv::GaussianBlur(temp_match, temp_match, cv::Size(3, 3), 2, 2);//高斯滤波
			dilate(temp_match, temp_match, cv::Mat());//膨胀
			erode(temp_match, temp_match, cv::Mat());//腐蚀
			//cv::imshow("target", match_img);
			//cv::waitKey(0);
			cv::matchTemplate(match_img, temp_match, image_matched, cv::TM_CCOEFF_NORMED);
			double minVal, maxVal;
			cv::Point minLoc, maxLoc;
			//寻找最佳匹配位置
			cv::minMaxLoc(image_matched, &minVal, &maxVal, &minLoc, &maxLoc);
			minvalScore.push_back(maxVal);
		}
		int score = max_element(minvalScore.begin(), minvalScore.end()) - minvalScore.begin();
		if (minvalScore[score] < 0.8)
		{
			return;
		}
		if (score == 26)
		{
			updataCardVector(15);
		}
		else
		{
			updataCardVector(score / 2 + 1);
		}
	}
}


cv::Rect CardCount::rectSelect()
{
	cv::Mat img_current;
	slotGrabFullScreen("current");
	img_current = cv::imread("pic/current.jpg");
	cv::Rect ROI = cv::selectROI(img_current,false);
	cv::destroyWindow("ROI selector");
	return ROI;
}

//右键显示菜单
void CardCount::settingMenuShow()
{
	QMenu *rightClickMenu = new QMenu();
	QAction *closeExe = rightClickMenu->addAction("放牌区");
	closeExe->setIcon(QIcon(":/MUBAN/putting.png"));
	//QAction *fixedWindow = rightClickMenu->addAction("出牌区");
	//fixedWindow->setIcon(QIcon(":/MUBAN/play1.png"));
	QAction *floatWindow = rightClickMenu->addAction("出牌区1");
	floatWindow->setIcon(QIcon(":/MUBAN/play2.png"));
	QAction *topWindow = rightClickMenu->addAction("出牌区2");
	topWindow->setIcon(QIcon(":/MUBAN/play3.png"));
	QAction *makeMask1 = rightClickMenu->addAction("制作模板");
	makeMask1->setIcon(QIcon(":/MUBAN/select.png"));

	//选择自己的放牌区
	connect(closeExe, &QAction::triggered, [=]() {
		myputting = rectSelect();
		synchro_seIinfo(false);
	});

	//选择自己的出牌区
	/*connect(fixedWindow, &QAction::triggered, [=]() {
		myplaying = rectSelect();
		synchro_seIinfo(false);
	});*/

	//选择其他人的出牌区
	connect(floatWindow, &QAction::triggered, [=]() {
		playing2 = rectSelect();
		synchro_seIinfo(false);
	});

	//选择其他人的出牌区
	connect(topWindow, &QAction::triggered, [=]() {
		playing3 = rectSelect();
		synchro_seIinfo(false);
	});

	//制作模板
	connect(makeMask1, &QAction::triggered, [=]() {
		makemask_stat = true;
	});

	QPoint pos;
	pos = QCursor::pos();
	rightClickMenu->show();
	rightClickMenu->activateWindow();
	rightClickMenu->exec(pos);
}

//同步本地的设置信息
void CardCount::synchro_seIinfo(bool setOrSave = true)
{
	QString fileDir = "setting.ini";
	QSettings setting(fileDir, QSettings::IniFormat);
	setting.beginGroup("config");
	if (setOrSave)
	{
		if (setting.contains("myputting"))
		{
			QRect temp = setting.value("myputting").value<QRect>();
			myputting = cv::Rect(temp.left(), temp.top(), temp.width(), temp.height());
			temp = setting.value("myplaying").value<QRect>();
			myplaying = cv::Rect(temp.left(), temp.top(), temp.width(), temp.height());
			temp = setting.value("playing2").value<QRect>();
			playing2 = cv::Rect(temp.left(), temp.top(), temp.width(), temp.height());
			temp = setting.value("playing3").value<QRect>();
			playing3 = cv::Rect(temp.left(), temp.top(),temp.width(),temp.height());
		}
		else
		{
			myputting = cv::Rect(251,523,890,163);
			myplaying = cv::Rect(503, 410, 350, 104);
			playing2 = cv::Rect(685, 291, 326, 108);
			playing3 = cv::Rect(373, 292, 306, 111);
		}
	}
	else
	{
		QRect temp = QRect(myputting.x, myputting.y, myputting.width, myputting.height);
		setting.setValue("myputting", QVariant(temp));
		temp = QRect(myplaying.x, myplaying.y, myplaying.width, myplaying.height);
		setting.setValue("myplaying", QVariant(temp));
		temp = QRect(playing2.x, playing2.y, playing2.width, playing2.height);
		setting.setValue("playing2", QVariant(temp));
		temp = QRect(playing3.x, playing3.y, playing3.width, playing3.height);
		setting.setValue("playing3", QVariant(temp));
		setting.endGroup();
	}
}

//构造函数
CardCount::CardCount(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	slotGrabFullScreen("second");
	//killTimer(timerId);
	synchro_seIinfo();
	setCardVector();
	connect(ui.button_close, &QPushButton::clicked, this, &QWidget::close);
	connect(ui.button_start, &QPushButton::clicked, [=]() {
		if (!start_on)
		{
			first_flag = true;
			start_on = true;
			ui.button_start->setText("停止");
			timerId = startTimer(1000);
		}
		else
		{
			start_on = false;
			first_flag = false;
			ui.button_start->setText("开始");
			killTimer(timerId);
			setCardVector();
			updataCardVector(0);
		}
	});
	connect(ui.button_setting, &QPushButton::clicked, [=]() {settingMenuShow();});
}
