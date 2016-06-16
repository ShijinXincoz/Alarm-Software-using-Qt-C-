#include <QtMultimedia/qmediaplayer.h>
#include "landingpage.h"
#include <QFileDialog>
#include <windows.h>
#include <QProcess>
#include <QDir>
int LandingPage::count = 0;

MainPage::MainPage ()
{
//	CHECK IF THE PROCESS IS ALREADY RUNNING
	this->sharedMemory.setKey ("alarm");
	if (this->sharedMemory.attach ())
		EventLog (1, "Process is already running");

//	IF THE PROCESS IS ALREADY RUNNING TERMINATE IT
	if (sharedMemory.isAttached())
	{
		sharedMemory.lock ();
		EventLog (1,(char*) sharedMemory.data());
		QString pid = (char*) sharedMemory.data();
		sharedMemory.unlock ();
		HANDLE handy;
		handy =OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE, TRUE,pid.toInt());
		TerminateProcess(handy,0);
	}

//	STORE THE CURRENT PROCESS ID
	EventLog (1, sharedMemory.errorString());
	QString from =  QString::number (GetCurrentProcessId ());
	int size = sizeof (from);
	if (sharedMemory.create (size) == false)
	{
		EventLog (1, "Error creation of shared memory");
	}
	sharedMemory.lock();
	char *to = (char*) sharedMemory.data();
	memcpy(to, from.toLatin1().data(), size);
	EventLog (1, (char*)sharedMemory.data());
	sharedMemory.unlock ();

	this->setFixedSize (500, 320);
	this->contantPane = new QWidget (this);
	this->landingPage = new LandingPage (this);
}

void MainPage::closeEvent (QCloseEvent *event)
{
	EventLog (1);
	event->ignore();
	this->hide ();
}

LandingPage::LandingPage (MainPage *parent)
{
	this->setFixedSize (500, 320);
	this->parent = parent;
	this->setParent (this->parent->contantPane);

//	SHOW TIME
	this->time = new QLabel (this);
	this->time->setText (QTime::currentTime().toString ());
	this->time->setGeometry (this->width()/2-165, 10, 330, 80);
	this->time->setStyleSheet ("font-size:80px; color:black");

//	SET TIMER
	this->timer = new QTimer (this);
	connect (this->timer, SIGNAL (timeout ()), SLOT (refreshTime()));
	this->timer->start (1000);

//	SET CREATE BUTTON
	this->create = new QPushButton (this);
	this->create->setText ("Create Alarm");
	this->create->setStyleSheet ("font-size:20px; color:black");
	this->create->setGeometry (100, 100, 140, 30);
	connect (this->create, SIGNAL (clicked ()), SLOT (createAlarm()));

//	SET EDIT BUTTON
	this->edit = new QPushButton (this);
	this->edit->setText ("Edit Alarm");
	this->edit->setStyleSheet ("font-size:20px; color:black");
	this->edit->setGeometry (this->width()-240, 100, 140, 30);
	connect (this->edit, SIGNAL (clicked ()), SLOT (editAlarm ()));
	this->readDatabase ();

//	SET CHANGE TUNE TEXT
	this->changeText = new QLabel (this);
	this->changeText->setText ("Change Tune");
	this->changeText->setGeometry (100, 140, 120, 20);
	this->changeText->setStyleSheet ("font-size:16px; color:black;");

//	SET AUDIO FILE
	this->tune = new QPushButton (this);
	this->tune->setText ("Browse");
	this->tune->setGeometry (this->width()/2+60,165,80,25);
	this->tune->setStyleSheet ("font-size:16px; color:black");
	connect (this->tune, SIGNAL (clicked ()), SLOT (changeTune ()));

//	SET CURRENT TUNE
	this->currentTune =new QLabel (this);
	this->currentTune->setGeometry (100,165, 200,30);
	this->currentTune->setStyleSheet ("font-size:12px; color:black;");
	this->currentTune->setText (this->getCurrentTune ());

//	SET ACTIVE ALARM
	this->layout = new QWidget (this);
	this->layout->setGeometry (100, 190, 300, 100);
	this->formLayout.setParent (this->layout);
	this->layout->setLayout (&this->formLayout);
	this->activeAlarm ();
	this->formLayout.installEventFilter (this);

//	SET NOTIFICATION TEXT
	this->notification = new QLabel (this);
	this->notification->setGeometry (100,300 ,300,20);
	this->notification->setText ("Notify me");
	this->notification->setStyleSheet ("font-size:16px; color:red" );
	this->notification->setAlignment (Qt::AlignCenter);
	this->show();
}

