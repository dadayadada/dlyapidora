#ifndef INPUTBTN_H
#define INPUTBTN_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QIntValidator>

class NumberEditButton : public QWidget
{
    Q_OBJECT
public:
    explicit NumberEditButton(const QString& labelText, int defaultValue, QWidget *parent = nullptr);
    int value() const;
    void setValue(int value);

private:
    QLineEdit *m_edit;
};

#endif
