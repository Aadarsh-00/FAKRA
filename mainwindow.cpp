#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    filename("proverbs.json")
{
    setupUi();
    loadProverbs();
    loadProverbList();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi()
{
    centralWidget = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    splitter = new QSplitter(Qt::Horizontal);

    // Left panel setup
    leftPanel = new QWidget();
    leftLayout = new QVBoxLayout(leftPanel);

    // Search area
    searchLayout = new QHBoxLayout();
    searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Search proverbs...");
    connect(searchInput, &QLineEdit::textChanged, this, &MainWindow::searchProverbs);
    searchLayout->addWidget(searchInput);

    leftLayout->addLayout(searchLayout);

    // Filter options
    filterLayout = new QHBoxLayout();

    tagFilter = new QComboBox();
    regionFilter = new QComboBox();

    filterLayout->addWidget(new QLabel("Tag:"));
    filterLayout->addWidget(tagFilter);
    filterLayout->addWidget(new QLabel("Region:"));
    filterLayout->addWidget(regionFilter);

    leftLayout->addLayout(filterLayout);

    // Proverb list
    proverbList = new QListWidget();
    connect(proverbList, &QListWidget::currentRowChanged, this, &MainWindow::showSelectedProverb);
    leftLayout->addWidget(proverbList);

    // Buttons
    buttonLayout = new QHBoxLayout();

    addButton = new QPushButton("Add New");
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addProverb);

    editButton = new QPushButton("Edit");
    connect(editButton, &QPushButton::clicked, this, &MainWindow::editProverb);

    deleteButton = new QPushButton("Delete");
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteProverb);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);

    leftLayout->addLayout(buttonLayout);

    // Right panel setup
    rightPanel = new QWidget();
    rightLayout = new QVBoxLayout(rightPanel);

    proverbDisplay = new QLabel("");
    proverbDisplay->setWordWrap(true);
    proverbDisplay->setAlignment(Qt::AlignCenter);
    proverbDisplay->setStyleSheet(
        "font-size: 24px;"
        "color: #4b0082;"
        "margin: 10px;"
        "padding: 15px;"
        "background-color: #f0f0f0;"
        "border-radius: 10px;"
        );

    transliterationDisplay = new QLabel("");
    transliterationDisplay->setWordWrap(true);
    transliterationDisplay->setAlignment(Qt::AlignCenter);
    transliterationDisplay->setStyleSheet(
        "font-size: 18px;"
        "font-style: italic;"
        "color: #6a5acd;"
        "margin: 5px;"
        );

    meaningTitle = new QLabel("Meaning:");
    meaningTitle->setStyleSheet("font-weight: bold; font-size: 16px;");

    meaningDisplay = new QLabel("");
    meaningDisplay->setWordWrap(true);
    meaningDisplay->setStyleSheet(
        "font-size: 16px;"
        "padding: 10px;"
        "background-color: #0000FF;"
        "border-radius: 8px;"
        );

    equivalentTitle = new QLabel("English Equivalent:");
    equivalentTitle->setStyleSheet("font-weight: bold; font-size: 16px;");

    equivalentDisplay = new QLabel("");
    equivalentDisplay->setWordWrap(true);

    tagsTitle = new QLabel("Tags:");
    tagsTitle->setStyleSheet("font-weight: bold; font-size: 16px;");

    tagsDisplay = new QLabel("");

    regionTitle = new QLabel("Region:");
    regionTitle->setStyleSheet("font-weight: bold; font-size: 16px;");

    regionDisplay = new QLabel("");

    contextTitle = new QLabel("Usage Context:");
    contextTitle->setStyleSheet("font-weight: bold; font-size: 16px;");

    contextDisplay = new QLabel("");
    contextDisplay->setWordWrap(true);

    // Add all widgets to right layout
    rightLayout->addWidget(proverbDisplay);
    rightLayout->addWidget(transliterationDisplay);
    rightLayout->addWidget(meaningTitle);
    rightLayout->addWidget(meaningDisplay);
    rightLayout->addWidget(equivalentTitle);
    rightLayout->addWidget(equivalentDisplay);
    rightLayout->addWidget(tagsTitle);
    rightLayout->addWidget(tagsDisplay);
    rightLayout->addWidget(regionTitle);
    rightLayout->addWidget(regionDisplay);
    rightLayout->addWidget(contextTitle);
    rightLayout->addWidget(contextDisplay);

    rightLayout->addStretch();

    // Add panels to splitter
    splitter->addWidget(leftPanel);
    splitter->addWidget(rightPanel);

    // Set initial splitter sizes
    splitter->setSizes({300, 600});

    // Add splitter to main layout
    mainLayout->addWidget(splitter);

    // Set central widget
    setCentralWidget(centralWidget);

    // Connect filter comboboxes
    connect(tagFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::applyFilters);
    connect(regionFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::applyFilters);
}

