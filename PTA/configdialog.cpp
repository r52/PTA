#include "configdialog.h"

#include "configpages.h"

#include <QtWidgets>

ConfigDialog::ConfigDialog(ItemAPI* api)
{
    setAttribute(Qt::WA_DeleteOnClose);

    contentsWidget = new QListWidget;
    contentsWidget->setViewMode(QListView::IconMode);
    contentsWidget->setIconSize(QSize(32, 32));
    contentsWidget->setMovement(QListView::Static);
    contentsWidget->setMaximumWidth(128);
    contentsWidget->setMinimumWidth(128);
    contentsWidget->setMinimumHeight(300);
    contentsWidget->setSpacing(12);
    contentsWidget->setFlow(QListView::TopToBottom);
    contentsWidget->setWrapping(false);

    pagesWidget = new QStackedWidget;
    pagesWidget->addWidget(new GeneralPage(results));
    pagesWidget->addWidget(new UIPage(results));
    pagesWidget->addWidget(new HotkeyPage(results));
    pagesWidget->addWidget(new PriceCheckPage(results, api));
    pagesWidget->addWidget(new MacrosPage(results));

    QPushButton* saveButton  = new QPushButton(tr("Save"));
    QPushButton* closeButton = new QPushButton(tr("Close"));

    createIcons();
    contentsWidget->setCurrentRow(0);

    connect(saveButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(closeButton, &QAbstractButton::clicked, this, &QDialog::reject);

    QHBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(contentsWidget);
    horizontalLayout->addWidget(pagesWidget, 1);

    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(saveButton);
    buttonsLayout->addWidget(closeButton);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Settings"));
    setMinimumWidth(600);
}

void ConfigDialog::createIcons()
{
    QListWidgetItem* generalButton = new QListWidgetItem(contentsWidget);
    generalButton->setText(tr("General"));
    generalButton->setTextAlignment(Qt::AlignHCenter);
    generalButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    generalButton->setIcon(QIcon(":/Resources/settings.png"));

    QListWidgetItem* uiButton = new QListWidgetItem(contentsWidget);
    uiButton->setText(tr("UI"));
    uiButton->setTextAlignment(Qt::AlignHCenter);
    uiButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    uiButton->setIcon(QIcon(":/Resources/ui.png"));

    QListWidgetItem* hotkeyButton = new QListWidgetItem(contentsWidget);
    hotkeyButton->setText(tr("Hotkey"));
    hotkeyButton->setTextAlignment(Qt::AlignHCenter);
    hotkeyButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    hotkeyButton->setIcon(QIcon(":/Resources/hotkey.png"));

    QListWidgetItem* pcButton = new QListWidgetItem(contentsWidget);
    pcButton->setText(tr("Price Check"));
    pcButton->setTextAlignment(Qt::AlignHCenter);
    pcButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    pcButton->setIcon(QIcon(":/Resources/pricing.png"));

    QListWidgetItem* macroButton = new QListWidgetItem(contentsWidget);
    macroButton->setText(tr("Custom Macros"));
    macroButton->setTextAlignment(Qt::AlignHCenter);
    macroButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    macroButton->setIcon(QIcon(":/Resources/macros.png"));

    connect(contentsWidget, &QListWidget::currentItemChanged, this, &ConfigDialog::changePage);
}

void ConfigDialog::changePage(QListWidgetItem* current, QListWidgetItem* previous)
{
    if (!current)
        current = previous;

    pagesWidget->setCurrentIndex(contentsWidget->row(current));
}
