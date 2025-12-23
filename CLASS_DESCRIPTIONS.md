# Описание классов системы управления общественным транспортом

## 1. Класс Vehicle

Данный класс предназначен для представления транспортного средства в системе управления общественным транспортом. Является базовым классом для всех типов транспортных средств. Обеспечивает хранение и обработку основной информации о транспорте: тип, модель и номерной знак.

Поля:

– std::string type – поле для хранения типа транспортного средства;

– std::string model – поле для хранения модели транспортного средства;

– std::string licensePlate – поле для хранения номерного знака транспортного средства;

Методы:

– Vehicle(const std::string& t, const std::string& m, const std::string& lp) – конструктор с параметрами;

– virtual ~Vehicle() – виртуальный деструктор;

– std::string getInfo() const – метод получения информации о транспортном средстве;

– std::string getType() const – метод получения типа транспортного средства;

– std::string getModel() const – метод получения модели транспортного средства;

– std::string getLicensePlate() const – метод получения номерного знака;

– virtual std::string serialize() const – виртуальный метод сериализации в строку;

– static std::shared_ptr<Vehicle> deserialize(const std::string& data) – статический метод десериализации из строки;

## 2. Класс FuelTransport

Данный класс является наследником класса Vehicle и представляет транспортные средства, работающие на топливе (бензин, дизель, газ). Обеспечивает хранение и обработку информации о вместимости пассажиров и типе топлива.

Поля:

– int capacity – поле для хранения вместимости пассажиров;

– std::string fuelType – поле для хранения типа топлива (бензин, дизель, газ);

Методы:

– FuelTransport(const std::string& t, const std::string& m, const std::string& lp, int cap = 50, const std::string& fuel = "дизель") – конструктор с параметрами;

– int getCapacity() const – метод получения вместимости;

– void setCapacity(int cap) – метод изменения вместимости;

– std::string getFuelType() const – метод получения типа топлива;

– void setFuelType(const std::string& fuel) – метод изменения типа топлива;

– std::string serialize() const override – переопределенный метод сериализации в строку;

## 3. Класс ElectricTransport

Данный класс является наследником класса Vehicle и представляет транспортные средства, работающие на электричестве. Обеспечивает хранение и обработку информации о вместимости пассажиров и напряжении питания.

Поля:

– int capacity – поле для хранения вместимости пассажиров;

– double voltage – поле для хранения напряжения питания (В);

Методы:

– ElectricTransport(const std::string& t, const std::string& m, const std::string& lp, int cap = 50, double volt = 600.0) – конструктор с параметрами;

– int getCapacity() const – метод получения вместимости;

– void setCapacity(int cap) – метод изменения вместимости;

– double getVoltage() const – метод получения напряжения;

– void setVoltage(double volt) – метод изменения напряжения;

– std::string serialize() const override – переопределенный метод сериализации в строку;

## 4. Класс Bus

Данный класс является наследником класса FuelTransport и представляет автобус с топливным двигателем в системе управления общественным транспортом.

Методы:

– Bus(const std::string& m, const std::string& lp, int cap = 50, const std::string& fuel = "дизель") – конструктор с параметрами;

## 5. Класс Trolleybus

Данный класс является наследником класса ElectricTransport и представляет троллейбус с электрическим двигателем в системе управления общественным транспортом.

Методы:

– Trolleybus(const std::string& m, const std::string& lp, int cap = 50, double volt = 600.0) – конструктор с параметрами;

## 6. Класс Tram

Данный класс является наследником класса ElectricTransport и представляет трамвай с электрическим двигателем в системе управления общественным транспортом.

Методы:

– Tram(const std::string& m, const std::string& lp, int cap = 100, double volt = 600.0) – конструктор с параметрами;

## 7. Класс Driver

Данный класс предназначен для представления водителя в системе управления общественным транспортом. Обеспечивает хранение и обработку информации о водителе: имя, фамилию, отчество и категорию водительских прав.

Поля:

– std::string firstName – поле для хранения имени водителя;

– std::string lastName – поле для хранения фамилии водителя;

– std::string middleName – поле для хранения отчества водителя;

– std::string category – поле для хранения категории водительских прав (D, T, и т.д.);

Методы:

– Driver(const std::string& fname, const std::string& lname, const std::string& mname = "", const std::string& cat = "") – конструктор с параметрами;

– std::string getFullName() const – метод получения полного имени водителя;

– std::string getFirstName() const – метод получения имени водителя;

– std::string getLastName() const – метод получения фамилии водителя;

– std::string getMiddleName() const – метод получения отчества водителя;

– std::string getCategory() const – метод получения категории водительских прав;

– void setCategory(const std::string& cat) – метод изменения категории водительских прав;

– std::string serialize() const – метод сериализации в строку;

– static std::shared_ptr<Driver> deserialize(const std::string& data) – статический метод десериализации из строки;

Операторы:

– bool operator==(const Driver& other) const – перегрузка оператора сравнения на равенство;

## 8. Класс Route

Данный класс предназначен для представления маршрута в системе управления общественным транспортом. Обеспечивает хранение и обработку информации о маршруте: номер, тип транспорта, список остановок, начальную и конечную остановки, дни недели работы.

Поля:

– int number – поле для хранения номера маршрута;

– std::string vehicleType – поле для хранения типа транспортного средства;

– std::string startStop – поле для хранения начальной остановки;

– std::string endStop – поле для хранения конечной остановки;

– List<std::string> allStops – поле для хранения списка всех остановок маршрута;

– std::set<int> weekDays – поле для хранения дней недели работы маршрута (1-понедельник, 2-вторник, ..., 7-воскресенье);

Методы:

