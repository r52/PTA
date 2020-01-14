#include "statdialog.h"

#include "pta_types.h"

#include <QtWidgets>

const QMap<std::string, QString> g_labelStyles = {{"Unique", "color: #af6025;"}, {"Rare", "color: #ff7;"}, {"Magic", "color: #88f;"}};

StatDialog::StatDialog(PItem* item)
{
    QSettings settings;

    bool prefillmin     = settings.value(PTA_CONFIG_PREFILL_MIN, PTA_CONFIG_DEFAULT_PREFILL_MIN).toBool();
    bool prefillmax     = settings.value(PTA_CONFIG_PREFILL_MAX, PTA_CONFIG_DEFAULT_PREFILL_MAX).toBool();
    bool prefillnormals = settings.value(PTA_CONFIG_PREFILL_NORMALS, PTA_CONFIG_DEFAULT_PREFILL_NORMALS).toBool();
    bool prefillpseudos = settings.value(PTA_CONFIG_PREFILL_PSEUDOS, PTA_CONFIG_DEFAULT_PREFILL_PSEUDOS).toBool();
    bool prefillilvl    = settings.value(PTA_CONFIG_PREFILL_ILVL, PTA_CONFIG_DEFAULT_PREFILL_ILVL).toBool();
    bool prefillbase    = settings.value(PTA_CONFIG_PREFILL_BASE, PTA_CONFIG_DEFAULT_PREFILL_BASE).toBool();

    setAttribute(Qt::WA_DeleteOnClose);

    int current_row = 0;

    Qt::WindowFlags flags = Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint;

    setWindowFlags(flags);

    setStyleSheet("background-color: #1c1b19; color: #a38d6d;");

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
    addSeparator(layout, current_row);

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

    // separator
    addSeparator(layout, current_row);

    // weapon/armour base mods

    if (item->is_weapon)
    {
        if (item->f_weapon.pdps.avg() > 0)
        {
            // current value
            int pdps = (int) (item->f_weapon.pdps.avg() * item->f_weapon.aps);

            createBaseMod(layout,
                          current_row,
                          tr("Physical DPS"),
                          item->f_weapon.search_pdps,
                          item->f_weapon.pdps_filter.min,
                          item->f_weapon.pdps_filter.max,
                          QString::number(pdps),
                          prefillmin,
                          prefillmax);
        }

        if (item->f_weapon.edps.avg() > 0)
        {
            // current value
            int edps = (int) (item->f_weapon.edps.avg() * item->f_weapon.aps);

            createBaseMod(layout,
                          current_row,
                          tr("Elemental DPS"),
                          item->f_weapon.search_edps,
                          item->f_weapon.edps_filter.min,
                          item->f_weapon.edps_filter.max,
                          QString::number(edps),
                          prefillmin,
                          prefillmax);
        }
    }

    if (item->is_armour)
    {
        if (item->f_armour.ar > 0)
        {
            createBaseMod(layout,
                          current_row,
                          tr("Armour"),
                          item->f_armour.search_ar,
                          item->f_armour.ar_filter.min,
                          item->f_armour.ar_filter.max,
                          QString::number(item->f_armour.ar),
                          prefillmin,
                          prefillmax);
        }

        if (item->f_armour.ev > 0)
        {
            createBaseMod(layout,
                          current_row,
                          tr("Evasion"),
                          item->f_armour.search_ev,
                          item->f_armour.ev_filter.min,
                          item->f_armour.ev_filter.max,
                          QString::number(item->f_armour.ev),
                          prefillmin,
                          prefillmax);
        }

        if (item->f_armour.es > 0)
        {
            createBaseMod(layout,
                          current_row,
                          tr("Energy Shield"),
                          item->f_armour.search_es,
                          item->f_armour.es_filter.min,
                          item->f_armour.es_filter.max,
                          QString::number(item->f_armour.es),
                          prefillmin,
                          prefillmax);
        }
    }

    if (item->is_weapon || item->is_armour)
    {
        // separator
        addSeparator(layout, current_row);
    }

    // mods

    for (auto [f, e] : item->filters.items())
    {
        QString modLabel = QString::fromStdString(e["text"].get<std::string>());

        if (e["type"] == "crafted" || e["type"] == "implicit" || e["type"] == "enchant")
        {
            modLabel += QString(" (%1)").arg(QString::fromStdString(e["type"].get<std::string>()));
        }

        auto val_count = e["value"].size();
        bool vcount    = (val_count > 0 && val_count < 3);

        createModLine(layout, current_row, modLabel, e, vcount, prefillmin, prefillmax, prefillnormals);
    }

    // separator
    addSeparator(layout, current_row);

    // pseudos
    if (item->pseudos.size())
    {
        for (auto [f, e] : item->pseudos.items())
        {
            QString modLabel = "(pseudo) " + QString::fromStdString(e["text"].get<std::string>());

            auto val_count = e["value"].size();

            createModLine(layout, current_row, modLabel, e, val_count, prefillmin, prefillmax, prefillpseudos);
        }

        // separator
        addSeparator(layout, current_row);
    }

    // Misc options
    QHBoxLayout* miscLayout = new QHBoxLayout();

    if (item->f_socket.sockets.total() > 0)
    {
        QCheckBox* socketCB = createMiscCheckBox(tr("Use Sockets") + QString(" (%1)").arg(item->f_socket.sockets.total()), "use_sockets", false);
        miscLayout->addWidget(socketCB);
    }

    if (item->f_socket.links > 0)
    {
        QCheckBox* linkCB = createMiscCheckBox(tr("Use Links") + QString(" (%1)").arg(item->f_socket.links), "use_links", false);
        miscLayout->addWidget(linkCB);
    }

    QCheckBox* ilvlCB = createMiscCheckBox(tr("iLvl (min):"), "use_ilvl", prefillilvl);

    QLineEdit* ilvlEdit = new QLineEdit();
    ilvlEdit->setValidator(new QIntValidator(1, 100, this));
    ilvlEdit->setMaximumWidth(30);
    connect(ilvlEdit, &QLineEdit::textChanged, [=](const QString& text) {
        if (!text.isEmpty())
        {
            int val      = text.toInt();
            misc["ilvl"] = val;
        }
    });

    ilvlEdit->setText(QString::number(item->f_misc.ilvl));

    connect(ilvlEdit, &QLineEdit::textEdited, [=](const QString& text) { ilvlCB->setChecked(true); });

    miscLayout->addWidget(ilvlCB);
    miscLayout->addWidget(ilvlEdit);

    QCheckBox* itembaseCB = createMiscCheckBox(tr("Use Item Base"), "use_item_base", prefillbase);
    miscLayout->addWidget(itembaseCB);

    if (!item->f_misc.influences.empty())
    {
        for (auto i : item->f_misc.influences)
        {
            QString influence = QString::fromStdString(i);
            QString influcap  = influence;
            influcap[0]       = influcap.at(0).toTitleCase();

            QString lbl = influcap + " Influence";

            QCheckBox* base = createInfluenceCheckBox(lbl, i, prefillbase);
            miscLayout->addWidget(base);
        }
    }

    if (item->f_misc.synthesised_item)
    {
        QCheckBox* base = createMiscCheckBox(tr("Synthesis Base"), "use_synthesis_base", prefillbase);
        miscLayout->addWidget(base);
    }

    miscLayout->addStretch();

    layout->addLayout(miscLayout, current_row++, 0, 1, 4);

    // separator
    addSeparator(layout, current_row);

    QDialogButtonBox* buttonBox = new QDialogButtonBox();
    buttonBox->addButton("&Search", QDialogButtonBox::AcceptRole);
    buttonBox->addButton("Op&en on pathofexile.com", QDialogButtonBox::HelpRole);
    buttonBox->addButton("&Close", QDialogButtonBox::RejectRole);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(buttonBox, &QDialogButtonBox::helpRequested, [=]() { this->done(SEARCH_ON_SITE); });

    layout->addWidget(buttonBox);
}

