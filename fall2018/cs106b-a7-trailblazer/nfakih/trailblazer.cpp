/*                  Homework #7: trailblazer.cpp
 * This program gives the user 6 different methods they can use from to find
 * a path from the user selected start and end point on a map.
 * by Noor Fakih        CS106B          Marty Stepp
 */

#include "trailblazer.h"
#include "queue.h"
#include "priorityqueue.h"
#include "stack.h"
using namespace std;

/*              DFS
 * It is a helper function for the depthFirstSearch algorithim. It finds the
 * first appropriate path that reaches from start to end by exploring each
 * branch as far as possible before backtracking.
 */
bool DFS(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path)
{
    //at the same location so true
    if(start == end)
    {
        start->setColor(GREEN);
        path.add(start);
        return true;
    }
    //has already been visited
    if(start->getColor() == GREEN)
    {
        return false;
    }
    path.add(start);
    start->setColor(GREEN);

    for(Edge* edge : start->edges)
    {
        if(!(edge->finish->getColor() == GREEN))
        {
            if(DFS(graph, edge->finish, end, path))
            {
                return true;
            }
        }
    }
    path.remove(path.size()-1);
    start->setColor(GRAY);

    return false;
}

/*              depthFirstSearch
 * This function runs a depth first search algorithm for finding a possible path
 * from the selected start and end by calling the helper function DFS which
 * traverses the graph and indicates where a path can be found.
 */
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end)
{
    graph.resetData();
    Vector<Vertex*> path;
    if(!DFS(graph, start, end, path))
    {
        path.clear();
    }
    return path;
}

/*              breadthFirstSearch
 * This algorithim runs a breadth first search which finds a possible path by
 * traversing through each vertex on the graph by one step at a time until one
 * of the branchs reaches the user selected end.
 */
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end)
{
    graph.resetData();
    Vector<Vertex*> path;
    Queue<Vertex*> queue;
    queue.enqueue(start);
    while(!queue.isEmpty())
    {
        Vertex* current = queue.dequeue();
        current->setColor(GREEN);

        if(BasicGraph::compare(current, end) == 0)
        {
            break;
        }

        for(Edge* edge : current->edges)
        {
            if(!(edge->finish->getColor()== GREEN) &&
                    edge->finish->getColor() != YELLOW)
            {
                edge->finish->setColor(YELLOW);
                queue.enqueue(edge->finish);
            }
        }
    }

    return path;
}

/*              dijkstrasAlgorithm
 * This search algorithm finds the shortest path from the user selected start vertex
 * to the end vertex.
 */
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end)
{
    graph.resetData();
    PriorityQueue<Vector<Vertex*>> pqueue;
    Set<Vertex*> visited;
    Vector<Vertex*> emptyPath;
    Vector<Vertex*> path;

    pqueue.enqueue({start}, 1);
    start->setColor(GREEN);

    while(!pqueue.isEmpty() && !visited.contains(end))
    {
            double pathCost = pqueue.peekPriority();
            path = pqueue.dequeue();
            Vertex* current = path.get(path.size()-1);
            current->setColor(GREEN);
            if(current==end)
            {
                return path;
            }
            if(!visited.contains(current))
            {
                visited.add(current);
                Set<Vertex*> neighborSet=graph.getNeighbors(current);
                for(Vertex* neighbor:neighborSet)
                {
                    Edge* edge = graph.getEdge(current, neighbor);
                    double edgeCost = edge->cost;
                    Vector<Vertex*> altPath = path;
                    altPath.add(neighbor);
                    pqueue.enqueue(altPath, pathCost + edgeCost);
                    if(!visited.contains(neighbor))
                    {
                        neighbor->setColor(YELLOW);
                    }
                }
            }
        }

    return emptyPath;
}

