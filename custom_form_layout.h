#ifndef CUSTOM_FORM_LAYOUT_H
#define CUSTOM_FORM_LAYOUT_H
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDebug>

class Custom_form_layout : public QFormLayout
{
public:
    Custom_form_layout(QString text);
    void edit_box_grab_keyboard();
    double get_val();
    QString get_function();

private:
    QLabel * label;
    QLineEdit * edit_box;

    void init();
};

#endif // CUSTOM_FORM_LAYOUT_H
