#include "qt_ui.h"
#include "bus.h"
#include "tram.h"
#include "trolleybus.h"
#include "driver.h"
#include "route.h"
#include "trip.h"
#include "time.h"
#include "stop.h"
#include "exceptions.h"
#include "fuel_transport.h"
#include "electric_transport.h"
#include "commands.h"
#include "journey.h"
#include "journey_planner.h"
#include <QApplication>
#include <QHeaderView>
#include <QDateTime>
#include <QInputDialog>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <set>

// ==================== MainWindow ====================

MainWindow::MainWindow(TransportSystem* system, QWidget *parent)
    : QMainWindow(parent), transportSystem(system), isAdminMode(false) {
    setWindowTitle("Система расписания городского транспорта");
    setMinimumSize(800, 600);

    setupUI();
    createLoginScreen();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
}

void MainWindow::createLoginScreen() {
    // Очистка предыдущих виджетов
    QLayoutItem* item;
    while ((item = mainLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QLabel* titleLabel = new QLabel("Система расписания городского транспорта", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    adminButton = new QPushButton("Войти как администратор", this);
    adminButton->setMinimumHeight(50);
    adminButton->setStyleSheet("QPushButton { font-size: 14px; }");

    guestButton = new QPushButton("Войти как гость", this);
    guestButton->setMinimumHeight(50);
    guestButton->setStyleSheet("QPushButton { font-size: 14px; }");

    exitButton = new QPushButton("Сохранить и выйти", this);
    exitButton->setMinimumHeight(50);
    exitButton->setStyleSheet("QPushButton { font-size: 14px; }");

    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(adminButton);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(guestButton);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(exitButton);
    mainLayout->addStretch();

    connect(adminButton, &QPushButton::clicked, this, &MainWindow::showLoginDialog);
    connect(guestButton, &QPushButton::clicked, this, &MainWindow::showGuestMode);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::saveAndExit);
}

void MainWindow::showLoginDialog() {
    AdminLoginDialog dialog(transportSystem, this);
    if (dialog.exec() == QDialog::Accepted && dialog.isAuthenticated()) {
        isAdminMode = true;
        showAdminMode();
    }
}

void MainWindow::showGuestMode() {
    isAdminMode = false;
    GuestModeWidget* guestWidget = new GuestModeWidget(transportSystem, this);
    connect(guestWidget, &GuestModeWidget::backToMainRequested, this, &MainWindow::returnToLoginScreen);

    QLayoutItem* item;
    while ((item = mainLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    mainLayout->addWidget(guestWidget);
}

void MainWindow::showAdminMode() {
    isAdminMode = true;
    AdminModeWidget* adminWidget = new AdminModeWidget(transportSystem, this);
    connect(adminWidget, &AdminModeWidget::backToMainRequested, this, &MainWindow::returnToLoginScreen);

    QLayoutItem* item;
    while ((item = mainLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    mainLayout->addWidget(adminWidget);
}

void MainWindow::returnToLoginScreen() {
    // Удаляем текущий виджет из layout
    QLayoutItem* item;
    while ((item = mainLayout->takeAt(0)) != nullptr) {
        QWidget* widget = item->widget();
        if (widget) {
            widget->hide();
            widget->deleteLater();
        }
        delete item;
    }

    // Создаем экран входа
    createLoginScreen();
}

void MainWindow::saveAndExit() {
    try {
        transportSystem->saveData();
        QMessageBox::information(this, "Сохранение", "Данные успешно сохранены.");
        QApplication::quit();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка при сохранении: %1").arg(e.what()));
    }
}

// ==================== GuestModeWidget ====================

GuestModeWidget::GuestModeWidget(TransportSystem* system, QWidget *parent)
    : QWidget(parent), transportSystem(system) {
    setupUI();
}

void GuestModeWidget::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("Гостевой режим", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    QPushButton* scheduleBtn = new QPushButton("Просмотр расписания транспорта", this);
    QPushButton* stopBtn = new QPushButton("Просмотр расписания остановки", this);
    QPushButton* searchBtn = new QPushButton("Поиск маршрутов между остановками", this);
    QPushButton* backBtn = new QPushButton("Назад", this);

    scheduleBtn->setMinimumHeight(40);
    stopBtn->setMinimumHeight(40);
    searchBtn->setMinimumHeight(40);
    backBtn->setMinimumHeight(40);

    layout->addWidget(titleLabel);
    layout->addSpacing(20);
    layout->addWidget(scheduleBtn);
    layout->addWidget(stopBtn);
    layout->addWidget(searchBtn);
    layout->addStretch();
    layout->addWidget(backBtn);

    connect(scheduleBtn, &QPushButton::clicked, this, &GuestModeWidget::viewTransportSchedule);
    connect(stopBtn, &QPushButton::clicked, this, &GuestModeWidget::viewStopTimetable);
    connect(searchBtn, &QPushButton::clicked, this, &GuestModeWidget::searchRoutes);
    connect(backBtn, &QPushButton::clicked, this, &GuestModeWidget::backToMain);
}

void GuestModeWidget::viewTransportSchedule() {
    TransportScheduleDialog dialog(transportSystem, this, true);
    dialog.exec();
}

void GuestModeWidget::viewStopTimetable() {
    StopTimetableDialog dialog(transportSystem, this, true);
    dialog.exec();
}

void GuestModeWidget::searchRoutes() {
    RouteSearchDialog dialog(transportSystem, this);
    dialog.exec();
}

void GuestModeWidget::backToMain() {
    emit backToMainRequested();
}

// ==================== AdminModeWidget ====================

AdminModeWidget::AdminModeWidget(TransportSystem* system, QWidget *parent)
    : QWidget(parent), transportSystem(system) {
    setupUI();
    refreshAll();
}

void AdminModeWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("Административный режим", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    tabWidget = new QTabWidget(this);

    // Вкладка просмотра
    QWidget* viewTab = new QWidget();
    QVBoxLayout* viewLayout = new QVBoxLayout(viewTab);
    viewLayout->setSpacing(15);
    viewLayout->setContentsMargins(20, 20, 20, 20);

    QPushButton* scheduleBtn = new QPushButton("Просмотр расписания транспорта", viewTab);
    QPushButton* stopBtn = new QPushButton("Просмотр расписания остановки", viewTab);
    QPushButton* searchBtn = new QPushButton("Поиск маршрутов", viewTab);
    QPushButton* calcBtn = new QPushButton("Расчет времени прибытия", viewTab);

    scheduleBtn->setMinimumHeight(50);
    stopBtn->setMinimumHeight(50);
    searchBtn->setMinimumHeight(50);
    calcBtn->setMinimumHeight(50);

    QFont buttonFont = scheduleBtn->font();
    buttonFont.setPointSize(11);
    scheduleBtn->setFont(buttonFont);
    stopBtn->setFont(buttonFont);
    searchBtn->setFont(buttonFont);
    calcBtn->setFont(buttonFont);

    viewLayout->addWidget(scheduleBtn);
    viewLayout->addWidget(stopBtn);
    viewLayout->addWidget(searchBtn);
    viewLayout->addWidget(calcBtn);
    viewLayout->addStretch();

    connect(scheduleBtn, &QPushButton::clicked, this, &AdminModeWidget::viewTransportSchedule);
    connect(stopBtn, &QPushButton::clicked, this, &AdminModeWidget::viewStopTimetable);
    connect(searchBtn, &QPushButton::clicked, this, &AdminModeWidget::searchRoutes);
    connect(calcBtn, &QPushButton::clicked, this, &AdminModeWidget::calculateArrivalTime);

    tabWidget->addTab(viewTab, "Просмотр");

    // Вкладка добавления
    QWidget* addTab = new QWidget();
    QGridLayout* addLayout = new QGridLayout(addTab);

    QPushButton* addRouteBtn = new QPushButton("Добавить маршрут", addTab);
    QPushButton* addTripBtn = new QPushButton("Добавить рейс", addTab);
    QPushButton* addVehicleBtn = new QPushButton("Добавить транспорт", addTab);
    QPushButton* addStopBtn = new QPushButton("Добавить остановку", addTab);
    QPushButton* addDriverBtn = new QPushButton("Добавить водителя", addTab);

    addLayout->addWidget(addRouteBtn, 0, 0);
    addLayout->addWidget(addTripBtn, 0, 1);
    addLayout->addWidget(addVehicleBtn, 1, 0);
    addLayout->addWidget(addStopBtn, 1, 1);
    addLayout->addWidget(addDriverBtn, 2, 0);
    addLayout->setRowStretch(3, 1);

    connect(addRouteBtn, &QPushButton::clicked, this, &AdminModeWidget::addRoute);
    connect(addTripBtn, &QPushButton::clicked, this, &AdminModeWidget::addTrip);
    connect(addVehicleBtn, &QPushButton::clicked, this, &AdminModeWidget::addVehicle);
    connect(addStopBtn, &QPushButton::clicked, this, &AdminModeWidget::addStop);
    connect(addDriverBtn, &QPushButton::clicked, this, &AdminModeWidget::addDriver);

    tabWidget->addTab(addTab, "Добавление");

    // Вкладка удаления
    QWidget* removeTab = new QWidget();
    QGridLayout* removeLayout = new QGridLayout(removeTab);

    QPushButton* removeRouteBtn = new QPushButton("Удалить маршрут", removeTab);
    QPushButton* removeTripBtn = new QPushButton("Удалить рейс", removeTab);
    QPushButton* removeVehicleBtn = new QPushButton("Удалить транспорт", removeTab);
    QPushButton* removeStopBtn = new QPushButton("Удалить остановку", removeTab);
    QPushButton* removeDriverBtn = new QPushButton("Удалить водителя", removeTab);

    removeLayout->addWidget(removeRouteBtn, 0, 0);
    removeLayout->addWidget(removeTripBtn, 0, 1);
    removeLayout->addWidget(removeVehicleBtn, 1, 0);
    removeLayout->addWidget(removeStopBtn, 1, 1);
    removeLayout->addWidget(removeDriverBtn, 2, 0);
    removeLayout->setRowStretch(3, 1);

    connect(removeRouteBtn, &QPushButton::clicked, this, &AdminModeWidget::removeRoute);
    connect(removeTripBtn, &QPushButton::clicked, this, &AdminModeWidget::removeTrip);
    connect(removeVehicleBtn, &QPushButton::clicked, this, &AdminModeWidget::removeVehicle);
    connect(removeStopBtn, &QPushButton::clicked, this, &AdminModeWidget::removeStop);
    connect(removeDriverBtn, &QPushButton::clicked, this, &AdminModeWidget::removeDriver);

    tabWidget->addTab(removeTab, "Удаление");

    // Вкладка просмотра всех данных
    QWidget* dataTab = new QWidget();
    QVBoxLayout* dataLayout = new QVBoxLayout(dataTab);

    QPushButton* viewAllBtn = new QPushButton("Обновить данные", dataTab);
    dataLayout->addWidget(viewAllBtn);

    QTabWidget* dataTabs = new QTabWidget(dataTab);

    routesTable = new QTableWidget(dataTab);
    routesTable->setColumnCount(4);
    routesTable->setHorizontalHeaderLabels(QStringList() << "Номер" << "Тип транспорта" << "Начало" << "Конец");
    routesTable->horizontalHeader()->setStretchLastSection(true);

    tripsTable = new QTableWidget(dataTab);
    tripsTable->setColumnCount(6);
    tripsTable->setHorizontalHeaderLabels(QStringList() << "ID" << "Маршрут" << "Транспорт" << "Водитель" << "Время начала" << "День недели");
    tripsTable->horizontalHeader()->setStretchLastSection(true);

    vehiclesTable = new QTableWidget(dataTab);
    vehiclesTable->setColumnCount(4);
    vehiclesTable->setHorizontalHeaderLabels(QStringList() << "Номер" << "Модель" << "Вместимость" << "Тип");
    vehiclesTable->horizontalHeader()->setStretchLastSection(true);

    stopsTable = new QTableWidget(dataTab);
    stopsTable->setColumnCount(2);
    stopsTable->setHorizontalHeaderLabels(QStringList() << "ID" << "Название");
    stopsTable->horizontalHeader()->setStretchLastSection(true);

    driversTable = new QTableWidget(dataTab);
    driversTable->setColumnCount(4);
    driversTable->setHorizontalHeaderLabels(QStringList() << "Имя" << "Фамилия" << "Отчество" << "Категория");
    driversTable->horizontalHeader()->setStretchLastSection(true);

    dataTabs->addTab(routesTable, "Маршруты");
    dataTabs->addTab(tripsTable, "Рейсы");
    dataTabs->addTab(vehiclesTable, "Транспорт");
    dataTabs->addTab(stopsTable, "Остановки");
    dataTabs->addTab(driversTable, "Водители");

    dataLayout->addWidget(dataTabs);

    // Автоматическое обновление при переключении на вкладку
    connect(dataTabs, &QTabWidget::currentChanged, this, [this](int index) {
        if (index == 0) refreshRoutesTable();
        else if (index == 1) refreshTripsTable();
        else if (index == 2) refreshVehiclesTable();
        else if (index == 3) refreshStopsTable();
        else if (index == 4) refreshDriversTable();
    });

    connect(viewAllBtn, &QPushButton::clicked, this, &AdminModeWidget::viewAllData);

    tabWidget->addTab(dataTab, "Все данные");

    // Вкладка управления
    QWidget* manageTab = new QWidget();
    QVBoxLayout* manageLayout = new QVBoxLayout(manageTab);

    QPushButton* saveBtn = new QPushButton("Сохранить данные", manageTab);
    QPushButton* undoBtn = new QPushButton("Отменить последнее действие", manageTab);
    QPushButton* redoBtn = new QPushButton("Повторить последнее действие", manageTab);
    QPushButton* backBtn = new QPushButton("Выйти из режима администратора", manageTab);

    saveBtn->setMinimumHeight(40);
    undoBtn->setMinimumHeight(40);
    redoBtn->setMinimumHeight(40);
    backBtn->setMinimumHeight(40);

    manageLayout->addWidget(saveBtn);
    manageLayout->addWidget(undoBtn);
    manageLayout->addWidget(redoBtn);
    manageLayout->addStretch();
    manageLayout->addWidget(backBtn);

    connect(saveBtn, &QPushButton::clicked, this, &AdminModeWidget::saveData);
    connect(undoBtn, &QPushButton::clicked, this, &AdminModeWidget::undoLastAction);
    connect(redoBtn, &QPushButton::clicked, this, &AdminModeWidget::redoLastAction);
    connect(backBtn, &QPushButton::clicked, this, &AdminModeWidget::backToMain);

    tabWidget->addTab(manageTab, "Управление");

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(tabWidget);
}

void AdminModeWidget::refreshAll() {
    refreshRoutesTable();
    refreshTripsTable();
    refreshVehiclesTable();
    refreshStopsTable();
    refreshDriversTable();
}

void AdminModeWidget::refreshRoutesTable() {
    routesTable->setRowCount(0);
    const auto& routes = transportSystem->getRoutes();
    for (const auto& route : routes) {
        int row = routesTable->rowCount();
        routesTable->insertRow(row);
        routesTable->setItem(row, 0, new QTableWidgetItem(QString::number(route->getNumber())));
        routesTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(route->getVehicleType())));
        routesTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(route->getStartStop())));
        routesTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(route->getEndStop())));
    }
}

