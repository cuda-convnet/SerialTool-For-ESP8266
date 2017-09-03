#include "controlwindow.h"
#include "ui_controlwindow.h"
#include <mainwindow.h>

controlWindow::controlWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::controlWindow)
{
	ui->setupUi(this);
	setFixedSize(280, 400);
	//this->setWindowFlag(Qt::WindowStaysOnTopHint);
	//ui->toolBox->setCurrentIndex(0);
}

controlWindow::~controlWindow()
{
	delete ui;
}
void controlWindow::on_pushButton_AT_clicked()
{
	sendCmd(1);
}

void controlWindow::on_pushButton_RST_clicked()
{
	sendCmd(2);
}

void controlWindow::on_pushButton_ATGMR_clicked()
{
	sendCmd(3);
}

void controlWindow::on_pushButton_ATRESTORE_clicked()
{
	sendCmd(4);
}

void controlWindow::on_pushButton_getCWMODE_clicked()
{
	sendCmd(5);
}

void controlWindow::on_pushButton_setCWMODE_clicked()
{
	sendCmd(6);
}

void controlWindow::on_pushButton_CWJAP_clicked()
{
	sendCmd(7);
}

void controlWindow::on_pushButton_CWLAP_clicked()
{
	sendCmd(8);
}

void controlWindow::on_pushButton_ATE0_clicked()
{
	sendCmd(9);
}

void controlWindow::on_pushButton_ATE1_clicked()
{
	sendCmd(10);
}

void controlWindow::on_pushButton_CWQAP_clicked()
{
	sendCmd(11);
}

void controlWindow::on_pushButton_getCWSAP_clicked()
{
	sendCmd(12);
}

void controlWindow::on_pushButton_setCWSAP_clicked()
{
	sendCmd(13);
}

void controlWindow::on_pushButton_getCWJAP_clicked()
{
	sendCmd(14);
}


void controlWindow::on_pushButton_CIFSR_clicked()
{
	sendCmd(15);
}

void controlWindow::on_pushButton_CIPMUX_clicked()
{
	sendCmd(16);
}

void controlWindow::on_pushButton_CIPSERVER_clicked()
{
	sendCmd(17);
}

void controlWindow::on_pushButton_CIPMUX_2_clicked()
{
	sendCmd(18);
}

void controlWindow::on_pushButton_CIPSEND_clicked()
{
	sendCmd(19);
}
