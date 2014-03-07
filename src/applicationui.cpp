#include "applicationui.hpp"

#include <iostream>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>

extern "C" {
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xmlschemas.h>
#include <libxml/xmlschemastypes.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
}

// XML namespace and url for xpath
#define NS_PREFIX "xs"
#define NS_URL "http://www.w3.org/2001/XMLSchema"

#define SHUTTLE_DATA "app/native/uwl-shuttle.xml"

using namespace bb::cascades;
using namespace std;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        				QObject(app)
{
	// prepare the localization
	m_pTranslator = new QTranslator(this);
	m_pLocaleHandler = new LocaleHandler(this);
	if(!QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()))) {
		// This is an abnormal situation! Something went wrong!
		// Add own code to recover here
		qWarning() << "Recovering from a failed connect()";
	}
	// initial load
	onSystemLanguageChanged();

	// Create scene document from main.qml asset, the parent is set
	// to ensure the document gets destroyed properly at shut down.
	QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

	qml->setContextProperty("app", this);

	// Create root object for the UI
	AbstractPane *root = qml->createRootObject<AbstractPane>();

	// Set created root object as the application scene
	app->setScene(root);

	// set status message of the button
	//this->setStatus(QString("Next bus information here"));

	//set a value of a component in QML from CPP
	//root->setProperty("tempText", "Hello World...");

}

void ApplicationUI::onSystemLanguageChanged()
{
	QCoreApplication::instance()->removeTranslator(m_pTranslator);
	// Initiate, load and install the application translation files.
	QString locale_string = QLocale().name();
	QString file_name = QString("cppandqml_%1").arg(locale_string);
	if (m_pTranslator->load(file_name, "app/native/qm")) {
		QCoreApplication::instance()->installTranslator(m_pTranslator);
	}
}

QString ApplicationUI::status()
{
	return m_status;
}

//! [6]
void ApplicationUI::setStatus(const QString status)
{

	m_status = status;
	emit statusChanged();

}