QCheckBox* StatDialog::createMiscCheckBox(const QString& text, std::string key, bool prefill)
{
    QCheckBox* box = new QCheckBox(text);
    connect(box, &QCheckBox::stateChanged, [=](int checked) { misc[key] = (checked == Qt::Checked); });

    if (prefill)
    {
        box->setChecked(true);
    }

    return box;
}

QCheckBox* StatDialog::createInfluenceCheckBox(const QString& text, std::string infl, bool prefill)
{
    QCheckBox* box = new QCheckBox(text);
    connect(box, &QCheckBox::stateChanged, [=](int checked) { misc["influences"][infl] = (checked == Qt::Checked); });

    if (prefill)
    {
        box->setChecked(true);
    }

    return box;
}

void StatDialog::addSeparator(QGridLayout* layout, int& curRow)
{
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    layout->addWidget(line, curRow++, 0, 1, 4);
}

void StatDialog::createBaseMod(QGridLayout*   layout,
                               int&           curRow,
                               const QString& label,
                               bool&          checker,
                               int&           min,
                               int&           max,
                               const QString& curval,
                               bool           prefillmin,
                               bool           prefillmax)
{
    QCheckBox* elab = new QCheckBox(label);
    connect(elab, &QCheckBox::stateChanged, [&](int checked) { checker = (checked == Qt::Checked); });

    layout->addWidget(elab, curRow, 0);

    // min box
    QLineEdit* minEdit = new QLineEdit();
    minEdit->setMaximumWidth(30);
    minEdit->setValidator(new QIntValidator(this));

    connect(minEdit, &QLineEdit::textChanged, [&](const QString& text) {
        if (!text.isEmpty())
        {
            int val = text.toInt();
            min     = val;
        }
    });

    connect(minEdit, &QLineEdit::textEdited, [=](const QString& text) { elab->setChecked(true); });

    layout->addWidget(minEdit, curRow, 1);

    // current value label
    QLabel* curvalLabel = new QLabel();
    layout->addWidget(curvalLabel, curRow, 2);

    // max box
    QLineEdit* maxEdit = new QLineEdit();
    maxEdit->setMaximumWidth(30);
    maxEdit->setValidator(new QIntValidator(this));

    connect(maxEdit, &QLineEdit::textChanged, [&](const QString& text) {
        if (!text.isEmpty())
        {
            int val = text.toInt();
            max     = val;
        }
    });

    connect(maxEdit, &QLineEdit::textEdited, [=](const QString& text) { elab->setChecked(true); });

    layout->addWidget(maxEdit, curRow, 3);

    // current value
    curvalLabel->setText(curval);

    // Process settings
    if (prefillmin)
    {
        minEdit->setText(curval);
    }

    if (prefillmax)
    {
        maxEdit->setText(curval);
    }

    curRow++;
}

