

import fileinput

print("max_temp, min_temp, max_humidity, min_humidity, mean_pressure, mean_visibility, wind_speed, precipitation, is_hailing, is_raining, is_thunderstorm, is_fog")

mapping = { }
mapping['hail'] = 0
mapping['rain'] = 1
mapping['thunderstorm'] = 2
mapping['fog'] = 3

for line in fileinput.input():

  line = line[:-1] # strip newline character
  if (line == ''): continue
  line = list(line.split(','))

  # (0) CET
  # (1) Max TemperatureC
  # (2) Mean TemperatureC
  # (3) Min TemperatureC
  # (4) Dew PointC
  # (5) MeanDew PointC
  # (6) Min DewpointC
  # (7) Max Humidity
  # (8) Mean Humidity
  # (9) Min Humidity
  # (10) Max Sea Level PressurehPa
  # (11) Mean Sea Level PressurehPa
  # (12) Min Sea Level PressurehPa
  # (13) Max VisibilityKm
  # (14) Mean VisibilityKm
  # (15) Min VisibilitykM
  # (16) Max Wind SpeedKm/h
  # (17) Mean Wind SpeedKm/h
  # (18) Max Gust SpeedKm/h
  # (19) Precipitationmm
  # (20) CloudCover
  # (21) Events
  # (22) WindDirDegrees

  # I am interested in features (1), (3), (7), (9), (11), (14), (17), (19) ,(21)

  out = ""

  out += line[1] + ", "
  out += line[3] + ", "
  out += line[7] + ", "
  out += line[9] + ", "
  out += line[11] + ", "
  out += line[14] + ", "
  out += line[17] + ", "
  out += line[19] + ", "

  weather_condition = line[21]
  if (weather_condition != ''):

    labels = ['0','0','0','0']

    weather_types = list(map(lambda x:x.lower(), weather_condition.split("-")))

    for weather in weather_types:

      weather = weather.lower()

      # skip the row with the tornado outlier
      if weather in mapping:

        index = mapping[weather]
        labels[index] = '1'

    out += ','.join(labels)

    print(out)

