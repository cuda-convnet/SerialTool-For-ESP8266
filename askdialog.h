#ifndef ASKDIALOG_H
#define ASKDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QVector>

namespace Ui {
class askDialog;
}

class askDialog : public QDialog
{
	Q_OBJECT

public:
	explicit askDialog(QWidget *parent = 0);
	int *p, *chl, *ecn, *maxConnect, *TCPMode, *port;
	bool *ssidHidden;
	std::string *ssid, *password, *message, *deviceNO;
	~askDialog();
	void setWidget_CWMODE(int *index);
	void setWidget_CWJAP(std::string *a, std::string *b);
	void setWidget_CWSAP(std::string *a, std::string *b, int *c, int *d, int *e, bool *f);
	void setWidget_CIPSERVER(int *a, int *b);
	void setWidget_CIPSEND(QVector<int> &a, std::string *b, std::string *c);

private slots:
	void on_pushButton_clicked();
	void on_pushButton_2_clicked();

private:
	Ui::askDialog *ui;
};

#endif // ASKDIALOG_H
