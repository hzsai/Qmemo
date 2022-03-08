#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QPushButton>

class PushButton : public QPushButton
{
public:
    explicit PushButton(QWidget *parent = 0);
    ~PushButton();

private:
    void initForm();
};

#endif // PUSHBUTTON_H
