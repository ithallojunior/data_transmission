import numpy as np
import matplotlib.pyplot as plt


def main():
    data = np.loadtxt( "data_2019-05-27_16-49-30.txt" )
    t = data[:, 0] 
    y = data[:, 1] - data[:,1].mean()

    plt.figure(figsize = (14,8))
    plt.ylim([-0.6, 0.6]) # to get it just like the previous
    plt.xlim([0., 4.])
    plt.plot(t, y, c="b", linewidth=2)
    
    #plt.rc('font', size=24)
    #plt.rc('axes', titlesize=24)

    plt.title("sEMG signal", size=20)
    plt.xlabel("Time (s)", size=20)
    plt.ylabel("Amplitude (V)", size=20)
    plt.grid()
    plt.savefig("plot.png")
    plt.close()



if (__name__ == "__main__"):
    main()
