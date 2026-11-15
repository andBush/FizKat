# ФизКат

Консольный учёт парка кикшеринга на C++20. Программа принимает команды со стандартного ввода и хранит парк в памяти на время сеанса.

## Быстрая проверка

На Windows x64 можно скачать `FizKat-win-x64.zip` из раздела **Releases**, если он приложен к релизу. Распакуйте архив и откройте PowerShell в полученной папке. Для ручного ввода запустите `.\FizKat.exe`. Для прогона примера выполните:

```powershell
Get-Content .\commands.txt | .\FizKat.exe
```

Сравните результат с `expected.txt` из архива. Для запуска этой сборки Visual Studio не нужна.

## Сборка из исходников

Нужны CMake 3.16 или новее и компилятор с поддержкой C++20. Скачайте код через **Code → Download ZIP** или клонируйте репозиторий. Команды ниже выполняются из папки с этим README.

**Windows с Visual Studio.** Откройте [ConsoleApplication1.slnx](ConsoleApplication1.slnx), выберите `Release | x64` и нажмите `Ctrl+F5`. Для этого решения нужен набор инструментов MSVC `v145` и установленная рабочая нагрузка «Разработка классических приложений на C++».

**Windows с CMake и генератором Visual Studio.**

```powershell
cmake -S ConsoleApplication1 -B build
cmake --build build --config Release
Get-Content .\demo\commands.txt | .\build\Release\FizKat.exe
```

Если CMake использует одноконфигурационный генератор, например Ninja, путь к программе будет `.\build\FizKat.exe`.

**Linux и macOS с CMake.**

```sh
cmake -S ConsoleApplication1 -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/FizKat < demo/commands.txt
```

## Что проверяет сценарий

Готовый ввод — [demo/commands.txt](demo/commands.txt), полный ожидаемый вывод — [demo/expected.txt](demo/expected.txt). Для ручной проверки начните с пустого парка.

| Проверка | Ожидаемое поведение |
| --- | --- |
| Техосмотр и ошибки | `BrOkEnX`, повторный активный ID и велосипед с оценкой `11` отклоняются. Программа читает следующую команду |
| `list` и `novice` | Все пять принятых позиций идут по ID. Для новичков подходят только `S-1` с оценкой `8` и `E-1` с оценкой ровно `6` |
| `energy` и `count` | Отчёты выводят `14.0` кВт·ч и `5` позиций |
| Витрина по типам | `show_focus scooter` показывает самокат, `show_focus all` — шлем и самокат в порядке ID |
| Удаление и повторный ID | После удаления `H-1 Pro` и добавления `H-1 Giro` фокус шлемов выводит `expired`. Новый шлем появляется в витрине после нового `focus H-1` |

## Команды

| Команда | Действие |
| --- | --- |
| `add scooter id name energy score` | Добавить электросамокат |
| `add bike id name score` | Добавить велосипед |
| `add ebike id name energy score` | Добавить электровелосипед |
| `add helmet id name` | Добавить шлем |
| `add dock id name energy` | Добавить док-станцию |
| `list` | Вывести все позиции по возрастанию ID |
| `novice` | Показать технику с оценкой от 6 в порядке добавления |
| `energy` | Вывести общий суточный расход с одним знаком после точки |
| `count` | Вывести число позиций на балансе |
| `focus id` | Запомнить выбор для типа предмета и как последний выбор по парку |
| `show_focus` | Показать последний выбор или `expired` |
| `show_focus type` | Показать выбор для `scooter`, `bike`, `ebike`, `helmet` или `dock` |
| `show_focus all` | Показать все действующие позиции в фокусе по ID |
| `remove id` | Удалить позицию из парка |
| `quit` | Выйти. Остаток строки игнорируется |

ID и имя — по одному слову. Расход — конечное неотрицательное число с десятичной точкой, оценка — целое число от 1 до 10. У активных позиций ID уникальны. Правило техосмотра применяется только к транспорту и ищет `broken` в имени без учёта регистра.

## Устройство проекта

```text
InventoryItem
├── Transport
│   ├── Bike
│   ├── Scooter  (также EnergyConsumer)
│   └── EBike    (также EnergyConsumer)
├── Helmet
└── Dock         (также EnergyConsumer)
```

| Что искать | Где это реализовано |
| --- | --- |
| Абстрактные базисы, множественное наследование и виртуальные вызовы | [Inventory.hpp](ConsoleApplication1/Inventory.hpp), [Transport.hpp](ConsoleApplication1/Transport.hpp), [Things.hpp](ConsoleApplication1/Things.hpp) |
| Владение через `unique_ptr` и витрина через `weak_ptr` | [Fleet.hpp](ConsoleApplication1/Fleet.hpp), [Fleet.cpp](ConsoleApplication1/Fleet.cpp) |
| Общая спецификация модели через `shared_ptr<const ModelSpec>` | [Transport.cpp](ConsoleApplication1/Transport.cpp) |
| Шаблонный реестр `Registry<Entry>` для парка и `Registry<const InventoryItem*>` для сортируемого представления | [Registry.hpp](ConsoleApplication1/Registry.hpp), [Fleet.cpp](ConsoleApplication1/Fleet.cpp) |
| `operator<` для сортировки, `operator<<` для вывода и `operator+=` для суммы расходов | [Inventory.cpp](ConsoleApplication1/Inventory.cpp), [Fleet.cpp](ConsoleApplication1/Fleet.cpp) |
| `InspectionError` при отказе техосмотра, `CommandError` при ошибке команды, единый `catch` в цикле ввода | [Errors.hpp](ConsoleApplication1/Errors.hpp), [Commands.cpp](ConsoleApplication1/Commands.cpp), [main.cpp](ConsoleApplication1/main.cpp) |

Креативное расширение — витрина по типам. Парк сам владеет предметами, а витрина хранит по одной слабой ссылке на каждый тип. Выбор нового предмета меняет фокус только для этого типа. `show_focus all` пропускает удалённые позиции и выводит `expired`, если показывать больше нечего.

## Использование ИИ

Я советовался с ИИ по архитектуре и техническим вопросам: какие классы выделить, в каком порядке их реализовывать и как писать отдельные части на C++. С его помощью разбирался, как работают `virtual` и `override`, в специфике работы ссылок и указателей в proction коде, как `unique_ptr`, `shared_ptr` и `weak_ptr` управляют временем жизни объектов, и как применять шаблоны и лямбды.

Также я старался внедрить поэтапную разработку проекта, что легче его дебажить. Считаю, что с этим я справился с этим на отлично, ведь я все равно ловил ошибки линковки и компиляции, хотя мог быстро найти причину поломки и вернуться к продолжению работы. ИИ также помог мне написать cmake файл и оформить readme, чтобы его было приятнее читать.