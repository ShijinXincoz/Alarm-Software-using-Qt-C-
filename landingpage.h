#ifndef LANDINGPAGE_H
#define LANDINGPAGE_H

#include <QWidget>
#include <QLabel>
#include <QTime>
#include <QTimer>
#include "debug.h"
#include "alarm.h"
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QCloseEvent>
#include <QSharedMemory>
#include "editalarm.h"
#include <QFormLayout>

class MainPage : public QWidget
{
public:
	MainPage ();
	QWidget *contantPane;
	class LandingPage *landingPage;
	class Alarm *createAlarm;

protected:
	void closeEvent (QCloseEvent *e);
private:
	QSharedMemory sharedMemory;
};


class LandingPage : public QWidget
{
	Q_OBJECT
public:
	LandingPage (class MainPage *parent);
	QLabel *time, *currentTune, *changeText, *notification;
	QLabel *value [5], *change [5];
	QPushButton *create, *edit, *tune;
	QTimer *timer;
	QSqlDatabase database;
	QSqlQuery *query;
	QFormLayout formLayout;
	QWidget *layout;
	QList <class AlarmRecord*> list;
	class MainPage *parent;
public slots:
	void refreshTime ();
	void createAlarm ();
	void editAlarm ();
	void changeTune ();
	void notificationTimer ();
protected:
	bool eventFilter (QObject *, QEvent *);
private:
	static int count;
	bool readDatabase ();
	QString getCurrentTune ();
	void activeAlarm ();
};

#endif // LANDINGPAGE_H
