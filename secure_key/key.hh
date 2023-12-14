#ifndef KEY_HH
#define KEY_HH

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Key; }
QT_END_NAMESPACE

class Key : public QMainWindow
{
    Q_OBJECT

public:
    Key(QWidget *parent = nullptr);
    ~Key();

private:
    Ui::Key *ui;
    void check_login(void);
};
#endif // KEY_HH
