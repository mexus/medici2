#ifndef GUI_HELPERS_GUI_EXCEPTION_H
#define GUI_HELPERS_GUI_EXCEPTION_H

#include <QObject>
#include <stdexcept>

class GuiException : public std::exception {
public:
    GuiException() = default;
    virtual ~GuiException() = default;
};

#endif /* GUI_HELPERS_GUI_EXCEPTION_H */
