# Места обработки ввода в проекте

## 1. Консольный ввод (std::cin, std::getline)

### main.cpp
- **Строки 48-54**: Обработка выбора в главном меню (логин/гость/выход)
  - `std::cin >> choice` - выбор пункта меню

### ui.cpp - Основной файл консольного UI

#### Функции авторизации и меню:
- **runAdminMode()** (строки 1042-1184)
  - Строки 1045-1048: Ввод логина и пароля администратора (`std::getline`)
  - Строки 1062-1068: Обработка выбора пункта меню (`std::cin >> choice`)
  - Строки 1098-1103: Ввод номера маршрута для удаления
  - Строки 1132-1137: Ввод ID рейса для удаления

- **runGuestMode()** (строки 1006-1040)
  - Строки 1012-1018: Обработка выбора пункта меню (`std::cin >> choice`)

#### Функции добавления данных:
- **adminAddRoute()** (строки 255-317)
  - Строки 262-268: Ввод номера маршрута (`std::cin >> number`)
  - Строка 271: Ввод типа транспорта (`std::getline`)
  - Строки 283-308: Ввод остановок маршрута в цикле (`std::getline`)

- **adminAddTrip()** (строки 319-429)
  - Строки 350-356: Ввод ID рейса (`std::cin >> tripId`)
  - Строки 358-364: Ввод номера маршрута (`std::cin >> routeNumber`)
  - Строка 372: Ввод номерного знака (`std::getline`)
  - Строки 382-388: Выбор водителя (`std::cin >> driverChoice`)
  - Строка 398: Ввод времени отправления (`std::getline`)
  - Строки 409-415: Ввод дня недели (`std::cin >> weekDay`)

- **adminAddVehicle()** (строки 431-489)
  - Строка 436: Ввод типа транспорта (`std::getline`)
  - Строка 448: Ввод модели (`std::getline`)
  - Строка 463: Ввод номерного знака (`std::getline`)

- **adminAddStop()** (строки 491-532)
  - Строки 496-502: Ввод ID остановки (`std::cin >> id`)
  - Строка 509: Ввод названия остановки (`std::getline`)

- **adminAddDriver()** (строки 534-614)
  - Строка 539: Ввод имени (`std::getline`)
  - Строка 558: Ввод фамилии (`std::getline`)
  - Строка 576: Ввод отчества (`std::getline`)
  - Строка 593: Ввод категории прав (`std::getline`)

#### Функции просмотра:
- **searchRoutes()** (строки 617-657)
  - Строка 624: Ввод начальной остановки (`std::getline`)
  - Строка 625: Ввод конечной остановки (`std::getline`)

- **viewStopTimetable()** (строки 659-672)
  - Строка 665: Ввод остановки (`std::getline`)

- **viewTransportSchedule()** (строки 744-936)
  - Строки 752-758: Выбор типа транспорта (`std::cin >> transportTypeChoice`)
  - Строки 791-797: Выбор маршрута (`std::cin >> routeChoice`)
  - Строки 818-824: Выбор направления (`std::cin >> directionChoice`)
  - Строки 847-853: Выбор остановки (`std::cin >> stopChoice`)
  - Строки 877-883: Выбор дня недели (`std::cin >> weekDayChoice`)

- **viewTransportScheduleGuest()** (строки 674-742)
  - Строки 682-688: Выбор типа транспорта (`std::cin >> transportTypeChoice`)

- **calculateArrivalTime()** (строки 938-988)
  - Строки 959-964: Ввод ID рейса (`std::cin >> tripId`)
  - Строки 965-971: Ввод средней скорости (`std::cin >> speed`)

#### Вспомогательные функции:
- **getStopNameByInput()** (строки 233-253)
  - Парсинг ввода остановки (по ID или названию)

## 2. Qt UI ввод (QInputDialog, QLineEdit, QComboBox)

### qt_ui.cpp

#### Диалоги удаления:
- **AdminModeWidget::removeRoute()** (строки 692-704)
  - Строка 694: `QInputDialog::getInt()` - ввод номера маршрута

- **AdminModeWidget::removeTrip()** (строки 706-718)
  - Строка 708: `QInputDialog::getInt()` - ввод ID рейса

- **AdminModeWidget::removeVehicle()** (строки 720-740)
  - Строка 722: `QInputDialog::getText()` - ввод номерного знака

- **AdminModeWidget::removeStop()** (строки 742-757)
  - Строка 744: `QInputDialog::getInt()` - ввод ID остановки

- **AdminModeWidget::removeDriver()** (строки 759-779)
  - Строка 761: `QInputDialog::getText()` - ввод имени
  - Строка 763: `QInputDialog::getText()` - ввод фамилии

#### Диалог авторизации:
- **AdminLoginDialog::tryLogin()** (строки ~880-890)
  - Строка 882: `usernameEdit->text()` - получение логина
  - Строка 883: `passwordEdit->text()` - получение пароля

#### Диалоги добавления (в qt_ui.cpp):
- **AddRouteDialog::onAddClicked()** (~строки 1620-1650)
  - `routeNumberSpinBox->value()` - номер маршрута
  - `vehicleTypeComboBox->currentText()` - тип транспорта
  - `stopsListWidget->item(i)->text()` - остановки из списка

