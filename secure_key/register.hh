#ifndef REGISTER_HH
#define REGISTER_HH

#include <QDialog>

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

private:
    Ui::Register *ui;
};

#endif // REGISTER_HH