– Route(int num, const std::string& vType, const List<std::string>& stops, const std::set<int>& days = {1,2,3,4,5,6,7}) – конструктор с параметрами (может выбрасывать ContainerException, если список остановок пуст);

– bool containsStop(const std::string& stop) const – метод проверки наличия остановки в маршруте;

– int getStopPosition(const std::string& stop) const – метод получения позиции остановки в маршруте;

– bool isStopBefore(const std::string& stopA, const std::string& stopB) const – метод проверки, находится ли остановка A перед остановкой B;

– int getNumber() const – метод получения номера маршрута;

– std::string getVehicleType() const – метод получения типа транспорта;

– std::string getStartStop() const – метод получения начальной остановки;

– std::string getEndStop() const – метод получения конечной остановки;

– const List<std::string>& getAllStops() const – метод получения списка всех остановок;

– const std::set<int>& getWeekDays() const – метод получения дней недели работы;

– bool operatesOnDay(int day) const – метод проверки работы маршрута в указанный день;

– std::string serialize() const – метод сериализации в строку;

– static std::shared_ptr<Route> deserialize(const std::string& data) – статический метод десериализации из строки;

## 9. Класс Stop

Данный класс предназначен для представления остановки в системе управления общественным транспортом. Обеспечивает хранение и обработку информации об остановке: идентификатор и название.

Поля:

– int id – поле для хранения идентификатора остановки;

– std::string name – поле для хранения названия остановки;

Методы:

– Stop(int stopId, std::string stopName) – конструктор с параметрами;

– int getId() const – метод получения идентификатора остановки;

– std::string getName() const – метод получения названия остановки;

– std::string serialize() const – метод сериализации в строку;

– static Stop deserialize(const std::string& data) – статический метод десериализации из строки;

Операторы:

– bool operator==(const Stop& other) const – перегрузка оператора сравнения на равенство;

## 10. Класс Time

Данный класс предназначен для представления времени в системе управления общественным транспортом. Хранит часы и минуты, обеспечивает операции сравнения и арифметические операции с временем.

Поля:

– int hours – поле для хранения часов;

– int minutes – поле для хранения минут;

Методы:

– explicit Time(int h = 0, int m = 0) – конструктор с параметрами;

– explicit Time(const std::string& timeStr) – конструктор из строки (может выбрасывать InputException при неверном формате времени);

– int getTotalMinutes() const – метод получения общего количества минут;

– int getHours() const – метод получения часов;

– int getMinutes() const – метод получения минут;

– std::string serialize() const – метод сериализации в строку;

– static Time deserialize(const std::string& data) – статический метод десериализации из строки;

Приватные методы:

– void normalize(int totalMinutes) – метод нормализации времени;

Операторы:

– bool operator<(const Time& other) const – перегрузка оператора сравнения меньше;

– bool operator<=(const Time& other) const – перегрузка оператора сравнения меньше или равно;

– bool operator>(const Time& other) const – перегрузка оператора сравнения больше;

– bool operator>=(const Time& other) const – перегрузка оператора сравнения больше или равно;

– bool operator==(const Time& other) const – перегрузка оператора сравнения на равенство;

– bool operator!=(const Time& other) const – перегрузка оператора сравнения на неравенство;

– Time operator+(int minutesToAdd) const – перегрузка оператора сложения с минутами;

– Time operator-(int minutesToSubtract) const – перегрузка оператора вычитания минут;

– int operator-(const Time& other) const – перегрузка оператора разницы между временами в минутах;

– friend std::ostream& operator<<(std::ostream& os, const Time& time) – перегрузка оператора вывода данных класса на экран;

– friend std::istream& operator>>(std::istream& is, Time& time) – перегрузка оператора ввода данных класса;

## 11. Класс Trip

Данный класс предназначен для представления рейса в системе управления общественным транспортом. Обеспечивает хранение и обработку информации о рейсе: идентификатор, маршрут, транспортное средство, водителя, время отправления, расписание прибытия на остановки и день недели.

Поля:

– int tripId – поле для хранения идентификатора рейса;

– std::shared_ptr<Route> route – поле для хранения указателя на маршрут рейса;

– std::shared_ptr<Vehicle> vehicle – поле для хранения указателя на транспортное средство;

– std::shared_ptr<Driver> driver – поле для хранения указателя на водителя;

– Time startTime – поле для хранения времени отправления;

– std::map<std::string, Time> schedule – поле для хранения расписания прибытия (остановка -> время прибытия);

– int weekDay – поле для хранения дня недели (1-понедельник, 2-вторник, ..., 7-воскресенье);

Методы:

– Trip(int id, std::shared_ptr<Route> r, std::shared_ptr<Vehicle> v, std::shared_ptr<Driver> d, const Time& start, int day = 1) – конструктор с параметрами (может выбрасывать InputException, если день недели не в диапазоне 1-7);

– void setArrivalTime(const std::string& stop, const Time& time) – метод установки времени прибытия на остановку;

– Time getArrivalTime(const std::string& stop) const – метод получения времени прибытия на остановку (может выбрасывать ContainerException, если остановка не найдена в расписании);

– bool hasStop(const std::string& stop) const – метод проверки наличия остановки в рейсе;

– int getTripId() const – метод получения идентификатора рейса;

– std::shared_ptr<Route> getRoute() const – метод получения маршрута;

– std::shared_ptr<Vehicle> getVehicle() const – метод получения транспортного средства;

– std::shared_ptr<Driver> getDriver() const – метод получения водителя;

– Time getStartTime() const – метод получения времени отправления;

– const std::map<std::string, Time>& getSchedule() const – метод получения расписания;

– int getWeekDay() const – метод получения дня недели;

