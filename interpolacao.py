import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt
NTC_10K_res = np.array([1214600, 844390, 592430, 419380, 299480,
                        215670, 156410, 114660, 84510, 62927,
                        47077, 35563, 27119, 20860, 16204,
                        12683, 10000, 7942, 6327, 5074,
                        4103, 3336, 2724, 2237, 1846,
                        1530, 1275, 1068, 899.3, 760.7,
                        645.2, 549.4, 470.0, 403.6, 347.4,
                        300.1, 260.1],
                       dtype=np.float64)
NTC_10K_temp = np.array([-55, -50, -45, -40, -35,
                         -30, -25, -20, -15, -10,
                         -5, 0, 5, 10, 15,
                         20, 25, 30, 35, 40,
                         45, 50, 55, 60, 65,
                         70, 75, 80, 85, 90,
                         95, 100, 105, 110, 115,
                         120, 125],
                        dtype=np.float64)
def funcao (x, a, b, c, d, e):
    return b* np.log(x - a)**3 + c * np.log(x - a)**2 + d * np.log(x - a) + e
popt, pcov = curve_fit(funcao, NTC_10K_res, NTC_10K_temp) #popt = lista de abc
# y é oq eu quero achar e x oq eu tenho
#                               x               y
x = np.linspace(260.1, 1214600, 1000)
erro_max = np.max(np.absolute(funcao(NTC_10K_res, *popt) - NTC_10K_temp))
erro_med = np.sqrt(np.sum((funcao(NTC_10K_res, *popt) - NTC_10K_temp)**2) / 37)#(tamanho de NTC_10K_temp)
print(f'Regression coefficients: {popt}')
print(f'Max error: {erro_max:.3f}')
print(f'Quadratic mean error: {erro_med:.3f}')
plt.plot(NTC_10K_res, NTC_10K_temp)
plt.plot(x, funcao(x, *popt))
plt.grid()
plt.xlabel("Resistence")
plt.ylabel("Temperature")
plt.xlim([1,1250000])
plt.ylim([-60, 150])
plt.tight_layout()
plt.show()
