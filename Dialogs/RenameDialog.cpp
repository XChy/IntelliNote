#include "RenameDialog.h"
#include "ui_RenameDialog.h"

RenameDialog::RenameDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::RenameDialog)
{
    ui->setupUi(this);
}
void RenameDialog::setOldName(const QString &oldName)
{
    ui->oldEdit->setText(oldName);
}
QString RenameDialog::getNewName() const { return ui->newEdit->text(); }

RenameDialog::~RenameDialog() { delete ui; }