/*      a*
 * A search algorithm that finds the "cheapest" cost and shortest path from the start to end.
 */
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end)
{
    graph.resetData();
    PriorityQueue<Vector<Vertex*>> pQueue;
    Set<Vertex*> visited;
    Vector<Vertex*> currentPath;
    Edge * edge;
    Vertex* current = start;
    double  theHeuristic;

    pQueue.enqueue({start}, 0.0);
    start->setColor(YELLOW);

    if(current == end)
    {
        return {start};
    }

    while(!pQueue.isEmpty())
    {
        currentPath = pQueue.dequeue();
        current = currentPath[currentPath.size()-1];
        if(!visited.contains(current))
        {
            current->setColor(GREEN);
            visited.add(current);

            if(current == end)
            {
                return currentPath;
            }

            for(Vertex* neighbor :  graph.getNeighbors(current))
            {
                if(!visited.contains(neighbor))
                {
                    current->setColor(YELLOW);
                    Vector<Vertex*> newPath = currentPath;
                    newPath.add(neighbor);
                    edge = graph.getEdge(current, neighbor);

                    theHeuristic = heuristicFunction(current, end);
                    double pathCost = 0;

                    for(int i = 1; i < newPath.size(); i++)
                    {
                        pathCost += graph.getEdge(newPath[i-1], newPath[i])->cost;
                    }
                    pQueue.enqueue(newPath, theHeuristic + edge->cost);
                }
            }
        }
    }
    return currentPath;
}

/*              differenceFormula
 * Helper function for alternate path that calcualtes if the alternate path is different enough from
 * the the best path which aStar created.
 */
double differenceFormula(Vector<Vertex*> altPath, Vector<Vertex*> bestPath)
{
    double numDifferent = bestPath.size() - altPath.size();
    return numDifferent / (double)bestPath.size();
}

/*                  alternatePath
 * Search algorithm similar to A* but finds a slightly diffrent path than what A* finds and considers to
 * be the best path
 */
Vector<Vertex*> alternatePath(BasicGraph& graph, Vertex* start, Vertex* end, double difference)
{
    Vector<Vertex*> bestPath = aStar(graph, start, end);
    Vector<Vertex*> emptyPath;
    PriorityQueue<Vector<Vertex*>> possibles;
    for(int i = 0; i < bestPath.size() - 1; i++)
    {
        BasicGraph graphCopy = graph;
        Edge* deletedEdge = graph.getEdge(bestPath.get(i), bestPath.get(i + 1));
        graphCopy.removeEdge(deletedEdge);
        Vector<Vertex*> altPath = aStar(graphCopy, start, end);
        if(altPath !=emptyPath)
        {
            possibles.enqueue(altPath, differenceFormula(altPath, bestPath));
        }
    }
    while(!possibles.isEmpty())
    {
        if(possibles.peekPriority() >= difference)
        {
            return possibles.dequeue();
        }
        else
        {
            possibles.dequeue();
        }
    }
    return emptyPath;
}

// helper function for kruskal
Set<Vertex*> getClusters(Vertex* node, Set<Set<Vertex*> >& clusterSet) {
    Set<Vertex*> foundCluster;
    for (Set<Vertex*> cluster : clusterSet) {
        if (cluster.contains(node))
            foundCluster = cluster;
    }
    return foundCluster;
}

/*      kruskal
 * function that creates randomly generated mazes
 */
Set<Edge*> kruskal(BasicGraph& graph)
{
    Set<Edge*> allEdgeSet = graph.getEdgeSet();
    Set<Edge*> mst;
    mst.clear();
    Set<Set<Vertex*> > clusterSet;

    for (Vertex* node : graph)
    {
        Set<Vertex*> newCluster;
        newCluster.add(node);
        clusterSet.add(newCluster);
    }

    PriorityQueue<Edge*> edgeQueue;
    for (Edge* edge : allEdgeSet)
    {
        edgeQueue.enqueue(edge, edge->weight);
    }

    while (clusterSet.size() > 1)
    {
        Edge* edge = edgeQueue.dequeue();
        bool yesOrNo = false;
        for (Set<Vertex*> cluster : clusterSet)
        {
            if (cluster.contains(edge->start) && cluster.contains(edge->finish))
            {
                yesOrNo = true;
            }
        }

        if (!yesOrNo)
        {
            Set<Vertex*> startCluster = getClusters(edge->start, clusterSet);
            Set<Vertex*> finishCluster = getClusters(edge->finish, clusterSet);

            clusterSet.remove(getClusters(edge->start, clusterSet));
            clusterSet.remove(getClusters(edge->finish, clusterSet));
            Set<Vertex*> mergeCluster = startCluster + finishCluster;
            clusterSet.add(mergeCluster);
            mst.add(edge);
        }
    }
    return mst;
}