– Time getEstimatedEndTime() const – метод получения расчетного времени окончания;

– std::string serialize() const – метод сериализации в строку;

– static std::shared_ptr<Trip> deserialize(const std::string& data, TransportSystem* system = nullptr) – статический метод десериализации из строки;

## 12. Класс Journey

Данный класс предназначен для представления поездки с пересадками в системе управления общественным транспортом. Обеспечивает хранение и обработку информации о последовательности рейсов, точках пересадок, времени начала и окончания поездки.

Поля:

– List<std::shared_ptr<Trip>> trips – поле для хранения списка рейсов в поездке;

– List<std::string> transferPoints – поле для хранения точек пересадок;

– Time startTime – поле для хранения времени начала поездки;

– Time endTime – поле для хранения времени окончания поездки;

– int transferCount – поле для хранения количества пересадок;

Методы:

– Journey(const List<std::shared_ptr<Trip>>& tripList, const List<std::string>& transfers, Time start, Time end) – конструктор с параметрами;

– int getTotalDuration() const – метод получения общей продолжительности поездки;

– int getTransferCount() const – метод получения количества пересадок;

– Time getStartTime() const – метод получения времени начала;

– Time getEndTime() const – метод получения времени окончания;

– const List<std::shared_ptr<Trip>>& getTrips() const – метод получения списка рейсов;

– const List<std::string>& getTransferPoints() const – метод получения точек пересадок;

– void display() const – метод вывода информации о поездке;

## 13. Класс IAlgorithm

Данный класс является базовым интерфейсом для всех алгоритмов в системе управления общественным транспортом. Реализует принцип полиморфизма ООП.

Методы:

– virtual ~IAlgorithm() – виртуальный деструктор;

– virtual void execute() = 0 – виртуальный метод выполнения алгоритма;

– virtual std::string getDescription() const = 0 – виртуальный метод получения описания алгоритма;

## 14. Класс Algorithm

Данный класс является шаблонным базовым классом для алгоритмов в системе управления общественным транспортом. Может принимать либо лямбда-функцию, либо шаблонный параметр (тип).

Поля:

– TransportSystem* system – поле для хранения указателя на транспортную систему;

– T algorithmObject – поле для хранения единственного объекта алгоритма;

Внутренние классы:

– Iterator – внутренний класс-итератор для обхода объекта алгоритма (singleton iterator);

Поля класса Iterator:

– T* ptr – поле для хранения указателя на объект алгоритма;

– bool visited – поле-флаг, указывающее, был ли элемент посещен;

Типы класса Iterator (для совместимости со STL):

– using iterator_category = std::forward_iterator_tag;

– using value_type = T;

– using difference_type = std::ptrdiff_t;

– using pointer = T*;

– using reference = T&;

Методы класса Iterator:

– Iterator(T* p, bool v = false) – конструктор с параметрами;

– T& operator*() – перегрузка оператора разыменования (неконстантная версия);

– const T& operator*() const – перегрузка оператора разыменования (константная версия);

– T* operator->() – перегрузка оператора доступа к членам (неконстантная версия);

– const T* operator->() const – перегрузка оператора доступа к членам (константная версия);

– Iterator& operator++() – перегрузка оператора инкремента (префиксная версия);

– Iterator operator++(int) – перегрузка оператора инкремента (постфиксная версия);

– bool operator==(const Iterator& other) const – перегрузка оператора сравнения на равенство;

– bool operator!=(const Iterator& other) const – перегрузка оператора сравнения на неравенство;

Методы:

– explicit Algorithm(TransportSystem* sys, const T& obj) – конструктор для лямбда-функций и функциональных объектов (копирование);

– explicit Algorithm(TransportSystem* sys, T&& obj) – конструктор для лямбда-функций и функциональных объектов (перемещение);

– explicit Algorithm(TransportSystem* sys) – конструктор по умолчанию;

– template<typename... Args> explicit Algorithm(TransportSystem* sys, Args&&... args) – конструктор с параметрами;

– virtual ~Algorithm() – виртуальный деструктор;

– void execute() override – метод выполнения алгоритма;

– std::string getDescription() const override – метод получения описания алгоритма;

– void setSystem(TransportSystem* sys) – метод установки системы;

– template<typename... Args> auto executeAlgorithm(Args&&... args) – метод для выполнения алгоритма с аргументами;

– template<typename... Args> auto operator()(Args&&... args) – оператор вызова функции;

– T& getAlgorithm() – метод получения объекта алгоритма (неконстантная версия);

– const T& getAlgorithm() const – метод получения объекта алгоритма (константная версия);

– Iterator begin() – метод получения итератора на начало (неконстантная версия);

– Iterator end() – метод получения итератора на конец (неконстантная версия);

– Iterator begin() const – метод получения итератора на начало (константная версия);

– Iterator end() const – метод получения итератора на конец (константная версия);

## 15. Класс BaseAlgorithm

Данный класс является базовым классом для алгоритмов с TransportSystem в системе управления общественным транспортом. Предназначен для обратной совместимости.

Поля:

– TransportSystem* system – поле для хранения указателя на транспортную систему;

Методы:

– explicit BaseAlgorithm(TransportSystem* sys) – конструктор с параметрами;

– virtual ~BaseAlgorithm() – виртуальный деструктор;

– void setSystem(TransportSystem* sys) – метод установки системы;

## 16. Класс PathFindingAlgorithm

Данный класс является базовым классом для алгоритмов поиска пути в системе управления общественным транспортом. Наследуется от BaseAlgorithm. Реализует паттерн Strategy.

Поля:

– TransportSystem* system – поле для хранения указателя на транспортную систему (наследуется от BaseAlgorithm);

