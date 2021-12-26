#include "custom_form_layout.h"

Custom_form_layout::Custom_form_layout(QString text)
{
    label = new QLabel(text);
    edit_box = new QLineEdit("-x");
    if (text=="Max x")
        edit_box->setText("1");
    else if (text =="Min x")
        edit_box->setText("-1");
    label->setBuddy(edit_box);
    this->addRow(label,edit_box);
    edit_box->setFont(QFont("Serif",-1,-1,true));
    label->setFont(QFont("Serif",-1,-1,true));
}

void Custom_form_layout::edit_box_grab_keyboard()
{
    edit_box->setFocus(Qt::FocusReason::ActiveWindowFocusReason);
}

double Custom_form_layout::get_val()
{
    return edit_box->text().toDouble();
}

QString Custom_form_layout::get_function()
{
    return edit_box->text();
}


void Custom_form_layout::init()
{
}