void AdminModeWidget::refreshTripsTable() {
    tripsTable->setRowCount(0);
    try {
        const auto& trips = transportSystem->getTrips();

        // Отладочная информация
        std::cout << "[DEBUG] Количество рейсов в системе: " << trips.size() << std::endl;

        if (trips.empty()) {
            tripsTable->insertRow(0);
            QTableWidgetItem* noDataItem = new QTableWidgetItem("Нет рейсов в системе. Добавьте рейсы через меню 'Добавление' -> 'Добавить рейс'");
            noDataItem->setTextAlignment(Qt::AlignCenter);
            tripsTable->setItem(0, 0, noDataItem);
            tripsTable->setSpan(0, 0, 1, 6);
            return;
        }

        // Собираем все рейсы для сортировки
        List<std::shared_ptr<Trip>> validTripsList;
        for (const auto& trip : trips) {
            if (!trip) {
                std::cout << "[DEBUG] Найден nullptr рейс" << std::endl;
                continue;
            }
            validTripsList.push_back(trip);
        }

        // Сортируем по дню недели, затем по времени начала
        validTripsList.sort([](const std::shared_ptr<Trip>& a, const std::shared_ptr<Trip>& b) {
                      int dayA = a->getWeekDay();
                      int dayB = b->getWeekDay();
                      if (dayA != dayB) {
                          return dayA < dayB;
                      }
                      // Если день одинаковый, сортируем по времени начала
                      return a->getStartTime() < b->getStartTime();
                  });

        int validTrips = 0;
        for (const auto& trip : validTripsList) {
            try {
                int row = tripsTable->rowCount();
                tripsTable->insertRow(row);

                int tripId = trip->getTripId();
                tripsTable->setItem(row, 0, new QTableWidgetItem(QString::number(tripId)));
                std::cout << "[DEBUG] Обработка рейса ID: " << tripId << std::endl;

                auto route = trip->getRoute();
                if (route) {
                    tripsTable->setItem(row, 1, new QTableWidgetItem(QString::number(route->getNumber())));
                } else {
                    std::cout << "[DEBUG] Рейс " << tripId << " не имеет маршрута" << std::endl;
                    tripsTable->setItem(row, 1, new QTableWidgetItem("N/A"));
                }

                auto vehicle = trip->getVehicle();
                if (vehicle) {
                    tripsTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(vehicle->getLicensePlate())));
                } else {
                    std::cout << "[DEBUG] Рейс " << tripId << " не имеет транспорта" << std::endl;
                    tripsTable->setItem(row, 2, new QTableWidgetItem("N/A"));
                }

                auto driver = trip->getDriver();
                if (driver) {
                    tripsTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(driver->getFullName())));
                } else {
                    std::cout << "[DEBUG] Рейс " << tripId << " не имеет водителя" << std::endl;
                    tripsTable->setItem(row, 3, new QTableWidgetItem("N/A"));
                }

                Time startTime = trip->getStartTime();
                QString timeStr = QString("%1:%2").arg(startTime.getHours(), 2, 10, QChar('0'))
                                                  .arg(startTime.getMinutes(), 2, 10, QChar('0'));
                tripsTable->setItem(row, 4, new QTableWidgetItem(timeStr));

                // День недели
                QStringList dayShortNames = {"", "Пн", "Вт", "Ср", "Чт", "Пт", "Сб", "Вс"};
                int weekDay = trip->getWeekDay();
                QString dayName = (weekDay >= 1 && weekDay <= 7) ? dayShortNames[weekDay] : "?";
                tripsTable->setItem(row, 5, new QTableWidgetItem(dayName));

                validTrips++;
            } catch (const std::exception& e) {
                std::cout << "[DEBUG] Ошибка при обработке рейса: " << e.what() << std::endl;
                // Пропускаем проблемный рейс и продолжаем
                continue;
            }
        }

        std::cout << "[DEBUG] Успешно отображено рейсов: " << validTrips << " из " << trips.size() << std::endl;

        if (validTrips == 0 && !trips.empty()) {
            tripsTable->setRowCount(0);
            tripsTable->insertRow(0);
            QTableWidgetItem* errorItem = new QTableWidgetItem("Ошибка при загрузке данных о рейсах. Проверьте консоль для деталей.");
            errorItem->setTextAlignment(Qt::AlignCenter);
            tripsTable->setItem(0, 0, errorItem);
            tripsTable->setSpan(0, 0, 1, 6);
        } else {
            // Автоматически подгоняем ширину колонок
            tripsTable->resizeColumnsToContents();
        }
    } catch (const std::exception& e) {
        std::cout << "[DEBUG] Критическая ошибка в refreshTripsTable: " << e.what() << std::endl;
        tripsTable->insertRow(0);
        QTableWidgetItem* errorItem = new QTableWidgetItem(QString("Ошибка: %1").arg(e.what()));
        errorItem->setTextAlignment(Qt::AlignCenter);
        tripsTable->setItem(0, 0, errorItem);
        tripsTable->setSpan(0, 0, 1, 6);
    }
}