Методы:

– explicit PathFindingAlgorithm(TransportSystem* sys) – конструктор с параметрами;

– virtual List<Journey> findPath(const std::string& start, const std::string& end, const Time& departureTime) = 0 – виртуальный метод поиска пути;

– void execute() override – реализация базового метода выполнения;

– std::string getDescription() const override – метод получения описания алгоритма;

## 17. Класс BFSAlgorithm

Данный класс является наследником класса PathFindingAlgorithm и представляет алгоритм поиска в ширину (BFS) для поиска маршрутов с пересадками в системе управления общественным транспортом.

Поля:

– int maxTransfers – поле для хранения максимального количества пересадок;

Методы:

– BFSAlgorithm(TransportSystem* sys, int maxTransfers = 2) – конструктор с параметрами;

– List<Journey> findPath(const std::string& start, const std::string& end, const Time& departureTime) override – метод поиска пути;

– void execute() override – метод выполнения алгоритма;

– std::string getDescription() const override – метод получения описания алгоритма;

## 18. Класс FastestPathAlgorithm

Данный класс является наследником класса PathFindingAlgorithm и представляет алгоритм поиска самого быстрого маршрута в системе управления общественным транспортом.

Методы:

– explicit FastestPathAlgorithm(TransportSystem* sys) – конструктор с параметрами;

– List<Journey> findPath(const std::string& start, const std::string& end, const Time& departureTime) override – метод поиска самого быстрого пути;

– void execute() override – метод выполнения алгоритма;

– std::string getDescription() const override – метод получения описания алгоритма;

## 19. Класс MinimalTransfersAlgorithm

Данный класс является наследником класса PathFindingAlgorithm и представляет алгоритм поиска маршрута с минимальными пересадками в системе управления общественным транспортом.

Методы:

– explicit MinimalTransfersAlgorithm(TransportSystem* sys) – конструктор с параметрами;

– List<Journey> findPath(const std::string& start, const std::string& end, const Time& departureTime) override – метод поиска пути с минимальными пересадками;

– void execute() override – метод выполнения алгоритма;

– std::string getDescription() const override – метод получения описания алгоритма;

## 20. Класс ArrivalTimeCalculationAlgorithm

Данный класс является наследником класса BaseAlgorithm и представляет алгоритм расчета времени прибытия на остановки в системе управления общественным транспортом.

Методы:

– explicit ArrivalTimeCalculationAlgorithm(TransportSystem* sys) – конструктор с параметрами;

– void calculateArrivalTimes(int tripId, double averageSpeed) – метод расчета времени прибытия (может выбрасывать InputException при неверной скорости (<=0), ContainerException при отсутствии рейса или пустом маршруте);

– void execute() override – метод выполнения алгоритма;

– std::string getDescription() const override – метод получения описания алгоритма;

## 21. Класс RouteSearchAlgorithm

Данный класс является наследником класса BaseAlgorithm и представляет алгоритм поиска маршрутов между остановками в системе управления общественным транспортом.

Методы:

– explicit RouteSearchAlgorithm(TransportSystem* sys) – конструктор с параметрами;

– List<std::shared_ptr<Route>> findRoutes(const std::string& stopA, const std::string& stopB) – метод поиска маршрутов;

– void execute() override – метод выполнения алгоритма;

– std::string getDescription() const override – метод получения описания алгоритма;

## 22. Класс TemplateAlgorithm

Данный класс является шаблонным классом алгоритмов в системе управления общественным транспортом. Принимает только 1 объект, может принимать либо лямбда-функцию, либо шаблонный параметр (тип).

Поля:

– T algorithmObject – поле для хранения единственного объекта алгоритма;

Внутренние классы:

– итератор отсутствует;

Методы:

– explicit TemplateAlgorithm(const T& obj) – конструктор для лямбда-функций и функциональных объектов (копирование);

– explicit TemplateAlgorithm(T&& obj) – конструктор для лямбда-функций и функциональных объектов (перемещение);

– TemplateAlgorithm() – конструктор по умолчанию;

– template<typename... Args> explicit TemplateAlgorithm(Args&&... args) – конструктор с параметрами;

– template<typename... Args> auto execute(Args&&... args) – метод для выполнения алгоритма;

– template<typename... Args> auto operator()(Args&&... args) – оператор вызова функции;

– T& getAlgorithm() – метод получения объекта алгоритма (неконстантная версия);

– const T& getAlgorithm() const – метод получения объекта алгоритма (константная версия);

## 23. Класс Command

Данный класс является базовым классом для команд в системе управления общественным транспортом. Реализует паттерн Command для поддержки операций Undo/Redo.

Методы:

– virtual ~Command() – виртуальный деструктор;

– virtual void execute() = 0 – виртуальный метод выполнения команды;

– virtual void undo() = 0 – виртуальный метод отмены команды;

– virtual std::string getDescription() const = 0 – виртуальный метод получения описания команды;

## 24. Класс CommandHistory

Данный класс предназначен для управления историей команд в системе управления общественным транспортом. Хранит историю выполненных команд и обеспечивает операции Undo/Redo.

Поля:

– List<std::unique_ptr<Command>> history – поле для хранения истории команд;

– size_t currentIndex – поле для хранения текущего индекса в истории;

– static const size_t MAX_HISTORY_SIZE – константа максимального размера истории (100);

Методы:

– void executeCommand(std::unique_ptr<Command> cmd) – метод выполнения команды;

– bool canUndo() const – метод проверки возможности отмены;

– void undo() – метод отмены последней команды (может выбрасывать ContainerException, если нет действий для отмены);

– bool canRedo() const – метод проверки возможности повтора;

