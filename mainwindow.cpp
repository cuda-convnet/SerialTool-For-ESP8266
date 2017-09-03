#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QCloseEvent>
#include <QList>
#include <QDebug>
#include <QMessageBox>
#include <QTextCursor>
#include <QTimer>
#include <controlwindow.h>
#include <askdialog.h>
#include <windows.h>
#include <QVector>
#include <algorithm>

QSerialPort serialPort;
QSerialPortInfo serialPortInfo;
QList<QSerialPortInfo> availablePort;
QVector<int> connectedDevice;
QTextCursor textCursor;
QTimer timer;
std::string message;

MainWindow::MainWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MainWindow)	//构造函数
{
	ui->setupUi(this);
    textCursor = ui->textEdit_Receive->textCursor();
    connect(&serialPort, SIGNAL(readyRead()), this, SLOT(serialPortReadyRead()));
	connect(&timer, SIGNAL(timeout()), this, SLOT(timer1Event()));
	connect(&cWindow, SIGNAL(sendCmd(int)), this, SLOT(switchCmd(int)));
    //timer.setInterval(1000);
	timer.start(1000);
}

void MainWindow::setApplication(QApplication *tempApp)
{
	app = tempApp;
}

MainWindow::~MainWindow()   //析构函数
{
	delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)	//关闭窗口事件
{
	app->quit();
}

void MainWindow::on_pushButton_Open_clicked()   //打开端口按钮被点击
{
	if (ui->pushButton_Open->text() == "打开端口" && ui->comboBox_COMNumber->count()) {
        serialPort.setPortName(availablePort[ui->comboBox_COMNumber->currentIndex()].portName());
		qDebug() << availablePort[ui->comboBox_COMNumber->currentIndex()].portName();
        //qDebug() << serialPort.isOpen();
        qDebug() << serialPort.open(QIODevice::ReadWrite);
        //qDebug() << availablePort[ui->comboBox_COMNumber->currentIndex()].description();
		if (serialPort.isOpen()) {      //打开端口成功
            serialPort.setBaudRate(ui->comboBox_BaudRate->currentText().toInt());
            serialPort.setStopBits(QSerialPort::OneStop);
            serialPort.setDataBits(QSerialPort::Data8);
            qDebug() << serialPort.baudRate();
            ui->pushButton_Open->setText("关闭端口");
			ui->pushButton_Send->setEnabled(true);
			ui->comboBox_COMNumber->setEnabled(false);
			ui->pushButton_ESP8266->setEnabled(true);
            QMessageBox temp;
            temp.setWindowTitle("提示：");
            temp.setIcon(QMessageBox::Information);
            //temp.setDefaultButton(QMessageBox::Retry);
            temp.setText("连接成功!");
            temp.exec();
		} else {                        //打开端口失败
            QMessageBox temp;
            temp.setWindowTitle("Oops!");
            temp.setIcon(QMessageBox::Warning);
            //temp.setDefaultButton(QMessageBox::Retry);
            temp.setText("连接失败!");
            temp.exec();
        }
	} else {                            //关闭端口
	  cWindow.hide();
	  serialPort.close();
	  ui->pushButton_Send->setEnabled(false);
	  ui->comboBox_COMNumber->setEnabled(true);
	  ui->pushButton_ESP8266->setEnabled(false);
      ui->pushButton_Open->setText("打开端口");
    }
}

void MainWindow::on_pushButton_Send_clicked()		//发送按钮被点击
{
    serialPort.write(ui->lineEdit_Send->text().toStdString().c_str());
    if (ui->checkBox_NewLine->isChecked())
		serialPort.write("\r\n");
}

void MainWindow::on_pushButton_ESP8266_clicked()    //载入ESP8266控制窗口
{
	if (cWindow.isVisible()) {
		cWindow.hide();
		cWindow.show();
	} else {
		cWindow.show();
		qDebug() << "窗口已载入";
	}
}

void MainWindow::on_lineEdit_Send_returnPressed()   //发送按钮被点击
{
	on_pushButton_Send_clicked();
}

void MainWindow::serialPortReadyRead()              //收到数据槽
{
    //qDebug() << "收到消息";
    QByteArray data = serialPort.readAll();
    ui->textEdit_Receive->insertPlainText(data);
    textCursor.movePosition(QTextCursor::End);
    ui->textEdit_Receive->setTextCursor(textCursor);
	if (data.toStdString().find(",CONNECT") != std::string::npos) {
		connectedDevice.push_back(atoi(data.toStdString().substr(0, data.toStdString().find(",CONNECT")).c_str()));
		qDebug() << connectedDevice[connectedDevice.size() - 1];
	} else if (data.toStdString().find(",CLOSED") != std::string::npos) {
		auto p = std::find(connectedDevice.begin(), connectedDevice.end(), atoi(data.toStdString().substr(0, data.toStdString().find(",CONNECT")).c_str()));
		if (p != connectedDevice.end())
			connectedDevice.erase(p);
	} else if (data.toStdString().find(">") != std::string::npos) {
		sendCmd(message);
	}
	/*qDebug() << data;
	qDebug() << data.toStdString().find("+IPD");
	if(data.toStdString().find("+IPD") != std::string::npos) {
		ui->checkBox_NewLine->setChecked(true);
		//qDebug() << "OK";
		//qDebug() << std::string("收到消息").size();
		sendCmd("AT+CIPSENDEX=0,12");
	}
	else if (data.toStdString().find(">") != std::string::npos)
		sendCmd("收到消息");
	qDebug() << data.c_str();
    qDebug() << data.find("WIFI GOT IP", 0);
    if (!data.find("WIFI GOT IP", 0)) {
        serialPort.write("AT");
        serialPort.write("\n");
    }*/
}