void AdminModeWidget::refreshVehiclesTable() {
    vehiclesTable->setRowCount(0);
    const auto& vehicles = transportSystem->getVehicles();
    for (const auto& vehicle : vehicles) {
        int row = vehiclesTable->rowCount();
        vehiclesTable->insertRow(row);
        vehiclesTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(vehicle->getLicensePlate())));
        vehiclesTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(vehicle->getModel())));

        // Получаем capacity через dynamic_cast
        int capacity = 0;
        if (auto fuelTransport = std::dynamic_pointer_cast<FuelTransport>(vehicle)) {
            capacity = fuelTransport->getCapacity();
        } else if (auto electricTransport = std::dynamic_pointer_cast<ElectricTransport>(vehicle)) {
            capacity = electricTransport->getCapacity();
        }
        vehiclesTable->setItem(row, 2, new QTableWidgetItem(QString::number(capacity)));
        vehiclesTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(vehicle->getType())));
    }
}

void AdminModeWidget::refreshStopsTable() {
    stopsTable->setRowCount(0);
    const auto& stops = transportSystem->getStops();
    for (const auto& stop : stops) {
        int row = stopsTable->rowCount();
        stopsTable->insertRow(row);
        stopsTable->setItem(row, 0, new QTableWidgetItem(QString::number(stop.getId())));
        stopsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(stop.getName())));
    }
}

void AdminModeWidget::refreshDriversTable() {
    driversTable->setRowCount(0);
    const auto& drivers = transportSystem->getDrivers();
    for (const auto& driver : drivers) {
        int row = driversTable->rowCount();
        driversTable->insertRow(row);
        driversTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(driver->getFirstName())));
        driversTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(driver->getLastName())));
        driversTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(driver->getMiddleName())));
        QString category = QString::fromStdString(driver->getCategory());
        if (category.isEmpty()) {
            category = "Не указана";
        }
        driversTable->setItem(row, 3, new QTableWidgetItem(category));
    }
}

void AdminModeWidget::viewTransportSchedule() {
    TransportScheduleDialog dialog(transportSystem, this);
    dialog.exec();
}

void AdminModeWidget::viewStopTimetable() {
    StopTimetableDialog dialog(transportSystem, this);
    dialog.exec();
}

void AdminModeWidget::searchRoutes() {
    RouteSearchDialog dialog(transportSystem, this);
    dialog.exec();
}

void AdminModeWidget::calculateArrivalTime() {
    ArrivalTimeDialog dialog(transportSystem, this);
    dialog.exec();
}

void AdminModeWidget::addRoute() {
    AddRouteDialog dialog(transportSystem, this);
    if (dialog.exec() == QDialog::Accepted) {
        refreshAll();
    }
}

void AdminModeWidget::addTrip() {
    AddTripDialog dialog(transportSystem, this);
    if (dialog.exec() == QDialog::Accepted) {
        refreshAll();
    }
}

void AdminModeWidget::addVehicle() {
    AddVehicleDialog dialog(transportSystem, this);
    if (dialog.exec() == QDialog::Accepted) {
        refreshAll();
    }
}

void AdminModeWidget::addStop() {
    AddStopDialog dialog(transportSystem, this);
    if (dialog.exec() == QDialog::Accepted) {
        refreshAll();
    }
}

void AdminModeWidget::addDriver() {
    AddDriverDialog dialog(transportSystem, this);
    if (dialog.exec() == QDialog::Accepted) {
        refreshAll();
    }
}

void AdminModeWidget::removeRoute() {
    bool ok;
    int number = QInputDialog::getInt(this, "Удаление маршрута", "Введите номер маршрута:", 0, 1, 9999, 1, &ok);
    if (ok) {
        try {
            transportSystem->removeRoute(number);
            QMessageBox::information(this, "Успех", "Маршрут удален.");
            refreshAll();
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Ошибка", QString("Ошибка: %1").arg(e.what()));
        }
    }
}

void AdminModeWidget::removeTrip() {
    bool ok;
    int id = QInputDialog::getInt(this, "Удаление рейса", "Введите ID рейса:", 0, 1, 9999, 1, &ok);
    if (ok) {
        try {
            transportSystem->removeTrip(id);
            QMessageBox::information(this, "Успех", "Рейс удален.");
            refreshAll();
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Ошибка", QString("Ошибка: %1").arg(e.what()));
        }
    }
}

void AdminModeWidget::removeVehicle() {
    bool ok;
    QString licensePlate = QInputDialog::getText(this, "Удаление транспорта", "Введите номер транспорта:",
                                                  QLineEdit::Normal, "", &ok);
    if (ok && !licensePlate.isEmpty()) {
        try {
            auto vehicle = transportSystem->getVehicleByLicensePlate(licensePlate.toStdString());
            if (!vehicle) {
                QMessageBox::warning(this, "Ошибка", "Транспорт не найден.");
                return;
            }
            // Используем команду удаления через систему
            RemoveVehicleCommand cmd(transportSystem, licensePlate.toStdString());
            cmd.execute();
            QMessageBox::information(this, "Успех", "Транспорт удален.");
            refreshAll();
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Ошибка", QString("Ошибка: %1").arg(e.what()));
        }
    }
}

void AdminModeWidget::removeStop() {
    bool ok;
    int id = QInputDialog::getInt(this, "Удаление остановки", "Введите ID остановки:", 0, 1, 9999, 1, &ok);
    if (ok) {
        try {
            auto stop = transportSystem->getStopById(id);
            // Используем команду удаления через систему
            RemoveStopCommand cmd(transportSystem, id);
            cmd.execute();
            QMessageBox::information(this, "Успех", "Остановка удалена.");
            refreshAll();
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Ошибка", QString("Ошибка: %1").arg(e.what()));
        }
    }
}

void AdminModeWidget::removeDriver() {
    bool ok;
    QString firstName = QInputDialog::getText(this, "Удаление водителя", "Имя:", QLineEdit::Normal, "", &ok);
    if (!ok) return;
    QString lastName = QInputDialog::getText(this, "Удаление водителя", "Фамилия:", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    try {
        auto driver = transportSystem->findDriverByName(firstName.toStdString(), lastName.toStdString());
        if (!driver) {
            QMessageBox::warning(this, "Ошибка", "Водитель не найден.");
            return;
        }
        // Используем команду удаления через систему
        RemoveDriverCommand cmd(transportSystem, driver);
        cmd.execute();
        QMessageBox::information(this, "Успех", "Водитель удален.");
        refreshAll();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка: %1").arg(e.what()));
    }
}

void AdminModeWidget::viewAllData() {
    refreshAll();

    // Показываем статистику
    const auto& trips = transportSystem->getTrips();
    const auto& routes = transportSystem->getRoutes();
    const auto& vehicles = transportSystem->getVehicles();
    const auto& stops = transportSystem->getStops();
    const auto& drivers = transportSystem->getDrivers();

    QString stats = QString("Данные обновлены.\n\nСтатистика:\n")
                   + QString("• Рейсов: %1\n").arg(trips.size())
                   + QString("• Маршрутов: %1\n").arg(routes.size())
                   + QString("• Транспорта: %1\n").arg(vehicles.size())
                   + QString("• Остановок: %1\n").arg(stops.size())
                   + QString("• Водителей: %1").arg(drivers.size());

    QMessageBox::information(this, "Обновление", stats);
}

void AdminModeWidget::saveData() {
    try {
        transportSystem->saveData();
        QMessageBox::information(this, "Сохранение", "Данные успешно сохранены.");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка при сохранении: %1").arg(e.what()));
    }
}

void AdminModeWidget::undoLastAction() {
    try {
        if (transportSystem->canUndo()) {
            // Сохраняем описание команды ДО отмены
            std::string description = transportSystem->getLastCommandDescription();
            transportSystem->undo();
            QMessageBox::information(this, "Отмена",
                QString("Отменено: %1").arg(QString::fromStdString(description)));
            refreshAll();
        } else {
            QMessageBox::information(this, "Информация", "Нет действий для отмены.");
        }
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Ошибка", QString("Ошибка при отмене действия: %1").arg(e.what()));
        // Не закрываем программу, просто показываем ошибку
    }
}

void AdminModeWidget::redoLastAction() {
    try {
        if (transportSystem->canRedo()) {
            // Сохраняем описание команды ДО повтора
            std::string description = transportSystem->getNextCommandDescription();
            transportSystem->redo();
            QMessageBox::information(this, "Повтор",
                QString("Повторено: %1").arg(QString::fromStdString(description)));
            refreshAll();
        } else {
            QMessageBox::information(this, "Информация", "Нет действий для повтора.");
        }
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Ошибка", QString("Ошибка при повторе действия: %1").arg(e.what()));
        // Не закрываем программу, просто показываем ошибку
    }
}

void AdminModeWidget::backToMain() {
    emit backToMainRequested();
}

// ==================== AdminLoginDialog ====================

AdminLoginDialog::AdminLoginDialog(TransportSystem* system, QWidget *parent)
    : QDialog(parent), transportSystem(system), authenticated(false) {
    setWindowTitle("Вход администратора");
    setModal(true);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* usernameLabel = new QLabel("Имя пользователя:", this);
    usernameEdit = new QLineEdit(this);

    QLabel* passwordLabel = new QLabel("Пароль:", this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);

    QPushButton* loginBtn = new QPushButton("Войти", this);
    QPushButton* cancelBtn = new QPushButton("Отмена", this);

    layout->addWidget(usernameLabel);
    layout->addWidget(usernameEdit);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordEdit);
    layout->addWidget(loginBtn);
    layout->addWidget(cancelBtn);

    connect(loginBtn, &QPushButton::clicked, this, &AdminLoginDialog::tryLogin);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void AdminLoginDialog::tryLogin() {
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    if (transportSystem->authenticateAdmin(username.toStdString(), password.toStdString())) {
        authenticated = true;
        accept();
    } else {
        QMessageBox::warning(this, "Ошибка", "Неверное имя пользователя или пароль.");
    }
}

// ==================== TransportScheduleDialog ====================

TransportScheduleDialog::TransportScheduleDialog(TransportSystem* system, QWidget *parent, bool isGuestMode)
    : QDialog(parent), transportSystem(system), isGuestMode(isGuestMode) {
    setWindowTitle("Расписание транспорта");
    setMinimumSize(900, 700);

    QVBoxLayout* layout = new QVBoxLayout(this);

    // Выбор типа транспорта
    QLabel* typeLabel = new QLabel("Выберите тип транспорта:", this);
    vehicleTypeComboBox = new QComboBox(this);
    vehicleTypeComboBox->addItem("Автобус", "Автобус");
    vehicleTypeComboBox->addItem("Трамвай", "Трамвай");
    vehicleTypeComboBox->addItem("Троллейбус", "Троллейбус");

    // Выбор маршрута
    QLabel* routeLabel = new QLabel("Выберите маршрут:", this);
    routeComboBox = new QComboBox(this);
    routeComboBox->addItem("Все маршруты", 0);

    // Выбор дня недели
    QLabel* weekDayLabel = new QLabel("Выберите день недели:", this);
    weekDayComboBox = new QComboBox(this);
    weekDayComboBox->addItem("Все дни", 0);
    weekDayComboBox->addItem("Понедельник", 1);
    weekDayComboBox->addItem("Вторник", 2);
    weekDayComboBox->addItem("Среда", 3);
    weekDayComboBox->addItem("Четверг", 4);
    weekDayComboBox->addItem("Пятница", 5);
    weekDayComboBox->addItem("Суббота", 6);
    weekDayComboBox->addItem("Воскресенье", 7);

    scheduleTable = new QTableWidget(this);
    if (isGuestMode) {
        // Для гостей: ID рейса, Маршрут, Время начала, День недели, Расписание
        scheduleTable->setColumnCount(5);
        scheduleTable->setHorizontalHeaderLabels(QStringList() << "ID рейса" << "Маршрут"
            << "Время начала" << "День недели" << "Расписание");
    } else {
        // Для администраторов: все колонки включая Транспорт и Водитель
        scheduleTable->setColumnCount(7);
        scheduleTable->setHorizontalHeaderLabels(QStringList() << "ID рейса" << "Маршрут"
            << "Транспорт" << "Водитель" << "Время начала" << "День недели" << "Расписание");
    }
    scheduleTable->horizontalHeader()->setStretchLastSection(true);

    QPushButton* closeBtn = new QPushButton("Закрыть", this);

    layout->addWidget(typeLabel);
    layout->addWidget(vehicleTypeComboBox);
    layout->addWidget(routeLabel);
    layout->addWidget(routeComboBox);
    layout->addWidget(weekDayLabel);
    layout->addWidget(weekDayComboBox);
    layout->addWidget(scheduleTable);
    layout->addWidget(closeBtn);

    connect(vehicleTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TransportScheduleDialog::onVehicleTypeChanged);
    connect(routeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TransportScheduleDialog::onRouteChanged);
    connect(weekDayComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TransportScheduleDialog::onWeekDayChanged);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);

    // Инициализируем маршруты для первого типа транспорта
    onVehicleTypeChanged();
}

