import datetime
import json
import requests
import matplotlib.pyplot as plt
import numpy as np
import matplotlib as mpl


def get_request(url):
    req = requests.get(url)

    if req.status_code != 200:
        raise Exception('Erro de autenticação')

    return req.content


url = "https://iiot-dta-default-rtdb.firebaseio.com/challenge02.json"


def update_data():
    dados = json.loads(get_request(url))
    dados_len = len(dados)
    indices = np.array([int(x[-2:]) for x in dados.keys()])

    humidity = np.full(dados_len, np.nan, dtype=np.float64)
    temperature = np.full(dados_len, np.nan, dtype=np.float64)

    for i, j in zip(range(dados_len),indices):
        try:
            humidity[i] = dados[f"subsys_{j:02}"]["humidity"]
            temperature[i] = dados[f"subsys_{j:02}"]["temperature"]
        except KeyError:
            pass

    humidity_mean = np.mean(humidity[~np.isnan(humidity)])
    temperature_mean = np.mean(temperature[~np.isnan(temperature)])
    
    humidity_min = np.min(humidity[~np.isnan(humidity)])
    temperature_min = np.min(temperature[~np.isnan(temperature)])
    
    humidity_max = np.max(humidity[~np.isnan(humidity)])
    temperature_max = np.max(temperature[~np.isnan(temperature)])

    return humidity_mean, temperature_mean, temperature_min, humidity_min, temperature_max, humidity_max

fig, axs = plt.subplots(2, sharex=True, figsize=(16, 8), gridspec_kw={"hspace":0.4}) #retorna o grafico e os eixos
fig.supxlabel("Tempo")
ax_humidity, ax_temperature = axs

cmap = mpl.colormaps["coolwarm"]

ax_humidity.grid(True)
ax_humidity.set_ylabel("Umidade")

ax_temperature.grid(True)
ax_temperature.set_ylabel("Temperatura")


plt.ion()
fig.show()
fig.canvas.draw()

while True:
    humidity_mean, temperature_mean, temperature_min, humidity_min, temperature_max, humidity_max = update_data()
    current_time = datetime.datetime.now()

    temp_color = (temperature_mean - 30)/(35-25)

    ax_humidity.plot(current_time, humidity_max, linestyle='', marker='o', markersize=5, color='r')
    ax_humidity.plot(current_time, humidity_mean, linestyle='', marker='o', markersize=5, color='b')
    ax_humidity.plot(current_time, humidity_min, linestyle='', marker='o', markersize=5, color='c')
    ax_temperature.plot(current_time, temperature_max, linestyle='', marker='o', markersize=5, color="r")
    ax_temperature.plot(current_time, temperature_mean, linestyle='', marker='o', markersize=5, color="b")
    ax_temperature.plot(current_time, temperature_min, linestyle='', marker='o', markersize=5, color="c")

    fig.canvas.draw()
    plt.pause(3)