void MainWindow::loadProverbs()
{
    QFile file(filename);

    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonArray jsonArray = doc.array();

        for (const QJsonValue &value : jsonArray) {
            QJsonObject obj = value.toObject();
            proverbs.append(Proverb::fromJson(obj));
        }

        file.close();
    } else {
        // Add sample data if file doesn't exist
        Proverb p1;
        p1.proverb = "जे बाढ़ि अनैत अछि, ओ नौका सेहो अनैत अछि";
        p1.transliteration = "Je barhi anait achhi, o nauka seho anait achhi";
        p1.meaning = "With every problem comes a solution";
        p1.englishEquivalent = "Every cloud has a silver lining";
        p1.tags = {"hope", "optimism"};
        p1.region = "Mithila";
        p1.usageContext = "Used to encourage someone facing difficulties";

        Proverb p2;
        p2.proverb = "थारी मे भात, हाथ मे लाठी";
        p2.transliteration = "Thari me bhat, hath me lathi";
        p2.meaning = "Being prepared for all situations";
        p2.englishEquivalent = "Better safe than sorry";
        p2.tags = {"preparation", "caution"};
        p2.region = "Darbhanga";
        p2.usageContext = "When advising someone to be cautious while moving forward";

        Proverb p8;
        p8.proverb = "Ankar nun tel, ankar ghee; hamra tora lagai aa ki?";
        p8.transliteration = "Ankar nun tel, ankar ghee; hamra tora lagai aa ki?";
        p8.meaning = "You have your own oil and ghee, do I need to apply it for you?";
        p8.englishEquivalent = "Mind your own business.";
        p8.tags = {"self-reliance", "independence"};
        p8.region = "Mithila";
        p8.usageContext = "Used to tell someone to handle their own affairs.";

        Proverb p9;
        p9.proverb = "Aayl pani, gel pani, biche bilyal pani.";
        p9.transliteration = "Aayl pani, gel pani, biche bilyal pani.";
        p9.meaning = "Water came, water went, and the cat remained stuck in between.";
        p9.englishEquivalent = "Caught in the middle.";
        p9.tags = {"trouble", "confusion"};
        p9.region = "Mithila";
        p9.usageContext = "Used when someone is stuck between two opposing forces.";

        Proverb p3;
        p3.proverb = "Hey labna, tu bhitre rahna.";
        p3.transliteration = "Hey labna, tu bhitre rahna.";
        p3.meaning = "Hey foolish one, stay inside.";
        p3.englishEquivalent = "Better to stay silent than to expose ignorance.";
        p3.tags = {"ignorance", "wisdom"};
        p3.region = "Mithila";
        p3.usageContext = "Used to advise someone to avoid making a fool of themselves.";

        Proverb p4;
        p4.proverb = "Aaitho khelau, peto nai bharal.";
        p4.transliteration = "Aaitho khelau, peto nai bharal.";
        p4.meaning = "I played the game, but my stomach remains empty.";
        p4.englishEquivalent = "All play and no gain.";
        p4.tags = {"effort", "reward"};
        p4.region = "Mithila";
        p4.usageContext = "Used when efforts don't yield expected results.";

        Proverb p5;
        p5.proverb = "Du du gam bhoj bhel, kukurak pet bhukhle.";
        p5.transliteration = "Du du gam bhoj bhel, kukurak pet bhukhle.";
        p5.meaning = "A feast in two villages, but the dog remains hungry.";
        p5.englishEquivalent = "Left out despite abundance.";
        p5.tags = {"exclusion", "injustice"};
        p5.region = "Mithila";
        p5.usageContext = "Used when someone is left out despite there being plenty.";


        proverbs.append(p1);
        proverbs.append(p2);
        proverbs.append(p3);
        proverbs.append(p4);
        proverbs.append(p5);
        proverbs.append(p8);
        proverbs.append(p9);

        saveProverbs();
    }

    // Update the filters
    tagFilter->clear();
    tagFilter->addItem("All Tags");
    tagFilter->addItems(getAllTags());

    regionFilter->clear();
    regionFilter->addItem("All Regions");
    regionFilter->addItems(getAllRegions());

    // Set initial filtered proverbs to all proverbs
    filteredProverbs = proverbs;
}

