#include <QIntValidator>
#include "landingpage.h"
#include <QSqlQuery>
#include <QEvent>
#include <QTime>
#include <QDir>
#include "alarm.h"
#include "debug.h"

Alarm::Alarm (MainPage *parent)
{
	this->setFixedSize (500, 300);
	this->parent = parent;
	this->setParent (parent->contantPane);

//	SET CURRENT DIRECTORY
	QString dir = QDir::homePath () + FILE_APPDIR;
	QDir().mkpath (dir);
	QDir::setCurrent (dir);

//	INITIALIZE DATABASE CONNECTION
	this->db = QSqlDatabase::addDatabase ("QSQLITE", "alarm.db");
	this->db.setDatabaseName ("alarm.db");
	if (this->db.open() == false)
		EventLog (1, "Error Open Database");

//	CREATE NEW TABLE
	this->query = new QSqlQuery (this->db);
	if (query->exec ("CREATE TABLE IF NOT EXISTS alarm("\
					"id INTEGER PRIMARY KEY AUTOINCREMENT,"\
					"hh	    TEXT    NOT NULL,"\
					"mm	    TEXT    NOT NULL,"\
					"zone	    TEXT    NOT NULL,"\
					"sun	    TEXT    NOT NULL,"\
					"mon	    TEXT    NOT NULL,"\
					"tue	    TEXT    NOT NULL,"\
					"wen	    TEXT    NOT NULL,"\
					"thr	    TEXT    NOT NULL,"\
					"fri	    TEXT    NOT NULL,"\
					"sat	    TEXT    NOT NULL);") == false)
	{
		EventLog (1, "Error Creation table");
	}

//	SET LABEL TO DISPLAY CURRENT TIME
	this->time = new QLabel (this);
	this->time->setText (QTime::currentTime().toString ());
	this->time->setGeometry (this->width()/2-125, 10, 250, 80);
	this->time->setFixedSize (250, 80);
	this->time->setStyleSheet ("color:black; font-size: 60px");
	this->time->setAlignment (Qt::AlignHCenter);

//	SET TITLE
	this->title = new QLabel (this);
	this->title->setText ("Create Alarm");
	this->title->setStyleSheet ("color:black; font-size:20px");
	this->title->setGeometry (this->width()/2-75, 105, 150, 30);

//	SET TIMER TO REFRESH TIME
	this->timer = new QTimer;
	connect (this->timer, SIGNAL (timeout ()), SLOT (refreshTimer ()));
	this->timer->start (1000);

//	SET REPEAT ALARM OPTION
	this->repeat = new QLabel (this);
	this->repeat->setText ("Repeat");
	this->repeat->setGeometry (30, 140, 80, 25);
	this->repeat->setStyleSheet ("color:#404040; font-size:16px;");

//	SET SUNDAY
	this->sun = new QToolButton (this);
	this->sun->setText ("Sunday");
	this->sun->setGeometry (10, 170, 60, 20);
	this->sun->installEventFilter (this);
	this->sun->setCheckable (false);

//	SET MONDAY
	this->mon = new QToolButton (this);
	this->mon->setText ("Monday");
	this->mon->setGeometry (80, 170, 60, 20);
	this->mon->installEventFilter (this);
	this->mon->setCheckable (false);

//	SET TUESDAY
	this->tue = new QToolButton (this);
	this->tue->setText ("Tuesday");
	this->tue->setGeometry (150, 170, 60, 20);
	this->tue->installEventFilter (this);
	this->tue->setCheckable (false);

//	SET WEDNESDAY
	this->wen = new QToolButton (this);
	this->wen->setText ("Wednesday");
	this->wen->setGeometry (220, 170, 60, 20);
	this->wen->installEventFilter (this);
	this->wen->setCheckable (false);

//	SET THURSDAY
	this->thr = new QToolButton (this);
	this->thr->setText ("Thursday");
	this->thr->setGeometry (290, 170, 60, 20);
	this->thr->installEventFilter (this);
	this->thr->setCheckable (false);

//	SET FRIDAY
	this->fri = new QToolButton (this);
	this->fri->setText ("Friday");
	this->fri->setGeometry (360, 170, 60, 20);
	this->fri->installEventFilter (this);
	this->fri->setCheckable (false);

//	SET SATURDAY
	this->sat = new QToolButton (this);
	this->sat->setText ("Saturday");
	this->sat->setGeometry (430, 170, 60, 20);
	this->sat->installEventFilter (this);
	this->sat->setCheckable (false);

//	SET HOUR
	this->hh = new QLineEdit (this);
	this->hh->setGeometry (this->width()/2-63,200,40, 50);
	this->hh->setStyleSheet ("font-size:30px");
	this->hh->setMaxLength(2);
	this->hh->setFrame (false);
	this->hh->setValidator( new QIntValidator (0, 12, this));
	this->hh->setText ("00");
	this->hh->installEventFilter (this);

//	SET COLON BETWEEN HH & MM
	this->colon = new QLabel (this);
	this->colon->setText (":");
	this->colon->setStyleSheet ("font-size:30px;");
	this->colon->setGeometry (this->width()/2-25, 200, 10,50);

//	SET MINUTE
	this->mm = new QLineEdit (this);
	this->mm->setGeometry (this->width()/2-15,200, 40, 50);
	this->mm->setStyleSheet ("font-size:30px");
	this->mm->setMaxLength (2);
	this->mm->setValidator (new QIntValidator (0,59, this));
	this->mm->setFrame (false);
	this->mm->installEventFilter (this);
	this->mm->setText ("00");

//	SET AM/PM
	this->zone = new QComboBox (this);
	this->zone->addItem ("AM");
	this->zone->addItem ("PM");
	this->zone->setGeometry (this->width()/2+25, 205, 40,40);
	this->zone->setStyleSheet ("font-size:16px");
	this->zone->setFrame (false);

//	SET SAVE BUTTON
	this->save = new QPushButton (this);
	this->save->setText ("Save");
	this->save->setGeometry (150, 250, 100, 30);
	this->save->setStyleSheet ("font-size:20px");
	connect (this->save, SIGNAL (clicked ()), SLOT (clickSave ()));

//	SET CANCEL BUTTON
	this->cancel = new QPushButton (this);
	this->cancel->setText ("Cancel");
	this->cancel->setGeometry (this->width()-250, 250, 100,30);
	this->cancel->setStyleSheet ("font-size:20px");
	connect (this->cancel, SIGNAL (clicked ()), SLOT (clickCancel ()));

	this->setStyleSheet (
		"QToolButton {color: #000; background: #fff;}"\
		"QToolButton:pressed {color: #fff; background: #000;}"\
				"QToolButton:true {color:red;}"\
				"background-color:white;");
	this->show ();
}

