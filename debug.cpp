#include <QTextEdit>
#include "debug.h"

#ifdef EVENTLOG_ENABLE

Logger* Logger::logger = NULL;

Event::Event (char type, QString func, QString msg, QString info)
{
	connect (
		this,
		SIGNAL(event(char,QString,QString,QString)),
		Logger::logger,
		SLOT(onEvent(char,QString,QString,QString)),
		Qt::QueuedConnection);
	emit event (type, func, msg, info);
}

Logger::Logger ()
{
	this->edit = new QTextEdit;
	this->edit->setLineWrapMode (QTextEdit::NoWrap);
	this->edit->setReadOnly (true);
	this->edit->setMinimumSize (160*7, 90*7);
	this->edit->move (200, 100);
	this->edit->show ();
	this->edit->setWindowTitle ("Reelmonk Realtime Log Viewer");
	this->edit->setStyleSheet ("font-family:consolas; font-size:24px; background:#001");
	onEvent (1, Q_FUNC_INFO, "Realtime Event Viewer activated", "");
}

void Logger::onEvent (char type, QString func, QString msg, QString info)
{
	QString string;
	const char *color [] = {"#f66", "#eef", "#ddd", "#bbb", "#aaa"};
	const char *tags [] = {"[fail]", "[done]", "[....]", "[ at ]", "[info]"};

	edit->setTextColor (color [type]);
	if (!info.isEmpty ())
		info.prepend ("- ");

	short colon = func.indexOf (":");
	for (short pos = func.indexOf (" "); pos < colon && pos > 0; pos = func.indexOf (" "))
		func = func.mid (pos+1);
	func = func.mid (0, func.indexOf ("("));
	func.replace ("::", ": ");

	switch (type)
	{
	case 0:
	case 1:
	case 2:
		string = QString ("%1 %2 | %3 %4").arg (tags[type], func, msg, info);
		break;
	case 3:
		string = QString ("%1 %2 %3 %4").arg(tags[type], func, msg, info);
		break;
	case 4:
		string = QString ("%1 %2 %3").arg(tags[type], func, msg);
		break;
	}
	edit->append (string);
}

void print (QString event, long code)
{
	EventLog (4, event, QString::number (code));
}

void print (QString event, QString msg)
{
	EventLog (4, event, msg);
}

#endif
