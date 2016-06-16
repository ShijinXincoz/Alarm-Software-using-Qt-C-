#include "alarm.h"
#include "debug.h"
#include "editalarm.h"
#include "landingpage.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Logger::logger = new Logger;
	//LandingPage page;
	//page.show();
	MainPage page;
	page.show ();
	//Alarm w (&page);
	//w.show();
	return a.exec();
}