void TransportScheduleDialog::onVehicleTypeChanged() {
    QString selectedType = vehicleTypeComboBox->currentData().toString();
    populateRoutes(selectedType.toStdString());
    onFilterChanged();
}

void TransportScheduleDialog::onRouteChanged() {
    onFilterChanged();
}

void TransportScheduleDialog::onWeekDayChanged() {
    onFilterChanged();
}

void TransportScheduleDialog::onFilterChanged() {
    QString selectedType = vehicleTypeComboBox->currentData().toString();
    int routeNumber = routeComboBox->currentData().toInt();
    int weekDay = weekDayComboBox->currentData().toInt();
    populateSchedule(selectedType.toStdString(), weekDay, routeNumber);
}

void TransportScheduleDialog::populateRoutes(const std::string& vehicleType) {
    routeComboBox->clear();
    routeComboBox->addItem("Все маршруты", 0);

    const auto& routes = transportSystem->getRoutes();
    for (const auto& route : routes) {
        if (route->getVehicleType() == vehicleType) {
            QString routeText = QString("Маршрут %1: %2 → %3")
                .arg(route->getNumber())
                .arg(QString::fromStdString(route->getStartStop()))
                .arg(QString::fromStdString(route->getEndStop()));
            routeComboBox->addItem(routeText, route->getNumber());
        }
    }
}

void TransportScheduleDialog::populateSchedule(const std::string& vehicleType, int weekDay, int routeNumber) {
    scheduleTable->setRowCount(0);
    try {
        const auto& trips = transportSystem->getTrips();

        if (trips.empty()) {
            scheduleTable->insertRow(0);
            QTableWidgetItem* noDataItem = new QTableWidgetItem("Нет данных о рейсах.\nДобавьте рейсы через административный режим:\n'Добавление' -> 'Добавить рейс'");
            noDataItem->setTextAlignment(Qt::AlignCenter);
            scheduleTable->setItem(0, 0, noDataItem);
            scheduleTable->setSpan(0, 0, 1, isGuestMode ? 5 : 7);
            scheduleTable->setRowHeight(0, 60);
            return;
        }

        QStringList dayNames = {"", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота", "Воскресенье"};
        QStringList dayShortNames = {"", "Пн", "Вт", "Ср", "Чт", "Пт", "Сб", "Вс"};

        int validTripsCount = 0;
        for (const auto& trip : trips) {
            if (!trip) continue;

            auto route = trip->getRoute();
            if (!route) continue;

            // Фильтрация по типу транспорта
            if (route->getVehicleType() != vehicleType) {
                continue;
            }

            // Фильтрация по маршруту
            if (routeNumber != 0 && route->getNumber() != routeNumber) {
                continue;
            }

            // Фильтрация по дню недели
            int tripDay = trip->getWeekDay();
            bool dayMatches = false;
            if (weekDay == 0) {
                // Все дни
                dayMatches = true;
            } else {
                // Конкретный день: показываем только рейсы этого дня
                dayMatches = (tripDay == weekDay);
            }

            if (!dayMatches) {
                continue;
            }

            try {
                int row = scheduleTable->rowCount();
                scheduleTable->insertRow(row);

                scheduleTable->setItem(row, 0, new QTableWidgetItem(QString::number(trip->getTripId())));
                scheduleTable->setItem(row, 1, new QTableWidgetItem(QString::number(route->getNumber())));

                int colIndex = 2;
                if (!isGuestMode) {
                    // Для администраторов показываем транспорт и водителя
                    auto vehicle = trip->getVehicle();
                    if (vehicle) {
                        scheduleTable->setItem(row, colIndex, new QTableWidgetItem(QString::fromStdString(vehicle->getLicensePlate())));
                    } else {
                        scheduleTable->setItem(row, colIndex, new QTableWidgetItem("N/A"));
                    }
                    colIndex++;

                    auto driver = trip->getDriver();
                    if (driver) {
                        scheduleTable->setItem(row, colIndex, new QTableWidgetItem(QString::fromStdString(driver->getFullName())));
                    } else {
                        scheduleTable->setItem(row, colIndex, new QTableWidgetItem("N/A"));
                    }
                    colIndex++;
                }

                Time startTime = trip->getStartTime();
                QString timeStr = QString("%1:%2").arg(startTime.getHours(), 2, 10, QChar('0'))
                                                  .arg(startTime.getMinutes(), 2, 10, QChar('0'));
                scheduleTable->setItem(row, colIndex, new QTableWidgetItem(timeStr));
                colIndex++;

                scheduleTable->setItem(row, colIndex, new QTableWidgetItem(
                    tripDay >= 1 && tripDay <= 7 ? dayShortNames[tripDay] : "?"));
                colIndex++;

                // Расписание остановок
                const auto& schedule = trip->getSchedule();
                QString scheduleText;
                if (!schedule.empty()) {
                    List<std::pair<std::string, Time>> sortedSchedule(schedule.begin(), schedule.end());
                    sortedSchedule.sort([](const auto& a, const auto& b) { return a.second < b.second; });
                    for (const auto& [stop, time] : sortedSchedule) {
                        if (!scheduleText.isEmpty()) scheduleText += "; ";
                        QString timeStr = QString("%1:%2").arg(time.getHours(), 2, 10, QChar('0'))
                                                          .arg(time.getMinutes(), 2, 10, QChar('0'));
                        scheduleText += QString::fromStdString(stop) + " " + timeStr;
                    }
                } else {
                    scheduleText = "не рассчитано";
                }
                scheduleTable->setItem(row, colIndex, new QTableWidgetItem(scheduleText));

                validTripsCount++;
            } catch (const std::exception& e) {
                // Пропускаем проблемный рейс
                continue;
            }
        }

        if (validTripsCount == 0) {
            scheduleTable->setRowCount(0);
            scheduleTable->insertRow(0);
            QString message;
            if (weekDay > 0 && weekDay <= 7) {
                message = QString("Нет рейсов для транспорта типа '%1' на %2")
                    .arg(QString::fromStdString(vehicleType))
                    .arg(dayNames[weekDay]);
            } else if (routeNumber > 0) {
                message = QString("Нет рейсов для маршрута %1").arg(routeNumber);
            } else {
                message = QString("Нет рейсов для транспорта типа '%1'").arg(QString::fromStdString(vehicleType));
            }
            QTableWidgetItem* errorItem = new QTableWidgetItem(message);
            errorItem->setTextAlignment(Qt::AlignCenter);
            scheduleTable->setItem(0, 0, errorItem);
            scheduleTable->setSpan(0, 0, 1, isGuestMode ? 5 : 7);
        } else {
            // Автоматически подгоняем ширину колонок
            scheduleTable->resizeColumnsToContents();
        }
    } catch (const std::exception& e) {
        scheduleTable->setRowCount(0);
        scheduleTable->insertRow(0);
        QTableWidgetItem* errorItem = new QTableWidgetItem(QString("Ошибка: %1").arg(e.what()));
        errorItem->setTextAlignment(Qt::AlignCenter);
        scheduleTable->setItem(0, 0, errorItem);
        scheduleTable->setSpan(0, 0, 1, isGuestMode ? 5 : 7);
    }
}

// ==================== StopTimetableDialog ====================

StopTimetableDialog::StopTimetableDialog(TransportSystem* system, QWidget *parent, bool isGuestMode)
    : QDialog(parent), transportSystem(system), isGuestMode(isGuestMode) {
    setWindowTitle("Расписание остановки");
    setMinimumSize(800, 600);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* stopLabel = new QLabel("Выберите остановку:", this);
    stopComboBox = new QComboBox(this);

    // Выбор дня недели
    QLabel* weekDayLabel = new QLabel("Выберите день недели:", this);
    weekDayComboBox = new QComboBox(this);
    weekDayComboBox->addItem("Все дни", 0);
    weekDayComboBox->addItem("Понедельник", 1);
    weekDayComboBox->addItem("Вторник", 2);
    weekDayComboBox->addItem("Среда", 3);
    weekDayComboBox->addItem("Четверг", 4);
    weekDayComboBox->addItem("Пятница", 5);
    weekDayComboBox->addItem("Суббота", 6);
    weekDayComboBox->addItem("Воскресенье", 7);

    timetableTable = new QTableWidget(this);
    if (isGuestMode) {
        // Для гостей: Рейс, Маршрут, Время прибытия
        timetableTable->setColumnCount(3);
        timetableTable->setHorizontalHeaderLabels(QStringList() << "Рейс" << "Маршрут"
            << "Время прибытия");
    } else {
        // Для администраторов: все колонки включая Транспорт и Водитель
        timetableTable->setColumnCount(5);
        timetableTable->setHorizontalHeaderLabels(QStringList() << "Рейс" << "Маршрут"
            << "Время прибытия" << "Транспорт" << "Водитель");
    }
    timetableTable->horizontalHeader()->setStretchLastSection(true);

    QPushButton* closeBtn = new QPushButton("Закрыть", this);

    layout->addWidget(stopLabel);
    layout->addWidget(stopComboBox);
    layout->addWidget(weekDayLabel);
    layout->addWidget(weekDayComboBox);
    layout->addWidget(timetableTable);
    layout->addWidget(closeBtn);

    connect(stopComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StopTimetableDialog::onStopSelected);
    connect(weekDayComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StopTimetableDialog::onWeekDayChanged);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);

    populateStops();
}

