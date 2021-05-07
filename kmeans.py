import sys


def main():
    datapoint = []
    centroid = []
    sum_array = []
    datap_cluster_assignment = []
    count_array = []

    update = True

    assert len(sys.argv) > 1, "Not enough arguments\n"
    k = int(sys.argv[1])
    max_iter = 200

    if len(sys.argv) > 2:
        max_iter = int(sys.argv[2])

    assert max_iter > 0, "max_iter input needs to be positive\n"
    assert k > 0, "k input needs to be positive\n"

    first_line = sys.stdin.readline().strip()

    d = first_line.count(',') + 1

    arraySize = initializeDatapointArray(datapoint, centroid, first_line, d, k)

    datap_cluster_assignment = [0 for i in range(arraySize)]
    sum_array = [[0 for j in range(d)] for i in range(arraySize)]
    count_array = [0 for i in range(arraySize)]

    while (max_iter > 0 and update):
        update = updateCentroidsPerDatap( datapoint, centroid, datap_cluster_assignment, sum_array, count_array)
        max_iter = max_iter - 1

    printFinalCentroids(centroid)

def initializeDatapointArray(datapoint, centroid, first_line, d, k):
    current_datapoint = [float(first_line.split(',')[i]) for i in range(d)]
    datapoint += [current_datapoint]
    centroid += [current_datapoint]
    count = 1
    for line in sys.stdin:
        current_datapoint = [float(line.split(',')[i]) for i in range(d)]
        datapoint += [current_datapoint]
        if count < k:
            centroid += [current_datapoint]
        count = count + 1

    assert k <= len(datapoint), "Provided k is too big"

    return len(datapoint)

def updateCentroidsPerDatap( datapoint, centroid, datap_cluster_assignment, sum_array, count_array):
    update = 0
    size = len(datapoint)
    k = len(centroid)
    d = len(datapoint[0])


    for i in range(size):
        min_dist = sys.float_info.max
        min_cluster = -1

        for j in range(k):
            dist = 0
            for v in range(d):
                dist += (datapoint[i][v] - centroid[j][v])*(datapoint[i][v] - centroid[j][v])

            if min_dist > dist:
                min_dist = dist
                min_cluster = j

        if datap_cluster_assignment[i] != min_cluster:
            update = 1

        datap_cluster_assignment[i] = min_cluster


    for i in range(size):
        currCluster = datap_cluster_assignment[i]
        count_array[currCluster] = count_array[currCluster] + 1
        for v in range(d):
            sum_array[currCluster][v] = sum_array[currCluster][v] + datapoint[i][v]

    for j in range(k):
        for v in range(d):
            new_value = sum_array[j][v] / count_array[j]
            centroid[j][v] = new_value
            sum_array[j][v] = 0
        count_array[j] = 0


    return update


def printFinalCentroids(centroid):
    k = len(centroid)
    d = len(centroid[0])

    for i in range(k):
        for j in range(d):
            print("{:.4f}".format(centroid[i][j]), end ="")
            if (j != d - 1):
                print(",", end ="")
            else:
                print("\n", end ="")
    return



if __name__ == "__main__":
    main()