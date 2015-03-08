#ifndef GUI_EXCEPTION_H
#define GUI_EXCEPTION_H

#include <QObject>
#include <stdexcept>

class GuiException : public std::exception {
public:
	GuiException(QObject* object) : object(object) {}
	QObject* GetObject() const {return object;}
private:
	QObject* object;
};

#endif /* GUI_EXCEPTION_H */