template <typename E>
void StatDialog::createModLine(QGridLayout* layout, int& curRow, const QString& label, E e, bool vcount, bool prefillmin, bool prefillmax, bool prefillmod)
{
    std::string id = e["id"].get<std::string>();

    QCheckBox* elab = new QCheckBox(label);
    connect(elab, &QCheckBox::stateChanged, [=](int checked) { filters[id]["disabled"] = !(checked == Qt::Checked); });

    layout->addWidget(elab, curRow, 0);

    auto val_count    = e["value"].size();
    bool val_is_float = val_count ? e["value"][0].is_number_float() : false;

    // min box
    QLineEdit* minEdit = new QLineEdit();
    minEdit->setMaximumWidth(30);

    // Only support single or range values
    if (val_count > 0 && val_count < 3)
    {
        if (val_is_float)
        {
            minEdit->setValidator(new QDoubleValidator(this));
        }
        else
        {
            minEdit->setValidator(new QIntValidator(this));
        }
    }
    else
    {
        minEdit->setEnabled(false);
    }

    connect(minEdit, &QLineEdit::textChanged, [=](const QString& text) {
        if (!text.isEmpty())
        {
            if (val_is_float)
            {
                double val                  = text.toDouble();
                filters[id]["value"]["min"] = val;
            }
            else
            {
                int val                     = text.toInt();
                filters[id]["value"]["min"] = val;
            }
        }
        else
        {
            filters[id]["value"].erase("min");
        }
    });

    connect(minEdit, &QLineEdit::textEdited, [=](const QString& text) { elab->setChecked(true); });

    layout->addWidget(minEdit, curRow, 1);

    // current value label
    QLabel* curvalLabel = new QLabel();
    layout->addWidget(curvalLabel, curRow, 2);

    // max box
    QLineEdit* maxEdit = new QLineEdit();
    maxEdit->setMaximumWidth(30);

    if (vcount)
    {
        if (val_is_float)
        {
            maxEdit->setValidator(new QDoubleValidator(this));
        }
        else
        {
            maxEdit->setValidator(new QIntValidator(this));
        }
    }
    else
    {
        maxEdit->setEnabled(false);
    }

    connect(maxEdit, &QLineEdit::textChanged, [=](const QString& text) {
        if (!text.isEmpty())
        {
            if (val_is_float)
            {
                double val                  = text.toDouble();
                filters[id]["value"]["max"] = val;
            }
            else
            {
                int val                     = text.toInt();
                filters[id]["value"]["max"] = val;
            }
        }
        else
        {
            filters[id]["value"].erase("max");
        }
    });

    connect(maxEdit, &QLineEdit::textEdited, [=](const QString& text) { elab->setChecked(true); });

    layout->addWidget(maxEdit, curRow, 3);

    // current value
    QString currval("");

    if (vcount)
    {
        int    curr_val_int = e["value"][0].get<int>();
        double curr_val_dbl = e["value"][0].get<double>();

        if (val_count > 1)
        {
            for (size_t i = 1; i < val_count; i++)
            {
                curr_val_int += e["value"][i].get<int>();
                curr_val_dbl += e["value"][i].get<double>();
            }

            curr_val_int /= val_count;
            curr_val_dbl /= val_count;
        }

        currval = val_is_float ? QString::number(curr_val_dbl) : QString::number(curr_val_int);
    }

    curvalLabel->setText(currval);

    // Process settings
    if (prefillmin)
    {
        minEdit->setText(currval);
    }

    if (prefillmax)
    {
        maxEdit->setText(currval);
    }

    if (prefillmod)
    {
        elab->setChecked(true);
    }

    curRow++;
}
