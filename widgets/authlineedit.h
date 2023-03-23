#ifndef AUTHLINEEDIT_H
#define AUTHLINEEDIT_H

#include <QLineEdit>
#include <QObject>
#include <QWidget>
#include <QFocusEvent>

class AuthLineEdit : public QLineEdit
{
public:
    explicit AuthLineEdit(QString text,QWidget *parent = nullptr);
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
private :
    QString m_text;
};

#endif // AUTHLINEEDIT_H
