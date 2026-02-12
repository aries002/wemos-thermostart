#include <Arduino.h>

static const char* index_html ICACHE_FLASH_ATTR = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Thermostat Control</title>
</head>
<body>
    <h1>Thermostat Control</h1>
    <h2>Temperature : %TEMPERATUR%[%SET_TEMPERATURE%] &deg;C</h2>
    <h2>Hummidity   : %KELEMBAPAN%</h2>
    <form method="post">
        <label for="temp">Temperature:</label>
        <input type="number" id="temp" name="temp" value="%SET_TEMPERATURE%"><br><br>
        <label for="fan">Fan Control:</label>
        <select id="fan" name="fan">
            <option value="off" %KIPAS_OFF%>Off</option>
            <option value="on" %KIPAS_ON%>On</option>
        </select><br><br>
        <input type="submit" value="Submit">
    </form>
</body>
</html>
)rawliteral";

