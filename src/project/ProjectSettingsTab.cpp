#include "ProjectSettingsTab.h"

#include "MainWindow.h"
#include "Activatable.h"
#include "Project.h"
#include "Log.h"
#include "kiss-compiler/CompilerManager.h"

#include <QStyledItemDelegate>
#include <QComboBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QDebug>

class ComboDelegate : public QStyledItemDelegate
{
public:
    ComboDelegate(QObject *parent, QStringList keys)
        : QStyledItemDelegate(parent) {
        this->keys = keys;
    }

    ~ComboDelegate() {

    }

    virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        if(index.column() != 0)
            return QStyledItemDelegate::createEditor(parent, option, index);

        QComboBox *box = new QComboBox(parent);
        box->addItems(keys);
        box->addItem("<custom>");

        return box;
    }
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const {
        QComboBox *box = qobject_cast<QComboBox*>(editor);
        if(box) {
            QString currentText = index.data(Qt::EditRole).toString();
            int boxIndex = box->findText(currentText);
            if(boxIndex >= 0)
                box->setCurrentIndex(boxIndex);
        }

        else
            QStyledItemDelegate::setEditorData(editor, index);
    }
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
        QComboBox *box = qobject_cast<QComboBox *>(editor);
        if(box)
            model->setData(index, box->currentText(), Qt::EditRole);
        else
            QStyledItemDelegate::setModelData(editor, model, index);
    }

private:
    QStringList keys;
};

ProjectSettingsTab::ProjectSettingsTab(Project* project, MainWindow* mainWindow)
    : QWidget(mainWindow), TabbedWidget(this, mainWindow), m_block(false)
{
    setupUi(this);

    ui_settingsTable->setItemDelegate(new ComboDelegate(ui_settingsTable, CompilerManager::ref().keys()));

    setAssociatedProject(project);
    reload();
}

void ProjectSettingsTab::activate()
{
    QList<Menuable*> menus = mainWindow()->menuablesExcept(mainWindow()->standardMenus());
    foreach(Menuable* menu, menus) {
        ActivatableObject* activatable = dynamic_cast<ActivatableObject*>(menu);
        if(activatable) activatable->setActive(0);
    }

    mainWindow()->showProjectDock();
}

bool ProjectSettingsTab::beginSetup()
{
    return true;
}

void ProjectSettingsTab::completeSetup()
{
    mainWindow()->setTabName(this, associatedProject()->name() + " Settings");
    connect(associatedProject(), SIGNAL(settingsChanged()), SLOT(reload()));
}

bool ProjectSettingsTab::close()
{
    associatedProject()->sync();
    return true;
}

void ProjectSettingsTab::refreshSettings()
{

}

void ProjectSettingsTab::reload()
{
    const QStringMap& settings = associatedProject()->settings();

    Log::ref().debug("Reloaded settings tab");

    m_block = true;

    ui_settingsTable->clearContents();
    ui_settingsTable->setRowCount(settings.keys().size());
    quint16 i = 0;
    foreach(const QString& key, settings.keys()) {
        ui_settingsTable->setItem(i, 0, new QTableWidgetItem(key));
        ui_settingsTable->setItem(i, 1, new QTableWidgetItem(settings[key]));

        ++i;
    }

    m_block = false;
}

void ProjectSettingsTab::on_ui_settingsTable_cellChanged(int row, int column)
{
    if(m_block) return;

    if(column == 0 && ui_settingsTable->item(row, column)->text() == "<custom>") {
        bool accepted;
        QString text = QInputDialog::getText(this, tr("Custom Key"), tr("Enter key:"), QLineEdit::Normal, "", &accepted);
        if(accepted)
            ui_settingsTable->setItem(row, column, new QTableWidgetItem(text));
        else
            ui_settingsTable->setItem(row, column, new QTableWidgetItem(""));
    }

    QTableWidgetItem* key = ui_settingsTable->item(row, 0);
    QTableWidgetItem* value = ui_settingsTable->item(row, 1);

    if(key && value) associatedProject()->updateSetting(key->text(), value->text());
}

void ProjectSettingsTab::on_ui_add_clicked()
{
    ui_settingsTable->insertRow(ui_settingsTable->rowCount());
}

void ProjectSettingsTab::on_ui_remove_clicked()
{
    int row = ui_settingsTable->currentRow();
    QTableWidgetItem* key = ui_settingsTable->item(row, 0);
    QString keyString = key ? key->text() : "";
    ui_settingsTable->removeRow(row);

    associatedProject()->removeSetting(keyString);
}