int ApplicationUI:: lookup_next_bus(QString destination, QString location, int cur_hour, int cur_min)
{
	/* Declare variables */
	int i;
	int size;
	int first_bus;
	int first;
	int second;
	int third;
	int fourth;
	int next_bus;
	char *dest;
	char *loc;
	char *last;
	char xpath_exp[1000];
	xmlDocPtr doc_database = NULL;

	// Converting Qstring to char
	QByteArray ba_dest = destination.toLocal8Bit();
	dest = ba_dest.data();

	QByteArray ba_loc = location.toLocal8Bit();
	loc = ba_loc.data();

	cout <<"received new look up request for" << endl;
	cout << "Destination " << dest << " Current Location " << loc <<endl;
	cout <<"Current Hour " << cur_hour << " Current minute " << cur_min <<endl;

	cout <<"checking in shuttle timetable database...\n";
	///////////////////// Initialising XML document ///////////////////


	xmlKeepBlanksDefault(0);
	doc_database = xmlReadFile(SHUTTLE_DATA, NULL, 0);
	if (doc_database == NULL) {
		printf("could not parse xml file %s", SHUTTLE_DATA);
	}

	xmlXPathContextPtr xpathp = NULL;
	xmlXPathObjectPtr result = NULL;

	///////////////////// Initialising XPATH ///////////////////

	/* setup xpath context */
	xpathp = xmlXPathNewContext(doc_database);
	if (xpathp == NULL) {
		printf("Error in xmlXPathNewContext.");
		xmlXPathFreeContext(xpathp);
		return -1;
	}

	if(xmlXPathRegisterNs(xpathp, (const xmlChar *)NS_PREFIX, (const xmlChar *)NS_URL) != 0) {
		printf("Error: unable to register NS.");
		xmlXPathFreeContext(xpathp);
		return -1;
	}

	///////////////////// Evaluating XPATH expression ///////////////////

	sprintf(xpath_exp, "/shuttle/%s/%s/*", dest, loc);

	cout <<"xpath exp "<< xpath_exp << endl;

	/* Evaluate xpath expression */
	result = xmlXPathEvalExpression((const xmlChar *)xpath_exp, xpathp);

	if (result == NULL) {
		printf("Error in xmlXPathEvalExpression.");
		xmlXPathFreeObject(result);
		xmlXPathFreeContext(xpathp);
		return -1;
	}

	///////////////////// Processing result ///////////////////

	/* check if xml doc consists of data with title matching value from movie_title variable" */
	if(xmlXPathNodeSetIsEmpty(result->nodesetval)) {
		xmlXPathFreeObject(result);
		xmlXPathFreeContext(xpathp);
		cout <<"the record does not exist on the database" <<endl;
		return -1;
	}
	else {

		size = result->nodesetval->nodeNr;
		xmlNodePtr cur = result->nodesetval->nodeTab[0];

		for(i = 0; i < size; i++)
		{
			if(!(xmlStrcmp(cur->name, (const xmlChar *)"first-bus")))
			{
				xmlChar *key;
				key = xmlNodeListGetString(doc_database, cur->xmlChildrenNode, 1);
				first_bus = atoi((char *)key);
				cout << "first bus hour" << first_bus <<endl;

				xmlFree(key);
			}

			if(!(xmlStrcmp(cur->name, (const xmlChar *)"first")))
			{
				xmlChar *key;
				key = xmlNodeListGetString(doc_database, cur->xmlChildrenNode, 1);
				first = atoi((char *)key);
				cout << "first bus min" << first <<endl;

				xmlFree(key);
			}
			if(!(xmlStrcmp(cur->name, (const xmlChar *)"last-bus")))
			{
				xmlChar *key;
				key = xmlNodeListGetString(doc_database, cur->xmlChildrenNode, 1);
				last = strdup((char *)key);
				cout << "last bus" << last <<endl;

				xmlFree(key);
			}

			cur = cur->next;
		}
	}
	///////////////////// Comparing search data with shuttle database ///////////////////

	check_next_bus(first_bus, first, cur_hour, cur_min, last);

	///////////////////// Freeing ///////////////////

	xmlFreeDoc(doc_database);
	return next_bus;

}

int ApplicationUI :: check_next_bus(int first_bus, int first, int cur_hour, int cur_min, char *last)
{
	int diff;
	int next = -1;
	char *last_bus;
	int last_bus_hour;
	char *last_bus_min;
	char msg[100];

	last_bus = strdup(last);

	strtok(last, ":");
	last_bus_hour = atoi(last);

	last_bus_min = strtok(NULL, ":");

	cout << last_bus_hour << last_bus_min <<endl;

	if(cur_hour < first_bus) {
		sprintf(msg, "First bus arrives at %d:%d", first_bus, first);
		this->setStatus(QString(msg));
	}
	else if(cur_hour > last_bus_hour) {
		sprintf(msg, "You missed the last bus at %s", last_bus);
		this->setStatus(QString(msg));
	}

	else if(cur_hour == first_bus && cur_min <= first) {
		next = first - cur_min;
		sprintf(msg, "Next bus arrives in %d minutes", next);
		this->setStatus(QString(msg));
	}
	else {
		diff = cur_min - first;

		if(diff > -15 && diff <= 0) {
			next = abs(diff);
		}
		else if(diff <= -15 && diff > -30) {
			next = abs(diff) - 15;
		}
		else if(diff <= -30 && diff > -45) {
			next = abs(diff) - 30;
		}
		else if(diff > 0 && diff <= 15) {
			next = 15 - diff;
		}
		else if(diff > 15 && diff <= 30) {
			next = 30 - diff;
		}
		else if(diff > 30 && diff <= 45) {
			next = 45 - diff;
		}
		else if(diff > 45) {
			next = 60 - diff;
		} else if(diff <= -45) {
			next = abs(diff) - 45;
		}


		sprintf(msg, "Next bus arrives in %d minutes", next);
		this->setStatus(QString(msg));
	}
	cout << msg <<endl;

	return next;
}