– void redo() – метод повтор команды (может выбрасывать ContainerException, если нет действий для повтора);

– void clear() – метод очистки истории;

– std::string getLastCommandDescription() const – метод получения описания последней команды;

– std::string getNextCommandDescription() const – метод получения описания следующей команды;

## 25. Класс AddRouteCommand

Данный класс является наследником класса Command и представляет команду добавления маршрута в систему управления общественным транспортом.

Поля:

– TransportSystem* system – поле для хранения указателя на транспортную систему;

– std::shared_ptr<Route> route – поле для хранения указателя на добавляемый маршрут;

Методы:

– AddRouteCommand(TransportSystem* sys, std::shared_ptr<Route> r) – конструктор с параметрами;

– void execute() override – метод выполнения команды;

– void undo() override – метод отмены команды;

– std::string getDescription() const override – метод получения описания команды;

## 26. Класс RemoveRouteCommand

Данный класс является наследником класса Command и представляет команду удаления маршрута из системы управления общественным транспортом.

Поля:

– TransportSystem* system – поле для хранения указателя на транспортную систему;

– std::shared_ptr<Route> route – поле для хранения указателя на удаляемый маршрут;

– int routeNumber – поле для хранения номера маршрута;

Методы:

– RemoveRouteCommand(TransportSystem* sys, int num) – конструктор с параметрами;

– void execute() override – метод выполнения команды;

– void undo() override – метод отмены команды;

– std::string getDescription() const override – метод получения описания команды;

## 27. Класс AddTripCommand

Данный класс является наследником класса Command и представляет команду добавления рейса в систему управления общественным транспортом.

Поля:

– TransportSystem* system – поле для хранения указателя на транспортную систему;

– std::shared_ptr<Trip> trip – поле для хранения указателя на добавляемый рейс;

Методы:

– AddTripCommand(TransportSystem* sys, std::shared_ptr<Trip> t) – конструктор с параметрами;

– void execute() override – метод выполнения команды;

– void undo() override – метод отмены команды;

– std::string getDescription() const override – метод получения описания команды;

## 28. Класс RemoveTripCommand

Данный класс является наследником класса Command и представляет команду удаления рейса из системы управления общественным транспортом.

Поля:

– TransportSystem* system – поле для хранения указателя на транспортную систему;

– std::shared_ptr<Trip> trip – поле для хранения указателя на удаляемый рейс;

– int tripId – поле для хранения идентификатора рейса;

Методы:

– RemoveTripCommand(TransportSystem* sys, int id) – конструктор с параметрами;

– void execute() override – метод выполнения команды;

– void undo() override – метод отмены команды;

– std::string getDescription() const override – метод получения описания команды;

## 29. Класс AddVehicleCommand

Данный класс является наследником класса Command и представляет команду добавления транспортного средства в систему управления общественным транспортом.

Поля:

– TransportSystem* system – поле для хранения указателя на транспортную систему;

– std::shared_ptr<Vehicle> vehicle – поле для хранения указателя на добавляемое транспортное средство;

Методы:

– AddVehicleCommand(TransportSystem* sys, std::shared_ptr<Vehicle> v) – конструктор с параметрами;

– void execute() override – метод выполнения команды;

– void undo() override – метод отмены команды;

– std::string getDescription() const override – метод получения описания команды;

## 30. Класс RemoveVehicleCommand

Данный класс является наследником класса Command и представляет команду удаления транспортного средства из системы управления общественным транспортом.

Поля:

– TransportSystem* system – поле для хранения указателя на транспортную систему;

– std::shared_ptr<Vehicle> vehicle – поле для хранения указателя на удаляемое транспортное средство;

– std::string licensePlate – поле для хранения номерного знака;

Методы:

– RemoveVehicleCommand(TransportSystem* sys, const std::string& lp) – конструктор с параметрами;

– void execute() override – метод выполнения команды;

– void undo() override – метод отмены команды;

– std::string getDescription() const override – метод получения описания команды;

## 31. Класс AddStopCommand

Данный класс является наследником класса Command и представляет команду добавления остановки в систему управления общественным транспортом.

Поля:

– TransportSystem* system – поле для хранения указателя на транспортную систему;

– Stop stop – поле для хранения добавляемой остановки;

Методы:

– AddStopCommand(TransportSystem* sys, const Stop& s) – конструктор с параметрами;

– void execute() override – метод выполнения команды;

– void undo() override – метод отмены команды;

– std::string getDescription() const override – метод получения описания команды;

## 32. Класс RemoveStopCommand

Данный класс является наследником класса Command и представляет команду удаления остановки из системы управления общественным транспортом.

Поля:

– TransportSystem* system – поле для хранения указателя на транспортную систему;

– Stop stop – поле для хранения удаляемой остановки;

– int stopId – поле для хранения идентификатора остановки;

Методы:

– RemoveStopCommand(TransportSystem* sys, int id) – конструктор с параметрами;

– void execute() override – метод выполнения команды;

– void undo() override – метод отмены команды;

– std::string getDescription() const override – метод получения описания команды;

## 33. Класс AddDriverCommand

Данный класс является наследником класса Command и представляет команду добавления водителя в систему управления общественным транспортом.

Поля:

– TransportSystem* system – поле для хранения указателя на транспортную систему;

– std::shared_ptr<Driver> driver – поле для хранения указателя на добавляемого водителя;

Методы:

– AddDriverCommand(TransportSystem* sys, std::shared_ptr<Driver> d) – конструктор с параметрами;

– void execute() override – метод выполнения команды;

– void undo() override – метод отмены команды;

– std::string getDescription() const override – метод получения описания команды;

