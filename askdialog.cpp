#include "askdialog.h"
#include "ui_askdialog.h"
#include <QDebug>
#include <QMessageBox>

askDialog::askDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::askDialog)
{
	ui->setupUi(this);
	this->setWindowFlag(Qt::WindowStaysOnTopHint);
	this->setModal(true);
	setFixedSize(this->minimumSize());
}

askDialog::~askDialog()
{
	delete ui;
}

void askDialog::setWidget_CWMODE(int *index)
{
	p = index;
	int cur = 0;
	int tabWidgetSize = ui->tabWidget->count();
	for (int i = 0; i < tabWidgetSize; i++) {
		//qDebug() << ui->tabWidget->tabText(0);
		if (ui->tabWidget->tabText(cur) == "Tab 1")
			cur++;
		ui->tabWidget->removeTab(cur);
	}
}

void askDialog::setWidget_CWJAP(std::string *a, std::string *b)
{
	ssid = a;
	password = b;
	int cur = 0;
	int tabWidgetSize = ui->tabWidget->count();
	for (int i = 0; i < tabWidgetSize; i++) {
		//qDebug() << ui->tabWidget->tabText(0);
		if (ui->tabWidget->tabText(cur) == "Tab 2")
			cur++;
		ui->tabWidget->removeTab(cur);
	}
}

void askDialog::setWidget_CWSAP(std::string *a, std::string *b, int *c, int *d, int *e, bool *f)
{
	ssid = a;
	password = b;
	chl = c;
	ecn = d;
	maxConnect = e;
	ssidHidden = f;
	int cur = 0;
	int tabWidgetSize = ui->tabWidget->count();
	for (int i = 0; i < tabWidgetSize; i++) {
		//qDebug() << ui->tabWidget->tabText(0);
		if (ui->tabWidget->tabText(cur) == "Tab 3")
			cur++;
		ui->tabWidget->removeTab(cur);
	}
}

void askDialog::setWidget_CIPSERVER(int *a, int *b)
{
	TCPMode = a;
	port = b;
	int cur = 0;
	int tabWidgetSize = ui->tabWidget->count();
	for (int i = 0; i < tabWidgetSize; i++) {
		//qDebug() << ui->tabWidget->tabText(0);
		if (ui->tabWidget->tabText(cur) == "Tab 4")
			cur++;
		ui->tabWidget->removeTab(cur);
	}
}

void askDialog::setWidget_CIPSEND(QVector<int> &a, std::string *b, std::string *c)
{
	int cur = 0;
	message = b;
	deviceNO = c;
	int tabWidgetSize = ui->tabWidget->count();
	for (int i = 0; i < tabWidgetSize; i++) {
		//qDebug() << ui->tabWidget->tabText(0);
		if (ui->tabWidget->tabText(cur) == "Tab 5")
			cur++;
		ui->tabWidget->removeTab(cur);
	}
	char d[10];
	for (int i = 0; i < a.size(); i++) {
		itoa(a[i], d, 10);
		ui->comboBox_DeviceNO->addItem(QString(d));
	}

}

void askDialog::on_pushButton_clicked()
{
	if (ui->tabWidget->tabText(0).toStdString() == "Tab 1") {
		*p = ui->comboBox_CWMODE->currentIndex() + 1;
		accept();
	} else if (ui->tabWidget->tabText(0).toStdString() == "Tab 2") {
		*ssid = ui->lineEdit_SSID1->text().toStdString();
		*password = ui->lineEdit_Password1->text().toStdString();
		accept();
	} else if (ui->tabWidget->tabText(0).toStdString() == "Tab 3") {
		//qDebug() << (ui->lineEdit_Password2->text().toStdString() == "" && ui->comboBox_ecn->currentIndex() == 0);
		//qDebug() << (ui->lineEdit_SSID2->text().toStdString() == "");
		if (ui->lineEdit_SSID2->text().toStdString() == "" || (ui->lineEdit_Password2->text().toStdString() == "" && ui->comboBox_ecn->currentIndex() != 0)) {
			QMessageBox temp(QMessageBox::Warning, "提示：", "请输入SSID与密码！\n如果不想设置密码，加密方式请选择OPEN！");
			temp.setWindowFlag(Qt::WindowStaysOnTopHint);
			temp.exec();
		} else if (ui->lineEdit_Password2->text().toStdString().size() < 8 && ui->comboBox_ecn->currentIndex()) {
			QMessageBox temp(QMessageBox::Warning, "提示：", "密码长度不能小于8Byte！");
			temp.setWindowFlag(Qt::WindowStaysOnTopHint);
			temp.exec();
		} else {
			*ssid = ui->lineEdit_SSID2->text().toStdString();
			*password = ui->lineEdit_Password2->text().toStdString();
			*maxConnect = ui->spinBox_maxConnect->value();
			*chl = ui->spinBox_chanel->value();
			//加密方式和SoftAp是否可见
			if (ui->comboBox_ecn->currentIndex() == 0)
				*ecn = ui->comboBox_ecn->currentIndex();
			else
				*ecn = ui->comboBox_ecn->currentIndex()	+ 1;
			*ssidHidden	= !ui->checkBox_SSIDHidden->isChecked();
			accept();
		}
	} else if (ui->tabWidget->tabText(0).toStdString() == "Tab 4") {
		*port = ui->lineEdit_Port->text().toInt();
		*TCPMode = ui->comboBox_TCPMode->currentIndex();
		accept();
	} else if (ui->tabWidget->tabText(0).toStdString() == "Tab 5") {
		*message = ui->lineEdit_sendMessage->text().toStdString();
		*deviceNO = ui->comboBox_DeviceNO->currentText().toStdString();
		accept();
	}
}

void askDialog::on_pushButton_2_clicked()
{
	this->reject();
}