void Alarm::clickCancel ()
{
	EventLog (1);
	this->parent->landingPage->show ();
	delete this;
}

void Alarm::clickSave ()
{
	if (query->exec (QString ("INSERT INTO alarm (hh,mm,zone,sun,mon,tue,wen,thr,fri,sat) VALUES (" +
							  this->hh->text()+ ","+ this->mm->text() + ","+ ((this->zone->currentText() == "AM")? "\"AM\"" : "\"PM\"")+
							 "," + (this->sun->isCheckable()?"1":"0") + "," + (this->mon->isCheckable()?"1":"0") + "," +
							 (this->tue->isCheckable()?"1":"0")+ "," + (this->wen->isCheckable()?"1":"0") + "," + (this->thr->isCheckable()?"1":"0")+
							 "," + (this->fri->isCheckable()?"1":"0") + ","+ (this->sat->isCheckable()?"1":"0")+");")) == false)
		EventLog (1, "Error insert table");
}

bool Alarm::eventFilter (QObject *obj, QEvent *event)
{
	if (obj == this->hh && event->type () == QEvent::MouseButtonPress)
	{
		EventLog (1);
		this->hh->setText ("");
		return true;
	}

	if (obj == this->mm && event->type () == QEvent::MouseButtonPress)
	{
		EventLog (1);
		this->mm->setText ("");
		return true;
	}

	if (obj == this->sun && event->type () == QEvent::MouseButtonPress)
	{
		this->toggleColor (this->sun);
		return true;
	}

	if (obj == this->mon && event->type () == QEvent::MouseButtonPress)
	{
		this->toggleColor (this->mon);
		return true;
	}

	if (obj == this->tue && event->type () == QEvent::MouseButtonPress)
	{
		this->toggleColor (this->tue);
		return true;
	}

	if (obj == this->wen && event->type () == QEvent::MouseButtonPress)
	{
		this->toggleColor (this->wen);
		return true;
	}

	if (obj == this->thr && event->type () == QEvent::MouseButtonPress)
	{
		this->toggleColor (this->thr);
		return true;
	}

	if (obj == this->fri && event->type () == QEvent::MouseButtonPress)
	{
		this->toggleColor (this->fri);
		return true;
	}

	if (obj == this->sat && event->type () == QEvent::MouseButtonPress)
	{
		this->toggleColor (this->sat);
		return true;
	}
	return false;
}

void Alarm::toggleColor (QToolButton *button)
{
	if (button->isCheckable() == false)
	{
		button->setStyleSheet ("background-color:blue; color:black");
		button->setCheckable (true);
	}
	else
	{
		button->setCheckable (false);
		button->setStyleSheet ("color: #000; background: #fff;");
	}
}

void Alarm::refreshTimer ()
{
	this->time->setText (QTime::currentTime().toString ());
}