void MainWindow::saveProverbs()
{
    QJsonArray jsonArray;

    for (const Proverb &proverb : proverbs) {
        jsonArray.append(proverb.toJson());
    }

    QJsonDocument doc(jsonArray);
    QFile file(filename);

    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    } else {
        QMessageBox::warning(this, "Error", "Could not save proverbs to file.");
    }
}

void MainWindow::loadProverbList()
{
    proverbList->clear();

    for (const Proverb &proverb : filteredProverbs) {
        proverbList->addItem(proverb.proverb);
    }
}

void MainWindow::showSelectedProverb(int index)
{
    if (index < 0 || index >= filteredProverbs.size()) {
        clearProverbDisplay();
        return;
    }

    const Proverb &proverb = filteredProverbs[index];

    proverbDisplay->setText(proverb.proverb);
    transliterationDisplay->setText(proverb.transliteration);
    meaningDisplay->setText(proverb.meaning);
    equivalentDisplay->setText(proverb.englishEquivalent);
    tagsDisplay->setText(proverb.tags.join(", "));
    regionDisplay->setText(proverb.region);
    contextDisplay->setText(proverb.usageContext);
}

void MainWindow::clearProverbDisplay()
{
    proverbDisplay->setText("");
    transliterationDisplay->setText("");
    meaningDisplay->setText("");
    equivalentDisplay->setText("");
    tagsDisplay->setText("");
    regionDisplay->setText("");
    contextDisplay->setText("");
}

void MainWindow::addProverb()
{
    ProverbDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        proverbs.append(dialog.getProverbData());
        saveProverbs();
        refreshUi();
    }
}

void MainWindow::editProverb()
{
    int currentRow = proverbList->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Warning", "Please select a proverb to edit.");
        return;
    }

    const Proverb &currentProverb = filteredProverbs[currentRow];
    ProverbDialog dialog(this, &currentProverb);

    if (dialog.exec() == QDialog::Accepted) {
        // Find the index in the original list
        for (int i = 0; i < proverbs.size(); ++i) {
            if (proverbs[i].proverb == currentProverb.proverb) {
                proverbs[i] = dialog.getProverbData();
                break;
            }
        }

        saveProverbs();
        refreshUi();
    }
}

void MainWindow::deleteProverb()
{
    int currentRow = proverbList->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Warning", "Please select a proverb to delete.");
        return;
    }

    QMessageBox::StandardButton confirmation = QMessageBox::question(
        this, "Confirm Deletion",
        "Are you sure you want to delete this proverb?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (confirmation == QMessageBox::Yes) {
        const Proverb &currentProverb = filteredProverbs[currentRow];

        // Find and remove from the original list
        for (int i = 0; i < proverbs.size(); ++i) {
            if (proverbs[i].proverb == currentProverb.proverb) {
                proverbs.removeAt(i);
                break;
            }
        }

        saveProverbs();
        refreshUi();
    }
}

void MainWindow::searchProverbs()
{
    applyFilters();
}

void MainWindow::applyFilters()
{
    QString searchText = searchInput->text().toLower();
    QString tagText = tagFilter->currentText();
    QString regionText = regionFilter->currentText();

    // Start with search results or all proverbs
    if (!searchText.isEmpty()) {
        filteredProverbs = searchProverbsList(searchText);
    } else {
        filteredProverbs = proverbs;
    }

    // Apply tag filter if selected
    if (tagText != "All Tags") {
        QList<Proverb> tagFiltered;
        for (const Proverb &p : filteredProverbs) {
            if (p.tags.contains(tagText)) {
                tagFiltered.append(p);
            }
        }
        filteredProverbs = tagFiltered;
    }

    // Apply region filter if selected
    if (regionText != "All Regions") {
        QList<Proverb> regionFiltered;
        for (const Proverb &p : filteredProverbs) {
            if (p.region == regionText) {
                regionFiltered.append(p);
            }
        }
        filteredProverbs = regionFiltered;
    }

    // Update the list
    loadProverbList();
}

QList<Proverb> MainWindow::searchProverbsList(const QString &query) const
{
    QList<Proverb> results;

    for (const Proverb &p : proverbs) {
        if (p.proverb.toLower().contains(query) ||
            p.transliteration.toLower().contains(query) ||
            p.meaning.toLower().contains(query)) {
            results.append(p);
            continue;
        }

        // Search in tags
        for (const QString &tag : p.tags) {
            if (tag.toLower().contains(query)) {
                results.append(p);
                break;
            }
        }
    }

    return results;
}

