import matplotlib.pyplot as plt

buffer_size =[4, 10, 20, 200, 254]
total_time = [20.789, 10.572, 6.634, 4.562, 4.557]
avg = [0.000693, 0.000352, 0.000221, 0.000152, 0.000152]

plt.subplot(211)
plt.plot(buffer_size, total_time)
plt.xlabel("buffer size")
plt.ylabel("total time")

plt.subplot(212)
plt.plot(buffer_size, total_time)
plt.xlabel("buffer size")
plt.ylabel("avg")

plt.show()

