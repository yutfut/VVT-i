<div  align="left">

## 🔑 VVT-i
<h4>Linux-user-friendly console utility that provides terminal access to remote file storage with different user modes</h4>
<br>  

## Code analysis ⚙️📈📈

[![Build Status](https://app.travis-ci.com/frontend-park-mail-ru/2021_2_MonKeys.svg?branch=development)](https://app.travis-ci.com/frontend-park-mail-ru/2021_2_MonKeys)
![GitHub commit activity (branch)](https://img.shields.io/github/commit-activity/m/yutfut/VVT-i/dev)
[![code style: cppcheck](https://img.shields.io/badge/code_style-cppcheck-ff69b4.svg?style=flat-square)](https://github.com/danmar/cppcheck)
<br>
![CI](https://github.com/frontend-park-mail-ru/2021_2_MonKeys/workflows/Build/badge.svg)
![CI](https://github.com/frontend-park-mail-ru/2021_2_MonKeys/workflows/Linter/Formatter/badge.svg)
  <br>
<br>

  
## 📝 Content table
  
-  [About project](#about)
  
-  [Library installing](#library_installing)

-  [Launching the application](#getting_started)

-  [Usage in real life](#usage)

-  [Libraries & technologies](#lib_technologies)

-  [Languages](#languages)

-  [Developers](#developers)

-  [Mentors & tutors](#mentors_tutors)

-  [Sources for project presentation](#presentation_src)
<br>
  

  
## 🧐 About project<a name = "about"></a>

  В последнее время наш вуз активно переходит на семейство ОС Linux, однако распространенных решений для удаленного хранения файлов для них не существует. Помимо этого, многие существующие решения требуют обязательную регистрацию, что является проблемой для пользователей, не имеющих с собой паролей. Также возможна утечка личной информации в случае, если пользователь забывает выйти из аккаунта.
  
  Решением этой проблемы является создание продукта, который будет максимально простым в использовании и предусматривать работу с неавторизованными пользователями.
  
Наш проект удовлетворяет этим требованиям. Он имеет Linux-friendly синтаксис команд, что также упрощает пользование продуктом. Помимо этого, для удобства пользования используется отправка уведомлений на почту, указанную пользователем. Это помогает ему не хранить код извне, а получить его в удобной пользователю момент через почту.
  <br>
<br>

  

## 📚 Libraries installing <a name = "library_installing"></a>
  
- Установка PostgreSQL & LibPQXX:
```
  sudo apt install postgresql postgresql-contrib
  
  sudo apt install libpqxx-dev
```
  
- Установка fmt:
```
  sudo add-apt-repository universe
  
  sudo apt update
  
  sudo apt install libfmt-dev
```

- Установка Boost:
```
  sudo apt-get install libboost-all-dev
```
  
- Установка Mailio:
```
  git clone git://github.com/karastojko/mailio

  cd mailio

  mkdir build

  cd build

  cmake ..

  sudo make install
```

- Установка OpenSSL & JWT:
```
  sudo apt install nlohmann-json3-dev 
  
  sudo apt install libgtest-dev
  
  sudo apt install libssl-dev
  
  mkdir build
  
  cd build
  
  cmake ..
  
  cmake --build . -j
  
  sudo make install
```
<br>
  
  
  
## 🏁 Launching the application <a name = "getting_started"></a>

Пошаговая инструкция, как запустить приложение у себя
  
  Введите команду для клонирования репозитория приложения:
  ```
  git clone git@github.com:yutfut/VVT-i.git
  ```
  Через терминал зайдите в директорию сервера и выполните следующие команды:
  ```
  sudo ./server.sh build
  
  sudo ./server.sh create --config
  ```
  
  Теперь сервер готов к работе, остается только заполнить конфигурационный файл, 
    указав в нем количество процессов-воркеров, пути для логов, порт и адрес сервера, 
    а также заполнить блочную директиву database, и простую fs_root с путем до корня файловой системы.
 
  Запуск сервера:
  ```
  sudo ./server.sh start
  ```
  Запуск клиента:
  ```
  ./vvti_client
  ```
  Для получения информации о командах введите в терминале:
  ```
  help
  ```
<br>
  
  

## 🌟 Usage in real life <a name="usage"></a>
  
  Используя наш сервис, можно не заботиться о наличии с собой переносного носителя или данных своего аккаунта, к тому же сервис исключает утечку личных данных в случае если пользователь выходит, забывая выйти из своего аккаунта. 
  
  Пользователь может с любого компьютера, на котором есть 5-7Мб свободного места, отправить и получить свои файлы максимально быстро и безопасно без лишних действий. Для работы с клиентским приложением достаточно установить пару легковесных библиотек и более ни о чем не заботиться, сервис все сделает сам. 
  <br>
<br>
  

  
## ⛏️ Libraries & technologies<a name = "lib_technologies"></a>

[PostgreSQL](https://www.postgresql.org/) - Database
  
[LibPQXX](http://pqxx.org/development/libpqxx/) - The C++ connector for PostgreSQL
  
[Boost](https://www.boost.org/) - Multiprocessing Logging & gzip file compression
  
[Epoll](https://ru.wikipedia.org/wiki/Epoll) - Asynchronous Multiplex Technology
  
[JWT](https://jwt.io/) - JWT Authorization Technology
  
[Mailio](https://github.com/karastojko/mailio) - Send Emails Module

[LIBMAGIC](https://man7.org/linux/man-pages/man3/libmagic.3.html) - Getting file type for compression
  <br>
<br>
  
  
  
##  Languages <a name = "languages"></a>
  
![C++](https://img.shields.io/badge/-C++-090909?style=for-the-badge&logo=C%2b%2b&logoColor=6296CC)
![SQL](https://img.shields.io/badge/-SQL-090909?style=for-the-badge&logo=postgresql&logoColor=6296CC)
  <br>
<br>

  
## ✍️ Developers <a name = "developers"></a>
  
[@And_Dayk](https://github.com/Andrey123815) - Дьяконов Андрей [Team Lead, Database development]
  
[@kirill555101](https://github.com/kirill555101) - Каташинский Кирилл [Server development]
  
[@yut_fut](https://github.com/yutfut) - Голубев Сергей [Team Lead, Client development]
  
[@skvortsova_na](https://github.com/Natali-Skv) - Скворцова Наталья [Filesystem development & Integration Db and FS to server]
  <br>
<br>

 
## 🙇 Mentors & tutors <a name = "mentors_tutors"></a>

Наш ментор: 
  
[@znjuko](https://github.com/znjuko) - Никита Черных

Преподаватели:

[@Ciberst](https://github.com/ciberst) - Александр Жиров
  
  И вся команда Технопарк VK!
  <br>
<br>

  
## 🔆 Sources for project presentation <a name = "presentation_src"></a>

Ссылка на презентацию: [Презентация](https://docs.google.com/presentation/d/1GjSVECv2N7I9ksSQ0BhA4O3BCKgyCcM_lY23q1cRnEg/edit#slide=id.p)
