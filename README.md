<h1 style="font-family: 'Academy Engraved LET', sans-serif; font-size: 2.5em; font-weight: bold;">Parking Management System</h1>

<p style="font-size: 1.2em;">Проект представляет собой систему управления парковкой, включающую работу с транспортными средствами и парковочными местами. Приложение разработано на C++ с использованием Qt и поддерживает интеграцию с базой данных SQLite.</p>

<h2 style="font-family: 'Academy Engraved LET', sans-serif; font-size: 2em; font-weight: bold;">Функциональные возможности</h2>

<ul style="font-size: 1.1em;">
  <li><strong>Информация о парковке:</strong>
    <ul>
      <li>Просмотр статуса парковочных мест и транспортных средств.</li>
      <li>Доступ для обычных пользователей и администраторов с разным уровнем привилегий.</li>
    </ul>
  </li>
  <li><strong>Управление транспортными средствами:</strong>
    <ul>
      <li><strong>Добавление:</strong> Ввод модели и номера транспорта с проверкой уникальности.</li>
      <li><strong>Удаление:</strong> Удаление автомобиля и освобождение связанного места.</li>
      <li><strong>Назначение:</strong> Привязка транспортного средства к парковочному месту.</li>
    </ul>
  </li>
  <li><strong>Управление парковочными местами:</strong>
    <ul>
      <li><strong>Добавление:</strong> Создание мест с уникальными номерами.</li>
      <li><strong>Удаление:</strong> Удаление парковочных мест по номеру.</li>
      <li><strong>Освобождение:</strong> Снятие привязки транспорта от места.</li>
    </ul>
  </li>
  <li><strong>Экспорт данных:</strong> Сохранение информации о парковке в файлы формата JSON.</li>
  <li><strong>Очередь транспортных средств:</strong> Управление очередью легковых и грузовых автомобилей для парковки.</li>
</ul>

<h2 style="font-family: 'Academy Engraved LET', sans-serif; font-size: 2em; font-weight: bold;">Сценарии использования</h2>

<p style="font-size: 1.2em;">Программа может быть полезна в различных сценариях:</p>
<ul style="font-size: 1.1em;">
  <li>Управление парковкой офисного здания с возможностью быстрого назначения мест.</li>
  <li>Организация парковки на временных мероприятиях, таких как выставки или концерты.</li>
  <li>Автоматизация учета в платной парковке с расчетом стоимости времени стоянки.</li>
</ul>

<h2 style="font-family: 'Academy Engraved LET', sans-serif; font-size: 2em; font-weight: bold;">Техническая архитектура</h2>

<ul style="font-size: 1.1em;">
  <li><strong>Классы:</strong>
    <ul>
      <li><strong>Car и Truck:</strong> Представляют легковые и грузовые автомобили.</li>
      <li><strong>ParkingSpot:</strong> Управляет данными о парковочных местах, включая их размер, статус и привязанный транспорт.</li>
      <li><strong>ParkingLot:</strong> Центральный компонент, управляющий логикой парковки и взаимодействием с базой данных.</li>
      <li><strong>ExportManager:</strong> Обеспечивает экспорт данных о парковке в JSON-формат.</li>
    </ul>
  </li>
  <li><strong>Интерфейс:</strong> Реализован с использованием Qt, включает окна входа и основного управления.</li>
  <li><strong>База данных:</strong> SQLite используется для хранения данных о транспортных средствах и парковочных местах.</li>
</ul>

<h2 style="font-family: 'Academy Engraved LET', sans-serif; font-size: 2em; font-weight: bold;">Примеры команд</h2>

<ol style="font-size: 1.1em;">
  <li><strong>Добавление автомобиля:</strong> Введите модель и номер через интерфейс, система проверит уникальность данных.</li>
  <li><strong>Назначение места:</strong> Выберите парковочное место из доступных, система обновит статус места и транспорта.</li>
  <li><strong>Экспорт данных:</strong> Нажмите кнопку "Сохранить отчет" для создания JSON-файла с текущим состоянием парковки.</li>
</ol>

<h2 style="font-family: 'Academy Engraved LET', sans-serif; font-size: 2em; font-weight: bold;">Тестирование</h2>

<p style="font-size: 1.2em;">Система протестирована в следующих аспектах:</p>
<ul style="font-size: 1.1em;">
  <li>Добавление и удаление автомобилей и парковочных мест.</li>
  <li>Назначение и освобождение парковочных мест.</li>
  <li>Экспорт данных и интеграция с SQLite.</li>
</ul>

<h2 style="font-family: 'Academy Engraved LET', sans-serif; font-size: 2em; font-weight: bold;">Известные ограничения</h2>

<ul style="font-size: 1.1em;">
  <li>Отсутствие поддержки других баз данных кроме SQLite.</li>
  <li>Нет мультиформатного экспорта, кроме JSON.</li>
  <li>Ограничение на единственный интерфейс (только десктопное приложение).</li>
</ul>

<h2 style="font-family: 'Academy Engraved LET', sans-serif; font-size: 2em; font-weight: bold;">Логирование и обработка ошибок</h2>

<ul style="font-size: 1.1em;">
  <li>Вывод ошибок базы данных в консоль.</li>
  <li>Проверка уникальности данных при добавлении автомобилей и мест.</li>
  <li>Отображение сообщений об ошибках пользователю через всплывающие окна.</li>
</ul>

<h2 style="font-family: 'Academy Engraved LET', sans-serif; font-size: 2em; font-weight: bold;">Как использовать</h2>

<ol style="font-size: 1.1em;">
  <li>Скомпилируйте проект с использованием Qt (например, Qt Creator).</li>
  <li>Запустите приложение и выберите тип входа (пользователь или администратор).</li>
  <li>Следуйте инструкциям в интерфейсе для управления парковкой.</li>
</ol>

<h2 style="font-family: 'Academy Engraved LET', sans-serif; font-size: 2em; font-weight: bold;">Требования</h2>

<ul style="font-size: 1.1em;">
  <li><strong>Qt 6.0</strong> или выше.</li>
  <li><strong>C++20</strong>.</li>
  <li><strong>SQLite</strong> для работы с базой данных.</li>
</ul>

<h2 style="font-family: 'Academy Engraved LET', sans-serif; font-size: 2em; font-weight: bold;">Перспективы развития</h2>

<ul style="font-size: 1.1em;">
  <li>Добавление веб-интерфейса или мобильного приложения.</li>
  <li>Интеграция системы оплаты за парковку.</li>
  <li>Поддержка других форматов экспорта (Excel, XML).</li>
  <li>Добавление аналитических функций, таких как отчеты по загруженности парковки.</li>
</ul>
