#ifndef QLONGSPINBOX_H
#define QLONGSPINBOX_H

#include <QAbstractSpinBox>
#include <QWidget>
#include <QObject>


using namespace std;

class QLongSpinBox : public QAbstractSpinBox{

    Q_OBJECT
    Q_PROPERTY(qlonglong value READ value WRITE setValue)
public:
    QLongSpinBox(QWidget * parent = 0);

    /* Just wraps call to setValue(i_value + step) */
    void stepBy(int step){
        setValue(i_value + step);
    }

    qlonglong value();
    void setValue(qlonglong val){
        i_value = val;
    }

    /* Checks that the str can be converted to a long */
    QValidator::State validate(QString * str, int pos);

protected:
    qlonglong i_value;
};


#endif // QLONGSPINBOX_H
