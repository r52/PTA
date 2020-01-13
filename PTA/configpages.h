#pragma once

#include <nlohmann/json.hpp>

#include <QSettings>
#include <QWidget>

class ItemAPI;

using json = nlohmann::json;

class GeneralPage : public QWidget
{
    Q_OBJECT

public:
    GeneralPage(json& set, QWidget* parent = 0);
};

class UIPage : public QWidget
{
    Q_OBJECT

public:
    UIPage(json& set, QWidget* parent = 0);
};

class HotkeyPage : public QWidget
{
    Q_OBJECT

public:
    HotkeyPage(json& set, QWidget* parent = 0);
};

class PriceCheckPage : public QWidget
{
    Q_OBJECT

public:
    PriceCheckPage(json& set, ItemAPI* api, QWidget* parent = 0);
};

class MacrosPage : public QWidget
{
    Q_OBJECT

public:
    MacrosPage(json& set, QWidget* parent = 0);
};
