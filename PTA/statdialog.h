#pragma once

#include <pitem.h>

#include <QDialog>

constexpr auto SEARCH_ON_SITE = 2;

class QCheckBox;
class QGridLayout;

class StatDialog : public QDialog
{
    Q_OBJECT

public:
    StatDialog(PItem* item);

    json filters;
    json misc;

private:
    QCheckBox* createMiscCheckBox(const QString& text, const char* key, bool prefill);
    void       addSeparator(QGridLayout* layout, int& curRow);
    void       createBaseMod(QGridLayout*   layout,
                             int&           curRow,
                             const QString& label,
                             bool&          checker,
                             int&           min,
                             int&           max,
                             const QString& curval,
                             bool           prefillmin,
                             bool           prefillmax);

    template <typename E>
    void createModLine(QGridLayout* layout, int& curRow, const QString& label, E e, bool vcount, bool prefillmin, bool prefillmax, bool prefillmod);
};