- **AddTripDialog::onAddClicked()** (~строки 1830-1890)
  - `tripIdSpinBox->value()` - ID рейса
  - `routeComboBox->currentText()` - маршрут
  - `vehicleComboBox->currentText()` - транспорт
  - `driverComboBox->currentText()` - водитель
  - `startTimeEdit->time()` - время отправления
  - `weekDaySpinBox->value()` - день недели

- **AddVehicleDialog::onAddClicked()** (~строки 1960-2040)
  - Строка 1966: `licensePlateEdit->text()` - номерной знак
  - Строка 1967: `modelEdit->text()` - модель
  - Строка 2020: `fuelCapacityEdit->text()` - емкость топлива (автобус)
  - Строка 2026: `batteryCapacityEdit->text()` - напряжение батареи

- **AddStopDialog::onAddClicked()** (~строки 2080-2090)
  - Строка 2085: `stopNameEdit->text()` - название остановки
  - `stopIdSpinBox->value()` - ID остановки

- **AddDriverDialog::onAddClicked()** (~строки 2170-2180)
  - Строка 2172: `firstNameEdit->text()` - имя
  - Строка 2173: `lastNameEdit->text()` - фамилия
  - Строка 2174: `middleNameEdit->text()` - отчество
  - `categoryComboBox->currentText()` - категория прав

#### Диалоги просмотра:
- **RouteSearchDialog::onSearchClicked()** (~строки 1380-1390)
  - Строка 1381: `stopAComboBox->currentText()` - начальная остановка
  - Строка 1382: `stopBComboBox->currentText()` - конечная остановка

- **StopTimetableDialog::onStopSelected()** (~строки 1205-1225)
  - `stopComboBox->currentText()` - выбранная остановка

- **ArrivalTimeDialog::onCalculateClicked()** (~строки ~1650)
  - `tripComboBox->currentText()` - выбранный рейс
  - `speedSpinBox->value()` - скорость

## 3. Парсинг данных из файлов

### data_manager.cpp
- **loadStops()** (строки ~370-438)
  - Строка 381: `std::getline(file, line)` - чтение строк из файла stops.txt

- **loadVehiclesFromFile()** (строки ~500-620)
  - Строка 502: `std::getline(file, line)` - чтение строк транспортных средств
  - Строки 521-523: Парсинг типа, модели, номерного знака через `std::getline(ss, ...)`

- **loadRoutes()** (строки ~660-690)
  - Строка 660: `std::getline(file, line)` - чтение маршрутов

- **loadTrips()** (строки ~760-890)
  - Строка 761: `std::getline(file, line)` - чтение рейсов

- **loadDrivers()** (строки ~880-920)
  - Строка 883: `std::getline(file, line)` - чтение водителей

- **loadAdmins()** (строки ~940-970)
  - Строки 954-960: Бинарное чтение (`file.read()`) - чтение администраторов

## 4. Парсинг строк (десериализация)

### trip.cpp
- **Trip::deserialize()** (строки ~100-380)
  - Строки 106-145: Парсинг строки рейса через `std::getline(ss, token, '|')`
  - Парсинг маршрута, транспорта, водителя, времени, расписания

### route.cpp
- **Route::deserialize()** (строки ~100-125)
  - Строки 101-104: Парсинг номера, типа, остановок, дней недели через `std::getline(ss, ...)`
  - Строки 110, 119: Парсинг списков через `std::getline(..., ';')` и `std::getline(..., ',')`

### stop.cpp
- **Stop::deserialize()** (строки ~30-40)
  - Строки 31-32: Парсинг ID и названия через `std::getline(ss, ...)`

### driver.cpp
- **Driver::deserialize()** (строки ~50-60)
  - Строки 54-57: Парсинг имени, фамилии, отчества, категории через `std::getline(ss, ...)`

### vehicle.cpp
- **Vehicle::deserialize()** (строки ~35-45)
  - Строки 38-40: Парсинг типа, модели, номерного знака через `std::getline(ss, ...)`

### time.cpp
- **Time::Time(const std::string&)** (строки ~20-35)
  - Парсинг строки времени формата HH:MM

## 5. Валидация ввода

### ui.cpp - Функции валидации (строки 19-184)
- `containsOnlyDigits()` - проверка на цифры
- `containsOnlyLetters()` - проверка на буквы
- `isValidText()` - проверка текста
- `hasCapitalFirstLetter()` - проверка заглавной буквы
- `isValidNameFormat()` - проверка формата имени
- `isValidLength()` - проверка длины
- `isValidLicensePlate()` - проверка номерного знака
- `isValidDriverCategory()` - проверка категории прав

## Итого: Основные файлы обработки ввода

1. **ui.cpp** - основная консольная обработка ввода (~1184 строки)
2. **qt_ui.cpp** - Qt UI обработка ввода (~2300 строк)
3. **main.cpp** - главное меню (строки 48-54)
4. **data_manager.cpp** - чтение из файлов
5. **trip.cpp, route.cpp, stop.cpp, driver.cpp, vehicle.cpp, time.cpp** - парсинг строк




