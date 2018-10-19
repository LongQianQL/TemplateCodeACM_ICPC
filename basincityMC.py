import time
import random
start = time.time()

first_mask = 0
first_placed = 0


def place_drone(graph, covered, index):
    covered |= 1 << index
    for j in graph[index]:
        covered |= 1 << j - 1
    return covered


def kernel(graph, covered):
    pruned = 0
    for index in range(len(graph)):

        # This node is already blocked off
        if covered & 1 << index:
            continue

        # Counts nodes not yet blocked off
        degree = 0
        for j in graph[index]:
            if not covered & 1 << j-1:
                degree += 1

        # If this node is now a leaf, block it and its neighbours off
        if degree == 1:
            pruned += 1
            covered = place_drone(graph, covered, index)

    if pruned:
        extra_pruned, covered = kernel(graph, covered)
        return pruned + extra_pruned, covered
    else:
        return pruned, covered


def monte(graph, covered, placed, drones):
    while True:
        # Base case - if all drones placed, success. If graph is filled, start again. If time is up, guess not valid.
        if placed >= drones:
            return True
        if time.time() - start > 0.95:
            return False
        if covered == (1 << len(graph)) - 1:
            covered = first_mask
            placed = first_placed

        # Kernelise graph then check that more drones still can/need to be placed
        pruned, covered = kernel(graph, covered)
        placed += pruned
        if placed >= drones:
            return True
        if covered == (1 << len(graph)) - 1:
            covered = first_mask
            placed = first_placed

        # Pick random node place
        choice = random.randint(0, len(graph)-1)
        while True:
            if not covered & 1 << choice:
                covered = place_drone(graph, covered, choice)
                placed += 1
                break
            choice = (choice+1) % len(graph)


def solve():
    k = int(input())
    n = int(input())
    if n >= k * 5:
        print("possible")
        return

    graph = []
    for i in range(n):
        temp = list(map(int, input().split()))
        graph.append(temp[1:])

    # First round of kernelisation done so the recursion doesn't redo
    global first_mask
    global first_placed
    placed, first_mask = kernel(graph, 0)

    if monte(graph, first_mask, placed, k):
        print("possible")
    else:
        print("impossible")

solve()