void LandingPage::activeAlarm ()
{
	for (int i = 0; i <this->list.size(); i++)
	{
		this->value [i] = new QLabel (this);
		this->value [i]->setText (QString ("%1:%2:%3").arg(QString::number(this->list.at(i)->hh),
												 QString::number(this->list.at(i)->mm)
												 , QString(this->list.at(i)->zone)));
		this->value[i]->setGeometry (0, 0, 150, 15);
		this->value[i]->setStyleSheet ("color:black; font-size: 12px");
		this->value[i]->setAlignment (Qt::AlignCenter);

		this->change [i] = new QLabel (this);
		this->change[i]->setCursor (Qt::PointingHandCursor);
		this->change[i]->setText ("Edit");
		this->change[i]->setGeometry (0,0, 150,15);
		this->change [i]->setStyleSheet ("color:black; font-size: 12px");
		this->change[i]->setAlignment (Qt::AlignCenter);
		this->change [i]->installEventFilter (this);
		this->formLayout.addRow (this->value [i], this->change [i]);
	}
}

bool LandingPage::eventFilter (QObject *obj, QEvent *event)
{
	if (obj == this->change [0]  && event->type() == QEvent::MouseButtonPress)
	{
		EventLog (1, "first");
		this->hide ();
		EditAlarm *edit = new EditAlarm (this->list.at (0), this->parent);
		return true;
	}

	if (obj == this->change [1]  && event->type() == QEvent::MouseButtonPress)
	{
		EventLog (1, "2nd");
		this->hide ();
		EditAlarm *edit = new EditAlarm (this->list.at (1), this->parent);
		return true;
	}

	if (obj == this->change [2]  && event->type() == QEvent::MouseButtonPress)
	{
		EventLog (1, "3rd");
		this->hide ();
		EditAlarm *edit = new EditAlarm (this->list.at (2), this->parent);
		return true;
	}

	if (obj == this->change [3]  && event->type() == QEvent::MouseButtonPress)
	{
		EventLog (1,"4TH");
		this->hide ();
		EditAlarm *edit = new EditAlarm (this->list.at (3), this->parent);
		return true;
	}

	if (obj == this->change [4]  && event->type() == QEvent::MouseButtonPress)
	{
		EventLog (1, "5th");
		this->hide ();
		EditAlarm *edit = new EditAlarm (this->list.at (4), this->parent);
		return true;
	}
}

QString LandingPage::getCurrentTune ()
{
	QFile file ("tune.txt");
	file.open (QFile::ReadOnly);
	QString fileName = file.readAll();
	file.close ();
	return fileName;
}

void LandingPage::changeTune ()
{
	QString fileName = QFileDialog::getOpenFileName ();
	QFile file ("tune.txt");
	file.open (QFile::WriteOnly | QFile::Text);
	file.write (fileName.toLatin1(), fileName.length());
	file.close ();
	this->currentTune->setText (fileName);
}

void LandingPage::notificationTimer ()
{
	this->notification->setText (" ");
}

void LandingPage::createAlarm ()
{
	EventLog (1);
	count++;
	if (count > 5)
	{
		count--;
		this->notification->setText ("Maximun Number Reached");
		QTimer::singleShot (5000, this, SLOT (notificationTimer()));
		return;
	}
	this->hide ();
	Alarm *alarm = new Alarm (this->parent);
}

void LandingPage::editAlarm ()
{
	EventLog (1);
}

void LandingPage::refreshTime ()
{
	this->time->setText (QTime::currentTime().toString ());
}

bool LandingPage::readDatabase ()
{
//	SET CURRENT DIRECTORY
	QString dir = QDir::homePath() + FILE_APPDIR;
	QDir().mkpath (dir);
	QDir::setCurrent (dir);
//	INITIALIZE DATABASE CONNECTION
	this->database = QSqlDatabase::addDatabase ("QSQLITE", "alarm");
	this->database.setDatabaseName ("alarm.db");
	this->database.open ();

//	READ DATA FROM DATABASE
	this->query = new QSqlQuery (this->database);
	this->query->exec ("select * from alarm");
	while (this->query->next() != NULL )
	{
		int no = this->query->value(0).toInt ();
		int hh = this->query->value(1).toInt ();
		int mm = this->query->value(2).toInt ();
		QString zone = this->query->value (3).toString ();
		bool sun = this->query->value (4).toBool ();
		bool mon = this->query->value (5).toBool ();
		bool tue = this->query->value (6).toBool ();
		bool wen = this->query->value (7).toBool ();
		bool thr = this->query->value (8).toBool ();
		bool fri = this->query->value (9).toBool ();
		bool sat = this->query->value (10).toBool ();

		AlarmRecord *record = new AlarmRecord (no, hh, mm, zone, sun, mon,
											   tue, wen, thr, fri, sat);
		list.append (record);
	}
}
