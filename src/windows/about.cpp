#include "about.h"
#include "../common/util.h"

AboutDialog::AboutDialog() : QDialog(nullptr, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint) {
    auto *mainwidget = new QWidget;
    auto *textLayout = new QVBoxLayout(mainwidget);
    auto link = new QLabel;

    link->setText("<a href=\"https://github.com/GregWagner/et3400/\">https://github.com/GregWagner/et3400</a>");
    link->setTextFormat(Qt::RichText);
    link->setTextInteractionFlags(Qt::TextBrowserInteraction);
    link->setOpenExternalLinks(true);

    auto *title = new QLabel("ET-3400 Trainer Emulator");

    title->setStyleSheet("QLabel { font-size:20px; font-weight: bold }");
    textLayout->addWidget(title);
    textLayout->addWidget(new QLabel("v" + getVersion()));
    textLayout->addWidget(new QLabel("©2026 Greg Wagner"));
    textLayout->addWidget(new QLabel("rupert.avery@gmail.com"));
    textLayout->addWidget(link);
    mainwidget->setFixedHeight(150);

    auto *mainLayout = new QVBoxLayout;
    mainLayout->addStretch(1);
    mainLayout->addWidget(mainwidget);
    mainLayout->addStretch(1);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    setLayout(mainLayout);

    this->setStyleSheet("QLabel { font-size:12px; height: 20px }");

    setFixedSize(QSize(350, 250));
    setWindowTitle("About ET-3400");
}