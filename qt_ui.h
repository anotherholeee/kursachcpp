#ifndef QT_UI_H
#define QT_UI_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QTextEdit>
#include <QTabWidget>
#include <QGroupBox>
#include <QMessageBox>
#include <QDialog>
#include <QSpinBox>
#include <QTimeEdit>
#include <QCheckBox>
#include <QListWidget>
#include <QSplitter>
#include <QHeaderView>
#include <QScrollArea>
#include "transport_system.h"

// Главное окно приложения
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(TransportSystem* system, QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void returnToLoginScreen();

private slots:
    void showLoginDialog();
    void showGuestMode();
    void showAdminMode();
    void saveAndExit();

private:
    TransportSystem* transportSystem;
    bool isAdminMode;
    QWidget* centralWidget;
    QVBoxLayout* mainLayout;
    QPushButton* adminButton;
    QPushButton* guestButton;
    QPushButton* exitButton;
    
    void setupUI();
    void createLoginScreen();
};

// Гостевой режим
class GuestModeWidget : public QWidget {
    Q_OBJECT

public:
    explicit GuestModeWidget(TransportSystem* system, QWidget *parent = nullptr);

signals:
    void backToMainRequested();

private slots:
    void viewTransportSchedule();
    void viewStopTimetable();
    void searchRoutes();
    void backToMain();

private:
    TransportSystem* transportSystem;
    QTabWidget* tabWidget;
    
    void setupUI();
};

// Административный режим
class AdminModeWidget : public QWidget {
    Q_OBJECT

public:
    explicit AdminModeWidget(TransportSystem* system, QWidget *parent = nullptr);
    void refreshAll();

signals:
    void backToMainRequested();

private slots:
    void viewTransportSchedule();
    void viewStopTimetable();
    void searchRoutes();
    void calculateArrivalTime();
    void addRoute();
    void addTrip();
    void addVehicle();
    void addStop();
    void addDriver();
    void removeRoute();
    void removeTrip();
    void removeVehicle();
    void removeStop();
    void removeDriver();
    void viewAllData();
    void saveData();
    void undoLastAction();
    void redoLastAction();
    void backToMain();

private:
    TransportSystem* transportSystem;
    QTabWidget* tabWidget;
    QTableWidget* routesTable;
    QTableWidget* tripsTable;
    QTableWidget* vehiclesTable;
    QTableWidget* stopsTable;
    QTableWidget* driversTable;
    
    void setupUI();
    void refreshRoutesTable();
    void refreshTripsTable();
    void refreshVehiclesTable();
    void refreshStopsTable();
    void refreshDriversTable();
};

// Диалог входа администратора
class AdminLoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit AdminLoginDialog(TransportSystem* system, QWidget *parent = nullptr);
    bool isAuthenticated() const { return authenticated; }

private slots:
    void tryLogin();

private:
    TransportSystem* transportSystem;
    bool authenticated;
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;
};

// Диалог просмотра расписания транспорта
class TransportScheduleDialog : public QDialog {
    Q_OBJECT

public:
    explicit TransportScheduleDialog(TransportSystem* system, QWidget *parent = nullptr);

private slots:
    void onVehicleTypeChanged();
    void onWeekDayChanged();
    void onRouteChanged();
    void onFilterChanged();

private:
    TransportSystem* transportSystem;
    QComboBox* vehicleTypeComboBox;
    QComboBox* routeComboBox;
    QComboBox* weekDayComboBox;
    QTableWidget* scheduleTable;
    void populateSchedule(const std::string& vehicleType = "", int weekDay = 0, int routeNumber = 0);
    void populateRoutes(const std::string& vehicleType);
};

// Диалог расписания остановки
class StopTimetableDialog : public QDialog {
    Q_OBJECT

public:
    explicit StopTimetableDialog(TransportSystem* system, QWidget *parent = nullptr);

private slots:
    void onStopSelected();
    void onWeekDayChanged();

private:
    TransportSystem* transportSystem;
    QComboBox* stopComboBox;
    QComboBox* weekDayComboBox;
    QTableWidget* timetableTable;
    void populateStops();
    void populateTimetable(const std::string& stopName, int weekDay = 0);
};

