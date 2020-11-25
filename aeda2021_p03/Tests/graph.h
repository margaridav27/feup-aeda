#include <iostream>
#include <vector>

using namespace std;

template <class N, class E> class Edge;

template <class N, class E>
class Node {
public:
    N info;
    vector< Edge<N,E> > edges;
    Node(N inf) {
        info = inf;
    }
};

template <class N, class E>
class Edge {
public:
    E value;
    Node<N,E> *destination;
    Edge(Node<N,E> *dest, E val) {
        value = val;
        destination = dest;
    }
};

template <class N, class E>
class Graph {
    vector< Node<N,E> *> nodes;
public:
    Graph() = default;
    ~Graph();
    Graph & addNode(const N &inf);
    Graph & addEdge(const N &begin, const N &end, const E &val);
    Graph & removeEdge(const N &begin, const N &end);
    E & edgeValue(const N &begin, const N &end);
    unsigned numEdges(void) const;
    unsigned numNodes(void) const;
    void print(std::ostream &os) const;
};

template <class N, class E>
std::ostream & operator<<(std::ostream &out, const Graph<N,E> &g);

// exception NodeAlreadyExists
template <class N>
class NodeAlreadyExists
{
public:
    N info;
    NodeAlreadyExists(N inf) { info=inf; }
};
template <class N>
std::ostream & operator<<(std::ostream &out, const NodeAlreadyExists<N> &no)
{ out << "Node already exists: " << no.info; return out; }

// exception NodeDoesNotExist
template <class N>
class NodeDoesNotExist {
public:
    N info;
    NodeDoesNotExist(N inf) {
        info = inf;
    }
};
template <class N>
std::ostream & operator<<(std::ostream &out, const NodeDoesNotExist<N> &no)
{ out << "Node does not exist: " << no.info; return out; }

// exception EdgeAlreadyExists
template <class N>
class EdgeAlreadyExists {
public:
    N begin;
    N end;
    EdgeAlreadyExists(N b, N e) {
        begin =  b;
        end = e;
    }
};
template <class N>
std::ostream & operator<<(std::ostream &out, const EdgeAlreadyExists<N> &ed)
{ out << "Edge already exists: " << ed.begin << " , " << ed.end; return out; }

// exception EdgeDoesNotExist
template <class N>
class EdgeDoesNotExist {
public:
    N begin;
    N end;
    EdgeDoesNotExist(N b, N e) {
        begin = b;
        end = e;
    }
};
template <class N>
std::ostream & operator<<(std::ostream &out, const EdgeDoesNotExist<N> &ed)
{ out << "Edge does not exist: " << ed.begin << " , " << ed.end; return out; }

template <class N, class E>
Graph<N, E>::~Graph() {
    for (unsigned i = 0; i < numNodes(); i++) {
        delete nodes[i];
    }
}

template <class N, class E>
Graph<N,E> & Graph<N, E>::addNode(const N &inf) {
    for (unsigned i = 0; i < numNodes(); i++) {
        if (nodes[i]->info == inf) {
            throw NodeAlreadyExists<N>(inf);
        }
    }
    Node<N, E> *newNode = new Node<N, E>(inf);
    nodes.push_back(newNode);
    return *this;
}

template <class N, class E>
Graph<N,E> & Graph<N, E>::addEdge(const N &begin, const N &end, const E &val) {
    for (unsigned d = 0; d < numNodes(); d++) {
        if (nodes[d]->info == end) { // i found the destination node, i.e. nodes[d]
            for (unsigned s = 0; s < numNodes(); s++) {
                if (nodes[s]->info == begin) { // i found the source node, i.e. nodes[s]
                    for (size_t e = 0; e < nodes[s]->edges.size(); e++) {
                        if (nodes[s]->edges[e].destination->info == end) {
                            throw EdgeAlreadyExists<N>(begin, end); // if the edge connected to the source node has the same destination node
                                                                    // it means that the edge i am trying to add already exists
                        }
                    }
                    Edge<N, E> newEdge(nodes[d], val); // the new edge has, as destination node, the one i found previously
                    nodes[s]->edges.push_back(newEdge);
                    return *this;
                }
            }
        }
    }
    throw NodeDoesNotExist<N>(begin); // since i haven't found neither the source nor the destination nodes
}

template <class N, class E>
Graph<N,E> & Graph<N, E>::removeEdge(const N &begin, const N &end) {
    for (unsigned n = 0; n < numNodes(); n++) {
        if (nodes[n]->info == begin) {
            for (unsigned e = 0; e < nodes[n]->edges.size(); e++) {
                if (nodes[n]->edges[e].destination->info == end) {
                    nodes[n]->edges.erase(nodes[n]->edges.begin() + e);
                    return *this;
                }
            }
            throw EdgeDoesNotExist<N>(begin, end);
        }
    }
    throw NodeDoesNotExist<N>(begin);
}

template <class N, class E>
E & Graph<N, E>::edgeValue(const N &begin, const N &end) {
    for (unsigned n = 0; n < numNodes(); n++) {
        if (nodes[n]->info == begin) {
            for (unsigned e = 0; e < nodes[n]->edges.size(); e++) {
                if (nodes[n]->edges[e].destination->info == end) {
                    return nodes[n]->edges[e].value;
                }
            }
            throw EdgeDoesNotExist<N>(begin, end);
        }
    }
    throw NodeDoesNotExist<N>(begin);
}

template <class N, class E>
unsigned Graph<N, E>::numNodes(void) const {
    return nodes.size();
}

template <class N, class E>
unsigned Graph<N, E>::numEdges(void) const {
    unsigned numEdges = 0;
    for (unsigned i = 0; i < numNodes(); i++) {
        numEdges += nodes[i]->edges.size();
    }
    return numEdges;
}

template <class N, class E>
void Graph<N, E>::print(std::ostream &os) const {
    for (unsigned n = 0; n < numNodes(); n++) {
        os << "( " << nodes[n]->info;
        for (unsigned e = 0; e < nodes[n]->edges.size(); e++) {
            os << "[ " << nodes[n]->edges[e].destination->info << " " << nodes[n]->edges[e].value << "] ";
        }
        os << ") ";
    }
}

template <class N, class E>
std::ostream & operator<<(std::ostream &out, const Graph<N,E> &g) { g.print(out); return out; }