void MainWindow::timer1Event()                       //时钟事件槽
{
    if (availablePort.size() != serialPortInfo.availablePorts().size()) {
        ui->comboBox_COMNumber->clear();
        availablePort = (serialPortInfo.availablePorts());
        for (int i = 0; i < availablePort.size(); i++)
                ui->comboBox_COMNumber->addItem(availablePort[i].description() + " - " + availablePort[i].portName());
	}
}

void MainWindow::switchCmd(int index)	//选择命令槽
{
	ui->checkBox_NewLine->setChecked(true);
	switch(index) {
		case 1:			//测试AT命令
			sendCmd("AT");
			break;
		case 2:			//重启模块
			sendCmd("AT+RST");
			break;
		case 3:			//查询固件版本
			sendCmd("AT+GMR");
			break;
		case 4 : {		//恢复出厂设置
			QMessageBox temp(QMessageBox::Question, "提示：", "确定要恢复出厂设置吗？", QMessageBox::Yes | QMessageBox::No, this);
			//temp.exec();
			cWindow.hide();
			if (temp.exec() == QMessageBox::Yes)
				sendCmd("AT+RESTORE");
			cWindow.show();
			break;
		}
		case 5:			//查询当前模式
			sendCmd("AT+CWMODE?");
			break;
		case 6: {		//设置模式
			int temp = 0;
			askDialog ask;
			ask.setWidget_CWMODE(&temp);
			ask.exec();
			qDebug() << temp;
			switch(temp) {
				case 1:
					sendCmd("AT+CWMODE=1");
				break;
				case 2:
					sendCmd("AT+CWMODE=2");
				break;
				case 3:
					sendCmd("AT+CWMODE=3");
				break;
			}
			if(temp)
				sendCmd("AT+RST");
			break;
		}
		case 7: {		//连接AP
			std::string ssid, password;
			askDialog ask;
			ask.setWidget_CWJAP(&ssid, &password);
			ask.exec();
			if (ssid != "")
				sendCmd("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
			break;
		}
		case 8:			//扫描可以连接到的AP
			sendCmd("AT+CWLAP");
			break;
		case 9:			//关闭回显
			sendCmd("ATE0");
			break;
		case 10:		//开启回显
			sendCmd("ATE1");
			break;
		case 11:		//断开与AP的连接
			sendCmd("AT+CWQAP");
			break;
		case 12:		//查询SoftAP设置
			sendCmd("AT+CWSAP?");
			break;
		case 13: {		//设置SoftAP
			std::string ssid, password;
			int chl, ecn, maxConnect;
			bool ssidHidden;
			askDialog ask;
			ask.setWidget_CWSAP(&ssid, &password, &chl, &ecn, &maxConnect, &ssidHidden);
			if (ask.exec() == QMessageBox::Rejected)
				break;
			std::string	ssidHidden_s;
			if (ssidHidden)
				ssidHidden_s = "1";
			else
				ssidHidden_s = "0";
			char a[100], b[100], c[100];
			itoa(maxConnect, a, 10);
			itoa(chl, b, 10);
			itoa(ecn, c, 10);
			std::string maxConnect_s(a);
			std::string chl_s(b);
			std::string ecn_s(c);
			sendCmd("AT+CWSAP=\"" + ssid + "\",\"" + password + "\"," + chl_s + "," + ecn_s + "," + maxConnect_s + "," + ssidHidden_s);
			break;
		}
		case 14:		//查询已连接到的AP信息
			sendCmd("AT+CWJAP?");
			break;
		case 15:
			sendCmd("AT+CIFSR");
			break;
		case 16:
			sendCmd("AT+CIPMUX=1");
			break;
		case 17: {
			askDialog ask;
			int TCPMode, port;
			ask.setWidget_CIPSERVER(&TCPMode, &port);
			if (ask.exec() == QDialog::Rejected)
				break;
			if (TCPMode == 0) {
				sendCmd("AT+CIPSERVER=0");
			}
			else {
				char a[100], b[100];
				itoa(TCPMode, a, 10);
				itoa(port, b, 10);
				sendCmd("AT+CIPSERVER=" + std::string(a) + "," + std::string(b));
			}
			break;
		}
		case 18:
			sendCmd("AT+CIPMUX=0");
			break;
		case 19:
			askDialog ask;
			std::string a, b;
			ask.setWidget_CIPSEND(connectedDevice, &a, &b);
			if(ask.exec() == QDialog::Rejected)
				break;
			qDebug() << a.c_str();
			sendCmd("AT+CIPSEND=" + b + "," + QString::number(a.size()).toStdString());
			message = a;
			break;
	}
}

void MainWindow::sendCmd(std::string cmd)
{
	ui->lineEdit_Send->setText(cmd.c_str());
	on_pushButton_Send_clicked();
}

void MainWindow::on_comboBox_BaudRate_editTextChanged(const QString &arg1)
{
	if (arg1.toInt() != 0 && serialPort.isOpen())
			serialPort.setBaudRate(arg1.toInt());
}
