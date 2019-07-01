#include "statdialog.h"

#include <QtWidgets>

const QMap<std::string, QString> g_labelStyles = {{"Unique", "QLabel { background-color: #0f0f0f; color : #af6025; }"},
                                                  {"Rare", "QLabel { background-color: #0f0f0f; color : #ff7; }"},
                                                  {"Magic", "QLabel { background-color: #0f0f0f; color : #88f; }"}};

StatDialog::StatDialog(PItem* item)
{
    int current_row = 0;

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QGridLayout* layout = new QGridLayout();

    setLayout(layout);

    QString s_itemLabel("");

    if (!item->name.empty())
    {
        s_itemLabel += QString::fromStdString(item->name) + " ";
    }

    if (!item->type.empty())
    {
        s_itemLabel += QString::fromStdString(item->type) + " ";
    }

    // title

    QLabel* itemLabel = new QLabel(s_itemLabel);

    if (g_labelStyles.contains(item->f_type.rarity))
    {
        itemLabel->setStyleSheet(g_labelStyles[item->f_type.rarity]);
    }

    layout->addWidget(itemLabel, current_row++, 0, 1, 4);

    // separator

    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    layout->addWidget(line, current_row++, 0, 1, 4);

    // Headers
    QLabel* modsLabel = new QLabel(tr("Mods"));
    QLabel* minLabel  = new QLabel(tr("Min"));
    QLabel* currLabel = new QLabel(tr("Current"));
    QLabel* maxLabel  = new QLabel(tr("Max"));

    layout->addWidget(modsLabel, current_row, 0);
    layout->addWidget(minLabel, current_row, 1);
    layout->addWidget(currLabel, current_row, 2);
    layout->addWidget(maxLabel, current_row, 3);

    current_row++;

    // mods

    for (auto [f, e] : item->filters)
    {
        QCheckBox* elab = new QCheckBox(QString::fromStdString(e["text"].get<std::string>()));

        layout->addWidget(elab, current_row, 0);

        auto   val_count    = e["value"].size();
        bool   val_is_float = e["value"][0].is_number_float();
        int    curr_val_int = e["value"][0].get<int>();
        double curr_val_dbl = e["value"][0].get<double>();

        if (val_count > 1)
        {
            for (size_t i = 1; i < val_count; i++)
            {
                curr_val_int += e["value"][i].get<int>();
                curr_val_dbl += e["value"][i].get<double>();
            }
        }

        // min box
        QLineEdit* minEdit = new QLineEdit();
        minEdit->setMaximumWidth(30);

        if (val_is_float)
        {
            minEdit->setValidator(new QDoubleValidator(this));
        }
        else
        {
            minEdit->setValidator(new QIntValidator(this));
        }

        layout->addWidget(minEdit, current_row, 1);

        // current value
        QLabel* curvalLabel = new QLabel(val_is_float ? QString::number(curr_val_dbl) : QString::number(curr_val_int));
        layout->addWidget(curvalLabel, current_row, 2);

        // max box
        QLineEdit* maxEdit = new QLineEdit();
        maxEdit->setMaximumWidth(30);

        if (val_is_float)
        {
            maxEdit->setValidator(new QDoubleValidator(this));
        }
        else
        {
            maxEdit->setValidator(new QIntValidator(this));
        }

        layout->addWidget(maxEdit, current_row, 3);

        current_row++;
    }

    // TODO pseudos

    // separator

    line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    layout->addWidget(line, current_row++, 0, 1, 4);

    // Misc options
    QHBoxLayout* miscLayout = new QHBoxLayout();

    QCheckBox* ilvlCB   = new QCheckBox(tr("iLvl:"));
    QLineEdit* ilvlEdit = new QLineEdit();
    ilvlEdit->setValidator(new QIntValidator(1, 100, this));
    ilvlEdit->setMaximumWidth(30);

    miscLayout->addWidget(ilvlCB);
    miscLayout->addWidget(ilvlEdit);

    QCheckBox* itembaseCB = new QCheckBox(tr("Use Item Base"));
    miscLayout->addWidget(itembaseCB);

    if (item->f_misc.shaper_item)
    {
        QCheckBox* shaperBase = new QCheckBox(tr("Shaper Base"));
        miscLayout->addWidget(shaperBase);
    }

    if (item->f_misc.elder_item)
    {
        QCheckBox* elderBase = new QCheckBox(tr("Elder Base"));
        miscLayout->addWidget(elderBase);
    }

    miscLayout->addStretch();

    layout->addLayout(miscLayout, current_row++, 0, 1, 4);

    // separator

    line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    layout->addWidget(line, current_row++, 0, 1, 4);

    QDialogButtonBox* buttonBox = new QDialogButtonBox();
    buttonBox->addButton("Search", QDialogButtonBox::AcceptRole);
    buttonBox->addButton("Open on pathofexile.com", QDialogButtonBox::HelpRole);
    buttonBox->addButton("Close", QDialogButtonBox::RejectRole);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(buttonBox, &QDialogButtonBox::helpRequested, [=]() { this->done(SEARCH_ON_SITE); });

    layout->addWidget(buttonBox);
}