#include <QtMultimedia/QMediaPlayer>
#include "editalarm.h"
#include "landingpage.h"
#include "alarm.h"
#include "main.h"
#include <QDate>
#include <QFile>
#include <QDir>

EditAlarm::EditAlarm (AlarmRecord *record, MainPage *parent)
{
	this->setFixedSize (500, 300);
	this->parent = parent;
	this->setParent (this->parent->contantPane);
	this->id = record->no;
	delete parent->landingPage;
	EventLog (1, "edit alarm opens");

//	SET CURRENT DIRECTORY
	QString dir = QDir::homePath () + FILE_APPDIR;
	QDir().mkpath (dir);
	QDir::setCurrent (dir);

//	INITIALIZE DATABASE CONNECTION
	this->db = QSqlDatabase::addDatabase ("QSQLITE", "alarm.db");
	this->db.setDatabaseName ("alarm.db");
	if (this->db.open() == false)
		EventLog (1, "Error Open Database");
	this->query = new QSqlQuery (this->db);

//	SET LABEL TO DISPLAY CURRENT TIME
	this->time = new QLabel (this);
	this->time->setText (QTime::currentTime().toString ());
	this->time->setGeometry (this->width()/2-125, 10, 250, 80);
	this->time->setFixedSize (250, 80);
	this->time->setStyleSheet ("color:black; font-size: 60px");
	this->time->setAlignment (Qt::AlignHCenter);

//	SET TITLE
	this->title = new QLabel (this);
	this->title->setText ("Edit Alarm");
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
	if (record->sun)
		this->sun->setCheckable (true);

//	SET MONDAY
	this->mon = new QToolButton (this);
	this->mon->setText ("Monday");
	this->mon->setGeometry (80, 170, 60, 20);
	this->mon->installEventFilter (this);
	this->mon->setCheckable (false);
	if (record->mon)
		this->mon->setCheckable (true);

//	SET TUESDAY
	this->tue = new QToolButton (this);
	this->tue->setText ("Tuesday");
	this->tue->setGeometry (150, 170, 60, 20);
	this->tue->installEventFilter (this);
	this->tue->setCheckable (false);
	if (record->tue)
		this->tue->setCheckable (true);

//	SET WEDNESDAY
	this->wen = new QToolButton (this);
	this->wen->setText ("Wednesday");
	this->wen->setGeometry (220, 170, 60, 20);
	this->wen->installEventFilter (this);
	this->wen->setCheckable (false);
	if (record->wen)
		this->wen->setCheckable (true);

//	SET THURSDAY
	this->thr = new QToolButton (this);
	this->thr->setText ("Thursday");
	this->thr->setGeometry (290, 170, 60, 20);
	this->thr->installEventFilter (this);
	this->thr->setCheckable (false);
	if (record->thr)
		this->thr->setCheckable (true);

//	SET FRIDAY
	this->fri = new QToolButton (this);
	this->fri->setText ("Friday");
	this->fri->setGeometry (360, 170, 60, 20);
	this->fri->installEventFilter (this);
	this->fri->setCheckable (false);
	if (record->fri)
		this->fri->setCheckable (true);

//	SET SATURDAY
	this->sat = new QToolButton (this);
	this->sat->setText ("Saturday");
	this->sat->setGeometry (430, 170, 60, 20);
	this->sat->installEventFilter (this);
	this->sat->setCheckable (false);
	if (record->sat)
		this->sat->setCheckable (true);

//	SET HOUR
	this->hh = new QLineEdit (this);
	this->hh->setGeometry (this->width()/2-63,200,40, 50);
	this->hh->setStyleSheet ("font-size:30px");
	this->hh->setMaxLength(2);
	this->hh->setFrame (false);
	this->hh->setValidator( new QIntValidator (0, 12, this));
	this->hh->setText (QString::number(record->hh));
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
	this->mm->setText (QString::number(record->mm));

//	SET AM/PM
	this->zone = new QComboBox (this);
	this->zone->addItem ("AM");
	this->zone->addItem ("PM");
	this->zone->setGeometry (this->width()/2+25, 205, 40,40);
	this->zone->setStyleSheet ("font-size:16px");
	this->zone->setFrame (false);
	if (record->zone == "AM")
		this->zone->setCurrentIndex(0);
	else
		this->zone->setCurrentIndex (1);

//	SET SAVE BUTTON
	this->save = new QPushButton (this);
	this->save->setText ("Save");
	this->save->setGeometry (150, 250, 100, 30);
	this->save->setStyleSheet ("font-size:20px");
	connect (this->save, SIGNAL (clicked ()), SLOT (clickSave ()));

//	SET CANCEL BUTTON
	this->dlt = new QPushButton (this);
	this->dlt->setText ("Delete");
	this->dlt->setGeometry (this->width()-250, 250, 100,30);
	this->dlt->setStyleSheet ("font-size:20px");
	connect (this->dlt, SIGNAL (clicked ()), SLOT (clickDelete()));

	this->setStyleSheet (
		"QToolButton {color: #000; background: #fff;}"\
		"QToolButton:pressed {color: #fff; background: #000;}"\
				"QToolButton:true {color:red;}"\
				"background-color:white;");
	this->show ();
}

