import numpy as np
import matplotlib.pyplot as plt
import random
import sys 


def distance(p1, p2):
    return np.sum((p1 - p2)**2)
  
def initialize(df, k):
    centroids = []
    centroids.append(df[np.random.randint(
            df.shape[0]), :])
  
    for c_id in range(k - 1):
         
        dist = []
        # for each in df 
        for i in range(df.shape[0]):
            point = df[i, :]
            d = sys.maxsize
             
            for j in range(len(centroids)):
                temp_dist = distance(point, centroids[j])
                d = min(d, temp_dist)
            dist.append(d)
             
        # minimal distanse for all points to centroid ( closest centroid )
        dist = np.array(dist)
        # point with biggest minimal distance 
        next_centroid = df[np.argmax(dist), :]
        centroids.append(next_centroid)
        # plot(df, np.array(centroids))
    return np.array(centroids)

def plot_clusters(df, points_centroids, centroids):
    plt.scatter(df[:, 0], df[:, 1], c=points_centroids, cmap="viridis", alpha=0.5)
    plt.scatter(centroids[:, 0], centroids[:, 1], c="blue", marker="o", s=200, label="Centroids")
    plt.legend()
    plt.show()

def KMeans(df, clusters):

    centroids = initialize(df, clusters)
    last_centroids = np.zeros(clusters)
    best_inertia = sys.maxsize
    best_centroids = centroids
    
    for _ in range(10):
        for _ in range(1000):
            # each point to centorid 
            points_centroid = np.zeros(len(df))
            points_centroids = np.argmin(np.linalg.norm(df - centroids[:, np.newaxis], axis=2), axis=0)
            
            # Update centroids
            last_centroids = centroids
            centroids = np.array([np.mean(df[points_centroids == i], axis=0) for i in range(clusters)])
            
            inertia = np.sum((df - centroids[points_centroids]) ** 2)

        if inertia < best_inertia:
            best_inertia = inertia
            best_centroids = centroids
            best_points_centroids= points_centroids
        
    plot_clusters(df, best_points_centroids, best_centroids)


def main(file, clusters):
    df = np.loadtxt(file)
    print(file)
    
    KMeans(df, clusters)
    

if __name__ == "__main__":
    main("normal.txt", 4)
    main("unbalance.txt", 8)