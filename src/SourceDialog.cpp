#include "SourceDialog.h"

SourceDialog::SourceDialog(QWidget* parent) : QDialog(parent) { setupUi(this); }

void SourceDialog::setSource(const QString& source) { ui_source->setText(source); }

QString SourceDialog::source() { return ui_source->text(); }