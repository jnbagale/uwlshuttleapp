APP_NAME = UWLShuttleApp

CONFIG += qt warn_on cascades10

simulator {
 INCLUDEPATH += \
		${QNX_TARGET}/x86/usr/include
	LIBS+=-L${PWD}library-dependencies/x86 -lxml2 
}

device {
INCLUDEPATH += \
	${QNX_TARGET}/armle-v7/usr/include
LIBS+=-L${PWD}library-dependencies/armle-v7 -lxml2
}

include(config.pri)
