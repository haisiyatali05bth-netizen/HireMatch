#include "mainwindow.h"
#include "matching.h"
#include "models.h"

#include <QTabWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QGroupBox>
#include <QFont>
#include <QTableWidgetItem>
#include <algorithm>
#include <sstream>
#include <cctype>

namespace {
QString qstr(const std::string& value) { return QString::fromStdString(value); }

std::vector<std::string> parseSkills(const QString& text) {
    std::vector<std::string> result;
    for (QString part : text.split(',', Qt::SkipEmptyParts)) {
        part = part.trimmed().toLower();
        if (!part.isEmpty()) result.push_back(part.toStdString());
    }
    return result;
}

QString joinSkills(const std::vector<std::string>& skills) {
    QStringList list;
    for (const auto& s : skills) list << qstr(s);
    return list.join(", ");
}

QTableWidget* makeTable(const QStringList& headers) {
    auto* table = new QTableWidget;
    table->setColumnCount(headers.size());
    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);
    return table;
}

void setCell(QTableWidget* table, int row, int col, const QString& value) {
    table->setItem(row, col, new QTableWidgetItem(value));
}
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), storage("data") {
    setWindowTitle("HireMatch - Smart Job Matching System");
    resize(1120, 720);
    buildUi();
    refreshAll();
}

