#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>

namespace bb
{
    namespace cascades
    {
        class Application;
        class LocaleHandler;
    }
}

class Test;

class QTranslator;

/*!
 * @brief Application object
 *
 *
 */

class ApplicationUI : public QObject
{
    Q_OBJECT
public:
    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI() { }

    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY statusChanged);
    Q_INVOKABLE   void callme();
    Q_INVOKABLE int lookup_next_bus(QString destination, QString location, int cur_hour, int cur_min);
    int check_next_bus(int first_bus, int first, int cur_hour, int cur_min, char *last);

public  Q_SLOTS:
    QString status();
    void setStatus(QString str);

    Q_SIGNALS:
        // The change notification signal for the property
        void statusChanged();

private slots:
    void onSystemLanguageChanged();
private:
    QString m_status;
    QTranslator* m_pTranslator;
    bb::cascades::LocaleHandler* m_pLocaleHandler;
};

#endif /* ApplicationUI_HPP_ */
