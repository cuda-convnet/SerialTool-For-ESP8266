#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <controlwindow.h>
#include <QCloseEvent>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	QApplication *app;
	Ui::MainWindow *ui;
	controlWindow cWindow;
	void setApplication(QApplication *tempApp);
	~MainWindow();

public slots:
	void on_pushButton_Open_clicked();
	void on_pushButton_Send_clicked();
	void on_pushButton_ESP8266_clicked();
    void on_lineEdit_Send_returnPressed();
    void serialPortReadyRead();
	void timer1Event();
	void switchCmd(int);
	void sendCmd(std::string cmd);

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void on_comboBox_BaudRate_editTextChanged(const QString &arg1);
};

#endif // MAINWINDOW_H
