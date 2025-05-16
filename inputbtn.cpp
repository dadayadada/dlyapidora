    #include "inputbtn.h"

    NumberEditButton::NumberEditButton(const QString& labelText, int defaultValue, QWidget *parent)
        : QWidget(parent)
    {
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 30, 0, 0);

        QLabel *label = new QLabel(labelText, this);
        m_edit = new QLineEdit(QString::number(defaultValue), this);
        m_edit->setValidator(new QIntValidator(1, 100, this));
        m_edit->setFixedWidth(50);

        layout->addWidget(label);
        layout->addWidget(m_edit);
    }

    int NumberEditButton::value() const
    {
        return m_edit->text().toInt();
    }

    void NumberEditButton::setValue(int value)
    {
        m_edit->setText(QString::number(value));
    }