void MainWindow::buildUi() {
    auto* central = new QWidget;
    auto* root = new QVBoxLayout(central);

    auto* heading = new QLabel("HIREMATCH");
    heading->setObjectName("heading");
    auto* subtitle = new QLabel("Smart Job Matching System  |  C++17 + Qt");
    subtitle->setObjectName("subtitle");
    root->addWidget(heading);
    root->addWidget(subtitle);

    tabs = new QTabWidget;
    tabs->addTab(buildDashboardTab(), "Dashboard");
    tabs->addTab(buildCandidateTab(), "Candidate Profile");
    tabs->addTab(buildJobsTab(), "Job Management");
    tabs->addTab(buildMatchingTab(), "Smart Matching");
    tabs->addTab(buildApplicationsTab(), "Applications");
    root->addWidget(tabs);
    setCentralWidget(central);

    setStyleSheet(R"(
        QMainWindow, QWidget { background: #f4f7fb; color: #172033; font-family: Segoe UI, Arial; font-size: 10pt; }
        QLabel#heading { color: #173b70; font-size: 25pt; font-weight: 800; padding-top: 8px; }
        QLabel#subtitle { color: #60718b; font-size: 10pt; padding-bottom: 10px; }
        QTabWidget::pane { border: 1px solid #dce4ef; background: white; border-radius: 8px; }
        QTabBar::tab { background: #e8eef7; padding: 11px 18px; margin-right: 3px; border-top-left-radius: 6px; border-top-right-radius: 6px; }
        QTabBar::tab:selected { background: #1f5da8; color: white; }
        QGroupBox { background: white; border: 1px solid #dce4ef; border-radius: 8px; margin-top: 12px; padding: 12px; font-weight: bold; }
        QLineEdit, QSpinBox { background: white; border: 1px solid #cbd5e1; border-radius: 5px; padding: 8px; }
        QPushButton { background: #1f5da8; color: white; border: 0; border-radius: 5px; padding: 9px 16px; font-weight: bold; }
        QPushButton:hover { background: #17477f; }
        QTableWidget { background: white; border: 1px solid #dce4ef; gridline-color: #e7edf5; selection-background-color: #d9eaff; selection-color: #172033; }
        QHeaderView::section { background: #eaf0f8; padding: 8px; border: 0; font-weight: bold; }
    )");
}

QWidget* MainWindow::buildDashboardTab() {
    auto* page = new QWidget;
    auto* layout = new QVBoxLayout(page);
    auto* intro = new QLabel("Welcome to HireMatch. Find opportunities that match your skills.");
    intro->setStyleSheet("font-size: 15pt; font-weight: 600; color: #203b60; padding: 16px;");
    dashboardLabel = new QLabel;
    dashboardLabel->setStyleSheet("background:white; border:1px solid #dce4ef; border-radius:8px; padding:24px; font-size:14pt;");
    candidateLabel = new QLabel;
    candidateLabel->setStyleSheet("background:white; border:1px solid #dce4ef; border-radius:8px; padding:18px;");
    layout->addWidget(intro);
    layout->addWidget(dashboardLabel);
    layout->addWidget(candidateLabel);
    layout->addStretch();
    return page;
}

QWidget* MainWindow::buildCandidateTab() {
    auto* page = new QWidget;
    auto* layout = new QVBoxLayout(page);
    auto* group = new QGroupBox("Candidate details");
    auto* form = new QFormLayout(group);

    nameEdit = new QLineEdit;
    nameEdit->setPlaceholderText("e.g. Rahul Sharma");
    educationEdit = new QLineEdit;
    educationEdit->setPlaceholderText("e.g. B.Tech CSE");
    experienceSpin = new QSpinBox;
    experienceSpin->setRange(0, 50);
    skillsEdit = new QLineEdit;
    skillsEdit->setPlaceholderText("C++, Java, DSA, SQL (comma separated)");

    form->addRow("Full name:", nameEdit);
    form->addRow("Education:", educationEdit);
    form->addRow("Experience (years):", experienceSpin);
    form->addRow("Skills:", skillsEdit);

    auto* save = new QPushButton("Save Candidate Profile");
    connect(save, &QPushButton::clicked, this, &MainWindow::saveCandidate);

    layout->addWidget(group);
    layout->addWidget(save);
    layout->addWidget(new QLabel("Tip: Use comma-separated skills. Matching ignores uppercase/lowercase differences."));
    layout->addStretch();
    return page;
}

QWidget* MainWindow::buildJobsTab() {
    auto* page = new QWidget;
    auto* layout = new QVBoxLayout(page);
    auto* group = new QGroupBox("Add a job opportunity");
    auto* form = new QFormLayout(group);

    companyEdit = new QLineEdit;
    titleEdit = new QLineEdit;
    locationEdit = new QLineEdit;
    jobExperienceSpin = new QSpinBox;
    jobExperienceSpin->setRange(0, 50);
    jobSkillsEdit = new QLineEdit;

    companyEdit->setPlaceholderText("e.g. Infosys");
    titleEdit->setPlaceholderText("e.g. Software Developer");
    locationEdit->setPlaceholderText("e.g. Chandigarh");
    jobSkillsEdit->setPlaceholderText("C++, DSA, SQL");

    form->addRow("Company:", companyEdit);
    form->addRow("Job title:", titleEdit);
    form->addRow("Location:", locationEdit);
    form->addRow("Experience required:", jobExperienceSpin);
    form->addRow("Required skills:", jobSkillsEdit);

    auto* add = new QPushButton("Add Job");
    connect(add, &QPushButton::clicked, this, &MainWindow::addJob);

    jobsTable = makeTable({"ID", "Company", "Job title", "Location", "Experience", "Required skills"});
    layout->addWidget(group);
    layout->addWidget(add);
    layout->addWidget(new QLabel("Available jobs"));
    layout->addWidget(jobsTable);
    return page;
}

QWidget* MainWindow::buildMatchingTab() {
    auto* page = new QWidget;
    auto* layout = new QVBoxLayout(page);
    auto* info = new QLabel("Recommendations are ranked using a custom Merge Sort implementation.\nMatch score = matched required skills / total required skills × 100.");
    info->setStyleSheet("background:#e8f1ff; padding:12px; border-radius:6px;");
    auto* refresh = new QPushButton("Find Matching Jobs");
    matchesTable = makeTable({"Rank", "Job ID", "Company", "Job title", "Match %", "Matched skills", "Missing skills"});
    connect(refresh, &QPushButton::clicked, this, &MainWindow::refreshMatches);
    layout->addWidget(info);
    layout->addWidget(refresh);
    layout->addWidget(matchesTable);
    return page;
}

QWidget* MainWindow::buildApplicationsTab() {
    auto* page = new QWidget;
    auto* layout = new QVBoxLayout(page);
    auto* info = new QLabel("Select a job in Job Management or Smart Matching, then use Apply for Selected Job.");
    info->setStyleSheet("background:#e8f1ff; padding:12px; border-radius:6px;");
    auto* apply = new QPushButton("Apply for Selected Job");
    auto* undo = new QPushButton("Undo Last Application");
    auto* process = new QPushButton("Process Next Pending Application");
    applicationsTable = makeTable({"Application ID", "Job ID", "Company / Job", "Status"});
    connect(apply, &QPushButton::clicked, this, &MainWindow::applyForSelectedJob);
    connect(undo, &QPushButton::clicked, this, &MainWindow::undoLastApplication);
    connect(process, &QPushButton::clicked, this, [this]() {
        auto apps = storage.loadApplications();
        PendingQueue<int> queue;
        for (const auto& a : apps) if (a.status == "Pending") queue.enqueue(a.id);
        if (queue.empty()) { showMessage("There are no pending applications."); return; }
        int next = queue.dequeue();
        for (auto& a : apps) if (a.id == next) a.status = "Under Review";
        storage.saveApplications(apps);
        refreshApplications();
        showMessage("The oldest pending application is now Under Review.");
    });
    auto* buttons = new QHBoxLayout;
    buttons->addWidget(apply);
    buttons->addWidget(undo);
    buttons->addWidget(process);
    layout->addWidget(info);
    layout->addLayout(buttons);
    layout->addWidget(applicationsTable);
    return page;
}

void MainWindow::refreshAll() {
    refreshJobs();
    refreshMatches();
    refreshApplications();

    auto candidates = storage.loadCandidates();
    auto jobs = storage.loadJobs();
    auto apps = storage.loadApplications();
    dashboardLabel->setText(QString(" <b>%1</b> jobs available<br><br><b>%2</b> candidate profiles<br><br><b>%3</b> applications submitted")
                             .arg(jobs.size()).arg(candidates.size()).arg(apps.size()));
    if (!candidates.empty()) {
        const auto& c = candidates.back();
        candidateLabel->setText("<b>Latest candidate:</b> " + qstr(c.name) +
                                "<br><b>Education:</b> " + qstr(c.education) +
                                "<br><b>Skills:</b> " + joinSkills(c.skills));
    } else {
        candidateLabel->setText("No candidate profile saved yet. Open Candidate Profile to get started.");
    }
}

void MainWindow::refreshJobs() {
    auto jobs = storage.loadJobs();
    jobsTable->setRowCount(static_cast<int>(jobs.size()));
    for (int i = 0; i < static_cast<int>(jobs.size()); ++i) {
        const auto& j = jobs[i];
        setCell(jobsTable, i, 0, QString::number(j.id));
        setCell(jobsTable, i, 1, qstr(j.company));
        setCell(jobsTable, i, 2, qstr(j.title));
        setCell(jobsTable, i, 3, qstr(j.location));
        setCell(jobsTable, i, 4, QString::number(j.experienceRequired) + " years");
        setCell(jobsTable, i, 5, joinSkills(j.skills));
    }
}

void MainWindow::refreshMatches() {
    if (!matchesTable) return;
    auto candidates = storage.loadCandidates();
    auto jobs = storage.loadJobs();
    matchesTable->setRowCount(0);
    if (candidates.empty()) return;
    auto results = MatchingEngine::recommend(candidates.back(), jobs);
    matchesTable->setRowCount(static_cast<int>(results.size()));
    for (int i = 0; i < static_cast<int>(results.size()); ++i) {
        const auto& m = results[i];
        setCell(matchesTable, i, 0, QString::number(i + 1));
        setCell(matchesTable, i, 1, QString::number(m.job.id));
        setCell(matchesTable, i, 2, qstr(m.job.company));
        setCell(matchesTable, i, 3, qstr(m.job.title));
        setCell(matchesTable, i, 4, QString::number(m.score, 'f', 2) + "%");
        setCell(matchesTable, i, 5, joinSkills(m.matchedSkills));
        setCell(matchesTable, i, 6, joinSkills(m.missingSkills));
    }
}

void MainWindow::refreshApplications() {
    auto apps = storage.loadApplications();
    auto jobs = storage.loadJobs();
    applicationsTable->setRowCount(static_cast<int>(apps.size()));
    for (int i = 0; i < static_cast<int>(apps.size()); ++i) {
        const auto& a = apps[i];
        QString jobName = "Unknown job";
        for (const auto& j : jobs)
            if (j.id == a.jobId) jobName = qstr(j.company) + " - " + qstr(j.title);
        setCell(applicationsTable, i, 0, QString::number(a.id));
        setCell(applicationsTable, i, 1, QString::number(a.jobId));
        setCell(applicationsTable, i, 2, jobName);
        setCell(applicationsTable, i, 3, qstr(a.status));
    }
}

void MainWindow::saveCandidate() {
    QString name = nameEdit->text().trimmed();
    QString education = educationEdit->text().trimmed();
    auto skills = parseSkills(skillsEdit->text());
    if (name.isEmpty() || education.isEmpty() || skills.empty()) {
        showMessage("Please enter name, education and at least one skill.");
        return;
    }
    Candidate c;
    c.id = storage.nextCandidateId();
    c.name = name.toStdString();
    c.education = education.toStdString();
    c.experienceYears = experienceSpin->value();
    c.skills = skills;
    auto candidates = storage.loadCandidates();
    candidates.push_back(c);
    if (!storage.saveCandidates(candidates)) { showMessage("Could not save candidate data."); return; }
    refreshAll();
    showMessage("Candidate profile saved successfully.");
}

void MainWindow::addJob() {
    QString company = companyEdit->text().trimmed();
    QString title = titleEdit->text().trimmed();
    QString location = locationEdit->text().trimmed();
    auto skills = parseSkills(jobSkillsEdit->text());
    if (company.isEmpty() || title.isEmpty() || location.isEmpty() || skills.empty()) {
        showMessage("Please complete company, title, location and required skills.");
        return;
    }
    Job j;
    j.id = storage.nextJobId();
    j.company = company.toStdString();
    j.title = title.toStdString();
    j.location = location.toStdString();
    j.experienceRequired = jobExperienceSpin->value();
    j.skills = skills;
    auto jobs = storage.loadJobs();
    jobs.push_back(j);
    if (!storage.saveJobs(jobs)) { showMessage("Could not save job data."); return; }

    // Rebuild a BST from stored IDs and verify the new ID is searchable.
    JobIdBST tree;
    for (const auto& item : jobs) tree.insert(item.id);
    refreshAll();
    companyEdit->clear(); titleEdit->clear(); locationEdit->clear(); jobSkillsEdit->clear();
    showMessage(tree.search(j.id) ? "Job added. Its ID was indexed in the Job BST." : "Job added.");
}

void MainWindow::applyForSelectedJob() {
    int jobId = -1;
    if (tabs->currentIndex() == 2 && jobsTable->currentRow() >= 0)
        jobId = jobsTable->item(jobsTable->currentRow(), 0)->text().toInt();
    else if (tabs->currentIndex() == 3 && matchesTable->currentRow() >= 0)
        jobId = matchesTable->item(matchesTable->currentRow(), 1)->text().toInt();
    else {
        if (jobsTable->currentRow() >= 0)
            jobId = jobsTable->item(jobsTable->currentRow(), 0)->text().toInt();
        else if (matchesTable->currentRow() >= 0)
            jobId = matchesTable->item(matchesTable->currentRow(), 1)->text().toInt();
    }
    if (jobId < 0) { showMessage("Select a job row in Job Management or Smart Matching first."); return; }

    auto apps = storage.loadApplications();
    for (const auto& a : apps)
        if (a.jobId == jobId) { showMessage("You have already applied for this job."); return; }

    Application a{storage.nextApplicationId(), jobId, "Pending"};
    apps.push_back(a);
    if (!storage.saveApplications(apps)) { showMessage("Could not save application."); return; }
    undoStack.push(a.id);
    pendingQueue.enqueue(a.id);
    refreshAll();
    showMessage("Application submitted. It has been added to the pending queue.");
}

void MainWindow::undoLastApplication() {
    if (undoStack.empty()) { showMessage("There is no application to undo in this session."); return; }
    int id = undoStack.pop();
    auto apps = storage.loadApplications();
    auto oldSize = apps.size();
    apps.erase(std::remove_if(apps.begin(), apps.end(),
                              [id](const Application& a) { return a.id == id; }), apps.end());
    if (apps.size() == oldSize) { showMessage("That application is no longer available to undo."); return; }
    storage.saveApplications(apps);
    refreshAll();
    showMessage("The most recent application from this session was removed.");
}

void MainWindow::showMessage(const QString& message) {
    QMessageBox::information(this, "HireMatch", message);
}
