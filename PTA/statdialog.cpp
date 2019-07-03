#include "statdialog.h"

#include <QtWidgets>

const QMap<std::string, QString> g_labelStyles = {{"Unique", "color: #af6025;"}, {"Rare", "color: #ff7;"}, {"Magic", "color: #88f;"}};

StatDialog::StatDialog(PItem* item)
{
    // TODO prefill options
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

    // TODO weapon/armour base mods

    // mods

    for (auto [f, e] : item->filters.items())
    {
        std::string id = e["id"].get<std::string>();

        QString modLabel = QString::fromStdString(e["text"].get<std::string>());

        if (e["type"] == "crafted")
        {
            modLabel += " (crafted)";
        }

        QCheckBox* elab = new QCheckBox(modLabel);
        connect(elab, &QCheckBox::stateChanged, [=](int checked) { filters[id]["disabled"] = !(checked == Qt::Checked); });

        layout->addWidget(elab, current_row, 0);

        auto val_count    = e["value"].size();
        bool val_is_float = val_count ? e["value"][0].is_number_float() : false;

        // min box
        QLineEdit* minEdit = new QLineEdit();
        minEdit->setMaximumWidth(30);

        if (val_count)
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
        });

        layout->addWidget(minEdit, current_row, 1);

        // current value
        QString currval("");

        if (val_count)
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

        QLabel* curvalLabel = new QLabel(currval);
        layout->addWidget(curvalLabel, current_row, 2);

        // max box
        QLineEdit* maxEdit = new QLineEdit();
        maxEdit->setMaximumWidth(30);

        if (val_count)
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
        });

        layout->addWidget(maxEdit, current_row, 3);

        current_row++;
    }

    // separator

    line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    layout->addWidget(line, current_row++, 0, 1, 4);

    // pseudos
    if (item->pseudos.size())
    {
        for (auto [f, e] : item->pseudos.items())
        {
            std::string id       = e["id"].get<std::string>();
            QString     modLabel = "(pseudo) " + QString::fromStdString(e["text"].get<std::string>());

            QCheckBox* elab = new QCheckBox(modLabel);
            connect(elab, &QCheckBox::stateChanged, [=](int checked) { filters[id]["disabled"] = !(checked == Qt::Checked); });

            layout->addWidget(elab, current_row, 0);

            auto val_count    = e["value"].size();
            bool val_is_float = val_count ? e["value"][0].is_number_float() : false;

            // min box
            QLineEdit* minEdit = new QLineEdit();
            minEdit->setMaximumWidth(30);

            if (val_count)
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
            });

            layout->addWidget(minEdit, current_row, 1);

            // current value
            QString currval("");

            if (val_count)
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

            QLabel* curvalLabel = new QLabel(currval);
            layout->addWidget(curvalLabel, current_row, 2);

            // max box
            QLineEdit* maxEdit = new QLineEdit();
            maxEdit->setMaximumWidth(30);

            if (val_count)
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
            });

            layout->addWidget(maxEdit, current_row, 3);

            current_row++;
        }

        // separator

        line = new QFrame();
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        layout->addWidget(line, current_row++, 0, 1, 4);
    }

    // Misc options
    QHBoxLayout* miscLayout = new QHBoxLayout();

    if (item->f_socket.sockets.total() > 0)
    {
        QCheckBox* socketCB = new QCheckBox(tr("Use Sockets") + QString(" (%1)").arg(item->f_socket.sockets.total()));
        connect(socketCB, &QCheckBox::stateChanged, [=](int checked) { misc["use_sockets"] = (checked == Qt::Checked); });

        miscLayout->addWidget(socketCB);
    }

    if (item->f_socket.links > 0)
    {
        QCheckBox* linkCB = new QCheckBox(tr("Use Links") + QString(" (%1)").arg(item->f_socket.links));
        connect(linkCB, &QCheckBox::stateChanged, [=](int checked) { misc["use_links"] = (checked == Qt::Checked); });
        miscLayout->addWidget(linkCB);
    }

    QCheckBox* ilvlCB = new QCheckBox(tr("iLvl (min):"));
    connect(ilvlCB, &QCheckBox::stateChanged, [=](int checked) { misc["use_ilvl"] = (checked == Qt::Checked); });

    QLineEdit* ilvlEdit = new QLineEdit();
    ilvlEdit->setValidator(new QIntValidator(1, 100, this));
    ilvlEdit->setMaximumWidth(30);

    connect(ilvlEdit, &QLineEdit::textChanged, [=](const QString& text) {
        int val      = text.toInt();
        misc["ilvl"] = val;
    });

    miscLayout->addWidget(ilvlCB);
    miscLayout->addWidget(ilvlEdit);

    QCheckBox* itembaseCB = new QCheckBox(tr("Use Item Base"));
    connect(itembaseCB, &QCheckBox::stateChanged, [=](int checked) { misc["use_item_base"] = (checked == Qt::Checked); });
    miscLayout->addWidget(itembaseCB);

    if (item->f_misc.shaper_item)
    {
        QCheckBox* shaperBase = new QCheckBox(tr("Shaper Base"));
        connect(shaperBase, &QCheckBox::stateChanged, [=](int checked) { misc["use_shaper_base"] = (checked == Qt::Checked); });
        miscLayout->addWidget(shaperBase);
    }

    if (item->f_misc.elder_item)
    {
        QCheckBox* elderBase = new QCheckBox(tr("Elder Base"));
        connect(elderBase, &QCheckBox::stateChanged, [=](int checked) { misc["use_elder_base"] = (checked == Qt::Checked); });
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