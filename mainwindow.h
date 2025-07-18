#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>

// Proverb structure definition
struct Proverb {
    QString proverb;
    QString transliteration;
    QString meaning;
    QString englishEquivalent;
    QStringList tags;
    QString region;
    QString usageContext;

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["proverb"] = proverb;
        obj["transliteration"] = transliteration;
        obj["meaning"] = meaning;
        obj["english_equivalent"] = englishEquivalent;

        QJsonArray tagsArray;
        for (const QString &tag : tags) {
            tagsArray.append(tag);
        }
        obj["tags"] = tagsArray;

        obj["region"] = region;
        obj["usage_context"] = usageContext;
        return obj;
    }

    static Proverb fromJson(const QJsonObject &obj) {
        Proverb p;
        p.proverb = obj["proverb"].toString();
        p.transliteration = obj["transliteration"].toString();
        p.meaning = obj["meaning"].toString();
        p.englishEquivalent = obj["english_equivalent"].toString();

        QJsonArray tagsArray = obj["tags"].toArray();
        for (const QJsonValue &tag : tagsArray) {
            p.tags.append(tag.toString());
        }

        p.region = obj["region"].toString();
        p.usageContext = obj["usage_context"].toString();
        return p;
    }
};

// Dialog for adding/editing proverbs
class ProverbDialog : public QDialog {
    Q_OBJECT

public:
    ProverbDialog(QWidget *parent = nullptr, const Proverb *proverbData = nullptr);
    Proverb getProverbData() const;

private:
    QTextEdit *proverbEdit;
    QTextEdit *transliterationEdit;
    QTextEdit *meaningEdit;
    QTextEdit *equivalentEdit;
    QLineEdit *tagsEdit;
    QLineEdit *regionEdit;
    QTextEdit *contextEdit;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showSelectedProverb(int index);
    void addProverb();
    void editProverb();
    void deleteProverb();
    void searchProverbs();
    void applyFilters();

private:
    // UI Elements
    QWidget *centralWidget;
    QSplitter *splitter;
    QWidget *leftPanel;
    QWidget *rightPanel;
    QVBoxLayout *leftLayout;
    QVBoxLayout *rightLayout;
    QHBoxLayout *searchLayout;
    QHBoxLayout *filterLayout;
    QHBoxLayout *buttonLayout;
    QLineEdit *searchInput;
    QComboBox *tagFilter;
    QComboBox *regionFilter;
    QListWidget *proverbList;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;

    // Proverb display elements
    QLabel *proverbDisplay;
    QLabel *transliterationDisplay;
    QLabel *meaningTitle;
    QLabel *meaningDisplay;
    QLabel *equivalentTitle;
    QLabel *equivalentDisplay;
    QLabel *tagsTitle;
    QLabel *tagsDisplay;
    QLabel *regionTitle;
    QLabel *regionDisplay;
    QLabel *contextTitle;
    QLabel *contextDisplay;

    // Data
    QList<Proverb> proverbs;
    QList<Proverb> filteredProverbs;
    QString filename;

    // Methods
    void setupUi();
    void loadProverbs();
    void saveProverbs();
    void refreshUi();
    void loadProverbList();
    void clearProverbDisplay();
    QStringList getAllTags() const;
    QStringList getAllRegions() const;
    QList<Proverb> searchProverbsList(const QString &query) const;
};

#endif // MAINWINDOW_H