## 34. Класс RemoveDriverCommand

Данный класс является наследником класса Command и представляет команду удаления водителя из системы управления общественным транспортом.

Поля:

– TransportSystem* system – поле для хранения указателя на транспортную систему;

– std::shared_ptr<Driver> driver – поле для хранения указателя на удаляемого водителя;

– std::string firstName – поле для хранения имени водителя;

– std::string lastName – поле для хранения фамилии водителя;

– std::string middleName – поле для хранения отчества водителя;

Методы:

– RemoveDriverCommand(TransportSystem* sys, std::shared_ptr<Driver> d) – конструктор с параметрами;

– void execute() override – метод выполнения команды;

– void undo() override – метод отмены команды;

– std::string getDescription() const override – метод получения описания команды;

## 35. Класс TransportSystem

Данный класс предназначен для управления транспортной системой в системе управления общественным транспортом. Управляет всеми маршрутами, рейсами, транспортными средствами, водителями, остановками. Реализует бизнес-логику работы системы, поиска маршрутов, расчета времени прибытия, управления расписанием водителей и операций Undo/Redo.

Поля:

– List<std::shared_ptr<Route>> routes – поле для хранения списка маршрутов;

– List<std::shared_ptr<Trip>> trips – поле для хранения списка рейсов;

– List<std::shared_ptr<Vehicle>> vehicles – поле для хранения списка транспортных средств;

– List<std::shared_ptr<Driver>> drivers – поле для хранения списка водителей;

– List<Stop> stops – поле для хранения массива остановок;

– std::unordered_map<int, std::string> stopIdToName – поле для хранения соответствия ID остановки и её названия;

– std::unordered_map<std::string, std::string> adminCredentials – поле для хранения учетных данных администраторов;

– JourneyPlanner journeyPlanner – поле для хранения планировщика поездок;

– DriverSchedule driverSchedule – поле для хранения расписания водителей;

– DataManager dataManager – поле для хранения менеджера данных;

– CommandHistory commandHistory – поле для хранения истории команд;

– std::unique_ptr<ArrivalTimeCalculationAlgorithm> arrivalTimeAlgorithm – поле для хранения алгоритма расчета времени прибытия;

– std::unique_ptr<RouteSearchAlgorithm> routeSearchAlgorithm – поле для хранения алгоритма поиска маршрутов;

Методы:

– TransportSystem() – конструктор по умолчанию;

– bool canUndo() const – метод проверки возможности отмены;

– void undo() – метод отмены действия;

– bool canRedo() const – метод проверки возможности повтора;

– void redo() – метод повтор действия;

– std::string getLastCommandDescription() const – метод получения описания последней команды;

– std::string getNextCommandDescription() const – метод получения описания следующей команды;

– bool authenticateAdmin(const std::string& username, const std::string& password) – метод аутентификации администратора;

– void addAdmin(const std::string& username, const std::string& password) – метод добавления администратора;

– const std::unordered_map<std::string, std::string>& getAdminCredentials() const – метод получения учетных данных администраторов;

– void setAdminCredentials(const std::unordered_map<std::string, std::string>& creds) – метод установки учетных данных администраторов;

– void saveData() – метод сохранения всех данных;

– void loadData() – метод загрузки всех данных;

– List<std::shared_ptr<Route>> findRoutes(const std::string& stopA, const std::string& stopB) – метод поиска маршрутов между остановками;

– void getStopTimetable(int stopId, const Time& startTime, const Time& endTime) – метод получения расписания остановки за период;

– void getStopTimetableAll(const std::string& stopName) – метод получения полного расписания остановки;

– void calculateArrivalTimes(int tripId, double averageSpeed) – метод расчета времени прибытия;

– ArrivalTimeCalculationAlgorithm* getArrivalTimeAlgorithm() const – метод получения алгоритма расчета времени прибытия;

– RouteSearchAlgorithm* getRouteSearchAlgorithm() const – метод получения алгоритма поиска маршрутов;

– void addRoute(std::shared_ptr<Route> route) – метод добавления маршрута;

– void addTrip(std::shared_ptr<Trip> trip) – метод добавления рейса;

– void addVehicle(std::shared_ptr<Vehicle> vehicle) – метод добавления транспортного средства;

– void addDriver(std::shared_ptr<Driver> driver) – метод добавления водителя;

– void addStop(const Stop& stop) – метод добавления остановки;

– void removeRoute(int routeNumber) – метод удаления маршрута;

– void removeTrip(int tripId) – метод удаления рейса;

– void displayAllRoutes() const – метод вывода всех маршрутов;

– void displayAllTrips() const – метод вывода всех рейсов;

– void displayAllVehicles() const – метод вывода всех транспортных средств;

– void displayAllStops() const – метод вывода всех остановок;

– const List<std::shared_ptr<Trip>>& getTrips() const – метод получения списка рейсов;

– const List<std::shared_ptr<Route>>& getRoutes() const – метод получения списка маршрутов;

– const List<std::shared_ptr<Vehicle>>& getVehicles() const – метод получения списка транспортных средств;

– const List<Stop>& getStops() const – метод получения массива остановок;

– const List<std::shared_ptr<Driver>>& getDrivers() const – метод получения списка водителей;

– JourneyPlanner& getJourneyPlanner() – метод получения планировщика поездок;

– DriverSchedule& getDriverSchedule() – метод получения расписания водителей;

– std::shared_ptr<Driver> findDriverByName(const std::string& firstName, const std::string& lastName, const std::string& middleName = "") const – метод поиска водителя по имени;

– std::shared_ptr<Vehicle> findVehicleByLicensePlate(const std::string& licensePlate) const – метод поиска транспортного средства по номерному знаку;

