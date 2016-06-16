#ifndef ALARM_H
#define ALARM_H

#include <QToolButton>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QLineEdit>
#include <QComboBox>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QTime>


class Alarm : public QWidget
{
	Q_OBJECT
public:
	Alarm (class MainPage *parent);
	QLabel *time, *title, *repeat, *colon;
	QTimer *timer;
	QString currentTime;
	QLineEdit *hh, *mm;
	QComboBox *zone;
	QToolButton *sun, *mon, *tue, *wen, *thr, *fri, *sat;
	QPushButton *save, *cancel;
	QSqlDatabase db;
	QSqlQuery *query;
	class MainPage *parent;
private slots:
	void refreshTimer ();
	void clickSave ();
	void clickCancel ();
protected:
	bool eventFilter (QObject *, QEvent *);
private:
	void toggleColor (QToolButton *);
};

#endif // ALARM_H
