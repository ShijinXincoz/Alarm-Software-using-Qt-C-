#ifndef EDITALARM_H
#define EDITALARM_H

#include <QToolButton>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QLineEdit>
#include <QComboBox>
#include <QThread>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QTime>

class EditAlarm : public QWidget
{
	Q_OBJECT
public:
	EditAlarm (class AlarmRecord *, class MainPage *parent);
	int id;
	QLabel *time, *title, *repeat, *colon;
	QTimer *timer;
	QString currentTime;
	QLineEdit *hh, *mm;
	QComboBox *zone;
	QToolButton *sun, *mon, *tue, *wen, *thr, *fri, *sat;
	QPushButton *save, *dlt;
	QSqlDatabase db;
	QSqlQuery *query;
	class MainPage *parent;
public slots:
	void refreshTimer ();
	void clickSave ();
	void clickDelete ();
protected:
	bool eventFilter (QObject *, QEvent *);
private:
	void toggleColor (QToolButton *);
};

class AlarmRecord : public QThread
{
public:
	AlarmRecord (int, int, int, QString, bool, bool, bool, bool, bool, bool, bool);
	int no,hh,mm;
	QString zone;
	bool sun, mon, tue, wen, thr, fri, sat, status;
private:
	void run ();
};

#endif // EDITALARM_H