– std::shared_ptr<Route> findRouteByNumber(int number) const – метод поиска маршрута по номеру;

– List<std::shared_ptr<Trip>> getTripsThroughStop(const std::string& stopName) const – метод получения рейсов через остановку;

– std::string getStopNameById(int id) const – метод получения названия остановки по ID;

– std::shared_ptr<Route> getRouteByNumber(int number) – метод получения маршрута по номеру;

– std::shared_ptr<Trip> getTripById(int id) – метод получения рейса по ID;

– std::shared_ptr<Vehicle> getVehicleByLicensePlate(const std::string& licensePlate) – метод получения транспортного средства по номерному знаку;

– Stop getStopById(int id) – метод получения остановки по ID;

– void addRouteDirect(std::shared_ptr<Route> route) – метод прямого добавления маршрута (без команды);

– void removeRouteDirect(int routeNumber) – метод прямого удаления маршрута (без команды);

– void addTripDirect(std::shared_ptr<Trip> trip) – метод прямого добавления рейса (без команды);

– void removeTripDirect(int tripId) – метод прямого удаления рейса (без команды);

– void addVehicleDirect(std::shared_ptr<Vehicle> vehicle) – метод прямого добавления транспортного средства (без команды);

– void removeVehicleDirect(const std::string& licensePlate) – метод прямого удаления транспортного средства (без команды);

– void addStopDirect(const Stop& stop) – метод прямого добавления остановки (без команды);

– void removeStopDirect(int stopId) – метод прямого удаления остановки (без команды);

– void addDriverDirect(std::shared_ptr<Driver> driver) – метод прямого добавления водителя (без команды);

– void removeDriverDirect(std::shared_ptr<Driver> driver) – метод прямого удаления водителя (без команды);

## 36. Класс JourneyPlanner

Данный класс предназначен для планирования поездок в системе управления общественным транспортом. Использует паттерн Facade для упрощения работы с алгоритмами поиска маршрутов. Управляет различными алгоритмами поиска пути.

Поля:

– TransportSystem* system – поле для хранения указателя на транспортную систему;

– std::unique_ptr<BFSAlgorithm> bfsAlgorithm – поле для хранения алгоритма поиска в ширину;

– std::unique_ptr<FastestPathAlgorithm> fastestAlgorithm – поле для хранения алгоритма поиска самого быстрого пути;

– std::unique_ptr<MinimalTransfersAlgorithm> minimalTransfersAlgorithm – поле для хранения алгоритма поиска с минимальными пересадками;

Методы:

– JourneyPlanner(TransportSystem* sys) – конструктор с параметрами;

– List<Journey> findJourneysWithTransfers(const std::string& startStop, const std::string& endStop, const Time& departureTime, int maxTransfers = 2) const – метод поиска поездок с пересадками;

– List<Journey> findAllJourneysWithTransfers(const std::string& startStop, const std::string& endStop, int maxTransfers = 2) const – метод поиска всех поездок с пересадками;

– Journey findFastestJourney(const std::string& startStop, const std::string& endStop, const Time& departureTime) – метод поиска самой быстрой поездки;

– Journey findJourneyWithLeastTransfers(const std::string& startStop, const std::string& endStop, const Time& departureTime) – метод поиска поездки с минимальными пересадками;

– void displayJourney(const Journey& journey) const – метод вывода информации о поездке;

## 37. Класс DriverSchedule

Данный класс предназначен для управления расписанием водителей в системе управления общественным транспортом. Отслеживает назначенные рейсы водителям, проверяет доступность водителей и соответствие рабочему времени.

Поля:

– std::unordered_map<std::shared_ptr<Driver>, List<std::shared_ptr<Trip>>> driverTrips – поле для хранения соответствия водителя и его рейсов;

– const int MAX_WORKING_HOURS – константа максимального рабочего времени (12 часов в минутах);

Методы:

– void assignTripToDriver(std::shared_ptr<Driver> driver, std::shared_ptr<Trip> trip) – метод назначения рейса водителю;

– void removeTripFromDriver(std::shared_ptr<Driver> driver, int tripId) – метод удаления рейса у водителя;

– bool isDriverAvailable(std::shared_ptr<Driver> driver, const Time& startTime, const Time& endTime) const – метод проверки доступности водителя;

– bool checkWorkingHoursCompliance(std::shared_ptr<Driver> driver) const – метод проверки соответствия рабочему времени;

– List<std::shared_ptr<Trip>> getDriverTrips(std::shared_ptr<Driver> driver) const – метод получения рейсов водителя;

– int getTotalWorkingMinutes(std::shared_ptr<Driver> driver) const – метод получения общего рабочего времени водителя;

## 38. Класс DataManager

Данный класс предназначен для управления сохранением и загрузкой данных системы управления общественным транспортом. Обеспечивает персистентность данных транспортной системы.

Поля:

– std::string dataDirectory – поле для хранения директории для хранения данных;

Методы:

– DataManager(const std::string& dir = "data/") – конструктор с параметрами;

– void saveAllData(TransportSystem& system) – метод сохранения всех данных;

– void loadAllData(TransportSystem& system) – метод загрузки всех данных;

Приватные методы:

– void saveStops(TransportSystem& system) – метод сохранения остановок;

– void saveVehicles(TransportSystem& system) – метод сохранения транспортных средств;

– void saveDrivers(TransportSystem& system) – метод сохранения водителей;

– void saveRoutes(TransportSystem& system) – метод сохранения маршрутов;

– void saveTrips(TransportSystem& system) – метод сохранения рейсов;

– void saveAdminCredentials(TransportSystem& system) – метод сохранения учетных данных администраторов;

