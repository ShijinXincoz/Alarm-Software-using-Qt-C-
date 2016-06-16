#ifndef DEBUG_H
#define DEBUG_H
#include "main.h"
#include <QObject>

#define FUNC1(x)		Event(x,Q_FUNC_INFO)
#define FUNC2(x,y)		Event(x,Q_FUNC_INFO,y)
#define FUNC3(x,y,z)	Event(x,Q_FUNC_INFO,y,z)

#define N_ARGS_IMPL2(_1, _2, _3, count,  ...) count
#define N_ARGS_IMPL1(args) N_ARGS_IMPL2 args
#define N_ARGS(...) N_ARGS_IMPL1((__VA_ARGS__, 3, 2, 1, 0))

#define CHOOSER3(count) FUNC##count
#define CHOOSER2(count) CHOOSER3(count)
#define CHOOSER1(count) CHOOSER2(count)
#define CHOOSER0(count) CHOOSER1(count)

#define TEST_GLUE(x, y) x y
#define EventLog(...) TEST_GLUE(CHOOSER0(N_ARGS(__VA_ARGS__)), (__VA_ARGS__))

class Event : public QObject
{
	Q_OBJECT
public:
	Event (char, QString, QString = QString (), QString = QString ());
signals:
	void event (char, QString, QString, QString);
};

class Logger : public QObject
{
	Q_OBJECT
public:
	Logger ();
	class QTextEdit *edit;
	static Logger *logger;
public slots:
	void onEvent (char, QString, QString, QString);
};

void print (QString, long);
void print (QString, QString = QString ());

#endif
