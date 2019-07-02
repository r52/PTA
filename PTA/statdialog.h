#pragma once

#include <pitem.h>

#include <QDialog>

constexpr auto SEARCH_ON_SITE = 2;

class StatDialog : public QDialog
{
    Q_OBJECT

public:
    StatDialog(PItem* item);

    json filters;
    json misc;
};