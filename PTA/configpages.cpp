#include "configpages.h"

#include "itemapi.h"
#include "pta_types.h"

#include <QtWidgets>

namespace
{
    constexpr const char*    s_MacroType[MACRO_TYPE_MAX] = {"Chat", "URL"};
    const QMap<QString, int> s_MacroTypeMap              = {{"Chat", 0}, {"URL", 1}};
}

GeneralPage::GeneralPage(json& set, QWidget* parent) : QWidget(parent)
{
    QSettings settings;

    // group
    QGroupBox* configGroup = new QGroupBox(tr("General"));

    // ------------------Log verbosity
    QLabel* logLabel = new QLabel(tr("Log Verbosity:"));

    QComboBox* logCombo = new QComboBox;
    logCombo->addItems(QStringList() << "Debug"
                                     << "Info"
                                     << "Warning"
                                     << "Critical");
    logCombo->setCurrentIndex(settings.value(PTA_CONFIG_LOGLEVEL, PTA_CONFIG_DEFAULT_LOGLEVEL).toInt());

    connect(logCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [=, &set](int index) { set[PTA_CONFIG_LOGLEVEL] = index; });

    QHBoxLayout* logLayout = new QHBoxLayout;
    logLayout->addWidget(logLabel);
    logLayout->addWidget(logCombo);

    // ------------------Log to file
    QCheckBox* logToFile = new QCheckBox("Save log to file");
    logToFile->setChecked(settings.value(PTA_CONFIG_LOGFILE, false).toBool());

    connect(logToFile, &QCheckBox::stateChanged, [=, &set](int checked) { set[PTA_CONFIG_LOGFILE] = (checked == Qt::Checked); });

    QVBoxLayout* configLayout = new QVBoxLayout;
    configLayout->addLayout(logLayout);
    configLayout->addWidget(logToFile);

    configGroup->setLayout(configLayout);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(configGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

UIPage::UIPage(json& set, QWidget* parent) : QWidget(parent)
{
    QSettings settings;

    QGroupBox* configGroup = new QGroupBox(tr("UI"));

    // ------------------Results Template
    QLabel* tmpLabel = new QLabel(tr("Price Results Template:"));

    QLineEdit* fedit = new QLineEdit;
    fedit->setText(settings.value(PTA_CONFIG_PRICE_TEMPLATE, PTA_CONFIG_DEFAULT_PRICE_TEMPLATE).toString());

    connect(fedit, &QLineEdit::textChanged, [=, &set](const QString& text) {
        if (!text.isEmpty())
        {
            set[PTA_CONFIG_PRICE_TEMPLATE] = text.toStdString();
        }
    });

    QPushButton* browseButton = new QPushButton(tr("Browse"));
    connect(browseButton, &QAbstractButton::clicked, [=]() {
        QString defpath = QDir::currentPath();

        QString fname = QFileDialog::getOpenFileName(this, tr("Load Template"), defpath, tr("Web Templates (*.html)"));

        if (!fname.isNull())
        {
            QFileInfo info(fname);

            if (info.exists())
            {
                fedit->setText(info.absoluteFilePath());
            }
        }
    });

    QHBoxLayout* tmpLayout = new QHBoxLayout;
    tmpLayout->addWidget(tmpLabel);
    tmpLayout->addWidget(fedit);
    tmpLayout->addWidget(browseButton);

    // ------------------Template Size
    int default_width  = settings.value(PTA_CONFIG_TEMPLATE_WIDTH, PTA_CONFIG_DEFAULT_TEMPLATE_WIDTH).toInt();
    int default_height = settings.value(PTA_CONFIG_TEMPLATE_HEIGHT, PTA_CONFIG_DEFAULT_TEMPLATE_HEIGHT).toInt();

    QFormLayout* formLayout = new QFormLayout;

    QSpinBox* wEdit = new QSpinBox;
    wEdit->setRange(1, 8192);
    wEdit->setSuffix("px");
    wEdit->setValue(default_width);
    connect(wEdit, QOverload<int>::of(&QSpinBox::valueChanged), [=, &set](int i) { set[PTA_CONFIG_TEMPLATE_WIDTH] = i; });

    QString wLabel = QString("Template Width (default %1px)").arg(PTA_CONFIG_DEFAULT_TEMPLATE_WIDTH);
    formLayout->addRow(wLabel, wEdit);

    QSpinBox* hEdit = new QSpinBox;
    hEdit->setRange(1, 8192);
    hEdit->setSuffix("px");
    hEdit->setValue(default_height);
    connect(hEdit, QOverload<int>::of(&QSpinBox::valueChanged), [=, &set](int i) { set[PTA_CONFIG_TEMPLATE_HEIGHT] = i; });

    QString hLabel = QString("Template Height (default %1px)").arg(PTA_CONFIG_DEFAULT_TEMPLATE_HEIGHT);
    formLayout->addRow(hLabel, hEdit);

    QVBoxLayout* configLayout = new QVBoxLayout;
    configLayout->addLayout(tmpLayout);
    configLayout->addLayout(formLayout);

    configGroup->setLayout(configLayout);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(configGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

HotkeyPage::HotkeyPage(json& set, QWidget* parent) : QWidget(parent)
{
    QSettings settings;

    QGroupBox* configGroup = new QGroupBox(tr("Hotkeys"));

    QFormLayout* formLayout = new QFormLayout;

    // ------------------Simple Price Check
    QCheckBox* smpLabel = new QCheckBox(tr("Price Check:"));
    smpLabel->setChecked(settings.value(PTA_CONFIG_SIMPLE_CHECK_HOTKEY_ENABLED, true).toBool());

    connect(smpLabel, &QCheckBox::stateChanged, [=, &set](int checked) { set[PTA_CONFIG_SIMPLE_CHECK_HOTKEY_ENABLED] = (checked == Qt::Checked); });

    QString           skeyseq  = settings.value(PTA_CONFIG_SIMPLE_CHECK_HOTKEY, PTA_CONFIG_DEFAULT_SIMPLE_CHECK_HOTKEY).toString();
    QKeySequenceEdit* skeyedit = new QKeySequenceEdit(QKeySequence(skeyseq));

    connect(skeyedit, &QKeySequenceEdit::keySequenceChanged, [=, &set](const QKeySequence& keySequence) {
        set[PTA_CONFIG_SIMPLE_CHECK_HOTKEY] = keySequence.toString().toStdString();
    });

    formLayout->addRow(smpLabel, skeyedit);

    // ------------------Advanced Price Check
    QCheckBox* ampLabel = new QCheckBox(tr("Advanced Price Check:"));
    ampLabel->setChecked(settings.value(PTA_CONFIG_ADV_CHECK_HOTKEY_ENABLED, true).toBool());

    connect(ampLabel, &QCheckBox::stateChanged, [=, &set](int checked) { set[PTA_CONFIG_ADV_CHECK_HOTKEY_ENABLED] = (checked == Qt::Checked); });

    QString           akeyseq  = settings.value(PTA_CONFIG_ADV_CHECK_HOTKEY, PTA_CONFIG_DEFAULT_ADV_CHECK_HOTKEY).toString();
    QKeySequenceEdit* akeyedit = new QKeySequenceEdit(QKeySequence(akeyseq));

    connect(akeyedit, &QKeySequenceEdit::keySequenceChanged, [=, &set](const QKeySequence& keySequence) {
        set[PTA_CONFIG_ADV_CHECK_HOTKEY] = keySequence.toString().toStdString();
    });

    formLayout->addRow(ampLabel, akeyedit);

    // ------------------Wiki Search
    QCheckBox* wikiLabel = new QCheckBox(tr("Open in Wiki:"));
    wikiLabel->setChecked(settings.value(PTA_CONFIG_WIKI_HOTKEY_ENABLED, true).toBool());

    connect(wikiLabel, &QCheckBox::stateChanged, [=, &set](int checked) { set[PTA_CONFIG_WIKI_HOTKEY_ENABLED] = (checked == Qt::Checked); });

    QString           wkeyseq  = settings.value(PTA_CONFIG_WIKI_HOTKEY, PTA_CONFIG_DEFAULT_WIKI_HOTKEY).toString();
    QKeySequenceEdit* wkeyedit = new QKeySequenceEdit(QKeySequence(wkeyseq));

    connect(wkeyedit, &QKeySequenceEdit::keySequenceChanged, [=, &set](const QKeySequence& keySequence) {
        set[PTA_CONFIG_WIKI_HOTKEY] = keySequence.toString().toStdString();
    });

    formLayout->addRow(wikiLabel, wkeyedit);

    // ------------------Ctrl Scroll Wheel

    QCheckBox* sclLabel = new QCheckBox(tr("Ctrl+Mouse Wheel scrolls through stash tabs"));
    sclLabel->setChecked(settings.value(PTA_CONFIG_CTRL_SCROLL_HOTKEY_ENABLED, true).toBool());

    connect(sclLabel, &QCheckBox::stateChanged, [=, &set](int checked) { set[PTA_CONFIG_CTRL_SCROLL_HOTKEY_ENABLED] = (checked == Qt::Checked); });

    formLayout->addRow(sclLabel);

    // =============END

    configGroup->setLayout(formLayout);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(configGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

PriceCheckPage::PriceCheckPage(json& set, ItemAPI* api, QWidget* parent) : QWidget(parent)
{
    QSettings settings;

    // ------------------Price group
    QGroupBox* priceGroup = new QGroupBox(tr("Price Check"));

    // ------------------League
    QLabel* lgLabel = new QLabel(tr("League:"));

    QStringList leagues;

    for (const auto& league : api->getLeagues())
    {
        leagues << QString::fromStdString(league.get<std::string>());
    }

    QComboBox* lgCombo = new QComboBox;
    lgCombo->addItems(leagues);
    lgCombo->setCurrentIndex(settings.value(PTA_CONFIG_LEAGUE, PTA_CONFIG_DEFAULT_LEAGUE).toInt());

    connect(lgCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [=, &set](int index) { set[PTA_CONFIG_LEAGUE] = index; });

    QHBoxLayout* lgLayout = new QHBoxLayout;
    lgLayout->addWidget(lgLabel);
    lgLayout->addWidget(lgCombo);

    // ------------------Corrupted
    QLabel* crLabel = new QLabel(tr("Corrupted:"));

    QComboBox* crCombo = new QComboBox;
    crCombo->addItems(QStringList() << "Any"
                                    << "Yes"
                                    << "No");
    int idx = crCombo->findText(settings.value(PTA_CONFIG_CORRUPTSEARCH, PTA_CONFIG_DEFAULT_CORRUPTSEARCH).toString());
    crCombo->setCurrentIndex(idx);

    connect(crCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [=, &set](int index) {
        set[PTA_CONFIG_CORRUPTSEARCH] = crCombo->itemText(index).toStdString();
    });

    bool ovride = settings.value(PTA_CONFIG_CORRUPTOVERRIDE, PTA_CONFIG_DEFAULT_CORRUPTOVERRIDE).toBool();

    crCombo->setEnabled(ovride);

    QCheckBox* croLabel = new QCheckBox(tr("Override"));
    croLabel->setChecked(ovride);

    connect(croLabel, &QCheckBox::stateChanged, [=, &set](int checked) {
        bool ov = (checked == Qt::Checked);

        set[PTA_CONFIG_CORRUPTOVERRIDE] = ov;
        crCombo->setEnabled(ov);
    });

    QHBoxLayout* crLayout = new QHBoxLayout;
    crLayout->addWidget(crLabel);
    crLayout->addWidget(crCombo);
    crLayout->addWidget(croLabel);

    // ------------------Primary Currency
    QLabel* pcLabel = new QLabel(tr("Primary Currency:"));

    QComboBox* pcCombo = new QComboBox;
    pcCombo->addItem("Orb of Alteration", "alt");
    pcCombo->addItem("Orb of Fusing", "fuse");
    pcCombo->addItem("Orb of Alchemy", "alch");
    pcCombo->addItem("Chaos Orb", "chaos");
    pcCombo->addItem("Gemcutter's Prism", "gcp");
    pcCombo->addItem("Exalted Orb", "exa");
    pcCombo->addItem("Chromatic Orb", "chrom");
    pcCombo->addItem("Jeweller's Orb", "jew");
    pcCombo->addItem("Engineer's Orb", "engineers-orb");
    pcCombo->addItem("Orb of Chance", "chance");
    pcCombo->addItem("Cartographer's Chisel", "chisel");
    pcCombo->addItem("Orb of Scouring", "scour");
    pcCombo->addItem("Blessed Orb", "blessed");
    pcCombo->addItem("Orb of Regret", "regret");
    pcCombo->addItem("Regal Orb", "regal");
    pcCombo->addItem("Divine Orb", "divine");
    pcCombo->addItem("Vaal Orb", "vaal");
    pcCombo->addItem("Orb of Annulment", "orb-of-annulment");
    pcCombo->addItem("Orb of Binding", "orb-of-binding");
    pcCombo->addItem("Ancient Orb", "ancient-orb");
    pcCombo->addItem("Orb of Horizons", "orb-of-horizons");
    pcCombo->addItem("Harbinger's Orb", "harbingers-orb");
    pcCombo->addItem("Scroll of Wisdom", "wis");
    pcCombo->addItem("Portal Scroll", "port");
    pcCombo->addItem("Armourer's Scrap", "scr");
    pcCombo->addItem("Blacksmith's Whetstone", "whe");
    pcCombo->addItem("Glassblower's Bauble", "ba");
    pcCombo->addItem("Orb of Transmutation", "tra");
    pcCombo->addItem("Orb of Augmentation", "aug");
    pcCombo->addItem("Mirror of Kalandra", "mir");
    pcCombo->addItem("Eternal Orb", "ete");
    pcCombo->addItem("Perandus Coin", "p");
    pcCombo->addItem("Silver Coin", "silver");

    int pidx = pcCombo->findData(settings.value(PTA_CONFIG_PRIMARY_CURRENCY, PTA_CONFIG_DEFAULT_PRIMARY_CURRENCY).toString());
    pcCombo->setCurrentIndex(pidx);

    connect(pcCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [=, &set](int index) {
        set[PTA_CONFIG_PRIMARY_CURRENCY] = pcCombo->itemData(index).toString().toStdString();
    });

    QHBoxLayout* pcLayout = new QHBoxLayout;
    pcLayout->addWidget(pcLabel);
    pcLayout->addWidget(pcCombo);

    // ------------------Secondary Currency
    QLabel* scLabel = new QLabel(tr("Secondary Currency:"));

    QComboBox* scCombo = new QComboBox;
    scCombo->addItem("Orb of Alteration", "alt");
    scCombo->addItem("Orb of Fusing", "fuse");
    scCombo->addItem("Orb of Alchemy", "alch");
    scCombo->addItem("Chaos Orb", "chaos");
    scCombo->addItem("Gemcutter's Prism", "gcp");
    scCombo->addItem("Exalted Orb", "exa");
    scCombo->addItem("Chromatic Orb", "chrom");
    scCombo->addItem("Jeweller's Orb", "jew");
    scCombo->addItem("Engineer's Orb", "engineers-orb");
    scCombo->addItem("Orb of Chance", "chance");
    scCombo->addItem("Cartographer's Chisel", "chisel");
    scCombo->addItem("Orb of Scouring", "scour");
    scCombo->addItem("Blessed Orb", "blessed");
    scCombo->addItem("Orb of Regret", "regret");
    scCombo->addItem("Regal Orb", "regal");
    scCombo->addItem("Divine Orb", "divine");
    scCombo->addItem("Vaal Orb", "vaal");
    scCombo->addItem("Orb of Annulment", "orb-of-annulment");
    scCombo->addItem("Orb of Binding", "orb-of-binding");
    scCombo->addItem("Ancient Orb", "ancient-orb");
    scCombo->addItem("Orb of Horizons", "orb-of-horizons");
    scCombo->addItem("Harbinger's Orb", "harbingers-orb");
    scCombo->addItem("Scroll of Wisdom", "wis");
    scCombo->addItem("Portal Scroll", "port");
    scCombo->addItem("Armourer's Scrap", "scr");
    scCombo->addItem("Blacksmith's Whetstone", "whe");
    scCombo->addItem("Glassblower's Bauble", "ba");
    scCombo->addItem("Orb of Transmutation", "tra");
    scCombo->addItem("Orb of Augmentation", "aug");
    scCombo->addItem("Mirror of Kalandra", "mir");
    scCombo->addItem("Eternal Orb", "ete");
    scCombo->addItem("Perandus Coin", "p");
    scCombo->addItem("Silver Coin", "silver");

    int sidx = scCombo->findData(settings.value(PTA_CONFIG_SECONDARY_CURRENCY, PTA_CONFIG_DEFAULT_SECONDARY_CURRENCY).toString());
    scCombo->setCurrentIndex(sidx);

    connect(scCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [=, &set](int index) {
        set[PTA_CONFIG_SECONDARY_CURRENCY] = scCombo->itemData(index).toString().toStdString();
    });

    QHBoxLayout* scLayout = new QHBoxLayout;
    scLayout->addWidget(scLabel);
    scLayout->addWidget(scCombo);

    // ------------------Online only
    QCheckBox* olLabel = new QCheckBox(tr("Online only"));
    olLabel->setChecked(settings.value(PTA_CONFIG_ONLINE_ONLY, PTA_CONFIG_DEFAULT_ONLINE_ONLY).toBool());

    connect(olLabel, &QCheckBox::stateChanged, [=, &set](int checked) { set[PTA_CONFIG_ONLINE_ONLY] = (checked == Qt::Checked); });

    // ------------------Buyout only
    QCheckBox* boLabel = new QCheckBox(tr("Buyout only"));
    boLabel->setChecked(settings.value(PTA_CONFIG_BUYOUT_ONLY, PTA_CONFIG_DEFAULT_BUYOUT_ONLY).toBool());

    connect(boLabel, &QCheckBox::stateChanged, [=, &set](int checked) { set[PTA_CONFIG_BUYOUT_ONLY] = (checked == Qt::Checked); });

    // ------------------Remove dupes
    QCheckBox* dupeLabel = new QCheckBox(tr("Remove duplicate listings from the same account"));
    dupeLabel->setChecked(settings.value(PTA_CONFIG_REMOVE_DUPES, PTA_CONFIG_DEFAULT_REMOVE_DUPES).toBool());

    connect(dupeLabel, &QCheckBox::stateChanged, [=, &set](int checked) { set[PTA_CONFIG_REMOVE_DUPES] = (checked == Qt::Checked); });

    // End price group

    QVBoxLayout* priceLayout = new QVBoxLayout;
    priceLayout->addLayout(lgLayout);
    priceLayout->addLayout(crLayout);
    priceLayout->addLayout(pcLayout);
    priceLayout->addLayout(scLayout);
    priceLayout->addWidget(olLabel);
    priceLayout->addWidget(boLabel);
    priceLayout->addWidget(dupeLabel);

    priceGroup->setLayout(priceLayout);

    // -----------------Select group
    QGroupBox* selGroup = new QGroupBox(tr("Pre-Select Options (Advanced Search)"));

    // ------------------pre-fill min
    QCheckBox* minLabel = new QCheckBox(tr("Pre-fill minimum values"));
    minLabel->setChecked(settings.value(PTA_CONFIG_PREFILL_MIN, PTA_CONFIG_DEFAULT_PREFILL_MIN).toBool());

    connect(minLabel, &QCheckBox::stateChanged, [=, &set](int checked) { set[PTA_CONFIG_PREFILL_MIN] = (checked == Qt::Checked); });

    // ------------------pre-fill max
    QCheckBox* maxLabel = new QCheckBox(tr("Pre-fill maximum values"));
    maxLabel->setChecked(settings.value(PTA_CONFIG_PREFILL_MAX, PTA_CONFIG_DEFAULT_PREFILL_MAX).toBool());

    connect(maxLabel, &QCheckBox::stateChanged, [=, &set](int checked) { set[PTA_CONFIG_PREFILL_MAX] = (checked == Qt::Checked); });

    // ------------------pre-fill normals
    QCheckBox* nmLabel = new QCheckBox(tr("Normal mods"));
    nmLabel->setChecked(settings.value(PTA_CONFIG_PREFILL_NORMALS, PTA_CONFIG_DEFAULT_PREFILL_NORMALS).toBool());

    connect(nmLabel, &QCheckBox::stateChanged, [=, &set](int checked) { set[PTA_CONFIG_PREFILL_NORMALS] = (checked == Qt::Checked); });

    // ------------------pre-fill pseudos
    QCheckBox* psLabel = new QCheckBox(tr("Pseudo mods"));
    psLabel->setChecked(settings.value(PTA_CONFIG_PREFILL_PSEUDOS, PTA_CONFIG_DEFAULT_PREFILL_PSEUDOS).toBool());

    connect(psLabel, &QCheckBox::stateChanged, [=, &set](int checked) { set[PTA_CONFIG_PREFILL_PSEUDOS] = (checked == Qt::Checked); });

    // ------------------pre-fill item level
    QCheckBox* ilLabel = new QCheckBox(tr("Item Level"));
    ilLabel->setChecked(settings.value(PTA_CONFIG_PREFILL_ILVL, PTA_CONFIG_DEFAULT_PREFILL_ILVL).toBool());

    connect(ilLabel, &QCheckBox::stateChanged, [=, &set](int checked) { set[PTA_CONFIG_PREFILL_ILVL] = (checked == Qt::Checked); });

    // ------------------pre-fill item base
    QCheckBox* ibLabel = new QCheckBox(tr("Item Base"));
    ibLabel->setChecked(settings.value(PTA_CONFIG_PREFILL_BASE, PTA_CONFIG_DEFAULT_PREFILL_BASE).toBool());

    connect(ibLabel, &QCheckBox::stateChanged, [=, &set](int checked) { set[PTA_CONFIG_PREFILL_BASE] = (checked == Qt::Checked); });

    // End select group

    QVBoxLayout* selLayout = new QVBoxLayout;
    selLayout->addWidget(minLabel);
    selLayout->addWidget(maxLabel);
    selLayout->addWidget(nmLabel);
    selLayout->addWidget(psLabel);
    selLayout->addWidget(ilLabel);
    selLayout->addWidget(ibLabel);

    selGroup->setLayout(selLayout);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(priceGroup);
    mainLayout->addWidget(selGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

class MacroEditDialog : public QDialog
{
public:
    MacroEditDialog(QString  title,
                    QWidget* parent = nullptr,
                    QString  key    = QString(),
                    QString  seq    = QString(),
                    QString  type   = "Chat",
                    QString  cmd    = QString()) :
        QDialog(parent)
    {
        setMinimumWidth(400);
        setWindowTitle(title);

        QLabel*      keyLabel  = nullptr;
        QLineEdit*   keyEdit   = nullptr;
        QHBoxLayout* keylayout = nullptr;

        if (key.isEmpty())
        {
            keyLabel = new QLabel(tr("Macro Name:"));
            keyEdit  = new QLineEdit;
            keyEdit->setText(key);

            keylayout = new QHBoxLayout();
            keylayout->addWidget(keyLabel);
            keylayout->addWidget(keyEdit);
        }

        QLabel*           seqLabel = new QLabel(tr("Hotkey:"));
        QKeySequenceEdit* seqEdit  = new QKeySequenceEdit(QKeySequence(seq, QKeySequence::PortableText));

        QHBoxLayout* seqlayout = new QHBoxLayout();
        seqlayout->addWidget(seqLabel);
        seqlayout->addWidget(seqEdit);

        QLabel*    typeLabel = new QLabel(tr("Macro Type:"));
        QComboBox* typeCombo = new QComboBox();
        typeCombo->addItem("Chat");
        typeCombo->addItem("URL");

        int tidx = typeCombo->findText(type);
        typeCombo->setCurrentIndex(tidx);

        QHBoxLayout* typelayout = new QHBoxLayout();
        typelayout->addWidget(typeLabel);
        typelayout->addWidget(typeCombo);

        QLabel*    cmdLabel = new QLabel(tr("Command:"));
        QLineEdit* cmdEdit  = new QLineEdit;
        cmdEdit->setText(cmd);

        QHBoxLayout* cmdlayout = new QHBoxLayout();
        cmdlayout->addWidget(cmdLabel);
        cmdlayout->addWidget(cmdEdit);

        QPushButton* okBtn     = new QPushButton(tr("OK"));
        QPushButton* cancelBtn = new QPushButton(tr("Cancel"));

        okBtn->setDefault(true);

        QHBoxLayout* btnlayout = new QHBoxLayout;
        btnlayout->addWidget(okBtn);
        btnlayout->addWidget(cancelBtn);

        QVBoxLayout* layout = new QVBoxLayout;

        if (keylayout)
        {
            layout->addLayout(keylayout);
        }

        layout->addLayout(seqlayout);
        layout->addLayout(typelayout);
        layout->addLayout(cmdlayout);
        layout->addLayout(btnlayout);

        // cancel button
        connect(cancelBtn, &QPushButton::clicked, [=](bool checked) { close(); });

        // ok button
        connect(okBtn, &QPushButton::clicked, [=](bool checked) {
            // validate
            auto seq = seqEdit->keySequence().toString();

            if ((keyEdit && keyEdit->text().isEmpty()) || seq.isEmpty() || cmdEdit->text().isEmpty())
            {
                QMessageBox::warning(this, title, "All fields must be filled out.");
            }
            else
            {
                if (keyEdit)
                {
                    m_key = keyEdit->text();
                }

                m_sequence = seq;

                int tidx  = typeCombo->currentIndex();
                m_type    = typeCombo->itemText(tidx);
                m_command = cmdEdit->text();
                accept();
            }
        });

        setLayout(layout);
    };

    QString& getKey() { return m_key; };
    QString& getSequence() { return m_sequence; };
    QString& getType() { return m_type; };
    QString& getCommand() { return m_command; };

private:
    QString m_key;
    QString m_sequence;
    QString m_type;
    QString m_command;
};

MacrosPage::MacrosPage(json& set, QWidget* parent) : QWidget(parent)
{
    QLabel* label = new QLabel(tr("Macros:"));

    QTableWidget* table = new QTableWidget();
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels(QStringList() << "Name"
                                                   << "Hotkey"
                                                   << "Type"
                                                   << "Command");
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    {
        QSettings settings;
        int       row = 0;

        auto macstr = settings.value(PTA_CONFIG_CUSTOM_MACROS).toString().toStdString();

        auto macrolist = json::object();

        if (!macstr.empty())
        {
            macrolist = json::parse(macstr);
        }

        set[PTA_CONFIG_CUSTOM_MACROS] = macrolist;

        for (const auto& [k, v] : macrolist.items())
        {
            auto key  = QString::fromStdString(k);
            auto seq  = QString::fromStdString(v["sequence"].get<std::string>());
            auto type = QString(s_MacroType[v["type"].get<int>()]);
            auto cmd  = QString::fromStdString(v["command"].get<std::string>());

            table->insertRow(row);
            QTableWidgetItem* keyitem  = new QTableWidgetItem(key);
            QTableWidgetItem* seqitem  = new QTableWidgetItem(seq);
            QTableWidgetItem* typeitem = new QTableWidgetItem(type);
            QTableWidgetItem* cmditem  = new QTableWidgetItem(cmd);

            table->setItem(row, 0, keyitem);
            table->setItem(row, 1, seqitem);
            table->setItem(row, 2, typeitem);
            table->setItem(row, 3, cmditem);

            ++row;
        }
    }

    QPushButton* deleteButton = new QPushButton(tr("Delete"));

    connect(deleteButton, &QPushButton::clicked, [=, &set](bool checked) {
        int row = table->currentRow();
        if (row >= 0)
        {
            QTableWidgetItem* keyitem = table->item(row, 0);
            auto              key     = keyitem->text().toStdString();

            set[PTA_CONFIG_CUSTOM_MACROS].erase(key);

            table->removeRow(table->currentRow());
        }
    });

    QPushButton* editButton = new QPushButton(tr("Edit"));

    connect(editButton, &QPushButton::clicked, [=, &set](bool checked) {
        int row = table->currentRow();

        if (row >= 0)
        {
            QTableWidgetItem* keyitem  = table->item(row, 0);
            QTableWidgetItem* seqitem  = table->item(row, 1);
            QTableWidgetItem* typeitem = table->item(row, 2);
            QTableWidgetItem* cmditem  = table->item(row, 3);

            auto key  = keyitem->text();
            auto seq  = seqitem->text();
            auto type = typeitem->text();
            auto cmd  = cmditem->text();

            MacroEditDialog editdialog("Edit Macro", this, key, seq, type, cmd);

            if (editdialog.exec() == QDialog::Accepted)
            {
                seq  = editdialog.getSequence();
                type = editdialog.getType();
                cmd  = editdialog.getCommand();

                seqitem->setText(seq);
                typeitem->setText(type);
                cmditem->setText(cmd);

                auto& macros = set[PTA_CONFIG_CUSTOM_MACROS];

                json nmac = json::object();

                nmac["sequence"] = seq.toStdString();
                nmac["type"]     = s_MacroTypeMap[type];
                nmac["command"]  = cmd.toStdString();

                macros[key.toStdString()] = nmac;
            }
        }
    });

    QPushButton* addButton = new QPushButton(tr("Add"));

    connect(addButton, &QPushButton::clicked, [=, &set](bool checked) {
        MacroEditDialog editdialog("New Macro", this);

        if (editdialog.exec() == QDialog::Accepted)
        {
            QString key  = editdialog.getKey();
            QString seq  = editdialog.getSequence();
            QString type = editdialog.getType();
            QString cmd  = editdialog.getCommand();

            auto elist = table->findItems(key, Qt::MatchFixedString);

            if (!elist.isEmpty())
            {
                QMessageBox::warning(this, "Duplicate Macro", "Macros must have a unique name");
            }
            else
            {
                int row = table->rowCount();
                table->insertRow(row);

                QTableWidgetItem* keyitem  = new QTableWidgetItem(key);
                QTableWidgetItem* seqitem  = new QTableWidgetItem(seq);
                QTableWidgetItem* typeitem = new QTableWidgetItem(type);
                QTableWidgetItem* cmditem  = new QTableWidgetItem(cmd);

                table->setItem(row, 0, keyitem);
                table->setItem(row, 1, seqitem);
                table->setItem(row, 2, typeitem);
                table->setItem(row, 3, cmditem);

                auto& macros = set[PTA_CONFIG_CUSTOM_MACROS];

                json nmac = json::object();

                nmac["sequence"] = seq.toStdString();
                nmac["type"]     = s_MacroTypeMap[type];
                nmac["command"]  = cmd.toStdString();

                macros[key.toStdString()] = nmac;
            }
        }
    });

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(addButton);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(table);
    layout->addLayout(buttonLayout);
    layout->addStretch(1);
    setLayout(layout);
}

ClientPage::ClientPage(json& set, QWidget* parent) : QWidget(parent)
{
    QSettings settings;

    QGroupBox* configGroup = new QGroupBox(tr("Game Client"));

    // ------------------Client Log
    QLabel* cliLabel = new QLabel(tr("Client Log location:"));

    QLineEdit* fedit = new QLineEdit;
    fedit->setText(settings.value(PTA_CONFIG_CLIENTLOG_PATH, QString()).toString());

    connect(fedit, &QLineEdit::textChanged, [=, &set](const QString& text) {
        if (!text.isEmpty())
        {
            set[PTA_CONFIG_CLIENTLOG_PATH] = text.toStdString();
        }
    });

    QPushButton* browseButton = new QPushButton(tr("Browse"));
    connect(browseButton, &QAbstractButton::clicked, [=]() {
        QString defpath = QDir::currentPath();

        QString fname = QFileDialog::getOpenFileName(this, tr("Load Client.txt"), defpath, tr("Client.txt (Client.txt)"));

        if (!fname.isNull())
        {
            QFileInfo info(fname);

            if (info.exists())
            {
                fedit->setText(info.absoluteFilePath());
            }
        }
    });

    QHBoxLayout* cliLayout = new QHBoxLayout;
    cliLayout->addWidget(cliLabel);
    cliLayout->addWidget(fedit);
    cliLayout->addWidget(browseButton);

    QVBoxLayout* configLayout = new QVBoxLayout;
    configLayout->addLayout(cliLayout);

    configGroup->setLayout(configLayout);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(configGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}