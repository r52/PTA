#pragma once

#include <nlohmann/json.hpp>

#include <QDialog>

QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QListWidgetItem)
QT_FORWARD_DECLARE_CLASS(QStackedWidget)

class ItemAPI;

using json = nlohmann::json;

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(json& set, ItemAPI* api);

public slots:
    void changePage(QListWidgetItem* current, QListWidgetItem* previous);

private:
    void createIcons();

    QListWidget*    contentsWidget;
    QStackedWidget* pagesWidget;
};