#ifndef MANUALADD_H
#define MANUALADD_H

#include <QDialog>

namespace Ui {
class manualAdd;
}

class manualAdd : public QDialog
{
    Q_OBJECT

public:
    explicit manualAdd(QWidget *parent = 0);
    ~manualAdd();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::manualAdd *ui;
};

#endif // MANUALADD_H
