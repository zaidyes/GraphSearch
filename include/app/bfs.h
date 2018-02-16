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
            std::vector<bool> visitedNodes(graph->size(), false);

            // Get root node and push it to our process queue
            auto rootNode = graph->getFirst();

            // mark the node visited we donot want to queue it again
            visitedNodes[rootNode->getIndex()] = 1;
            toProcess.push(std::move(rootNode));

            while(!toProcess.empty()) {
                auto node = toProcess.front();
                toProcess.pop();

                // If we found our result no need to do anything else...break
                if (node->getId() == id) {
                    result = node;
                    break;
                }

                // check that the node's connections have been visited otherwise enqueue them
                auto connections = node->getConnections();
                for(auto weakNode : connections) {
                    if (auto lockedNode = weakNode.lock()) {
                        // skip the node if its already visited else queue it up
                        if (visitedNodes[lockedNode->getIndex()]) {
                            visitedNodes[lockedNode->getIndex()] = true;
                            toProcess.push(std::move(lockedNode));
                        }
                    }
                }
            }          

            return result;
        }

};
