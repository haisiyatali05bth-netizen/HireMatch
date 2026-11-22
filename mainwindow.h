#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "storage.h"
#include "dsa.h"

class QTabWidget;
class QTableWidget;
class QLineEdit;
class QSpinBox;
class QLabel;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    Storage storage;
    QTabWidget* tabs = nullptr;
    QTableWidget* jobsTable = nullptr;
    QTableWidget* matchesTable = nullptr;
    QTableWidget* applicationsTable = nullptr;
    QLineEdit* nameEdit = nullptr;
    QLineEdit* educationEdit = nullptr;
    QLineEdit* skillsEdit = nullptr;
    QSpinBox* experienceSpin = nullptr;
    QLineEdit* companyEdit = nullptr;
    QLineEdit* titleEdit = nullptr;
    QLineEdit* locationEdit = nullptr;
    QLineEdit* jobSkillsEdit = nullptr;
    QSpinBox* jobExperienceSpin = nullptr;
    QLabel* dashboardLabel = nullptr;
    QLabel* candidateLabel = nullptr;

    UndoStack<int> undoStack;
    PendingQueue<int> pendingQueue;

    void buildUi();
    QWidget* buildDashboardTab();
    QWidget* buildCandidateTab();
    QWidget* buildJobsTab();
    QWidget* buildMatchingTab();
    QWidget* buildApplicationsTab();
    void refreshAll();
    void refreshJobs();
    void refreshMatches();
    void refreshApplications();
    void saveCandidate();
    void addJob();
    void applyForSelectedJob();
    void undoLastApplication();
    void showMessage(const QString& message);
};

#endif
