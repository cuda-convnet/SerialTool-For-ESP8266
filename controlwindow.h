#ifndef CONTROLWINDOW_H
#define CONTROLWINDOW_H

#include <QWidget>

namespace Ui {
	class controlWindow;
}

class controlWindow : public QWidget
{
	Q_OBJECT

public:
	explicit controlWindow(QWidget *parent = 0);
	~controlWindow();

signals:
	void sendCmd(int);

private slots:
	void on_pushButton_RST_clicked();
	void on_pushButton_AT_clicked();
	void on_pushButton_ATGMR_clicked();
	void on_pushButton_ATRESTORE_clicked();
	void on_pushButton_getCWMODE_clicked();
	void on_pushButton_setCWMODE_clicked();
	void on_pushButton_CWJAP_clicked();
	void on_pushButton_CWLAP_clicked();
	void on_pushButton_ATE0_clicked();
	void on_pushButton_ATE1_clicked();
	void on_pushButton_CWQAP_clicked();
	void on_pushButton_getCWSAP_clicked();
	void on_pushButton_setCWSAP_clicked();
	void on_pushButton_getCWJAP_clicked();
	void on_pushButton_CIPMUX_clicked();
	void on_pushButton_CIFSR_clicked();
	void on_pushButton_CIPSERVER_clicked();
	void on_pushButton_CIPMUX_2_clicked();
	void on_pushButton_CIPSEND_clicked();

	private:
	Ui::controlWindow *ui;
};

#endif // CONTROLWINDOW_H
