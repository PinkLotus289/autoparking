<h1 style="font-family: 'Academy Engraved LET', sans-serif; font-size: 2.5em; font-weight: bold;">Parking System</h1>

<p style="font-size: 1.2em;">Проект представляет собой систему управления парковкой, позволяющую добавлять и управлять автомобилями и парковочными местами. Приложение разработано на C++ и поддерживает функционал взаимодействия с пользователями через консольное меню.</p>

<h2 style="font-family: 'Academy Engraved LET', sans-serif; font-size: 2em; font-weight: bold;">Функциональные возможности</h2>

<ul style="font-size: 1.1em;">
  <li><strong>Отображение информации о парковке:</strong>
    <ul>
      <li>Вывод информации о всех автомобилях и парковочных местах.</li>
      <li>Раздельный доступ как для обычных пользователей, так и для администраторов.</li>
    </ul>
  </li>
  <li><strong>Управление автомобилями:</strong>
    <ul>
      <li><strong>Добавление автомобиля:</strong> Возможность добавлять новые автомобили с указанием модели и номерного знака через консольное меню.</li>
      <li><strong>Удаление автомобиля:</strong> Удаление автомобиля по номерному знаку.</li>
      <li><strong>Отслеживание статуса:</strong> Учет статуса автомобиля (запаркован или не запаркован).</li>
    </ul>
  </li>
  <li><strong>Управление парковочными местами:</strong>
    <ul>
      <li><strong>Добавление места:</strong> Возможность добавлять новые парковочные места с указанием номера и размера.</li>
      <li><strong>Назначение автомобиля на место:</strong> Привязка автомобиля к конкретному парковочному месту.</li>
      <li><strong>Удаление парковочного места:</strong> Удаление места по его номеру.</li>
      <li><strong>Освобождение места:</strong> Освобождение места при выезде автомобиля.</li>
    </ul>
  </li>
</ul>

<h2 style="font-family: 'Academy Engraved LET', sans-serif; font-size: 2em; font-weight: bold;">Структура проекта</h2>

<p style="font-size: 1.2em;">Проект разделен на следующие файлы:</p>

<ul style="font-size: 1.1em;">
  <li><strong>Car.h и Car.cpp:</strong> Класс <code>Car</code> управляет информацией о каждом автомобиле. Содержит данные о модели, номерном знаке и статусе парковки.</li>
  <li><strong>ParkingSpot.h и ParkingSpot.cpp:</strong> Класс <code>ParkingSpot</code> управляет парковочными местами. Включает информацию о номере места, его размере, занятости и назначенном автомобиле.</li>
  <li><strong>ParkingLot.h и ParkingLot.cpp:</strong> Класс <code>ParkingLot</code> управляет добавлением, удалением и назначением автомобилей на парковочные места. Реализует функции для отображения информации и управления парковкой.</li>
  <li><strong>main.cpp:</strong> Основной файл программы, который включает в себя консольное меню для взаимодействия с пользователем. Пользователь может добавлять, удалять и назначать автомобили, а также просматривать текущее состояние парковки.</li>
</ul>

<h2 style="font-family: 'Academy Engraved LET', sans-serif; font-size: 2em; font-weight: bold;">Как использовать</h2>

<ol style="font-size: 1.1em;">
  <li>Скомпилируйте проект с помощью компилятора C++ (например, <code>g++</code>, <code>clang</code>).</li>
  <li>Запустите полученный исполняемый файл.</li>
  <li>Следуйте инструкциям в консоли для взаимодействия с системой:
    <ul>
      <li>Выбирайте действия через числовое меню.</li>
      <li>Для добавления автомобиля введите его модель и номерной знак.</li>
      <li>Для назначения автомобиля на парковочное место выберите автомобиль и номер места.</li>
      <li>Используйте меню для удаления автомобиля или освобождения парковочного места.</li>
    </ul>
  </li>
</ol>

<h2 style="font-family: 'Academy Engraved LET', sans-serif; font-size: 2em; font-weight: bold;">Требования</h2>

<ul style="font-size: 1.1em;">
  <li><strong>C++20</strong> или новее.</li>
</ul>

<h2 style="font-family: 'Academy Engraved LET', sans-serif; font-size: 2em; font-weight: bold;">Возможности для расширения</h2>

<p style="font-size: 1.2em;">В дальнейшем можно добавить:</p>

<ul style="font-size: 1.1em;">
  <li>Возможность управления парковкой через веб-интерфейс или мобильное приложение.</li>
  <li>Поддержку базы данных для хранения информации об автомобилях и парковочных местах.</li>
  <li>Интеграцию с системой оплаты для управления платной парковкой.</li>
</ul>

<h2 style="font-family: 'Academy Engraved LET', sans-serif; font-size: 2em; font-weight: bold;">Ссылка на SonarCloud</h2>

<p style="font-size: 1.2em;"><a href="https://sonarcloud.io/project/overview?id=PinkLotus289_autoparking">SonarCloud Overview</a></p>