void EditAlarm::clickDelete ()
{
	EventLog (1);
	if (query->exec (QString("DELETE FROM ALARM WHERE ID = %1").arg(this->id)))
	this->parent->landingPage = new LandingPage (this->parent);
	delete this;
}

void EditAlarm::clickSave ()
{
	query->exec (QString("DELETE FROM ALARM WHERE ID = %1").arg(this->id));
	if (query->exec (QString ("INSERT INTO alarm (hh,mm,zone,sun,mon,tue,wen,thr,fri,sat) VALUES (" +
							  this->hh->text()+ ","+ this->mm->text() + ","+ ((this->zone->currentText() == "AM")? "\"AM\"" : "\"PM\"")+
							 "," + (this->sun->isCheckable()?"1":"0") + "," + (this->mon->isCheckable()?"1":"0") + "," +
							 (this->tue->isCheckable()?"1":"0")+ "," + (this->wen->isCheckable()?"1":"0") + "," + (this->thr->isCheckable()?"1":"0")+
							 "," + (this->fri->isCheckable()?"1":"0") + ","+ (this->sat->isCheckable()?"1":"0")+");")) == false);
		EventLog (1, "Error insert table");
	this->parent->landingPage = new LandingPage (this->parent);
		delete this;
}

bool EditAlarm::eventFilter (QObject *obj, QEvent *event)
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

void EditAlarm::toggleColor (QToolButton *button)
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

void EditAlarm::refreshTimer ()
{
	this->time->setText (QTime::currentTime().toString ());
}


AlarmRecord::AlarmRecord (int no, int hh, int mm, QString zone, bool sun, bool mon,
						  bool tue, bool wen, bool thr, bool fri, bool sat)
{
	this->no = no;
	this->hh = hh;
	this->mm = mm;
	this->zone = zone;
	this->sun = sun;
	this->mon = mon;
	this->tue = tue;
	this->wen = wen;
	this->thr = thr;
	this->fri = fri;
	this->sat = sat;
	this->status = true;
	this->start ();
}

void AlarmRecord::run ()
{
	while (this->status)
	{		
		if (this->sun || this->mon || this->tue || this->wen || this->thr
				|| this->fri || this->sat)
		{
			QString date = QDate::longDayName(QDate::currentDate().dayOfWeek());
			if ((date.contains ("sun") && this->sun) || (date.contains ("mon") && this->mon) ||
					(date.contains ("tue") && this->tue) || (date.contains ("wed") && this->wen) ||
					(date.contains ("thu") && this->thr) || (date.contains ("fri") && this->fri) ||
					(date.contains ("sat") && this->sat))
			{
				int hr = QTime::currentTime().hour(); QString zon;
				if (hr > 12 )
				{
					hr = hr - 12;
					zon = "PM";
				}
				else
					zon = "AM";
				if ((hr == this->hh ) && (QTime::currentTime().minute() == this->mm) && (this->zone == zon))
				{
					EventLog (1, "bell");
					QMediaPlayer *player = new QMediaPlayer;
					QFile file ("tune.txt");
					file.open (QFile::ReadOnly);
					player->setMedia(QUrl::fromLocalFile(file.readAll()));
					file.close ();
					player->setVolume(50);
					player->play();
				}
			}

		}
		QThread::sleep (1);
	}
}
