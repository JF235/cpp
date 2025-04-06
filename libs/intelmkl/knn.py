import numpy as np
import matplotlib.pyplot as plt
import time
import faiss
from tqdm import tqdm
from sklearn.linear_model import LinearRegression

def format_count(count):
    if count < 1e3:
        return f"{count:.0f}"
    elif count < 1e6:
        return f"{count / 1e3:.2f} K"
    elif count < 1e9:
        return f"{count / 1e6:.2f} M"
    else:
        return f"{count / 1e9:.2f} B"

def format_time(seconds):
    if seconds < 1e-6:
        return f"{seconds * 1e9:.2f} ns"
    elif seconds < 1e-3:
        return f"{seconds * 1e6:.2f} μs"
    elif seconds < 1:
        return f"{seconds * 1e3:.2f} ms"
    elif seconds < 60:
        return f"{seconds:.2f} s"
    elif seconds < 3600:
        return f"{seconds / 60:.2f} min"
    else:
        return f"{seconds / 3600:.2f} h"

def index_faiss(gallery, queries, k):
    index = faiss.IndexFlatL2(gallery.shape[1])
    index.add(gallery)
    D, I = index.search(queries, k)

    return I, D


def index_np(gallery, queries, k):
    dist_sq = np.sum(queries**2, axis=1, keepdims=True) + np.sum(gallery**2, axis=1) - 2 * np.dot(queries, gallery.T)

    partidioned_idxs = np.argpartition(dist_sq, kth=k-1, axis=1)[:, :k]
    partidioned_dists = np.take_along_axis(dist_sq, partidioned_idxs, axis=1)

    order = np.argsort(partidioned_dists, axis=1)

    I = np.take_along_axis(partidioned_idxs, order, axis=1)
    D = np.take_along_axis(partidioned_dists, order, axis=1)
    return I, D

def plot_time_complexity(T, N, nrepeats, ypred = None, xpred = None, model = None, figname='knn_time_complexity.png'):
    plt.figure(figsize=(10, 6))
    for j in range(nrepeats):
        plt.plot(N, T[j], color='blue', alpha=0.5)
    if ypred is not None and xpred is not None:
        if model is not None:
            # Get the coefficients of the linear regression model
            slope = model.coef_[0]
            intercept = model.intercept_
            label = f'y = {slope:.2e}x + {intercept:.2e}'
        else:
            label = 'Fitted Line'
        plt.plot(xpred, ypred, label=label, color='black', ls='--', linewidth=2)

    plt.xscale('log')
    plt.yscale('log')
    plt.xlabel('Number of gallery vectors (N)')
    plt.ylabel('Time (s)')
    plt.title('Time Complexity of KNN Search')
    plt.grid(True)
    plt.legend()
    plt.savefig(figname)
    plt.show()

def perform_knn_test():
    N = np.logspace(1, 7, num=100, base=10, dtype=int) # Number of gallery vectors
    m = 1           # Number of query vectors
    k = 10          # Number of nearest neighbors
    dim = 64        # Dimension of each vector
    nrepeats = 5    # Number of repetitions for each N
    T = np.zeros((nrepeats, len(N))) # Array to store time curves

    for j in range(nrepeats):
        times = []
        for n in tqdm(N):
            data = np.random.rand(n, dim).astype(np.float32)
            queries = np.random.rand(m, dim).astype(np.float32)

            ti = time.perf_counter()
            I, D = index_faiss(data, queries, k)
            #print(f"Results: {I.shape}, {D.shape}")
            tf = time.perf_counter()
            elapsed_time = tf - ti
            times.append(elapsed_time)

        T[j] = times
        np.save('knn_times.npy', T)
    
    plot_time_complexity(T, N, nrepeats, figname='knn_time_complexity.png')

def fit_data(T, N, nrepeats, figname='knn_time_complexity_fit.png'):
    # T = (nrepeats, len(N)) with each row being the time taken for each N

    # Considering only the data points N > 1e5
    mask = N > 1e5
    N_fit = N[mask]
    T_fit = T[:, mask]

    # Fit a linear regression model to the data

    # Repeat X
    X = N_fit.reshape(-1, 1)
    X = np.tile(X, (nrepeats, 1))
    y = T_fit.flatten()
    model = LinearRegression()
    model.fit(X, y)
    xpred = np.unique(X)
    ypred = model.predict(xpred.reshape(-1, 1))

    slope = model.coef_[0]
    intercept = model.intercept_

    print(f"Comparisons per second: {format_count(int(1/slope))}")

    plot_time_complexity(T, N, nrepeats, xpred=xpred, ypred=ypred, model = model, figname='knn_time_complexity_fit.png')

def main():
    np.random.seed(42)

    perform_knn_test()

    T = np.load('knn_times.npy')
    N = np.logspace(1, 7, num=100, base=10, dtype=int) # Number of gallery vectors
    nrepeats = 5    # Number of repetitions for each N
    fit_data(T, N, nrepeats, figname='knn_time_complexity_fit.png')


if __name__ == "__main__":
    main()