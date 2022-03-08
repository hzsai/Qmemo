#include "ui/Common/pushbutton.h"

PushButton::PushButton(QWidget *parent)
    :QPushButton(parent)
{
    this->initForm();
}

PushButton::~PushButton()
{

}
void PushButton::initForm()
{
    this->setFlat(true);
    this->setStyleSheet("QPushButton{border-radius: 5px;"
                        "color: #00f0f0;"
                        "background-color: #646464;"
                        "border-style: none;"
                        "QPushButton:hover{ background-color: #00f0f0;}");
}
