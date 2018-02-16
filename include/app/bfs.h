/*
 *
 * Breadth First Search (BFS)
 *
 */

#pragma once

#include <app/graph.h>

#include <auxiliary/logger.h>

#include <queue>
#include <string>
#include <vector>
#include <iostream>

/**
 * 
 *  Breadth-first-search (BFS) implementation
 * 
 */
class BreadthFirstSearch {
    
    public:
        /**
         * 
         * Find a named node.
         * 
         * This method performs a BFS and returns the first node with
         * a given identifier.
         * 
         * @param root Root node to start the search.
         * @param id Identifier to be found.
         * @return Returns the found node, or null in case no node
         * has been found.
         * 
         */
        NodeRef find(GraphRef graph, const std::string& id) {

            NodeRef result{nullptr};

            if (!graph) {
                Log::error("Graph invalid...skipping search");
                return result;
            }

            std::queue<NodeRef> toProcess;
            std::vector<NodeRef> visitedNodes;
            visitedNodes.reserve(graph->size());

            auto rootNode = graph->getFirst();
            rootNode->markVisited();
            visitedNodes.push_back(rootNode);
            toProcess.push(rootNode);

            while(!toProcess.empty()) {

                auto node = toProcess.front();
                toProcess.pop();

                if (node->getId() == id) {
                    result = node;
                    break;
                }

                auto connections = node->getConnections();
                for(auto itr = connections.begin(); itr != connections.end(); ++itr) {
                    if (auto lockedNode = (*itr).lock()) {
                        // skip the node if its already visited else queue it up
                        if (!lockedNode->visited()) {
                            lockedNode->markVisited();
                            visitedNodes.push_back(lockedNode);
                            toProcess.push(lockedNode);
                        }
                    }
                }

            }

            // set all nodes we visited to unvisited
            for (auto node : visitedNodes) {
                node->markVisited(false);
            }

            return result;

        }

};
