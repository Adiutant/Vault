#include "authlineedit.h"

AuthLineEdit::AuthLineEdit(QString text,QWidget *parent) : QLineEdit{parent}, m_text{text}
{
    setFocusPolicy(Qt::ClickFocus);
    setText(m_text);
}

void AuthLineEdit::focusInEvent(QFocusEvent *event)
{
    if(text() == m_text){
        clear();
    }
    QLineEdit::focusInEvent(event);
}

void AuthLineEdit::focusOutEvent(QFocusEvent *event)
{
    if(text().isEmpty()){
        setText(m_text);
    }
    QLineEdit::focusInEvent(event);
}