// Диалог поиска маршрутов
class RouteSearchDialog : public QDialog {
    Q_OBJECT

public:
    explicit RouteSearchDialog(TransportSystem* system, QWidget *parent = nullptr);

private slots:
    void onSearchClicked();

private:
    TransportSystem* transportSystem;
    QComboBox* stopAComboBox;
    QComboBox* stopBComboBox;
    QCheckBox* includeTransfersCheckBox;
    QTextEdit* resultsText;
    void populateStops();
};

// Диалог добавления маршрута
class AddRouteDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddRouteDialog(TransportSystem* system, QWidget *parent = nullptr);

private slots:
    void onAddClicked();
    void addStopToList();
    void removeStopFromList();

private:
    TransportSystem* transportSystem;
    QSpinBox* routeNumberSpinBox;
    QComboBox* vehicleTypeComboBox;
    QListWidget* stopsListWidget;
    QComboBox* stopComboBox;
    QCheckBox* mondayCheck;
    QCheckBox* tuesdayCheck;
    QCheckBox* wednesdayCheck;
    QCheckBox* thursdayCheck;
    QCheckBox* fridayCheck;
    QCheckBox* saturdayCheck;
    QCheckBox* sundayCheck;
    void populateStops();
};

// Диалог добавления рейса
class AddTripDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddTripDialog(TransportSystem* system, QWidget *parent = nullptr);

private slots:
    void onAddClicked();
    void onRouteChanged();
    void onAddDriverClicked();

private:
    TransportSystem* transportSystem;
    QSpinBox* tripIdSpinBox;
    QComboBox* routeComboBox;
    QComboBox* vehicleComboBox;
    QComboBox* driverComboBox;
    QTimeEdit* startTimeEdit;
    QSpinBox* weekDaySpinBox;
    void populateData();
    void populateVehicles(const std::string& vehicleType);
    void populateDrivers(const std::string& vehicleType);
};

// Диалог добавления транспортного средства
class AddVehicleDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddVehicleDialog(TransportSystem* system, QWidget *parent = nullptr);

private slots:
    void onVehicleTypeChanged(int index);
    void onAddClicked();

private:
    TransportSystem* transportSystem;
    QComboBox* vehicleTypeComboBox;
    QLineEdit* licensePlateEdit;
    QLineEdit* modelEdit;
    QSpinBox* capacityEdit;
    QLineEdit* fuelCapacityEdit; // для автобуса
    QLineEdit* batteryCapacityEdit; // для трамвая/троллейбуса
    QWidget* fuelWidget;
    QWidget* batteryWidget;
    void setupUI();
};

// Диалог добавления остановки
class AddStopDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddStopDialog(TransportSystem* system, QWidget *parent = nullptr);

private slots:
    void onAddClicked();

private:
    TransportSystem* transportSystem;
    QSpinBox* stopIdSpinBox;
    QLineEdit* stopNameEdit;
};

// Диалог добавления водителя
class AddDriverDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddDriverDialog(TransportSystem* system, QWidget *parent = nullptr);

private slots:
    void onAddClicked();

private:
    TransportSystem* transportSystem;
    QLineEdit* firstNameEdit;
    QLineEdit* lastNameEdit;
    QLineEdit* middleNameEdit;
    QComboBox* categoryComboBox;
};

// Диалог расчета времени прибытия
class ArrivalTimeDialog : public QDialog {
    Q_OBJECT

public:
    explicit ArrivalTimeDialog(TransportSystem* system, QWidget *parent = nullptr);

private slots:
    void onCalculateClicked();

private:
    TransportSystem* transportSystem;
    QComboBox* tripComboBox;
    QDoubleSpinBox* speedSpinBox;
    QTextEdit* resultsText;
    void populateTrips();
};

#endif // QT_UI_H