void StopTimetableDialog::populateStops() {
    stopComboBox->clear();
    const auto& stops = transportSystem->getStops();
    for (const auto& stop : stops) {
        stopComboBox->addItem(QString::fromStdString(stop.getName()), stop.getId());
    }
    if (stopComboBox->count() > 0) {
        int weekDay = weekDayComboBox->currentData().toInt();
        QString stopName = stopComboBox->currentText();
        populateTimetable(stopName.toStdString(), weekDay);
    }
}

void StopTimetableDialog::onStopSelected() {
    int index = stopComboBox->currentIndex();
    if (index >= 0) {
        QString stopName = stopComboBox->currentText();
        int weekDay = weekDayComboBox->currentData().toInt();
        populateTimetable(stopName.toStdString(), weekDay);
    }
}

void StopTimetableDialog::onWeekDayChanged() {
    int index = stopComboBox->currentIndex();
    if (index >= 0) {
        QString stopName = stopComboBox->currentText();
        int weekDay = weekDayComboBox->currentData().toInt();
        populateTimetable(stopName.toStdString(), weekDay);
    }
}

void StopTimetableDialog::populateTimetable(const std::string& stopName, int weekDay) {
    timetableTable->setRowCount(0);

    try {
        const auto& trips = transportSystem->getTrips();
        List<std::pair<int, std::pair<int, Time>>> relevantTrips; // tripId, routeNumber, time

        for (const auto& trip : trips) {
            if (!trip->hasStop(stopName)) {
                continue;
            }

            // Фильтрация по дню недели
            int tripDay = trip->getWeekDay();
            bool dayMatches = false;
            if (weekDay == 0) {
                // Все дни
                dayMatches = true;
            } else {
                // Конкретный день: показываем только рейсы этого дня
                dayMatches = (tripDay == weekDay);
            }

            if (!dayMatches) {
                continue;
            }

            try {
                Time arrivalTime = trip->getArrivalTime(stopName);
                relevantTrips.push_back({trip->getTripId(), {trip->getRoute()->getNumber(), arrivalTime}});
            } catch (...) {
                // Если время не установлено, пропускаем этот рейс
                continue;
            }
        }

        relevantTrips.sort([](const auto& a, const auto& b) { return a.second.second < b.second.second; });

        if (relevantTrips.empty()) {
            timetableTable->insertRow(0);
            QStringList dayNames = {"", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота", "Воскресенье"};
            QString message;
            if (weekDay > 0 && weekDay <= 7) {
                message = QString("Рейсов не найдено для остановки '%1' на %2.")
                    .arg(QString::fromStdString(stopName))
                    .arg(dayNames[weekDay]);
            } else {
                message = QString("Рейсов не найдено для остановки '%1'.").arg(QString::fromStdString(stopName));
            }
            QTableWidgetItem* noDataItem = new QTableWidgetItem(message);
            noDataItem->setTextAlignment(Qt::AlignCenter);
            timetableTable->setItem(0, 0, noDataItem);
            timetableTable->setSpan(0, 0, 1, isGuestMode ? 3 : 5);
            return;
        }

        for (const auto& [tripId, routeTime] : relevantTrips) {
            auto tripIt = std::find_if(trips.begin(), trips.end(),
                                      [tripId](const auto& t) { return t->getTripId() == tripId; });
            if (tripIt != trips.end()) {
                auto trip = *tripIt;
                int row = timetableTable->rowCount();
                timetableTable->insertRow(row);

                timetableTable->setItem(row, 0, new QTableWidgetItem(QString::number(tripId)));
                timetableTable->setItem(row, 1, new QTableWidgetItem(
                    QString("%1 (%2)").arg(routeTime.first).arg(QString::fromStdString(trip->getRoute()->getVehicleType()))));

                QString timeStr = QString("%1:%2").arg(routeTime.second.getHours(), 2, 10, QChar('0'))
                                                  .arg(routeTime.second.getMinutes(), 2, 10, QChar('0'));
                timetableTable->setItem(row, 2, new QTableWidgetItem(timeStr));

                if (!isGuestMode) {
                    // Для администраторов показываем транспорт и водителя
                    timetableTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(trip->getVehicle()->getInfo())));
                    timetableTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(trip->getDriver()->getFullName())));
                }
            } else {
                int row = timetableTable->rowCount();
                timetableTable->insertRow(row);
                timetableTable->setItem(row, 0, new QTableWidgetItem(QString::number(tripId)));
                timetableTable->setItem(row, 1, new QTableWidgetItem(QString::number(routeTime.first)));

                QString timeStr = QString("%1:%2").arg(routeTime.second.getHours(), 2, 10, QChar('0'))
                                                  .arg(routeTime.second.getMinutes(), 2, 10, QChar('0'));
                timetableTable->setItem(row, 2, new QTableWidgetItem(timeStr));
            }
        }

        // Автоматически подгоняем ширину колонок
        timetableTable->resizeColumnsToContents();
    } catch (const std::exception& e) {
        timetableTable->setRowCount(0);
        timetableTable->insertRow(0);
        QTableWidgetItem* errorItem = new QTableWidgetItem(QString("Ошибка: %1").arg(e.what()));
        errorItem->setTextAlignment(Qt::AlignCenter);
        timetableTable->setItem(0, 0, errorItem);
        timetableTable->setSpan(0, 0, 1, isGuestMode ? 3 : 5);
        QMessageBox::warning(this, "Ошибка", QString("Ошибка при получении расписания: %1").arg(e.what()));
    }
}

// ==================== RouteSearchDialog ====================

RouteSearchDialog::RouteSearchDialog(TransportSystem* system, QWidget *parent)
    : QDialog(parent), transportSystem(system) {
    setWindowTitle("Поиск маршрутов");
    setMinimumSize(600, 400);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* stopALabel = new QLabel("Остановка A:", this);
    stopAComboBox = new QComboBox(this);

    QLabel* stopBLabel = new QLabel("Остановка B:", this);
    stopBComboBox = new QComboBox(this);

    QPushButton* searchBtn = new QPushButton("Поиск", this);

    resultsText = new QTextEdit(this);
    resultsText->setReadOnly(true);

    QPushButton* closeBtn = new QPushButton("Закрыть", this);

    layout->addWidget(stopALabel);
    layout->addWidget(stopAComboBox);
    layout->addWidget(stopBLabel);
    layout->addWidget(stopBComboBox);
    layout->addWidget(searchBtn);
    layout->addWidget(resultsText);
    layout->addWidget(closeBtn);

    connect(searchBtn, &QPushButton::clicked, this, &RouteSearchDialog::onSearchClicked);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);

    populateStops();
}

void RouteSearchDialog::populateStops() {
    stopAComboBox->clear();
    stopBComboBox->clear();

    const auto& stops = transportSystem->getStops();
    for (const auto& stop : stops) {
        QString name = QString::fromStdString(stop.getName());
        stopAComboBox->addItem(name);
        stopBComboBox->addItem(name);
    }
}