– void loadStops(TransportSystem& system) – метод загрузки остановок;

– void loadVehicles(TransportSystem& system) – метод загрузки транспортных средств;

– void loadDrivers(TransportSystem& system) – метод загрузки водителей;

– void loadRoutes(TransportSystem& system) – метод загрузки маршрутов;

– void loadTrips(TransportSystem& system) – метод загрузки рейсов;

– void loadAdminCredentials(TransportSystem& system) – метод загрузки учетных данных администраторов;

– int loadVehiclesFromFile(std::ifstream& file, TransportSystem& system, const std::string& fileName) – вспомогательный метод загрузки транспортных средств из файла;

– int loadTripsFromFile(std::ifstream& file, TransportSystem& system, const std::string& fileName) – вспомогательный метод загрузки рейсов из файла;

## 39. Класс TransportException

Данный класс является базовым классом для всех исключений транспортной системы в системе управления общественным транспортом. Наследуется от std::exception.

Поля:

– std::string message – поле для хранения сообщения об ошибке;

Методы:

– explicit TransportException(const std::string& msg) – конструктор с параметрами;

– const char* what() const noexcept override – метод получения сообщения об ошибке;

– virtual ~TransportException() – виртуальный деструктор;

## 40. Класс InputException

Данный класс является наследником класса TransportException и представляет исключение для ошибок ввода данных в системе управления общественным транспортом.

Методы:

– explicit InputException(const std::string& msg) – конструктор с параметрами;

## 41. Класс FileException

Данный класс является наследником класса TransportException и представляет исключение для ошибок работы с файлами в системе управления общественным транспортом.

Методы:

– explicit FileException(const std::string& filename, const std::string& operation = "операция") – конструктор с именем файла и операцией;

– explicit FileException(const std::string& msg) – конструктор с сообщением;

## 42. Класс ContainerException

Данный класс является наследником класса TransportException и представляет исключение для ошибок работы с контейнерами в системе управления общественным транспортом.

Методы:

– explicit ContainerException(const std::string& msg) – конструктор с параметрами;

## 43. Класс List

Данный класс является самописным контейнером - двусвязным списком (аналог std::list) в системе управления общественным транспортом. Предоставляет функциональность для хранения и управления последовательностью элементов.

Поля:

– Node* head – поле для хранения указателя на головной узел списка;

– Node* tail – поле для хранения указателя на хвостовой узел списка;

– size_t size_ – поле для хранения размера списка;

Внутренние классы:

– struct Node – структура узла двусвязного списка с полями T data, Node* prev, Node* next;

– class Iterator – класс итератора для обхода элементов списка (bidirectional iterator);

Поля класса Iterator:

– Node* node – поле для хранения указателя на текущий узел;

Типы класса Iterator (для совместимости со STL):

– using iterator_category = std::bidirectional_iterator_tag – категория итератора;

– using value_type = T – тип значения;

– using difference_type = std::ptrdiff_t – тип разности;

– using pointer = T* – тип указателя;

– using reference = T& – тип ссылки;

Методы класса Iterator:

– Iterator(Node* n) – конструктор с параметрами;

– T& operator*() – оператор разыменования (неконстантная версия);

– const T& operator*() const – оператор разыменования (константная версия);

– T* operator->() – оператор доступа к членам (неконстантная версия);

– const T* operator->() const – оператор доступа к членам (константная версия);

– Iterator& operator++() – оператор префиксного инкремента;

– Iterator operator++(int) – оператор постфиксного инкремента;

– Iterator& operator--() – оператор префиксного декремента;

– Iterator operator--(int) – оператор постфиксного декремента;

– bool operator==(const Iterator& other) const – оператор сравнения на равенство;

– bool operator!=(const Iterator& other) const – оператор сравнения на неравенство;

– Node* getNode() const – метод получения указателя на узел (используется в erase);

Методы:

– List() – конструктор по умолчанию;

– List(const List& other) – конструктор копирования;

– template<typename InputIt> List(InputIt first, InputIt last) – конструктор из диапазона итераторов;

– List& operator=(const List& other) – оператор присваивания;

– ~List() – деструктор;

– void push_back(const T& value) – метод добавления элемента в конец списка (копирование);

– void push_back(T&& value) – метод добавления элемента в конец списка (перемещение);

– template<typename... Args> void emplace_back(Args&&... args) – метод создания элемента на месте в конце списка;

– Iterator erase(const Iterator& it) – метод удаления элемента по итератору;

– void erase(const Iterator& first, const Iterator& last) – метод удаления диапазона элементов;

– T& operator[](size_t index) – оператор доступа по индексу (неконстантная версия);

– const T& operator[](size_t index) const – оператор доступа по индексу (константная версия);

– T& front() – метод получения первого элемента (неконстантная версия);

– const T& front() const – метод получения первого элемента (константная версия);

– T& back() – метод получения последнего элемента (неконстантная версия);

– const T& back() const – метод получения последнего элемента (константная версия);

– size_t size() const – метод получения размера списка;

– bool empty() const – метод проверки пустоты списка;

– void clear() – метод очистки списка;

– template<typename Compare> void sort(Compare comp) – метод сортировки списка;

– void reverse() – метод обращения порядка элементов;

– Iterator begin() – метод получения итератора на начало списка (неконстантная версия);

– Iterator end() – метод получения итератора на конец списка (неконстантная версия);

– Iterator begin() const – метод получения итератора на начало списка (константная версия);

– Iterator end() const – метод получения итератора на конец списка (константная версия);

Приватные методы:

– void clearNodes() – метод очистки узлов списка;

– Node* getNodeAt(size_t index) const – метод получения узла по индексу;
