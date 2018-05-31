def main():
    grph = Grph(['A', 'B', 'C'])
    grph.set_cost('A', 'B', 6)
    grph.set_cost('A', 'C', 3)
    grph.set_cost('B', 'C', 2)
    min_cost, road = dijkstra_shortest_road(grph, 'A', 'B')
    print(min_cost)
    print(road)

# 算法参考《计算机网络自顶向下方法》中的网络层一章（解释的非常好）


class Grph:
    def __init__(self, nodes, undirected=True):
        self.nodes = nodes
        self.__undirected = undirected
        self.__cost = dict()
        for node in nodes:
            self.__cost[node] = dict()

    def set_cost(self, u, v, cost):
        self.__cost[u][v] = cost
        if self.__undirected:
            self.__cost[v][u] = cost

    def cost(self, u, v):
        if u == v:
            return 0
        if u not in self.__cost or v not in self.__cost[u]:
            return float("inf")
        return self.__cost[u][v]


def dijkstra(grph, start):
    unselected_nodes = set(grph.nodes)
    D = dict()
    prev = dict()

    for node in grph.nodes:
        D[node] = grph.cost(start, node)
        if D[node] != float("inf"):
            prev[node] = start

    while (len(unselected_nodes) > 0):
        w = min(unselected_nodes, key=lambda node: D[node])

        unselected_nodes.remove(w)

        for v in unselected_nodes:
            new_cost = D[w] + grph.cost(w, v)
            if new_cost < D[v]:
                D[v] = new_cost
                prev[v] = w
    return D, prev


def dijkstra_shortest_road(grph, start, end):
    D, prev = dijkstra(grph, start)
    road = [end]

    while True:
        prev_node = prev[road[len(road) - 1]]
        road.append(prev_node)
        if prev_node == start:
            break

    road.reverse()

    return D[end], road


if __name__ == '__main__':
    main()