void RouteSearchDialog::onSearchClicked() {
    QString stopA = stopAComboBox->currentText();
    QString stopB = stopBComboBox->currentText();

    if (stopA == stopB) {
        QMessageBox::warning(this, "Ошибка", "Выберите разные остановки.");
        return;
    }

    try {
        resultsText->clear();

        // Сначала ищем прямые маршруты
        auto routes = transportSystem->findRoutes(stopA.toStdString(), stopB.toStdString());

        if (!routes.empty()) {
            resultsText->append("========================================\n");
            resultsText->append("Прямые маршруты:\n");
            resultsText->append("========================================\n\n");
            resultsText->append(QString("Найдено прямых маршрутов: %1\n\n").arg(routes.size()));

            for (const auto& route : routes) {
                resultsText->append(QString("Маршрут %1 (%2)\n")
                    .arg(route->getNumber())
                    .arg(QString::fromStdString(route->getVehicleType())));

                const auto& allStops = route->getAllStops();
                int startPos = route->getStopPosition(stopA.toStdString());
                int endPos = route->getStopPosition(stopB.toStdString());

                if (startPos != -1 && endPos != -1 && startPos < endPos) {
                    QString path = QString::fromStdString(allStops[startPos]);
                    for (int i = startPos + 1; i <= endPos; i++) {
                        path += " → " + QString::fromStdString(allStops[i]);
                    }
                    resultsText->append(QString("Путь: %1\n\n").arg(path));
                }
            }
        } else {
            resultsText->append("Прямых маршрутов не найдено.\n\n");
        }

        // Ищем маршруты с пересадками, если прямых маршрутов нет
        if (routes.empty()) {
            resultsText->append("Ищем маршруты с пересадками...\n\n");

            try {
                auto& planner = transportSystem->getJourneyPlanner();
                auto journeys = planner.findAllJourneysWithTransfers(
                    stopA.toStdString(), stopB.toStdString(), 2);

                if (journeys.empty()) {
                    resultsText->append("Маршрутов с пересадками не найдено.\n");
                } else {
                    // Фильтруем уникальные варианты по комбинации маршрутов и точек пересадки
                    List<Journey> uniqueJourneys;
                    std::set<std::string> seenRoutes;

                    for (const auto& journey : journeys) {
                        // Создаем уникальный ключ: последовательность номеров маршрутов + точки пересадки
                        std::string routeKey;
                        const auto& trips = journey.getTrips();
                        const auto& transferPoints = journey.getTransferPoints();

                        for (size_t i = 0; i < trips.size(); ++i) {
                            routeKey += std::to_string(trips[i]->getRoute()->getNumber());
                            if (i < transferPoints.size()) {
                                routeKey += "@" + transferPoints[i] + "@";
                            }
                        }

                        // Добавляем только если такой комбинации еще не было
                        if (seenRoutes.find(routeKey) == seenRoutes.end()) {
                            seenRoutes.insert(routeKey);
                            uniqueJourneys.push_back(journey);
                        }
                    }

                    if (uniqueJourneys.empty()) {
                        resultsText->append("Маршрутов с пересадками не найдено.\n");
                    } else {
                        resultsText->append("========================================\n");
                        resultsText->append("Маршруты с пересадками:\n");
                        resultsText->append("========================================\n\n");

                        // Показываем до 5 уникальных вариантов
                        int count = std::min(5, static_cast<int>(uniqueJourneys.size()));
                        for (int i = 0; i < count; ++i) {
                            resultsText->append(QString("--- Вариант %1 ---\n").arg(i + 1));
                            const auto& journey = uniqueJourneys[i];

                            resultsText->append(QString("Пересадок: %1\n").arg(journey.getTransferCount()));
                            resultsText->append(QString("Общее время в пути: %1 минут\n").arg(journey.getTotalDuration()));

                            Time startTime = journey.getStartTime();
                            Time endTime = journey.getEndTime();
                            QString startTimeStr = QString("%1:%2")
                                .arg(startTime.getHours(), 2, 10, QChar('0'))
                                .arg(startTime.getMinutes(), 2, 10, QChar('0'));
                            QString endTimeStr = QString("%1:%2")
                                .arg(endTime.getHours(), 2, 10, QChar('0'))
                                .arg(endTime.getMinutes(), 2, 10, QChar('0'));

                            resultsText->append(QString("Время отправления: %1\n").arg(startTimeStr));
                            resultsText->append(QString("Время прибытия: %1\n").arg(endTimeStr));

                            const auto& trips = journey.getTrips();
                            const auto& transferPoints = journey.getTransferPoints();

                            resultsText->append("\nПуть:\n");
                            resultsText->append("  " + stopA);

                            for (size_t j = 0; j < trips.size(); ++j) {
                                const auto& trip = trips[j];
                                const auto& route = trip->getRoute();
                                const auto& routeStops = route->getAllStops();

                                // Определяем начальную и конечную остановки для этого участка
                                std::string segmentStart = (j == 0) ? stopA.toStdString() : transferPoints[j - 1];
                                std::string segmentEnd = (j < transferPoints.size()) ? transferPoints[j] : stopB.toStdString();

                                int startPos = route->getStopPosition(segmentStart);
                                int endPos = route->getStopPosition(segmentEnd);

                                if (startPos != -1 && endPos != -1 && startPos < endPos) {
                                    for (int k = startPos + 1; k <= endPos; ++k) {
                                        resultsText->append(" → " + QString::fromStdString(routeStops[k]));
                                    }
                                }

                                resultsText->append(QString(" [Маршрут %1 (%2)]")
                                    .arg(route->getNumber())
                                    .arg(QString::fromStdString(route->getVehicleType())));

                                if (j < transferPoints.size()) {
                                    resultsText->append(QString("\n  Пересадка на остановке: %1\n")
                                        .arg(QString::fromStdString(transferPoints[j])));
                                    resultsText->append("  " + QString::fromStdString(transferPoints[j]));
                                }
                            }
                            resultsText->append("\n\n");
                        }
                        resultsText->append("========================================\n");
                    }
                }
            } catch (const std::exception& e) {
                resultsText->append(QString("Ошибка при поиске маршрутов с пересадками: %1\n").arg(e.what()));
            }
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка при поиске: %1").arg(e.what()));
    }
}

// ==================== AddRouteDialog ====================

AddRouteDialog::AddRouteDialog(TransportSystem* system, QWidget *parent)
    : QDialog(parent), transportSystem(system) {
    setWindowTitle("Добавление маршрута");
    setMinimumSize(500, 600);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* numberLabel = new QLabel("Номер маршрута:", this);
    routeNumberSpinBox = new QSpinBox(this);
    routeNumberSpinBox->setMinimum(1);
    routeNumberSpinBox->setMaximum(9999);

    QLabel* typeLabel = new QLabel("Тип транспорта:", this);
    vehicleTypeComboBox = new QComboBox(this);
    vehicleTypeComboBox->addItems(QStringList() << "Автобус" << "Трамвай" << "Троллейбус");

    QLabel* stopsLabel = new QLabel("Остановки маршрута:", this);
    stopsListWidget = new QListWidget(this);

    QLabel* stopLabel = new QLabel("Добавить остановку:", this);
    stopComboBox = new QComboBox(this);
    QPushButton* addStopBtn = new QPushButton("Добавить", this);
    QPushButton* removeStopBtn = new QPushButton("Удалить выбранную", this);

    QHBoxLayout* stopLayout = new QHBoxLayout();
    stopLayout->addWidget(stopComboBox);
    stopLayout->addWidget(addStopBtn);
    stopLayout->addWidget(removeStopBtn);

    QGroupBox* daysGroup = new QGroupBox("Дни недели", this);
    QVBoxLayout* daysLayout = new QVBoxLayout(daysGroup);
    mondayCheck = new QCheckBox("Понедельник", daysGroup);
    tuesdayCheck = new QCheckBox("Вторник", daysGroup);
    wednesdayCheck = new QCheckBox("Среда", daysGroup);
    thursdayCheck = new QCheckBox("Четверг", daysGroup);
    fridayCheck = new QCheckBox("Пятница", daysGroup);
    saturdayCheck = new QCheckBox("Суббота", daysGroup);
    sundayCheck = new QCheckBox("Воскресенье", daysGroup);

    mondayCheck->setChecked(true);
    tuesdayCheck->setChecked(true);
    wednesdayCheck->setChecked(true);
    thursdayCheck->setChecked(true);
    fridayCheck->setChecked(true);
    saturdayCheck->setChecked(true);
    sundayCheck->setChecked(true);

    daysLayout->addWidget(mondayCheck);
    daysLayout->addWidget(tuesdayCheck);
    daysLayout->addWidget(wednesdayCheck);
    daysLayout->addWidget(thursdayCheck);
    daysLayout->addWidget(fridayCheck);
    daysLayout->addWidget(saturdayCheck);
    daysLayout->addWidget(sundayCheck);

    QPushButton* addBtn = new QPushButton("Добавить маршрут", this);
    QPushButton* cancelBtn = new QPushButton("Отмена", this);

    layout->addWidget(numberLabel);
    layout->addWidget(routeNumberSpinBox);
    layout->addWidget(typeLabel);
    layout->addWidget(vehicleTypeComboBox);
    layout->addWidget(stopsLabel);
    layout->addWidget(stopsListWidget);
    layout->addWidget(stopLabel);
    layout->addLayout(stopLayout);
    layout->addWidget(daysGroup);
    layout->addWidget(addBtn);
    layout->addWidget(cancelBtn);

    connect(addStopBtn, &QPushButton::clicked, this, &AddRouteDialog::addStopToList);
    connect(removeStopBtn, &QPushButton::clicked, this, &AddRouteDialog::removeStopFromList);
    connect(addBtn, &QPushButton::clicked, this, &AddRouteDialog::onAddClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    populateStops();
}

void AddRouteDialog::populateStops() {
    stopComboBox->clear();
    const auto& stops = transportSystem->getStops();
    for (const auto& stop : stops) {
        stopComboBox->addItem(QString::fromStdString(stop.getName()));
    }
}

void AddRouteDialog::addStopToList() {
    QString stop = stopComboBox->currentText();
    if (!stop.isEmpty()) {
        stopsListWidget->addItem(stop);
    }
}

void AddRouteDialog::removeStopFromList() {
    int row = stopsListWidget->currentRow();
    if (row >= 0) {
        delete stopsListWidget->takeItem(row);
    }
}

void AddRouteDialog::onAddClicked() {
    if (stopsListWidget->count() < 2) {
        QMessageBox::warning(this, "Ошибка", "Маршрут должен содержать минимум 2 остановки.");
        return;
    }

    try {
        int number = routeNumberSpinBox->value();
        std::string vehicleType = vehicleTypeComboBox->currentText().toStdString();

        List<std::string> stops;
        for (int i = 0; i < stopsListWidget->count(); ++i) {
            stops.push_back(stopsListWidget->item(i)->text().toStdString());
        }

        std::set<int> weekDays;
        if (mondayCheck->isChecked()) weekDays.insert(1);
        if (tuesdayCheck->isChecked()) weekDays.insert(2);
        if (wednesdayCheck->isChecked()) weekDays.insert(3);
        if (thursdayCheck->isChecked()) weekDays.insert(4);
        if (fridayCheck->isChecked()) weekDays.insert(5);
        if (saturdayCheck->isChecked()) weekDays.insert(6);
        if (sundayCheck->isChecked()) weekDays.insert(7);

        if (weekDays.empty()) {
            QMessageBox::warning(this, "Ошибка", "Выберите хотя бы один день недели.");
            return;
        }

        auto route = std::make_shared<Route>(number, vehicleType, stops, weekDays);
        transportSystem->addRoute(route);

        QMessageBox::information(this, "Успех", "Маршрут добавлен.");
        accept();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка: %1").arg(e.what()));
    }
}

// ==================== AddTripDialog ====================

AddTripDialog::AddTripDialog(TransportSystem* system, QWidget *parent)
    : QDialog(parent), transportSystem(system) {
    setWindowTitle("Добавление рейса");
    setMinimumSize(400, 300);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* idLabel = new QLabel("ID рейса:", this);
    tripIdSpinBox = new QSpinBox(this);
    tripIdSpinBox->setMinimum(1);
    tripIdSpinBox->setMaximum(9999);

    QLabel* routeLabel = new QLabel("Маршрут:", this);
    routeComboBox = new QComboBox(this);

    QLabel* vehicleLabel = new QLabel("Транспорт:", this);
    vehicleComboBox = new QComboBox(this);

    QLabel* driverLabel = new QLabel("Водитель:", this);
    QHBoxLayout* driverLayout = new QHBoxLayout();
    driverComboBox = new QComboBox(this);
    QPushButton* addDriverBtn = new QPushButton("+ Добавить", this);
    addDriverBtn->setMaximumWidth(100);
    driverLayout->addWidget(driverComboBox);
    driverLayout->addWidget(addDriverBtn);

    QLabel* timeLabel = new QLabel("Время начала:", this);
    startTimeEdit = new QTimeEdit(this);
    startTimeEdit->setDisplayFormat("HH:mm");

    QLabel* dayLabel = new QLabel("День недели (1-7):", this);
    weekDaySpinBox = new QSpinBox(this);
    weekDaySpinBox->setMinimum(1);
    weekDaySpinBox->setMaximum(7);
    weekDaySpinBox->setValue(1);

    QPushButton* addBtn = new QPushButton("Добавить рейс", this);
    QPushButton* cancelBtn = new QPushButton("Отмена", this);

    layout->addWidget(idLabel);
    layout->addWidget(tripIdSpinBox);
    layout->addWidget(routeLabel);
    layout->addWidget(routeComboBox);
    layout->addWidget(vehicleLabel);
    layout->addWidget(vehicleComboBox);
    layout->addWidget(driverLabel);
    layout->addLayout(driverLayout);
    layout->addWidget(timeLabel);
    layout->addWidget(startTimeEdit);
    layout->addWidget(dayLabel);
    layout->addWidget(weekDaySpinBox);
    layout->addWidget(addBtn);
    layout->addWidget(cancelBtn);

    connect(addBtn, &QPushButton::clicked, this, &AddTripDialog::onAddClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(addDriverBtn, &QPushButton::clicked, this, &AddTripDialog::onAddDriverClicked);
    connect(routeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AddTripDialog::onRouteChanged);

    populateData();
    if (routeComboBox->count() > 0) {
        onRouteChanged(); // Инициализируем транспорт и водителей для первого маршрута
    }
}

void AddTripDialog::populateData() {
    routeComboBox->clear();
    const auto& routes = transportSystem->getRoutes();
    for (const auto& route : routes) {
        QString routeText = QString("Маршрут %1 (%2)").arg(route->getNumber())
                                                      .arg(QString::fromStdString(route->getVehicleType()));
        routeComboBox->addItem(routeText, route->getNumber());
    }
}

void AddTripDialog::onRouteChanged() {
    int routeNumber = routeComboBox->currentData().toInt();
    auto route = transportSystem->getRouteByNumber(routeNumber);
    if (!route) {
        vehicleComboBox->clear();
        driverComboBox->clear();
        return;
    }

    std::string vehicleType = route->getVehicleType();
    populateVehicles(vehicleType);
    populateDrivers(vehicleType);
}

void AddTripDialog::populateVehicles(const std::string& vehicleType) {
    vehicleComboBox->clear();
    const auto& vehicles = transportSystem->getVehicles();
    for (const auto& vehicle : vehicles) {
        // Проверяем тип транспорта
        if (vehicle->getType() == vehicleType) {
            vehicleComboBox->addItem(QString::fromStdString(vehicle->getLicensePlate()));
        }
    }
}

void AddTripDialog::populateDrivers(const std::string& vehicleType) {
    driverComboBox->clear();
    const auto& drivers = transportSystem->getDrivers();

    // Определяем нужные категории для типа транспорта
    std::set<std::string> requiredCategories;
    if (vehicleType == "Автобус") {
        requiredCategories.insert("D");
        requiredCategories.insert("DT");
    } else if (vehicleType == "Трамвай" || vehicleType == "Троллейбус") {
        requiredCategories.insert("T");
        requiredCategories.insert("DT");
    }

    for (size_t i = 0; i < drivers.size(); ++i) {
        std::string category = drivers[i]->getCategory();
        // Если категория пустая или соответствует требованиям
        if (category.empty() || requiredCategories.find(category) != requiredCategories.end()) {
            driverComboBox->addItem(QString::fromStdString(drivers[i]->getFullName()),
                                    static_cast<int>(i));
        }
    }
}

void AddTripDialog::onAddDriverClicked() {
    AddDriverDialog dialog(transportSystem, this);
    if (dialog.exec() == QDialog::Accepted) {
        // Обновляем список водителей после добавления нового
        int routeNumber = routeComboBox->currentData().toInt();
        auto route = transportSystem->getRouteByNumber(routeNumber);
        if (route) {
            std::string vehicleType = route->getVehicleType();
            populateDrivers(vehicleType);

            // Выбираем последнего добавленного водителя (он будет последним в списке)
            const auto& drivers = transportSystem->getDrivers();
            if (!drivers.empty()) {
                // Ищем индекс последнего водителя в комбобоксе
                for (int i = driverComboBox->count() - 1; i >= 0; --i) {
                    int driverIndex = driverComboBox->itemData(i).toInt();
                    if (driverIndex >= 0 && driverIndex < static_cast<int>(drivers.size())) {
                        driverComboBox->setCurrentIndex(i);
                        break;
                    }
                }
            }
        }
    }
}

void AddTripDialog::onAddClicked() {
    try {
        int tripId = tripIdSpinBox->value();
        int routeNumber = routeComboBox->currentData().toInt();
        QString vehiclePlate = vehicleComboBox->currentText();
        QTime qTime = startTimeEdit->time();
        int weekDay = weekDaySpinBox->value();

        auto route = transportSystem->getRouteByNumber(routeNumber);
        if (!route) {
            QMessageBox::warning(this, "Ошибка", "Маршрут не найден.");
            return;
        }

        auto vehicle = transportSystem->getVehicleByLicensePlate(vehiclePlate.toStdString());
        if (!vehicle) {
            QMessageBox::warning(this, "Ошибка", "Транспорт не найден.");
            return;
        }

        // Получаем водителя по индексу из userData
        int driverIndex = driverComboBox->currentData().toInt();
        const auto& drivers = transportSystem->getDrivers();
        if (driverIndex < 0 || driverIndex >= static_cast<int>(drivers.size())) {
            QMessageBox::warning(this, "Ошибка", "Водитель не найден.");
            return;
        }

        auto driver = drivers[driverIndex];

        Time startTime(qTime.hour(), qTime.minute());
        auto trip = std::make_shared<Trip>(tripId, route, vehicle, driver, startTime, weekDay);
        transportSystem->addTrip(trip);

        QMessageBox::information(this, "Успех", "Рейс добавлен.");
        accept();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка: %1").arg(e.what()));
    }
}

// ==================== AddVehicleDialog ====================

AddVehicleDialog::AddVehicleDialog(TransportSystem* system, QWidget *parent)
    : QDialog(parent), transportSystem(system) {
    setWindowTitle("Добавление транспортного средства");
    setMinimumSize(400, 300);

    setupUI();
}

void AddVehicleDialog::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* typeLabel = new QLabel("Тип транспорта:", this);
    vehicleTypeComboBox = new QComboBox(this);
    vehicleTypeComboBox->addItems(QStringList() << "Автобус" << "Трамвай" << "Троллейбус");

    QLabel* plateLabel = new QLabel("Номер:", this);
    licensePlateEdit = new QLineEdit(this);

    QLabel* modelLabel = new QLabel("Модель:", this);
    modelEdit = new QLineEdit(this);

    QLabel* capacityLabel = new QLabel("Вместимость (макс. 60):", this);
    capacityEdit = new QSpinBox(this);
    capacityEdit->setMinimum(1);
    capacityEdit->setMaximum(60);
    capacityEdit->setValue(30);

    fuelWidget = new QWidget(this);
    QVBoxLayout* fuelLayout = new QVBoxLayout(fuelWidget);
    QLabel* fuelLabel = new QLabel("Тип топлива (по умолчанию: дизель):", fuelWidget);
    fuelCapacityEdit = new QLineEdit(fuelWidget);
    fuelCapacityEdit->setPlaceholderText("дизель");
    fuelLayout->addWidget(fuelLabel);
    fuelLayout->addWidget(fuelCapacityEdit);

    batteryWidget = new QWidget(this);
    QVBoxLayout* batteryLayout = new QVBoxLayout(batteryWidget);
    QLabel* batteryLabel = new QLabel("Напряжение (В, по умолчанию: 600):", batteryWidget);
    batteryCapacityEdit = new QLineEdit(batteryWidget);
    batteryCapacityEdit->setPlaceholderText("600");
    batteryLayout->addWidget(batteryLabel);
    batteryLayout->addWidget(batteryCapacityEdit);

    QPushButton* addBtn = new QPushButton("Добавить", this);
    QPushButton* cancelBtn = new QPushButton("Отмена", this);

    layout->addWidget(typeLabel);
    layout->addWidget(vehicleTypeComboBox);
    layout->addWidget(plateLabel);
    layout->addWidget(licensePlateEdit);
    layout->addWidget(modelLabel);
    layout->addWidget(modelEdit);
    layout->addWidget(capacityLabel);
    layout->addWidget(capacityEdit);
    layout->addWidget(fuelWidget);
    layout->addWidget(batteryWidget);
    layout->addWidget(addBtn);
    layout->addWidget(cancelBtn);

    connect(vehicleTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AddVehicleDialog::onVehicleTypeChanged);
    connect(addBtn, &QPushButton::clicked, this, &AddVehicleDialog::onAddClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    onVehicleTypeChanged(0);
}

void AddVehicleDialog::onVehicleTypeChanged(int index) {
    if (index == 0) { // Автобус
        fuelWidget->setVisible(true);
        batteryWidget->setVisible(false);
    } else { // Трамвай или Троллейбус
        fuelWidget->setVisible(false);
        batteryWidget->setVisible(true);
    }
}

void AddVehicleDialog::onAddClicked() {
    try {
        QString plate = licensePlateEdit->text();
        QString model = modelEdit->text();
        int capacity = capacityEdit->value();
        QString type = vehicleTypeComboBox->currentText();

        if (plate.isEmpty() || model.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Заполните все обязательные поля.");
            return;
        }

        if (capacity < 1 || capacity > 60) {
            QMessageBox::warning(this, "Ошибка", "Вместимость должна быть от 1 до 60 человек.");
            return;
        }

        std::shared_ptr<Vehicle> vehicle;

        if (type == "Автобус") {
            QString fuelType = fuelCapacityEdit->text().trimmed();
            if (fuelType.isEmpty()) {
                fuelType = "дизель";
            }
            vehicle = std::make_shared<Bus>(model.toStdString(), plate.toStdString(), capacity, fuelType.toStdString());
        } else if (type == "Трамвай") {
            double batteryVoltage = batteryCapacityEdit->text().toDouble();
            if (batteryVoltage <= 0) {
                batteryVoltage = 600.0; // Значение по умолчанию
            }
            vehicle = std::make_shared<Tram>(model.toStdString(), plate.toStdString(), capacity, batteryVoltage);
        } else if (type == "Троллейбус") {
            double batteryVoltage = batteryCapacityEdit->text().toDouble();
            if (batteryVoltage <= 0) {
                batteryVoltage = 600.0; // Значение по умолчанию
            }
            vehicle = std::make_shared<Trolleybus>(model.toStdString(), plate.toStdString(), capacity, batteryVoltage);
        }

        transportSystem->addVehicle(vehicle);
        QMessageBox::information(this, "Успех", "Транспорт добавлен.");
        accept();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка: %1").arg(e.what()));
    }
}

// ==================== AddStopDialog ====================

AddStopDialog::AddStopDialog(TransportSystem* system, QWidget *parent)
    : QDialog(parent), transportSystem(system) {
    setWindowTitle("Добавление остановки");
    setMinimumSize(300, 150);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* idLabel = new QLabel("ID остановки:", this);
    stopIdSpinBox = new QSpinBox(this);
    stopIdSpinBox->setMinimum(1);
    stopIdSpinBox->setMaximum(9999);

    QLabel* nameLabel = new QLabel("Название:", this);
    stopNameEdit = new QLineEdit(this);

    QPushButton* addBtn = new QPushButton("Добавить", this);
    QPushButton* cancelBtn = new QPushButton("Отмена", this);

    layout->addWidget(idLabel);
    layout->addWidget(stopIdSpinBox);
    layout->addWidget(nameLabel);
    layout->addWidget(stopNameEdit);
    layout->addWidget(addBtn);
    layout->addWidget(cancelBtn);

    connect(addBtn, &QPushButton::clicked, this, &AddStopDialog::onAddClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void AddStopDialog::onAddClicked() {
    try {
        int id = stopIdSpinBox->value();
        QString name = stopNameEdit->text();

        if (name.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Введите название остановки.");
            return;
        }

        Stop stop(id, name.toStdString());
        transportSystem->addStop(stop);

        QMessageBox::information(this, "Успех", "Остановка добавлена.");
        accept();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка: %1").arg(e.what()));
    }
}

// ==================== AddDriverDialog ====================

AddDriverDialog::AddDriverDialog(TransportSystem* system, QWidget *parent)
    : QDialog(parent), transportSystem(system) {
    setWindowTitle("Добавление водителя");
    setMinimumSize(350, 250);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* firstNameLabel = new QLabel("Имя:", this);
    firstNameEdit = new QLineEdit(this);

    QLabel* lastNameLabel = new QLabel("Фамилия:", this);
    lastNameEdit = new QLineEdit(this);

    QLabel* middleNameLabel = new QLabel("Отчество:", this);
    middleNameEdit = new QLineEdit(this);

    QLabel* categoryLabel = new QLabel("Категория водительских прав:", this);
    categoryComboBox = new QComboBox(this);
    categoryComboBox->addItem("D - Автобусы", "D");
    categoryComboBox->addItem("T - Трамваи и троллейбусы", "T");
    categoryComboBox->addItem("D и T - Универсальная", "DT");
    categoryComboBox->setCurrentIndex(0); // По умолчанию D

    QPushButton* addBtn = new QPushButton("Добавить", this);
    QPushButton* cancelBtn = new QPushButton("Отмена", this);

    layout->addWidget(firstNameLabel);
    layout->addWidget(firstNameEdit);
    layout->addWidget(lastNameLabel);
    layout->addWidget(lastNameEdit);
    layout->addWidget(middleNameLabel);
    layout->addWidget(middleNameEdit);
    layout->addWidget(categoryLabel);
    layout->addWidget(categoryComboBox);
    layout->addWidget(addBtn);
    layout->addWidget(cancelBtn);

    connect(addBtn, &QPushButton::clicked, this, &AddDriverDialog::onAddClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void AddDriverDialog::onAddClicked() {
    try {
        QString firstName = firstNameEdit->text();
        QString lastName = lastNameEdit->text();
        QString middleName = middleNameEdit->text();
        QString category = categoryComboBox->currentData().toString();

        if (firstName.isEmpty() || lastName.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Заполните обязательные поля (Имя и Фамилия).");
            return;
        }

        if (category.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Выберите категорию водительских прав.");
            return;
        }

        auto driver = std::make_shared<Driver>(
            firstName.toStdString(),
            lastName.toStdString(),
            middleName.toStdString(),
            category.toStdString()
        );

        transportSystem->addDriver(driver);
        QMessageBox::information(this, "Успех", "Водитель добавлен.");
        accept();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка: %1").arg(e.what()));
    }
}

// ==================== ArrivalTimeDialog ====================

ArrivalTimeDialog::ArrivalTimeDialog(TransportSystem* system, QWidget *parent)
    : QDialog(parent), transportSystem(system) {
    setWindowTitle("Расчет времени прибытия");
    setMinimumSize(500, 400);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* tripLabel = new QLabel("Выберите рейс:", this);
    tripComboBox = new QComboBox(this);

    QLabel* speedLabel = new QLabel("Средняя скорость (км/ч):", this);
    speedSpinBox = new QDoubleSpinBox(this);
    speedSpinBox->setMinimum(10.0);
    speedSpinBox->setMaximum(100.0);
    speedSpinBox->setValue(40.0);
    speedSpinBox->setDecimals(1);

    QPushButton* calcBtn = new QPushButton("Рассчитать", this);

    resultsText = new QTextEdit(this);
    resultsText->setReadOnly(true);

    QPushButton* closeBtn = new QPushButton("Закрыть", this);

    layout->addWidget(tripLabel);
    layout->addWidget(tripComboBox);
    layout->addWidget(speedLabel);
    layout->addWidget(speedSpinBox);
    layout->addWidget(calcBtn);
    layout->addWidget(resultsText);
    layout->addWidget(closeBtn);

    connect(calcBtn, &QPushButton::clicked, this, &ArrivalTimeDialog::onCalculateClicked);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);

    populateTrips();
}

void ArrivalTimeDialog::populateTrips() {
    tripComboBox->clear();
    const auto& trips = transportSystem->getTrips();
    for (const auto& trip : trips) {
        tripComboBox->addItem(QString("Рейс %1 (Маршрут %2)").arg(trip->getTripId()).arg(trip->getRoute()->getNumber()),
                             trip->getTripId());
    }
}

void ArrivalTimeDialog::onCalculateClicked() {
    try {
        int tripId = tripComboBox->currentData().toInt();
        double speed = speedSpinBox->value();

        transportSystem->calculateArrivalTimes(tripId, speed);

        auto trip = transportSystem->getTripById(tripId);
        if (trip) {
            resultsText->clear();
            resultsText->append(QString("Расчет времени прибытия для рейса %1:\n\n").arg(tripId));

            const auto& schedule = trip->getSchedule();
            for (const auto& [stopName, time] : schedule) {
                QString timeStr = QString("%1:%2").arg(time.getHours(), 2, 10, QChar('0'))
                                                  .arg(time.getMinutes(), 2, 10, QChar('0'));
                resultsText->append(QString("%1: %2\n")
                    .arg(QString::fromStdString(stopName))
                    .arg(timeStr));
            }
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка при расчете: %1").arg(e.what()));
    }
}