QStringList MainWindow::getAllTags() const
{
    QSet<QString> tagsSet;

    for (const Proverb &p : proverbs) {
        for (const QString &tag : p.tags) {
            tagsSet.insert(tag);
        }
    }

    QStringList tagsList = tagsSet.values();
    std::sort(tagsList.begin(), tagsList.end());
    return tagsList;
}

QStringList MainWindow::getAllRegions() const
{
    QSet<QString> regionsSet;

    for (const Proverb &p : proverbs) {
        if (!p.region.isEmpty()) {
            regionsSet.insert(p.region);
        }
    }

    QStringList regionsList = regionsSet.values();
    std::sort(regionsList.begin(), regionsList.end());
    return regionsList;
}

void MainWindow::refreshUi()
{
    // Save current filter selections
    QString currentTag = tagFilter->currentText();
    QString currentRegion = regionFilter->currentText();

    // Update filters
    tagFilter->clear();
    tagFilter->addItem("All Tags");
    tagFilter->addItems(getAllTags());

    regionFilter->clear();
    regionFilter->addItem("All Regions");
    regionFilter->addItems(getAllRegions());

    // Restore previous selections
    int tagIndex = tagFilter->findText(currentTag);
    tagFilter->setCurrentIndex(tagIndex >= 0 ? tagIndex : 0);

    int regionIndex = regionFilter->findText(currentRegion);
    regionFilter->setCurrentIndex(regionIndex >= 0 ? regionIndex : 0);

    // Apply filters to refresh the list
    applyFilters();
}

// ProverbDialog implementation
ProverbDialog::ProverbDialog(QWidget *parent, const Proverb *proverbData)
    : QDialog(parent)
{
    setWindowTitle(proverbData ? "Edit Proverb" : "Add New Proverb");
    setMinimumWidth(500);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    proverbEdit = new QTextEdit();
    proverbEdit->setPlaceholderText("Enter proverb in Maithili");

    transliterationEdit = new QTextEdit();
    transliterationEdit->setPlaceholderText("Enter transliteration");

    meaningEdit = new QTextEdit();
    meaningEdit->setPlaceholderText("Enter meaning");

    equivalentEdit = new QTextEdit();
    equivalentEdit->setPlaceholderText("English equivalent (if any)");

    tagsEdit = new QLineEdit();
    tagsEdit->setPlaceholderText("Enter tags separated by commas");

    regionEdit = new QLineEdit();
    regionEdit->setPlaceholderText("Region of origin");

    contextEdit = new QTextEdit();
    contextEdit->setPlaceholderText("When/how is this proverb typically used?");

    // Populate fields if editing
    if (proverbData) {
        proverbEdit->setText(proverbData->proverb);
        transliterationEdit->setText(proverbData->transliteration);
        meaningEdit->setText(proverbData->meaning);
        equivalentEdit->setText(proverbData->englishEquivalent);
        tagsEdit->setText(proverbData->tags.join(", "));
        regionEdit->setText(proverbData->region);
        contextEdit->setText(proverbData->usageContext);
    }

    formLayout->addRow("Proverb:", proverbEdit);
    formLayout->addRow("Transliteration:", transliterationEdit);
    formLayout->addRow("Meaning:", meaningEdit);
    formLayout->addRow("English Equivalent:", equivalentEdit);
    formLayout->addRow("Tags:", tagsEdit);
    formLayout->addRow("Region:", regionEdit);
    formLayout->addRow("Usage Context:", contextEdit);

    layout->addLayout(formLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *saveButton = new QPushButton("Save");
    connect(saveButton, &QPushButton::clicked, this, &QDialog::accept);

    QPushButton *cancelButton = new QPushButton("Cancel");
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);

    layout->addLayout(buttonLayout);
}

Proverb ProverbDialog::getProverbData() const
{
    Proverb p;
    p.proverb = proverbEdit->toPlainText();
    p.transliteration = transliterationEdit->toPlainText();
    p.meaning = meaningEdit->toPlainText();
    p.englishEquivalent = equivalentEdit->toPlainText();

    QString tagsString = tagsEdit->text();
    QStringList tagsList = tagsString.split(",", Qt::SkipEmptyParts);
    for (QString &tag : tagsList) {
        tag = tag.trimmed();
    }
    p.tags = tagsList;

    p.region = regionEdit->text();
    p.usageContext = contextEdit->toPlainText();

    return p;
}
